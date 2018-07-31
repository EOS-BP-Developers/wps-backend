#pragma once

#include <eosiolib/eosio.hpp>

#include <string>
#include <vector>
//#include <stdbool.h>

namespace eosiowps {
    using std::string;
    using std::vector;

    //@abi table
    struct committee {
        account_name committeeman;
        string category;
        bool is_oversight;
        uint64_t primary_key() const { return committeeman; }
        EOSLIB_SERIALIZE( committee, (committeeman)(category) );
    };

    typedef eosio::multi_index< N(committee), committee> committee_table;
} // eosiowps