'use strict';

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        schema = new Schema({
            proposal_id : {type : Number, index : true},
            account : {type : String, index : true},
            staked : {type : Number},
            is_agree : {type : Boolean}
        });

    schema.index({proposal_id : 1, account : 1}, {unique: true});

    return conn.model('LibVotingInfo', schema);
};
