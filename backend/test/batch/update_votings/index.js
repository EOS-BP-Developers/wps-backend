'use strict';

module.exports = exports = function() {
    describe('update_votings', () => {
        require('./check_voting')();
        require('./stakeunstake')();
        require('./vote')();
        require('./unvote')();
    });
};
