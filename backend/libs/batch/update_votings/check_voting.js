'use strict';

const _ = require('lodash'),
    mongo = require('models/mongo'),
    eosApi = require('external_apis/eos_api'),
    SUtil = require('libs/util'),
    SEnum = require('libs/enum'),
    log = require('libs/log');

const batchErrLog = log.batchErrLog;

const Proposal = mongo.LibProposal;
const WpsInfo = mongo.WpsInfo;

async function checkVoting(updateVoting) {
    let wpsInfo = await WpsInfo.findOne();
    if (_.isEmpty(wpsInfo)) {
        batchErrLog.info({reason : 'CANNT_GET_WPS_INFO_FROM_DB'});
        return;
    }

    const totalPercent = wpsInfo.total_voting_percent;
    const agreePercent = wpsInfo.agree_percent;
    const watchmanAccount = wpsInfo.watchman_account;
    const key = wpsInfo.watchman_prv;

    let proposal = await Proposal.findOne({proposal_id : updateVoting.proposal_id});
    if (_.isEmpty(proposal)) {
        batchErrLog.info({reason : 'CANNT_GET_PROPOSAL_FROM_DB', updateVoting});
        return updateVoting.remove();
    }
    let stats = await eosApi.getCurrencyStats();
    let proposalFromNode = await eosApi.getProposalById(updateVoting.proposal_id);
    if (_.isEmpty(stats) || _.isEmpty(proposalFromNode)) {
        batchErrLog.info({reason : 'CANNT_GET_STATS_OR_PROPOSAL_FROM_NODE', updateVoting, stats, proposalFromNode});
        return updateVoting.remove();
    }
    if (proposalFromNode.status !== SEnum.PROPOSAL_STATUS_CHECK_VOTE) {
        batchErrLog.info({reason : 'DIFFERENT_PROPOSAL_STATUS', proposal, proposalFromNode});
        return updateVoting.remove();
    }
    let supplyAmount = SUtil.toAmount(stats.EOS.supply);
    const params = {
        watchman : watchmanAccount,
        proposal_id : proposal.proposal_id,
        total_votes : proposal.total_votes,
        agree_votes : proposal.agree_votes,
        disagree_votes : proposal.disagree_votes,
    };
    if (_checkVoting(supplyAmount, proposal, totalPercent, agreePercent) === true) {
        await eosApi.commitVote(params, key, watchmanAccount);
    } else  {
        await eosApi.collbackVote(params);
    }
    await updateVoting.remove();
}

function _checkVoting(supplyAmount, proposal, totalPercent, agreePercent) {
    const totalVotesRate = proposal.total_votes / supplyAmount * 100;
    if (totalVotesRate >= totalPercent) {
        if (proposal.disagree_votes === 0) {
            return true;
        }
        const agreeRate = (proposal.agree_votes - proposal.disagree_votes) / proposal.disagree_votes * 100;
        if (agreeRate >= agreePercent) {
            return true;
        } else {
            batchErrLog.info({reason : 'ROLLBACK_VOTES', proposal, supplyAmount});
            return false;
        }
    }
    return false;
}

module.exports = exports = checkVoting;

