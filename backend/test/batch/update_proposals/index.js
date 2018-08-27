'use strict';

module.exports = exports = function() {
    describe('update_proposals', () => {
        const sinon = require('sinon'),
            SEnum = require('libs/enum'),
            mongo = require('models/mongo'),
            eosApi = require('external_apis/eos_api');

        require('sinon-mongoose');
        require('chai').should();

        const updateProposals = require('libs/batch/update_proposals').updateProposals;

        const Proposal = mongo.LibProposal;
        const UpdateProposal = mongo.LibUpdateProposal;
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
        const proposal = {
            _id : '5b7bc8ec9b53c169b2caa48c',
            proposer: 'xxxxxxxxxxx',
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
            status: SEnum.PROPOSAL_STATUS_PENDING,
            vote_start_time: 0,
            fund_start_time: 0,
            iteration_of_funding: 0
        };

        const proposalFromEOS = {
            proposer: 'xxxxxxxxxxx',
            id : 1,
            committee: 'committeeaaa',
            category: 'emergency',
            subcategory: 1,
            title: 'wps project title from EOS',
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

        const sandbox = sinon.createSandbox();
        describe('vote', () => {
            beforeEach(() => {
                sinon.mock(Proposal)
                    .expects('findOne')
                    .atLeast(1)
                    .atMost(10)
                    .withArgs({proposal_id : 1, status : 2})
                    .resolves(Object.assign({}, proposal));

                sandbox.replace(eosApi, 'getInfo', async function() {
                    return chainInfo;
                });

                sandbox.replace(eosApi, 'getProposalByOwner', async function(proposer) {
                    return Object.assign({}, proposalFromEOS);
                });

                sandbox.replace(eosApi, 'getProposalById', async function(proposalId) {
                    return Object.assign({}, proposalFromEOS);
                });

                sandbox.replace(eosApi, 'getRejectedProposalById', async function(proposalId) {
                    return Object.assign({}, proposalFromEOS);
                });

                sandbox.replace(eosApi, 'getFinishedProposalById', async function(proposalId) {
                    return Object.assign({}, proposalFromEOS);
                });

                sandbox.replace(eosApi, 'checkExpire', async function(params, key, authorization, endpoint) {
                    return {};
                });
            });

            afterEach(() => {
                sinon.restore();
                sandbox.restore();
            });

            it('create', async function() {
                sandbox.replace(Proposal, 'updateOne', async function(condition, data) {
                    // console.log(data);
                    // The following values must be deleted.
                    data.$set.should.have.not.own.property('total_votes');
                    data.$set.should.have.not.own.property('agree_votes');
                    data.$set.should.have.not.own.property('disagree_votes');
                });

                sinon.mock(UpdateProposal)
                    .expects('find')
                    .atLeast(1)
                    .chain('limit', 30)
                    .atLeast(1)
                    .chain('sort', '-id')
                    .atLeast(1)
                    .resolves([
                        {proposer : 'xxxxxxxxxxxx', proposal_id : 0, remove : async function() {}}
                    ]);

                await updateProposals();
            });

            it('update', async function() {
                sandbox.replace(Proposal, 'updateOne', async function(condition, data) {
                    // console.log(data);
                    // The following values must be deleted.
                    data.$set.should.have.not.own.property('total_votes');
                    data.$set.should.have.not.own.property('agree_votes');
                    data.$set.should.have.not.own.property('disagree_votes');
                });

                sinon.mock(UpdateProposal)
                    .expects('find')
                    .atLeast(1)
                    .chain('limit', 30)
                    .atLeast(1)
                    .chain('sort', '-id')
                    .atLeast(1)
                    .resolves([
                        {proposal_id : 1, remove : async function() {}}
                    ]);

                await updateProposals();
            });

        });
    });
};
