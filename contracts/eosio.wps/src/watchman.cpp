#include <eosio.wps/proposal.hpp>
#include <eosio.wps/committee.hpp>

namespace eosiowps {

	// @abi action
	void wps_contract::commitvote(account_name watchman, uint64_t proposal_id) {
		require_auth(watchman);

		committee_table committees(_self, _self);
		auto itr = committees.find(watchman);
		eosio_assert(itr != committees.end(), "Account not found in committees table");
		eosio_assert((*itr).is_oversight, "account does not have oversight privileges");

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert((*itr).is_oversight, "account does not have oversight privileges");
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::CHECK_VOTE, "Proposal's status is not CHECK_VOTE");

		idx_index.modify(itr_proposal, 0, [&](auto& proposal){
			proposal.status = PROPOSAL_STATUS::CHECKED_VOTE;
		});
	}//action permission should be linked to a separate key

	// @abi action
	void wps_contract::rollbackvote(account_name watchman, uint64_t proposal_id){
		require_auth(watchman);

		committee_table committees(_self, _self);
		auto itr = committees.find(watchman);
		eosio_assert(itr != committees.end(), "Account not found in committees table");
		eosio_assert((*itr).is_oversight, "account does not have oversight privileges");

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::CHECK_VOTE, "Proposal's status is not CHECK_VOTE");

		idx_index.modify(itr_proposal, 0, [&](auto& proposal) {
			proposal.status = PROPOSAL_STATUS::ON_VOTE; //roll back to on vote status
		});
	}//action permission should be linked to a separate key

	void wps_contract::checkexpire(account_name watchman, uint64_t proposal_id) {
		require_auth(watchman);

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::ON_VOTE, "Proposal::status is not PROPOSAL_STATUS::ON_VOTE");

		auto current_time = now();
		auto wps_env = m_wps_env_global.get();

		if(current_time - (*itr_proposal).vote_start_time > wps_env.duration_of_voting){
			rejected_proposal_table rejected_proposals(_self, _self);
			rejected_proposals.emplace(_self, [&](auto& _proposal){
				_proposal = (*itr_proposal);
				_proposal.status = PROPOSAL_STATUS::REJECTED;
			});
			idx_index.erase(itr_proposal);
		}
	}
} //eosiowps