#pragma once

#include <eosiolib/eosio.hpp>

#include <string>
#include <vector>

namespace eosiowps {
    using std::string;
    using std::vector;

    //@abi table
    struct reviewer {
        account_name owner;
        string first_name;
        string last_name;
        string committee;
        uint64_t primary_key() const { return owner; }
        EOSLIB_SERIALIZE( reviewer, (owner)(first_name)(last_name)(committee) )
    };

    typedef eosio::multi_index< N(reviewer), reviewer> reviewer_table;
} // eosiowps