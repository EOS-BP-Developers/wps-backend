'use strict';

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        schema = new Schema({
            system_account : {type : String},
            watchman_account : {type : String},
            watchman_prv : {type : String},
            watchman_pub : {type : String},
            total_voting_percent : {type : Number},
            agree_percent : {type : Number}
        }, {timestamps : true});
    return conn.model('WpsInfo', schema);
};
