'use strict';

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        schema = new Schema({
            block_id : {type : String},                         //
            block_num : {type : Number},                        //
            total_voters : {type : Number, default : 0},        //
            total_proposals : {type : Number, default : 0},     //
            funded_proposals : {type : Number, default : 0},    //
            ongoing_proposals : {type : Number, default : 0}    //
        });
    return conn.model('LibSummary', schema);
};
