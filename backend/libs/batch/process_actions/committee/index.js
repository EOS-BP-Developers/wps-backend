'use strict';

const _ = require('lodash'),
    mongo = require('models/mongo'),
    SEnum = require('libs/enum');
//    log = require('libs/log');

const Committee = mongo.LibCommittee;
const Proposal = mongo.LibProposal;

async function regcommittee(action, trx) {
    let data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    return Committee.create(data);
}

async function edcommittee(action, trx) {
    let data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    return Committee.update({committeeman: data.committeeman}, {$set : data});
}

async function rmvcommittee(action, trx) {
    let data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    return Committee.remove({committeeman: data.committeeman});
}

async function rejectfund(action, trx) {
    let data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    await Proposal.update({proposal_id : data.proposal_id}, {$set : {status : SEnum.PROPOSAL_STATUS_REJECT}});
}

module.exports = exports = {regcommittee, edcommittee, rmvcommittee, rejectfund};
