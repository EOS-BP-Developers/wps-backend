'use strict';

const _ = require('lodash'),
    sinon = require('sinon'),
    SEnum = require('libs/enum'),
    SUtil = require('libs/util'),
    mongo = require('models/mongo'),
    eosApi = require('external_apis/eos_api'),
    data = require('test/data');

require('sinon-mongoose');
require('chai').should();

const updateVotings = require('libs/batch/update_votings').updateVotings;

const UpdateVoting = mongo.LibUpdateVoting;

const WpsInfo = mongo.WpsInfo;

const Proposal = mongo.LibProposal;
const VoterInfo = mongo.LibVoterInfo;


function getVoters() {
    return _.map(data.eosAccounts, (account) => {
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
    PROPOSAL_STATUS_FINISHED_VOTING : 4,
    PROPOSAL_STATUS_CHECK_VOTE : 5,     // check count of votes
    PROPOSAL_STATUS_CHECKED_VOTE : 6,   // checked count of votes by platform
    PROPOSAL_STATUS_APPROVED : 7,       // approve
    PROPOSAL_STATUS_COMPLETED : 8,
*/

module.exports = exports = function() {
    const proposal = {
        _id : '5b7bc8ec9b53c169b2caa48c',
        proposer: 'xxxxxxxxxxx',
        id : 1,
        proposal_id : 1,
        committee: 'committeeaaa',
        category: 'emergency',
        subcategory: 1,
        title: 'wps project title',
        summary: 'wps proejct summary',
        project_img_url: 'http://www.google.com',
        description:
           'zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz',
        roadmap:
           'zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz',
        duration: 30,
        members: [ 'yepp4you1', 'yepp4you2', 'yepp4you3' ],
        funding_goal: '10.0000 EOS',
        total_votes:   10000000,
        agree_votes:    5200000,
        disagree_votes: 4800000,
        status: SEnum.PROPOSAL_STATUS_CHECK_VOTE,
        vote_start_time: 1534733451,
        fund_start_time: 0,
        iteration_of_funding: 0
    };
    const chainInfo = {
        server_version: 'bf28f8bb',
        chain_id: 'a4fe43c897279a677025624555f3835b949f995f87923c97f0392dcff835bfd0',
        head_block_num: 208607,
        last_irreversible_block_num: 208271,
        last_irreversible_block_id: '00032d8fd0bb3ca9d0b46dec6be826d2d57b06ee569abebcfbb4ffda105624de',
        head_block_id: '00032edf764f42f9f30dfce850ef60ac46a1b0c14eb90dd595da457c1bc14e8c',
        head_block_time: '2018-08-22T09:40:17.500',
        head_block_producer: 'producer111h',
        virtual_block_cpu_limit: 200000000,
        virtual_block_net_limit: 1048576000,
        block_cpu_limit: 199900,
        block_net_limit: 1048576,
        server_version_string: 'v1.2.1-dirty'
    };
    const stats = { EOS:
        { supply: '20000.0000 EOS', max_supply: '100000.0000 EOS', issuer: 'eosio' }
        // { supply: '1000004050.0000 EOS', max_supply: '10000000000.0000 EOS', issuer: 'eosio' }
    };

    const wpsInfo = {
        watchman_account : 'committeeaab',
        watchman_prv : '5K6LU8aVpBq9vJsnpCvaHCcyYwzPPKXfDdyefYyAMMs3Qy42fUr',
        watchman_pub : 'EOS7WnhaKwHpbSidYuh2DF1qAExTRUtPEdZCaZqt75cKcixuQUtdA',
        total_voting_percent : 5,
        agree_percent : 10
    };

    const sandbox = sinon.createSandbox();
    describe('watchman', () => {
        beforeEach(() => {
            /*
            const testObj = {
                limit : function() {return this;},
                sort : async function() {return [{proposal_id : 1, action : SEnum.VOTE_ACTION_CHECK, remove : async function() {} }];}
            };
            sandbox.mock(UpdateVoting)
                .expects('find')
                .returns(testObj);
                // .callsFake(function() {return testObj;});
            */
            sinon.mock(UpdateVoting)
                .expects('find')
                .atLeast(1)
                .chain('limit', 30)
                .atLeast(1)
                .chain('sort', '-id')
                .atLeast(1)
                .resolves([{proposal_id : 1, action : SEnum.VOTE_ACTION_CHECK, remove : async function() {} }]);

            sinon.mock(Proposal)
                .expects('findOne')
                .atLeast(1)
                .atMost(10)
                .withArgs({proposal_id : 1})
                .resolves(proposal);

            sinon.mock(WpsInfo)
                .expects('findOne')
                .atLeast(1)
                .atMost(10)
                .resolves(wpsInfo);

            sinon.mock(VoterInfo)
                .expects('create')
                .never()
                .resolves({});

            sandbox.replace(eosApi, 'getInfo', async function() {
                return chainInfo;
            });

            sandbox.replace(eosApi, 'getCurrencyStats', async function() {
                return stats;
            });

            sandbox.replace(eosApi, 'getVoterInfo', async function(voter) {
                const voters = getVoters();
                return _.find(voters, {owner : voter});
            });

            sandbox.replace(eosApi, 'getProposalByOwner', async function(proposer) {
                return proposal;
            });

            sandbox.replace(eosApi, 'getProposalById', async function(proposalId) {
                return proposal;
            });

            sandbox.replace(eosApi, 'getRejectedProposalById', async function(proposalId) {
                return proposal;
            });

            sandbox.replace(eosApi, 'getFinishedProposalById', async function(proposalId) {
                return proposal;
            });

            sandbox.replace(eosApi, 'checkExpire', async function(params, key, authorization, endpoint) {
                return {};
            });
        });

        afterEach(() => {
            sinon.restore();
            sandbox.restore();
        });

        it('commitVote', async function() {
            proposal.total_votes = 10000000;
            proposal.agree_votes = 5500000;
            proposal.disagree_votes = 4500000;

            sandbox.mock(eosApi)
                .expects('commitVote')
                .once()
                .resolves({});

            sandbox.mock(eosApi)
                .expects('rollbackVote')
                .never()
                .resolves({});

            await updateVotings();
        });

        it('rollbackVote', async function() {
            proposal.total_votes = 10000000;
            proposal.agree_votes = 5200000;
            proposal.disagree_votes = 4800000;

            sandbox.mock(eosApi)
                .expects('commitVote')
                .never()
                .resolves({});

            sandbox.mock(eosApi)
                .expects('rollbackVote')
                .once()
                .resolves({});

            await updateVotings();
        });

        it.skip('expire', async function() {
            // updateVotings();
        });
    });
};

