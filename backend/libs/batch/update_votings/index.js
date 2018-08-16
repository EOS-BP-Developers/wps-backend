'use strict';

const Promise = require('bluebird'),
    _ = require('lodash'),
    eosApi = require('external_apis/eos_api'),
    mongo = require('models/mongo'),
    SEnum = require('libs/enum'),
    log = require('libs/log'),
    checkVoting = require('./check_voting'),
    vote = require('./vote'),
    unvote = require('./unvote'),
    stakeunstake = require('./stakeunstake');

const batchErrLog = log.batchErrLog;

const UpdateVoting = mongo.LibUpdateVoting;
const Proposal = mongo.LibProposal;
const VotingInfo = mongo.LibVotingInfo;
const VoterInfo = mongo.LibVoterInfo;

async function updateVotings() {
    // console.log('updateVotings');
    const chainInfo = await eosApi.getInfo();
    if (_.isEmpty(chainInfo)) {
        batchErrLog.info({reason : 'CANNOT_GET_CHAIN_INFO'});
        return;
    }
    let updateVotings = await UpdateVoting.find().limit(20).sort({_id : 1});
    if (!_.isEmpty(updateVotings)) {
        return Promise.each(updateVotings, function(updateVoting) {
            return _updateVoting(updateVoting)
                .catch((err) => {
                    err.updateVoting = updateVoting;
                    batchErrLog.info(err);
                    throw err;
                });
        });
    }
}

async function _updateVoting(updateVoting) {
    if (updateVoting.action === SEnum.VOTE_ACTION_CHECK) {
        return checkVoting(updateVoting);
    }
    let votingInfo = await VotingInfo.findOne({account : updateVoting.account, proposal_id : updateVoting.proposal_id});
    if (_.isEmpty(votingInfo) && updateVoting.action !== SEnum.VOTE_ACTION_VOTE) {
        return updateVoting.remove();
    }
    let voter = await eosApi.getVoterInfo(updateVoting.account);
    let proposal = await Proposal.findOne({proposal_id : updateVoting.proposal_id});
    if (_.isEmpty(voter) || _.isEmpty(proposal)) {
        return updateVoting.remove();
    }
    const voterStaked = parseInt(voter.staked, 10);
    if (updateVoting.action === SEnum.VOTE_ACTION_VOTE) {
        await vote(proposal, updateVoting, votingInfo, voterStaked);
    } else if (updateVoting.action === SEnum.VOTE_ACTION_UNVOTE) {
        unvote(proposal, votingInfo);
        await votingInfo.remove();
        votingInfo = null;
    } else if (updateVoting.action === SEnum.VOTE_ACTION_STAKE || updateVoting.action === SEnum.VOTE_ACTION_UNSTAKE) {
        stakeunstake(proposal, votingInfo, voterStaked);
    }
    if (!_.isEmpty(votingInfo)) {
        votingInfo.staked = voterStaked;
        votingInfo.is_agree = updateVoting.is_agree;
        await votingInfo.save();
    }
    await proposal.save();
    await updateVoting.remove();

    await VoterInfo.create({account : updateVoting.account, staked : voterStaked});
}

module.exports = exports = {updateVotings};
