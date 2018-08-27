'use strict';

describe('[batch]', function() {
    require('./update_votings')();
    require('./update_proposals')();
    require('./update_summary')();
});
