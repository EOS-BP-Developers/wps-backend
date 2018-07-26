#include <eosio.wps/eosio.wps.hpp>
#include <eosiolib/dispatcher.hpp>

#include "proposer.cpp"
#include "proposal.cpp"
#include "reviewer.cpp"

namespace eosiowps {
	wps_contract::wps_contract(action_name self) : contract(self), _wps_info_global(self, self) {
	}
};

EOSIO_ABI( eosiowps::wps_contract,
	// proposer.hpp
	(regproposer)(editproposer)(rmvproposer)
	// proposal.hpp
	(regproposal)(editproposal)(rmvproposal)
	// reviewer.hpp
	(regreviewer)(editreviewer)(rmvreviewer)(acceptproposal)(rejectproposal)
)