'use strict';

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        TrxSchema = new Schema({
            trx_id : {type : String, index : true},
            block_num : {type : Number, index : true},
            account : {type : String, index : true},
            name : {type : String, index : true},
            authorization : [String],
            raw : Schema.Types.Mixed,
            created_at : {type : Date, default : Date.now}
        });

    return conn.model('LibAction', TrxSchema);
};
