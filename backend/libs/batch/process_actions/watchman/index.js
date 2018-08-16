'use strict';

const _ = require('lodash'),
    mongo = require('models/mongo');

const UpdateProposal = mongo.LibUpdateProposal;

async function commitvote(action) {
    // console.log('commitvote =====> ');
    // console.log(action);
    const data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    return UpdateProposal.create({proposal_id : data.proposal_id});
}

async function rollbackvote(action) {
    const data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    return UpdateProposal.create({proposal_id : data.proposal_id});
}

async function checkexpire(action) {
    const data = action.data;
    if (_.isEmpty(data)) {
        return;
    }
    return UpdateProposal.create({proposal_id : data.proposal_id});
}

module.exports = exports = {commitvote, rollbackvote, checkexpire};
