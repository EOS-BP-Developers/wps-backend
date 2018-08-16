'use strict';

const _ = require('lodash'),
    mongo = require('models/mongo');

const VotingInfo = mongo.LibVotingInfo;

async function vote(proposal, updateVoting, votingInfo, voterStaked) {
    if (_.isEmpty(votingInfo)) {
        await VotingInfo.create({proposal_id : updateVoting.proposal_id, account : updateVoting.account, staked : voterStaked, is_agree : updateVoting.is_agree});
        proposal.total_votes += voterStaked;
        if (updateVoting.is_agree === true) {
            proposal.agree_votes += voterStaked;
        } else {
            proposal.disagree_votes += voterStaked;
        }
    } else {
        proposal.total_votes -= votingInfo.staked;
        proposal.total_votes += voterStaked;
        if (votingInfo.is_agree === true) {
            if (updateVoting.is_agree === true) {
                proposal.agree_votes -= votingInfo.staked;
                proposal.agree_votes += voterStaked;
            } else {
                proposal.agree_votes -= votingInfo.staked;
                proposal.disagree_votes += voterStaked;
            }
        } else {
            if (updateVoting.is_agree === false) {
                proposal.disagree_votes -= votingInfo.staked;
                proposal.disagree_votes += voterStaked;
            } else {
                proposal.disagree_votes -= votingInfo.staked;
                proposal.agree_votes += voterStaked;
            }
        }
    }
}

module.exports = exports = vote;
