'use strict';

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        schema = new Schema({
            trx_id : {type : String, index : true},
            from : {type : String, index : true},
            receiver : {type : String},
            unstake_net_quantity : {type : String},
            unstake_cpu_quantity : {type : String},
            created_at : {type : Date, default : Date.now}
        });

    return conn.model('LibActionUnstake', schema);
};
