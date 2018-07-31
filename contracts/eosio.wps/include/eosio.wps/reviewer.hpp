#pragma once

#include <eosiolib/eosio.hpp>

#include <string>
#include <vector>

namespace eosiowps {
    using std::string;
    using std::vector;

    //@abi table
    struct reviewer {
        account_name account;
        account_name committee;
        string first_name;
        string last_name;
        uint64_t primary_key() const { return account; }
        EOSLIB_SERIALIZE( reviewer, (account)(first_name)(last_name)(committee) )
    };

    typedef eosio::multi_index< N(reviewer), reviewer> reviewer_table;
} // eosiowps