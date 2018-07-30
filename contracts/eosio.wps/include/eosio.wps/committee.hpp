#pragma once

#include <eosiolib/eosio.hpp>

#include <string>
#include <vector>

namespace eosiowps {
    using std::string;
    using std::vector;

    //@abi table
    struct committee {
        account_name owner;
        string category;
        uint64_t primary_key() const { return owner; }
        EOSLIB_SERIALIZE( committee, (owner)(category) );
    };

    typedef eosio::multi_index< N(committee), committee> committee_table;
} // eosiowps