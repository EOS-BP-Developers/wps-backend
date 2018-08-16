'use strict';

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        schema = new Schema({
            committeeman : {type : String, index : true, unique : true},
            category : {type : String},
            is_oversight : {type : Boolean},
        });

    return conn.model('LibCommittee', schema);
};
