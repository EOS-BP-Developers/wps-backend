'use strict';

const _ = require('lodash'),
    mongo = require('models/mongo'),
    SEnum = require('libs/enum');

const Proposal = mongo.LibProposal;
const UpdateProposal = mongo.LibUpdateProposal;

async function regproposal(action) {
    const data = Object.assign({}, action.data, {status : SEnum.PROPOSAL_STATUS_PENDING});
    if (_.isEmpty(data)) {
        return;
    }
    await Proposal.create(data);
    await UpdateProposal.create({proposer : data.proposer, proposal_id : 0});
}

async function editproposal(action) {
    const data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    await Proposal.update({proposer : data.proposer}, {$set : data});
    await UpdateProposal.create({proposer : data.proposer, proposal_id : 0});
}

function rmvproposal(action) {
    const data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    return Proposal.remove({proposer : data.proposer, status : {$nin : [SEnum.PROPOSAL_STATUS_REJECT, SEnum.PROPOSAL_STATUS_COMPLETE]}});
}

module.exports = exports = {regproposal, editproposal, rmvproposal};
