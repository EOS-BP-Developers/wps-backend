'use strict';

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        schema = new Schema({
            account : {type : String, index : true},
            staked : {type : Number},
        }, {timestamps : true});

    return conn.model('LibVoterInfo', schema);
};
