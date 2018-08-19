#include <eosio.wps/reviewer.hpp>
#include <eosio.wps/proposal.hpp>
#include <algorithm>
#include <eosiolib/print.hpp>

namespace eosiowps {
	// @abi action
	void wps_contract::vote(account_name voter, uint64_t proposal_id, bool is_agree) {
		require_auth(voter);

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::ON_VOTE, "Proposal::status is not PROPOSAL_STATUS::ON_VOTE");

		auto current_time = now();
		auto wps_env = m_wps_env_global.get();
		auto duration_of_voting = wps_env.duration_of_voting * seconds_per_day;

		eosio_assert(current_time - (*itr_proposal).vote_start_time < duration_of_voting, "The voting period for this proposal has expired.");

		voting_table voting(_self, _self);
		auto itr = voting.find(proposal_id);
		if (itr == voting.end()) {
			voting.emplace(_self, [&](auto& vote_info) {
				vote_info.proposal_id = proposal_id;
				if (is_agree == true) {
					vote_info.agrees.emplace_back(voter);
				} else {
					vote_info.disagrees.emplace_back(voter);
				}
			});
		} else {
			voting.modify(itr, 0, [&](auto& vote_info) {
				if (is_agree == true) {
					auto itr_agree = std::find(vote_info.agrees.begin(), vote_info.agrees.end(), voter);
					eosio_assert(itr_agree == vote_info.agrees.end(), "duplicate agree vote");
					vote_info.agrees.emplace_back(voter);

					auto itr_disagree = std::find(vote_info.disagrees.begin(), vote_info.disagrees.end(), voter);
					if (itr_disagree != vote_info.disagrees.end()) {
						vote_info.disagrees.erase(itr_disagree);
					}
				} else {
					auto itr_disagree = std::find(vote_info.disagrees.begin(), vote_info.disagrees.end(), voter);
					eosio_assert(itr_disagree == vote_info.disagrees.end(), "duplicate disagree vote");
					vote_info.disagrees.emplace_back(voter);
					auto itr_agree = std::find(vote_info.agrees.begin(), vote_info.agrees.end(), voter);
					if (itr_agree != vote_info.agrees.end()) {
						vote_info.agrees.erase(itr_agree);
					}
				}
			});
		}
	}

	// @abi action
	void wps_contract::unvote(account_name voter, uint64_t proposal_id) {
		require_auth(voter);

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::ON_VOTE, "Proposal::status is not proposal_status::PROPOSAL_STATUS");

		auto current_time = now();
		auto wps_env = m_wps_env_global.get();
		auto duration_of_voting = wps_env.duration_of_voting * seconds_per_day;
		eosio_assert(current_time - (*itr_proposal).vote_start_time < duration_of_voting, "The voting period for this proposal has expired.");

		voting_table voting(_self, _self);
		auto itr = voting.find(proposal_id);
		if (itr != voting.end()) {
			voting.modify(itr, 0, [&](auto& vote_info) {
				auto itr_agree = std::find(vote_info.agrees.begin(), vote_info.agrees.end(), voter);
				if(itr_agree != vote_info.agrees.end()) {
					vote_info.agrees.erase(itr_agree);
					return;
				}
				auto itr_disagree = std::find(vote_info.disagrees.begin(), vote_info.disagrees.end(), voter);
				if(itr_disagree != vote_info.disagrees.end()) {
					vote_info.disagrees.erase(itr_disagree);
				}
			});
		}
	}
} // eosiowps
