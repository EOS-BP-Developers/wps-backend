'use strict';

let useRoutes = function(app) {
    const committeeRouter = require('./committee');
    const proposerRouter = require('./proposer');
    const proposalRouter = require('./proposal');
    const userRouter = require('./user');
    const statsRouter = require('./stats');

    app.use('/api/', committeeRouter);
    app.use('/api/', proposerRouter);
    app.use('/api/', proposalRouter);
    app.use('/api/', userRouter);
    app.use('/api/', statsRouter);
};

module.exports = exports = {
    useRoutes : useRoutes
};
