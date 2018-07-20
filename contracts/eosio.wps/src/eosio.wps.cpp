#include <eosio.wps/eosio.wps.hpp>
#include <eosiolib/dispatcher.hpp>

#include "proposer.cpp"
#include "proposal.cpp"
#include "reviewer.cpp"

EOSIO_ABI( eosiowps::wps_contract,
	// proposer.hpp
	(regproposer)(editproposer)(rmvproposer)
	// proposal.hpp
	(regproposal)(editproposal)(rmvproposal)
	// reviewer.hpp
	(regreviewer)(editreviewer)(rmvreviewer)
)