'use strict';

const mongo = require('models/mongo');

const Proposal = mongo.LibProposal;

function getProposals(proposer) {
    return Proposal.find({proposer : proposer}).sort({_id : -1});
}

module.exports = exports = {getProposals};
