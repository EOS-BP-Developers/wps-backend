#pragma once

#include <eosiolib/eosio.hpp>

#include <string>
#include <vector>

namespace eosiowps {
    using std::string;
    using std::vector;

    //@abi table
    struct committee {
        account_name name;
        string category;
        uint64_t primary_key() const { return name; }
        EOSLIB_SERIALIZE( committee, (name)(category) );
    };

    typedef eosio::multi_index< N(committee), committee> committee_table;
} // eosiowps