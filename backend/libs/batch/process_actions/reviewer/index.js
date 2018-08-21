'use strict';

const _ = require('lodash'),
    mongo = require('models/mongo'),
    SEnum = require('libs/enum');

const Reviewer = mongo.LibReviewer;
const UpdateProposal = mongo.LibUpdateProposal;
const UpdateVoting = mongo.LibUpdateVoting;

async function regreviewer(action) {
    let data = Object.assign({}, action.data, {account : action.data.reviewer});
    return Reviewer.create(data);
}

async function editreviewer(action) {
    let data = Object.assign({}, action.data, {account : action.data.reviewer});
    return Reviewer.updateOne({account : data.account}, {$set : data});
}

async function rmvreviewer(action) {
    const account = action.data.reviewer;
    if (_.isEmpty(account)) {
        return;
    }
    return Reviewer.deleteOne({account : account});
}

async function acceptprop(action) {
    const data = action.data;
    return UpdateProposal.create({proposal_id : data.proposal_id});
}

async function rejectprop(action) {
    const data = action.data;
    return UpdateProposal.create({proposal_id : data.proposal_id});
}

async function checkvote(action) {
    const data = action.data;
    return UpdateVoting.create({proposal_id : data.proposal_id, action : SEnum.VOTE_ACTION_CHECK});
}

async function approve(action) {
    const data = action.data;
    return UpdateProposal.create({proposal_id : data.proposal_id});
}

async function rmvreject(action) {
    const data = action.data;
    return UpdateProposal.create({proposal_id : data.proposal_id});
}

async function rmvcompleted(action) {
}

module.exports = exports = {regreviewer, editreviewer, rmvreviewer, acceptprop, rejectprop, checkvote, approve, rmvreject, rmvcompleted};
