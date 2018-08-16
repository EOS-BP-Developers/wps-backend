'use strict';

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        schema = new Schema({
            account : {type : String, index : true, unique : true},
            first_name : {type : String},
            last_name : {type : String},
            img_url : {type : String},
            bio : {type : String},
            country : {type : String},
            telegram : {type : String},
            website : {type : String},
            linkedin : {type : String},
            last_claim_time : {type : Number}
        });

    return conn.model('LibProposer', schema);
};
