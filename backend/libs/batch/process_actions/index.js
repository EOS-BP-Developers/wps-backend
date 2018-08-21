'use strict';

const Promise = require('bluebird'),
    _ = require('lodash'),
    config = require('config'),
    eosApi = require('external_apis/eos_api'),
    mongo = require('models/mongo'),
    log = require('libs/log'),
    committee = require('./committee'),
    reviewer = require('./reviewer'),
    proposer = require('./proposer'),
    proposal = require('./proposal'),
    watchman = require('./watchman'),
    delegate = require('./delegate'),
    vote = require('./vote');

const batchErrLog = log.batchErrLog;
const batchLog = log.batchLog;
const Summary = mongo.LibSummary;
const Action = mongo.LibAction;

const eosNodeConfig = config.eosNode;

function getAccountsFuncMap() {
    return [{
        account : eosNodeConfig.systemAccount, //'eosio',
        funcMap : delegate
    }, {
        account : eosNodeConfig.wpsAccount,    //'eosio.wps',
        funcMap : _.extend(committee, reviewer, proposer, proposal, vote, watchman)
    }];
}

async function processActions() {
    let blockNum = 0;
    let blockId = '';
    let summary = await Summary.findOne();
    if (_.isEmpty(summary)) {
        batchErrLog.info({reason : 'NOT_EXIST_SUMMARY'});
        return;
    }
    blockNum = summary.block_num;

    const chainInfo = await eosApi.getInfo();
    if (_.isEmpty(chainInfo)) {
        batchErrLog.info({reason : 'CANNOT_GET_CHAIN_INFO'});
        return;
    }
    let diff = chainInfo.last_irreversible_block_num - blockNum;
    batchLog.info(`blockNum : ${blockNum}`);
    batchLog.info(`diff : ${diff}`);
    if (diff <= 0) {
        return;
    }
    if (diff > 100) {
        diff = 100;
    }
    return Promise.each(_.range(blockNum + 1, blockNum + diff + 1), (_blockNum) => {
        return eosApi.getBlock(_blockNum)
            .catch((err) => {
                if (!_.isEmpty(err.message)) {
                    const message = JSON.parse(err.message);
                    const error = message.error || {};
                    if (error.code === 3100002) { // unknown_block_exception
                        err.reason = 'NOT_EXIST_BLOCK';
                        err.blockNum = _blockNum;
                        batchErrLog.info(err);
                        return {};
                    }
                }
                err.blockNum = _blockNum;
                throw err;
            })
            .then(async function(_block) {
                if (_.isEmpty(_block)) {
                    return;
                }
                const transactions = _block.transactions;
                blockId = _block.id;
                if (!_.isEmpty(transactions)) {
                    return Promise.each(transactions, async function(transaction) {
                        const trx = transaction.trx;
                        return _processActions(transaction.trx.transaction.actions, trx, blockNum);
                    });
                }
            })
            .then(async function() {
                if (_.isEmpty(summary)) {
                    summary = await Summary.create({block_id : blockId, block_num : _blockNum});
                }
                return Summary.update({_id : summary._id}, {block_num : _blockNum, block_id : blockId});
            })
            .catch((err) => {
                batchErrLog.info(err);
            });
    });
}

async function _processActions(actions, trx, blockNum) {
    let accountsFuncMap = getAccountsFuncMap();
    return Promise.each(actions, async function(action) {
        let account =  _.find(accountsFuncMap, {account : action.account});
        if (!_.isEmpty(account) && !_.isNil(account.funcMap[action.name])) {
            await _saveAction(action, trx, blockNum);
            await account.funcMap[action.name](action, trx);
        }
    });
}

async function _saveAction(action, trx, blockNum) {
    const actionObj = {
        trx_id : trx.trx_id,
        block_num : blockNum,
        account : action.account,
        name : action.name,
        authorization : _.map(action.authorization, (authorization) => `${authorization.actor}@${authorization.permission}`),
        raw : action
    };
    await Action.create(actionObj);
}

module.exports = exports = {processActions};
