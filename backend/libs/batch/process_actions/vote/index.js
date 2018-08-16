'use strict';

const _ = require('lodash'),
    mongo = require('models/mongo'),
    SEnum = require('libs/enum');

const UpdateVoting = mongo.LibUpdateVoting;

async function vote(action, trx) {
    const data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    return UpdateVoting.create({account : data.voter, proposal_id : data.proposal_id, action : SEnum.VOTE_ACTION_VOTE, is_agree : data.is_agree});
}

async function unvote(action, trx) {
    const data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    return UpdateVoting.create({account : data.voter, proposal_id : data.proposal_id, action : SEnum.VOTE_ACTION_UNVOTE});
}

module.exports = exports = {vote, unvote};
