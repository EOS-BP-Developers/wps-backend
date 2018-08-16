'use strict';

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        schema = new Schema({
            block_id : {type : String},            //
            block_num : {type : Number},           //
            total_voters : {type : Number},        //
            total_projects : {type : Number},      //
            funded_projects : {type : Number},     //
            ongoing_projects : {type : Number}     //
        });
    return conn.model('LibSummary', schema);
};
