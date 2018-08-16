'use strict';

let Promise = require('bluebird'),
    chai = require('chai'),
    app = require('app');

let request = Promise.promisifyAll(require('supertest'));
chai.should();

/*
router.get('/1/committees', getCommittees);
*/

module.exports = exports = function() {
    it('get /api/1/committees should receive committees', function() {
        return request(app)
            .get('/api/1/committees')
            .endAsync().then(function(res) {
                res.body.should.not.have.property('error');
                res.body.should.have.property('committees');
                console.log(res.body.committees);
            });
    });
};
