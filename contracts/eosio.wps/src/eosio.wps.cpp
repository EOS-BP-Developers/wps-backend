#include <eosio.wps/eosio.wps.hpp>
#include <eosiolib/dispatcher.hpp>

#include "proposer.cpp"
#include "proposal.cpp"
#include "reviewer.cpp"
#include "vote.cpp"
#include "committee.cpp"

namespace eosiowps {
	wps_contract::wps_contract(action_name self) : contract(self), m_wps_info_global(self, self) {
		m_wps_info = m_wps_info_global.exists() ? m_wps_info_global.get() : wps_info();
	}
};

EOSIO_ABI( eosiowps::wps_contract,
	// proposer.hpp
	(regproposer)(editproposer)(rmvproposer)
	// proposal.hpp
	(regproposal)(editproposal)(rmvproposal)
	// reviewer.hpp
	(regreviewer)(editreviewer)(rmvreviewer)(acceptprop)(rejectprop)(approve)
	// vote.hpp
	(vote)(unvote) // (stake)(unstake)
	// committee.hpp
	(regcommittee)(editcommittee)(rmvcommittee)
)