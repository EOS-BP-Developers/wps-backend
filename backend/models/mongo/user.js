'use strict';

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        schema = new Schema({
            eos_account : {type : String, index : true, unique : true, minlength : 1, maxlength : 12},
            nick : {type : String, minlength : 1, maxlength : 32},
            img_url : {type : String, maxlength : 128},
            bio : {type : String, maxlength : 256},
            location : {type : String, maxlength : 50},
            telegram : {type : String, maxlength : 64},
            website : {type : String, maxlength : 128},
            linkedin : {type : String, maxlength : 128}
        }, {timestamps : true});
    return conn.model('User', schema);
};
