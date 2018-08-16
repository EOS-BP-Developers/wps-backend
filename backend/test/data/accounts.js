'use strict';

const _ = require('lodash');

const user = {
    eos_account : 'useraaaaaaam',
    nick : 'yepps',
    img_url : 'https://i.amz.mshcdn.com/3NbrfEiECotKyhcUhgPJHbrL7zM=/950x534/filters:quality(90)/2014%2F06%2F02%2Fc0%2Fzuckheadsho.a33d0.jpg',
    bio : 'Rodney Erickson is a content marketing professional at HubSpot, an inbound marketing and sales platform that helps companies attract visitors, convert leads, and close customers. Previously, Rodney worked as a marketing manager for a tech software startup.',
    location : 'USA, WA',
    telegram : 'yepp4you',
    website : 'www.google.com',
    linkedin : 'yepp4you'
};

const eosAccounts = [
    {'name':'eosio.wps', 'pvt':'5JtUScZK2XEp3g9gh7F8bwtPTRAkASmNrrftmx4AxDKD5K4zDnr', 'pub':'EOS69X3383RzBZj41k73CSjUNXM5MYGpnDxyPnWUKPEtYQmTBWz4D'},
    {'name':'committeeaaa', 'pvt':'5JUNYmkJ5wVmtVY8x9A1KKzYe9UWLZ4Fq1hzGZxfwfzJB8jkw6u', 'pub':'EOS7yBtksm8Kkg85r4in4uCbfN77uRwe82apM8jjbhFVDgEgz3w8S'},
    {'name':'committeeaab', 'pvt':'5K6LU8aVpBq9vJsnpCvaHCcyYwzPPKXfDdyefYyAMMs3Qy42fUr', 'pub':'EOS7WnhaKwHpbSidYuh2DF1qAExTRUtPEdZCaZqt75cKcixuQUtdA'},
    {'name':'revieweraaaa', 'pvt':'5KdRpt1juJfbPEryZsQYxyNxSTkXTdqEiL4Yx9cAjdgApt4ANce', 'pub':'EOS7Bn1YDeZ18w2N9DU4KAJxZDt6hk3L7eUwFRAc1hb5bp6xJwxNV'},
    {'name':'revieweraaab', 'pvt':'5JRMbcnc35NkvxKTZUnoe3W4ENQCjhMUFwjN5jQmAqN9D7N6y3N', 'pub':'EOS6cNcTC6WTFkKV4C8DoxcTXdDTDKvj3vgZEVDGVFckK1eTNJQtf'},
    {'name':'proposeraaaa', 'pvt':'5HqyipkJSm5fwYhbhGC3vmmoBwabtgJSPecnvmN2mMrCTQfWBSS', 'pub':'EOS8UkmsnCo4GxDihbKwgoZY6f2QLSMEqBZ2frGLckxrCHrz15r7X'},
    {'name':'proposeraaab', 'pvt':'5KPr55J2UQNUh3xP5Q6ebqqV6MK5usrXxG4qqRfpaLieGa8VpCm', 'pub':'EOS8Smcv2eMoFcp1EQSBxcAeuBowSS9xesuHjhvTnK4AACjRycTVA'},
    {'name':'proposeraaac', 'pvt':'5JV9UNEpPKa4sqxSxvGWYPY9ZBTzAttyq7ShPvLUJSetwAeSXFW', 'pub':'EOS57VTWSiPyx45cSWGdGNtAZnmpqMrAvASQmL9hmXnoLNrgadwf7'},
    {'name':'proposeraaad', 'pvt':'5K4GSGP2r1Yu3RqmPZPF8Hv6Zrv2YWsUEoCqwwHxKsZavz2tChg', 'pub':'EOS5dt9CWCKM1scrWpFsRbzY71Up9UYFmJs1ySFKLJDGdYJmgEH3f'},
    {'name':'proposeraaae', 'pvt':'5K4MmsY7Th8DqjEY2vbM7npaxSQ56XzvNULkJeqKmbYoVRmPPpB', 'pub':'EOS8FdMPpPxpG5QAqGLncY5kBrEQ9NXPKCKnLH6oWDMPR8q8BrEmT'},
    {'name':'proposeraaaf', 'pvt':'5K4d3ck3e36DoLDQDAqE2uHE6X831RYS8Ac5Hdir4CmT7WbvQJB', 'pub':'EOS6iwndPo58Y2ihWshfhnFbEBJHGkZtujR1bn7bVLngnTWFA8Hm3'},
    {'name':'proposeraaag', 'pvt':'5KWg3urAyLF2tt1Rz8ckuK7QSiKU1CvXKhhzBPfFQcT83vjyCD2', 'pub':'EOS6QBgrm2h5f9B2RxLVXeD3HrchTUgJLtuYWPDQvi5T73enWgvVC'},
    {'name':'useraaaaaaam', 'pvt':'5HqgpWWpkRqfi3JSxsR3bsnVCvdgyf98msTRymZvQHTHNSHSXx2', 'pub':'EOS5LF56ZLEUAh3G9vRxbBHkFBZdx9F7bxmaARuiz54qvCAxT6Lmz'},
    {'name':'useraaaaaaan', 'pvt':'5HrEMEE5dbNL1VZPhpedt7MvKaoKP9iaMGwLw6uu3LS9CXnsyGx', 'pub':'EOS5vs19cixeVh2LJ61moQwTSgeGBVj8rZqjE36tAXXF4tFcGdCJw'}
];

