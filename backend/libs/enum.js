'use strict';

module.exports = {
    VOTE_ACTION_VOTE : 1,
    VOTE_ACTION_UNVOTE : 2,
    VOTE_ACTION_STAKE : 3,
    VOTE_ACTION_UNSTAKE : 4,
    VOTE_ACTION_CHECK : 5,

    PROPOSAL_STATUS_PENDING : 1,
    PROPOSAL_STATUS_REJECTED : 2,
    PROPOSAL_STATUS_ON_VOTE : 3,
    PROPOSAL_STATUS_CHECK_VOTE : 4,     // check count of votes
    PROPOSAL_STATUS_CHECKED_VOTE : 5,   // checked count of votes by platform
    PROPOSAL_STATUS_APPROVED : 6,       // approve
    PROPOSAL_STATUS_COMPLETED : 7,

    // Redis Type
    REDIS_TYPE_BATCH_STATUS : 1,
    REDIS_TYPE_BATCH_CMD : 2,
    REDIS_TYPE_BATCH_CMD_ALL : 3,
};
