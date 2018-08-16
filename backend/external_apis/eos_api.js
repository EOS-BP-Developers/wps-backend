'use strict';

const _ = require('lodash'),
    Eos = require('eosjs'),
    config = require('config');

const {format /*, api, ecc, json, Fcbuffer*/} = Eos.modules;

const eosNodeConfig = config.eosNode;

function getConfig(key, httpEndPoint) {
    const endpoint = getEndpoint(httpEndPoint);
    return {
        keyProvider: key, // || eosNodeConfig.watchmanPriKey,
        httpEndpoint: endpoint,
        // mockTransactions: () => 'pass', // or 'fail'
        // transactionHeaders: (expireInSeconds, callback) => { callback(null/*error*/, headers)},
        expireInSeconds: 60,
        broadcast: true,
        debug: false,
        sign: true,
        chainId: eosNodeConfig.chainId
    };
}

function getEndpoint(endpoint) {
    if (_.isEmpty(endpoint)) {
        endpoint = `http://${eosNodeConfig.host}:${eosNodeConfig.httpPort}`;
    }
    return endpoint;
}

function getInfo(endpoint) {
    const options = getConfig(endpoint);
    const eos = Eos(options);
    return eos.getInfo({})
        .catch(function(err) {
            if (typeof err === 'string') {
                throw JSON.parse(err);
            }
            throw err;
        });
}

function getBlock(blockId, endpoint) {
    const options = getConfig(endpoint);
    const eos = Eos(options);
    return eos.getBlock(blockId)
        .catch(function(err) {
            if (typeof err === 'string') {
                throw JSON.parse(err);
            }
            throw err;
        });
}

function getTableRows(code, scope, table, tableKey, lowerBound, limit, keyType, indexPosition, endpoint) {
    const options = getConfig(endpoint);
    const eos = Eos(options);

    const params = {
        json : true, // {type : "bool", "default": false},
        code : code,
        scope : scope,
        table : table,
        table_key : tableKey,
        lower_bound : lowerBound,
        //upper_bound: {type : "string", default: '-1'},
        limit : limit,
        key_type : keyType,
        index_position : indexPosition
    };

    return eos.getTableRows(params)
        .catch(function(err) {
            if (typeof err === 'string') {
                throw JSON.parse(err);
            }
            throw err;
        });
}

function getVoterInfo(voter) {
    return getTableRows('eosio', 'eosio', 'voters', null, format.encodeName(voter, false), 1, 'i64', 1)
        .then(function(result) {
            if (!_.isEmpty(result.rows)) {
                return result.rows[0];
            }
            return {};
        });
}

function getProposalById(proposalId) {
    return getTableRows('eosio.wps', 'eosio.wps', 'proposals', null, proposalId, 1, 'i64', 2)
        .then(function(result) {
            if (!_.isEmpty(result.rows)) {
                return result.rows[0];
            }
            return {};
        });
}

function getProposalByOwner(proposer) {
    return getTableRows('eosio.wps', 'eosio.wps', 'proposals', null, format.encodeName(proposer, false), 1, 'i64', 1)
        .then(function(result) {
            if (!_.isEmpty(result.rows)) {
                return result.rows[0];
            }
            return {};
        });
}

function getRejectedProposalById(proposalId) {
    return getTableRows('eosio.wps', 'eosio.wps', 'rejectedpros', null, proposalId, 1, 'i64', 2)
        .then(function(result) {
            if (!_.isEmpty(result.rows)) {
                return result.rows[0];
            }
            return {};
        });
}

function getFinishedProposalById(proposalId) {
    return getTableRows('eosio.wps', 'eosio.wps', 'finishedpros', null, proposalId, 1, 'i64', 2)
        .then(function(result) {
            if (!_.isEmpty(result.rows)) {
                return result.rows[0];
            }
            return {};
        });
}


function getCurrencyStats(endpoint) {
    const options = getConfig(endpoint);
    const eos = Eos(options);
    return eos.getCurrencyStats('eosio.token', 'EOS');
}

function commitVote(params, key, authorization, endpoint) { // eosio.wps contract
    const options = getConfig(key, endpoint);
    const eos = Eos(options);
    const wpsAccount = eosNodeConfig.wpsAccount;
    return eos.contract(wpsAccount)
        .then((contract) => {
            return contract.commitvote(params, {authorization: authorization});
        })
        .catch(function(err) {
            if (typeof err === 'string') {
                throw JSON.parse(err);
            }
            throw err;
        });
}

function rollbackVote(params, key, authorization, endpoint) { // eosio.wps contract
    const options = getConfig(key, endpoint);
    const eos = Eos(options);
    const wpsAccount = eosNodeConfig.wpsAccount;
    return eos.contract(wpsAccount)
        .then((contract) => {
            return contract.rollbackvote(params, {authorization: authorization});
        })
        .catch(function(err) {
            if (typeof err === 'string') {
                throw JSON.parse(err);
            }
            throw err;
        });
}

function checkExpire(params, key, authorization, endpoint) { // eosio.wps contract
    const options = getConfig(key, endpoint);
    const eos = Eos(options);
    const wpsAccount = eosNodeConfig.wpsAccount;
    return eos.contract(wpsAccount)
        .then((contract) => {
            return contract.checkexpire(params, {authorization: authorization});
        })
        .catch(function(err) {
            if (typeof err === 'string') {
                throw JSON.parse(err);
            }
            throw err;
        });
}

function getEos(endpoint) {
    const options = getConfig(endpoint);
    return Eos(options);
}

module.exports = exports = {getInfo, getBlock, getTableRows,
    getVoterInfo, getProposalById, getProposalByOwner,
    getRejectedProposalById, getFinishedProposalById,
    getCurrencyStats,
    commitVote, rollbackVote, checkExpire,
    getEos,
};