const committees = [{
    committeeman : 'committeeaaa',
    category : 'emergency',
    is_oversight : 1
}, {
    committeeman : 'committeeaab',
    category : 'watchman',
    is_oversight : 1
}];

const reviewers = [{
    reviewer : 'revieweraaaa',
    committee : 'committeeaaa',
    first_name : 'Thomas',
    last_name : 'Do'
}, {
    reviewer : 'revieweraaab',
    committee : 'committeeaaa',
    first_name : 'Thomas',
    last_name : 'Cox'
}];

const proposers = [{
    account : 'proposeraaaa',
    first_name : 'Thomas',
    last_name : 'Do',
    img_url : 'http://www.google.com',
    bio : 'hi~',
    country : 'KR',
    telegram : '@yepp4you',
    website : 'http://www.block.one',
    linkedin : 'thomas-do-01911516a',
}, {
    account : 'proposeraaab',
    first_name : 'Thomas',
    last_name : 'Do',
    img_url : 'http://www.google.com',
    bio : 'hi~',
    country : 'KR',
    telegram : '@yepp4you',
    website : 'http://www.block.one',
    linkedin : 'thomas-do-01911516a',
}, {
    account : 'proposeraaac',
    first_name : 'Thomas',
    last_name : 'Do',
    img_url : 'http://www.google.com',
    bio : 'hi~',
    country : 'KR',
    telegram : '@yepp4you',
    website : 'http://www.block.one',
    linkedin : 'thomas-do-01911516a',
}, {
    account : 'proposeraaad',
    first_name : 'Thomas',
    last_name : 'Do',
    img_url : 'http://www.google.com',
    bio : 'hi~',
    country : 'KR',
    telegram : '@yepp4you',
    website : 'http://www.block.one',
    linkedin : 'thomas-do-01911516a',
}, {
    account : 'proposeraaae',
    first_name : 'Thomas',
    last_name : 'Do',
    img_url : 'http://www.google.com',
    bio : 'hi~',
    country : 'KR',
    telegram : '@yepp4you',
    website : 'http://www.block.one',
    linkedin : 'thomas-do-01911516a',
}, {
    account : 'proposeraaaf',
    first_name : 'Thomas',
    last_name : 'Do',
    img_url : 'http://www.google.com',
    bio : 'hi~',
    country : 'KR',
    telegram : '@yepp4you',
    website : 'http://www.block.one',
    linkedin : 'thomas-do-01911516a',
}, {
    account : 'proposeraaag',
    first_name : 'Thomas',
    last_name : 'Do',
    img_url : 'http://www.google.com',
    bio : 'hi~',
    country : 'KR',
    telegram : '@yepp4you',
    website : 'http://www.block.one',
    linkedin : 'thomas-do-01911516a',
}];

const proposals = _.map(proposers, (proposer) => {
    return {
        proposer : proposer.account,
        committee : 'committeeaaa',
        subcategory : 1,
        title : `wps project title ${proposer}`,
        summary : 'wps proejct summary',
        project_img_url : 'http://www.google.com',
        description : 'zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz',
        roadmap : 'zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz',
        duration : 30,
        members : ['yepp4you1', 'yepp4you2', 'yepp4you3'],
        funding_goal : '10.0000 EOS'
    };
});

module.exports = exports = {user, eosAccounts, committees, reviewers, proposers, proposals};
