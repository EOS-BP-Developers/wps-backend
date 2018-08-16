'use strict';

let Promise = require('bluebird'),
    chai = require('chai'),
    app = require('app');

let request = Promise.promisifyAll(require('supertest'));
chai.should();

/*
router.get('/1/summary', getSummary);
*/

module.exports = exports = function() {
    it('get /api/1/summary should receive summary', function() {
        return request(app)
            .get('/api/1/summary')
            .endAsync().then(function(res) {
                res.body.should.not.have.property('error');
                res.body.should.have.property('summary');
                console.log(res.body.summary);
            });
    });
};
