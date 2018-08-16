'use strict';

function stakeunstake(proposal, votingInfo, voterStaked) {
    proposal.total_votes -= votingInfo.staked;
    proposal.total_votes += voterStaked;
    if (votingInfo.is_agree === true) {
        proposal.agree_votes -= votingInfo.staked;
        proposal.agree_votes += voterStaked;
    } else {
        proposal.disagree_votes -= votingInfo.staked;
        proposal.disagree_votes += voterStaked;
    }
}

module.exports = exports = stakeunstake;
