'use strict';

var fs = require('fs'),
    path = require('path');

var dummy = {};

fs.readdirSync(__dirname).filter(function(file) {
    return (file.indexOf('.') !== 0) && (file !== 'index.js');
})
.forEach(function(file) {
    dummy = Object.assign({}, dummy, require(path.join(__dirname, file)));
});

module.exports = dummy;
