#include <eosio.wps/reviewer.hpp>
#include <eosio.wps/proposal.hpp>
#include <eosio.wps/committee.hpp>

namespace eosiowps {

    // @abi action
    void wps_contract::setwpsenv(uint32_t total_voting_percent, uint32_t duration_of_voting, uint32_t duration_of_funding, uint32_t total_iteration_of_funding) {
        //registration of committee requires contract account permissions
        require_auth(_self);

        eosio_assert(total_voting_percent >= 5, "total_voting_percent should be more than equal 5 long");
        eosio_assert(duration_of_voting > 0, "duration_of_voting should be more than 0");
        eosio_assert(duration_of_funding > 0, "duration_of_funding should be more than 0");
        eosio_assert(total_iteration_of_funding > 0, "total_iteration_of_funding should be more than 0");

        m_wps_env = m_wps_env_global.exists() ? m_wps_env_global.get() : wps_env();

        m_wps_env.total_voting_percent = total_voting_percent;
        m_wps_env.duration_of_voting = duration_of_voting;
        m_wps_env.duration_of_funding = duration_of_funding;
        m_wps_env.total_iteration_of_funding = total_iteration_of_funding;

        m_wps_env_global.set( m_wps_env, _self );
    }

    // @abi action
    void wps_contract::regcommittee(account_name committeeman, const string& category, bool is_oversight) {
        //registration of committee requires contract account permissions
        require_auth(_self);

        //verify that the committee account exists
        eosio_assert(is_account(committeeman), "committeeman account doesn't exist");

        //verify that the size of the category string is not too long/short
        eosio_assert(category.size() > 0, "category should be more than 0 characters long");
        eosio_assert(category.size() < 64, "category should be less than 64 characters long");

        //creates the committee table if it doesn't exist already
        committee_table committees(_self, _self);

        auto itr = committees.find(committeeman);
        // verify that the account doesn't already exist in the table
        eosio_assert(itr == committees.end(), "This account has already been registered as a committee");

        //add to the table
        committees.emplace(_self, [&](auto& committee){
            committee.committeeman = committeeman;
            committee.category = category;
            committee.is_oversight = is_oversight;
        });
    }

    // @abi action
    void wps_contract::editcommittee(account_name committeeman, const string& category, bool is_oversight) {
        //editing committee info requires contract account permissions
        require_auth(_self);

        //verify that the committee account exists
        eosio_assert(is_account(committeeman), "committee account doesn't exist");

        //verify that the size of the category string is not too long/short
        eosio_assert(category.size() > 0, "category should be more than 0 characters long");
        eosio_assert(category.size() < 64, "category should be less than 64 characters long");

        //creates the committee table if it doesn't exist already
        committee_table committees(_self, _self);

        auto itr = committees.find(committeeman);
        // verify that the account doesn't already exist in the table
        eosio_assert(itr != committees.end(), "Account not found in committee table");

        //add to the table
        committees.modify(itr, _self, [&](auto& committee){
            committee.committeeman = committeeman;
            committee.category = category;
            committee.is_oversight = is_oversight;
        });
    }

    // @abi action
    void wps_contract::rmvcommittee(account_name committeeman) {
        require_auth(_self);

        eosio_assert(is_account(committeeman), "committeeman account doesn't exist");

        committee_table committees(_self, _self);
        auto itr = committees.find(committeeman);

        eosio_assert(itr != committees.end(), "Account not found in committee table");
        committees.erase( itr );
    }

    // @abi action
	void wps_contract::rejectfunding(account_name committeeman, uint64_t proposal_id, const string& reason){
		require_auth(committeeman);

		eosio_assert(reason.size() > 0, "must provide a brief reason");
		eosio_assert(reason.size() < 256, "reason is too long");

		committee_table committees(_self, _self);

		auto itr = committees.find(committeeman);
		// verify that the committee is on committee table
		eosio_assert(itr != committees.end(), "Account not found in committee table");

		funded_proposal_table funded_proposals(_self, _self);

		auto idx_index = funded_proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in approved proposal table");

		eosio_assert(((*itr_proposal).committee==(*itr).committeeman) || (*itr).is_oversight, "Committee is not associated with this proposal");
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::APPROVED, "Proposal::status is not PROPOSAL_STATUS::APPROVED");

		rejected_proposal_table rejected_proposals(_self, _self);

		//add to the table
		funded_proposals.emplace((*itr_proposal).proposer, [&](auto& proposal){
			proposal = std::move(*itr_proposal);
			proposal.status = PROPOSAL_STATUS::REJECTED;
		});

		idx_index.erase(itr_proposal);
	}
} //eosiowps