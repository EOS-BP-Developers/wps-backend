'use strict';

const _ = require('lodash'),
    mongo = require('models/mongo');

const Proposer = mongo.LibProposer;
const UpdateProposal = mongo.LibUpdateProposal;

async function regproposer(action, trx) {
    const data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    return Proposer.create(data);
}

async function editproposer(action, trx) {
    const data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    return Proposer.updateOne({account : data.account}, {$set : data});
}

async function rmvproposer(action, trx) {
    let data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    return Proposer.deleteOne({account: data.account});
}

async function claimfunds(action, trx) {
    const data = action.data;
    return UpdateProposal.create({proposer : data.account, proposal_id : data.proposal_id});
}

module.exports = exports = {regproposer, editproposer, rmvproposer, claimfunds};
