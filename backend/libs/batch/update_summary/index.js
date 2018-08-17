'use strict';

const _ = require('lodash'),
    mongo = require('models/mongo'),
    SEnum = require('libs/enum');
//  log = require('libs/log');
//  const errLog = log.errLog;
//  const batchLog = log.batchLog;

const Summary = mongo.LibSummary;
const Proposal = mongo.LibProposal;
const VotingInfo = mongo.LibVotingInfo;

/*
    PROPOSAL_STATUS_PENDING : 1,
    PROPOSAL_STATUS_REJECTED : 2,
    PROPOSAL_STATUS_ON_VOTE : 3,
    PROPOSAL_STATUS_CHECK_VOTE : 4,     // check count of votes
    PROPOSAL_STATUS_CHECKED_VOTE : 5,   // checked count of votes by platform
    PROPOSAL_STATUS_APPROVED : 6,       // approve
    PROPOSAL_STATUS_COMPLETED : 7
*/

async function updateSummary() {
    // console.log('updateSummary');
    const summary = await Summary.findOne();
    if (_.isEmpty(summary)) {
        return;
    }
    const statusCnts = await Proposal.aggregate([{
        $group : {
            _id : '$status',
            count : {$sum : 1}
        }
    }]);

    const summaryObj = {
        total_proposals : 0,
        funded_proposals : 0,
        ongoing_proposals : 0,
        total_voters : 0
    };
    _.forEach(statusCnts, function(status) {
        if (status._id !== SEnum.PROPOSAL_STATUS_REJECTED) {
            summaryObj.total_projects += status.count;
        }
        if (status._id === SEnum.PROPOSAL_STATUS_COMPLETED) {
            summaryObj.funded_projects += status.count;
        }
        if (status._id === SEnum.PROPOSAL_STATUS_PENDING || status._id === SEnum.PROPOSAL_STATUS_ON_VOTE ||
            status._id === SEnum.PROPOSAL_STATUS_CHECK_VOTE || status._id === SEnum.PROPOSAL_STATUS_CHECKED_VOTE ||
            status._id === SEnum.PROPOSAL_STATUS_APPROVED) {
            summaryObj.ongoing_projects += status.count;
        }
    });
    summaryObj.total_voters = await VotingInfo.distinct('account').countDocuments();
    return Summary.update({_id : summary._id}, {$set : summaryObj});
}

module.exports = exports = {updateSummary};
