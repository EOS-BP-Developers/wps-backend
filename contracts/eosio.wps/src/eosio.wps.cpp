#include <eosio.wps/eosio.wps.hpp>
#include <eosiolib/dispatcher.hpp>

#include "proposer.cpp"
#include "proposal.cpp"
#include "reviewer.cpp"
#include "vote.cpp"
#include "committee.cpp"
#include "watchman.cpp"

namespace eosiowps {
	wps_contract::wps_contract(action_name self) : contract(self), m_wps_env_global(self, self) {
		m_wps_env = m_wps_env_global.exists() ? m_wps_env_global.get() : wps_env();
	}

	const uint32_t seconds_per_day = 60*60*24;
	const uint32_t duration_of_voting_seconds = m_wps_env.duration_of_voting*seconds_per_day;
	const uint32_t duration_of_funding_seconds = m_wps_env.duration_of_funding*seconds_per_day;
};

EOSIO_ABI( eosiowps::wps_contract,
	// proposer.cpp
	(regproposer)(editproposer)(rmvproposer)
	// proposal.cpp
	(regproposal)(editproposal)(rmvproposal)
	// reviewer.cpp
	(regreviewer)(editreviewer)(rmvreviewer)(acceptprop)(rejectprop)(checkvote)(approve)(rejectfunding)(rvmreject)
	// vote.cpp
	(vote)(unvote) // (stake)(unstake)
	// committee.cpp
    (setwpsenv)(regcommittee)(editcommittee)(rmvcommittee)
	// watchman.cpp
	(commitvote)(rollbackvote)

)