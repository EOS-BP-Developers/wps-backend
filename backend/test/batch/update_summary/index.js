'use strict';

module.exports = exports = function() {
    describe('update_summary', () => {
        const sinon = require('sinon'),
            SEnum = require('libs/enum'),
            mongo = require('models/mongo');

        require('sinon-mongoose');
        require('chai').should();

        const updateSummary = require('libs/batch/update_summary').updateSummary;

        const Summary = mongo.LibSummary;
        const Proposal = mongo.LibProposal;
        const VotingInfo = mongo.LibVotingInfo;
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
        const summary = {
            block_id : '5b7bc8ec9b53c169b2caa48c',
            block_num : 100,
            total_voters : 1000,
            total_proposals : 20,
            funded_proposals : 10,
            ongoing_proposals : 10,
        };

        const statusCnts = [{ _id: 1, count: 12 }, { _id: 3, count: 5 }];

        const sandbox = sinon.createSandbox();
        describe('vote', () => {
            beforeEach(() => {
                sinon.mock(Summary)
                    .expects('findOne')
                    .atLeast(1)
                    .atMost(10)
                    .resolves(Object.assign({}, summary));

                sinon.mock(Proposal)
                    .expects('aggregate')
                    .atLeast(1)
                    .atMost(10)
                    .resolves(Object.assign({}, statusCnts));

                sinon.mock(VotingInfo)
                    .expects('distinct')
                    .withArgs('account')
                    .chain('countDocuments')
                    .atLeast(1)
                    .atMost(10)
                    .resolves(101);
            });

            afterEach(() => {
                sinon.restore();
                sandbox.restore();
            });

            it('update', async function() {
                sandbox.replace(Summary, 'updateOne', async function(condition, data) {
                    data.$set.should.have.own.property('total_voters');
                    data.$set.should.have.own.property('total_proposals');
                    data.$set.should.have.own.property('funded_proposals');
                    data.$set.should.have.own.property('ongoing_proposals');
                });

                await updateSummary();
            });
        });
    });
};
