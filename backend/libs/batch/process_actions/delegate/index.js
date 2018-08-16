'use strict';

const mongo = require('models/mongo'),
    SEnum = require('libs/enum');

const ActionStake = mongo.LibActionStake;
const ActionUnstake = mongo.LibActionUnstake;
const UpdateVoting = mongo.LibUpdateVoting;

async function delegatebw(action, trx) {
    let data = Object.assign({}, action.data, {trx_id : trx.id});
    await ActionStake.create(data);
    await UpdateVoting.create({account : data.receiver, action : SEnum.VOTE_ACTION_STAKE});
}

async function undelegatebw(action, trx) {
    let data = Object.assign({}, action.data, {trx_id : trx.id});
    await ActionUnstake.create(data);
    await UpdateVoting.create({account : data.receiver, action : SEnum.VOTE_ACTION_UNSTAKE});
}

module.exports = exports = {delegatebw, undelegatebw};
