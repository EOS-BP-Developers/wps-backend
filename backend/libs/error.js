'use strict';

module.exports = exports = {
    BAD_REQUEST : {error_code : 400, error_msg : 'Incorrect parameter'},
    UNAUTHORIZED : {error_code : 401, error_msg : 'Authentication failed. Please sign in again'},
    NOT_FOUND : {error_code : 404, error_msg : 'Not Found Error'},
    NOT_ACCEPTABLE : {error_code : 406, error_msg : 'Not Acceptable'},
    INTERNAL_SERVER_ERROR : {error_code : 500, error_msg : 'Internal Server Error'},
    DUPLICATE_ERROR : {error_code : 11000, error_msg : 'Duplicate Error'},
};
