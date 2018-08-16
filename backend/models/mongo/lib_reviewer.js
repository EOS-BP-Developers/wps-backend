'use strict';

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        schema = new Schema({
            account : {type : String, index : true, unique : true},
            committee : {type : String},
            first_name : {type : String},
            last_name : {type : String}
        });

    return conn.model('LibReviewer', schema);
};
