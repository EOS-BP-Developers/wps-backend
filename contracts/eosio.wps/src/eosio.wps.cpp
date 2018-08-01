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
		if (m_wps_env_global.exists() == false) {
		    m_wps_env_global.set( wps_env(), _self );
		}
	}
};

EOSIO_ABI( eosiowps::wps_contract,
	// proposer.cpp
	(regproposer)(editproposer)(rvmproposer)(claimfunds)
	// proposal.cpp
	(regproposal)(editproposal)(rmvproposal)
	// reviewer.cpp
	(regreviewer)(editreviewer)(rmvreviewer)(acceptprop)(rejectprop)(checkvote)(approve)(rejectfunding)(rmvreject)(rmvcompleted)
	// vote.cpp
	(vote)(unvote) // (stake)(unstake)
	// committee.cpp
    (setwpsenv)(regcommittee)(editcommittee)(rmvcommittee)
	// watchman.cpp
	(commitvote)(rollbackvote)(checkexpire)

)

//To do: put a cap on the amount of funds that each committee can pull from eosio.wps
//Refactor vote.cpp