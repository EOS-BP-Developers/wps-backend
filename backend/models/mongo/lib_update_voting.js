'use strict';

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        schema = new Schema({
            account : {type : String, index : true},
            proposal_id : {type : Number, index : true},
            action : {type : Number},
            is_agree : {type : Boolean}
        });
    schema.index({account : 1, proposal_id : 1});
    return conn.model('LibUpdateVoting', schema);
};
