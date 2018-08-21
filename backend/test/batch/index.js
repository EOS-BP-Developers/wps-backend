'use strict';

const Promise = require('bluebird'),
    _ = require('lodash'),
    sinon = require('sinon'),
    SEnum = require('libs/enum'),
    SUtil = require('libs/util'),
    mongo = require('models/mongo'),
    eosApi = require('external_apis/eos_api'),
    data = require('test/data');

const updateProposals = require('libs/batch/update_proposals').updateProposals;
const updateVotings = require('libs/batch/update_votings').updateVotings;
const updateSummary = require('libs/batch/update_summary').updateSummary;

const Proposal = mongo.LibProposal;
const UpdateProposal = mongo.LibUpdateProposal;
const UpdateVoting = mongo.LibUpdateVoting;

const WpsInfo = mongo.WpsInfo;
const VotingInfo = mongo.LibVotingInfo;
const VoterInfo = mongo.LibVoterInfo;

require('chai').should();

async function makeUpdateProposals() {
    /*
    UpdateProposal.create({proposer : data.proposer, proposal_id : 0});
    UpdateProposal.create({proposal_id : 0});
    */
    return Promise.each(data.proposals, async function(proposal, index) {
        await Proposal.create(proposal);
        await UpdateProposal.create({proposer : proposal.proposer, proposal_id : 0});

        await UpdateProposal.create({proposal_id : index + 1});
    });
}

async function makeUpdateVotings() {
/*
    VOTE_ACTION_VOTE : 1,
    VOTE_ACTION_UNVOTE : 2,
    VOTE_ACTION_STAKE : 3,
    VOTE_ACTION_UNSTAKE : 4,
    VOTE_ACTION_CHECK : 5,
*/
/*
    UpdateVoting.create({account : data.receiver, action : SEnum.VOTE_ACTION_STAKE});
    UpdateVoting.create({account : data.receiver, action : SEnum.VOTE_ACTION_UNSTAKE});

    UpdateVoting.create({account : data.voter, proposal_id : data.proposal_id, action : SEnum.VOTE_ACTION_VOTE, is_agree : true});
    UpdateVoting.create({account : data.voter, proposal_id : data.proposal_id, action : SEnum.VOTE_ACTION_VOTE, is_agree : false});
    UpdateVoting.create({account : data.voter, proposal_id : data.proposal_id, action : SEnum.VOTE_ACTION_UNVOTE});
    UpdateVoting.create({account : data.voter, proposal_id : data.proposal_id, action : SEnum.VOTE_ACTION_CHECK});
*/
    const accounts = data.eosAccounts;
    return Promise.each(data.proposals, (proposal, index) => {
        return Promise.each(accounts, (account, idx) => {
            if (idx > 80) {
                return UpdateVoting.create({account : account.name, proposal_id : index + 1, action : SEnum.VOTE_ACTION_VOTE, is_agree : true});
            }
            return UpdateVoting.create({account : account.name, proposal_id : index + 1, action : SEnum.VOTE_ACTION_VOTE, is_agree : false});
        });
    })
    .then(function() {
        return Promise.each(accounts, (account, idx) => {
            if (idx > 50) {
                return UpdateVoting.create({account : account.name, action : SEnum.VOTE_ACTION_STAKE});
            }
            return UpdateVoting.create({account : account.name, action : SEnum.VOTE_ACTION_UNSTAKE});
        });
    })
    .then(function() {
        return Promise.each(data.proposals, (proposal, index) => {
            return Promise.each(accounts, (account, idx) => {
                if (idx > 90) {
                    return UpdateVoting.create({account : account.name, proposal_id : index + 1, action : SEnum.VOTE_ACTION_VOTE, is_agree : true});
                }
            });
        });
    });
}

function getVoters() {
    return _.map(data.acccounts, (account) => {
        return { owner: account.name,
            proxy: '',
            producers: [ 'producer1111', 'producer1112', 'producer1113', 'producer111j', 'producer111k', 'producer111l', 'producer111m', 'producer111n', 'producer111o', 'producer111p', 'producer111q',
                'producer111r', 'producer111s', 'producer111t', 'producer111u', 'producer111v', 'producer111w', 'producer111x', 'producer111y', 'producer111z'
            ],
            staked: SUtil.toAmount(account.staked),
            last_vote_weight: 0,
            proxied_vote_weight: 0.00000000000000000,
        };
    });
}


/*
    PROPOSAL_STATUS_PENDING : 1,
    PROPOSAL_STATUS_REJECTED : 2,
    PROPOSAL_STATUS_ON_VOTE : 3,
    PROPOSAL_STATUS_CHECK_VOTE : 4,     // check count of votes
    PROPOSAL_STATUS_CHECKED_VOTE : 5,   // checked count of votes by platform
    PROPOSAL_STATUS_APPROVED : 6,       // approve
    PROPOSAL_STATUS_COMPLETED : 7,
*/
function getProposals() {
    return _.map(data.proposals, (proposal, index) => {
        let status = 1;
        if (index < 7) {
            status = index + 1;
        }
        return {
            proposer: proposal.proposer,
            id : index + 1,
            committee: 'committeeaaa',
            category: 'emergency',
            subcategory: 1,
            title: `wps project title ${proposal.proposer}`,
            summary: 'wps proejct summary',
            project_img_url: 'http://www.google.com',
            description:
               'zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz',
            roadmap:
               'zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz',
            duration: 30,
            members: [ 'yepp4you1', 'yepp4you2', 'yepp4you3' ],
            funding_goal: '10.0000 EOS',
            total_votes: 0,
            agree_votes: 0,
            disagree_votes: 0,
            status: status,
            vote_start_time: 1534733451,
            fund_start_time: 0,
            iteration_of_funding: 0 };
    });
}

const sandbox = sinon.createSandbox();
describe('watchman commitvote', () => {
    before(() => {
        sandbox.replace(eosApi, 'getInfo', async function() {
            return {
                block_num : 10
            };
        });
        sandbox.replace(eosApi, 'getVoterInfo', async function(voter) {
            const voters = getVoters();
            return _.find(voters, {owner : voter});
        });
        sandbox.replace(eosApi, 'getProposalByOwner', async function(proposer) {
            const proposals = getProposals(SEnum);
            return _.find(proposals, {proposer : proposer});
        });
        sandbox.replace(eosApi, 'getProposalById', async function(proposalId) {
            const proposals = getProposals(SEnum);
            return _.find(proposals, {id : proposalId});
        });
        sandbox.replace(eosApi, 'getRejectedProposalById', async function(proposalId) {
            const proposals = getProposals(SEnum);
            return _.find(proposals, {id : proposalId, status : 2});
        });
        sandbox.replace(eosApi, 'getFinishedProposalById', async function(proposalId) {
            const proposals = getProposals(SEnum);
            return _.find(proposals, {id : proposalId, status : 7});
        });
    });
    after(() => {
        sandbox.restore();
    });

    it('watchman commitvote', async function() {
        return updateVotings();
    });
});

/*
describe('[batch]', function() {
    beforeEach(() => {
        return makeUpdateProposals()
            .then(() => {
                return makeUpdateVotings();
            });
    });

    afterEach(() => {
        const models = _.values(mongo);
        return Promise.each(models, (model) => {
            return model.deleteAll({});
        });
    });

    it('watchman commitvote', async function() {
    return updateSummary()
        .then(function(result) {
            console.log(result);
        });

    it('watchman rollbackvote', async function() {
    });
});
*/

