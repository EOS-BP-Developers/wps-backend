'use strict';

function unvote(proposal, votingInfo) {
    proposal.total_votes -= votingInfo.staked;
    if (votingInfo.is_agree === true) {
        proposal.agree_votes -= votingInfo.staked;
    } else {
        proposal.disagree_votes -= votingInfo.staked;
    }
}

module.exports = exports = unvote;
