'use strict';

const Promise = require('bluebird'),
    _ = require('lodash'),
    eosApi = require('external_apis/eos_api'),
    mongo = require('models/mongo'),
    SEnum = require('libs/enum'),
    log = require('libs/log');

const batchErrLog = log.batchErrLog;
const batchLog = log.batchLog;

const UpdateProposal = mongo.LibUpdateProposal;
const Proposal = mongo.LibProposal;

async function updateProposals() {
    // batchLog.info('updateProposals');
    const chainInfo = await eosApi.getInfo();
    if (_.isEmpty(chainInfo)) {
        batchErrLog.info({reason : 'CANNOT_GET_CHAIN_INFO'});
        return;
    }
    const updateProps = await UpdateProposal.find().limit(30).sort({_id : 1});
    if (!_.isEmpty(updateProps)) {
        return Promise.each(updateProps, function(prop) {
            return _updateProposal(prop)
                .catch((err) => {
                    err.updateProp = prop;
                    batchErrLog.info = err;
                    throw err;
                });
        });
    }
}

async function _updateProposal(prop) {
    let proposal = null;
    if (prop.proposal_id > 0) {
        const proposalId = prop.proposal_id;
        proposal = await Promise.join(
            eosApi.getProposalById(proposalId),
            eosApi.getRejectedProposalById(proposalId),
            eosApi.getFinishedProposalById(proposalId),
            (proposal, rejectedProposal, finishedProposal) => {
                if (!_.isEmpty(proposal)) {
                    return proposal;
                } else if (!_.isEmpty(rejectedProposal)) {
                    return rejectedProposal;
                } else if (!_.isEmpty(finishedProposal)) {
                    return finishedProposal;
                }
                return {};
            });
        if (!_.isEmpty(proposal)) {
            delete proposal.total_votes;
            delete proposal.agree_votes;
            delete proposal.disagree_votes;
            await Proposal.updateOne({proposal_id : proposal.id}, {$set : proposal});
        }
    } else {
        proposal = await eosApi.getProposalByOwner(prop.proposer);
        if (!_.isEmpty(proposal)) {
            delete proposal.total_votes;
            delete proposal.agree_votes;
            delete proposal.disagree_votes;

            proposal.proposal_id = proposal.id;
            await Proposal.updateOne({proposer : prop.proposer, status : SEnum.PROPOSAL_STATUS_PENDING}, {$set :proposal});
        }
    }
    if (!_.isEmpty(prop)) {
        return prop.remove();
    }
}

module.exports = exports = {updateProposals};
