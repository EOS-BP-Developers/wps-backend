'use strict';

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        schema = new Schema({
            proposer : {type : String, index : true},
            proposal_id : {type : Number, index : true}
        });
    return conn.model('LibUpdateProposal', schema);
};
