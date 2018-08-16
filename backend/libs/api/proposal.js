'use strict';

const mongo = require('models/mongo');

const Proposal = mongo.LibProposal;

function getProposals(params) {
    const committee = params.committee;
    const order = params.order;
    if (order === 'hot') {
        return Proposal.find({committee : committee}).sort({total_votes : -1});
    } else {
        return Proposal.find({committee : committee}).sort({_id : -1});
    }
}

function getProposal(proposalId) {
    return Proposal.findOne({proposal_id : proposalId});
}

module.exports = exports = {getProposals, getProposal};
