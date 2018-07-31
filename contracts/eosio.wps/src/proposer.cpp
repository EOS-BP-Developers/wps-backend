// #include <eosio.wps/eosio.wps.hpp>
#include <eosio.wps/proposer.hpp>

// extern struct permission_level;
// extern void require_auth(const permission_level& level);

namespace eosiowps {

	// @abi action
	void wps_contract::regproposer(
		account_name account,
		const string& first_name,
		const string& last_name,
		const string& img_url,
		const string& bio,
		const string& country,
		const string& telegram,
		const string& website,
		const string& linkedin) {

		// authority of the user's account is required
		require_auth(account);

		//verify that the inputs are not too short
		eosio_assert(first_name.size() > 0, "first name should be more than 0 characters long");
		eosio_assert(last_name.size() > 0, "last name should be more than 0 characters long");
		eosio_assert(img_url.size() > 0, "not a valid image URL");
		eosio_assert(bio.size() > 0, "bio should be more than 0 characters long");
		eosio_assert(country.size() > 0, "country name should be more than 0 characters long");
		eosio_assert(telegram.size() > 4, "not a valid Telegram username");
		eosio_assert(website.size() > 0, "not a valid website URL");
		eosio_assert(linkedin.size() > 0, "not a valid linkedin URL");

		//verify that the inputs aren't too long
		eosio_assert(first_name.size() < 128, "first name should be shorter than 128 characters.");
		eosio_assert(last_name.size() < 128, "last name should be shorter than 128 characters.");
		eosio_assert(img_url.size() < 128, "image URL should be shorter than 128 characters.");
		eosio_assert(bio.size() < 256, "description should be shorter than 256 characters.");
		eosio_assert(country.size() < 64, "country name should be shorter than 64 characters.");
		eosio_assert(telegram.size() < 64, "telegram username should be shorter than 64 characters.");
		eosio_assert(website.size() < 128, "website URL should be shorter than 128 characters.");
		eosio_assert(linkedin.size() < 128, "linked URL should be shorter than 128 characters.");

		// creates the proposers table if there isn't one already
		proposer_table proposers(_self, _self);

		auto itr = proposers.find(account);
		// verify that the account doesn't already exist in the table
		eosio_assert(itr == proposers.end(), "This account has already been registered as a proposer");

		// add to the table
		// storage is billed to the contract account
		proposers.emplace(account, [&](auto& proposer){
			proposer.account = account;
			proposer.first_name = first_name;
			proposer.last_name = last_name;
			proposer.img_url = img_url;
			proposer.bio = bio;
			proposer.country = country;
			proposer.telegram = telegram;
			proposer.website = website;
			proposer.linkedin = linkedin;
			proposer.last_claim_time = 0;
		});
	}

	//@abi action
	void wps_contract::editproposer(account_name account,
		const string& first_name,
		const string& last_name,
		const string& img_url,
		const string& bio,
		const string& country,
		const string& telegram,
		const string& website,
		const string& linkedin) {
		// authority of the user's account is required
		require_auth(account);

		//verify that the inputs are not too short
		eosio_assert(first_name.size() > 0, "first name should be more than 0 characters long");
		eosio_assert(last_name.size() > 0, "last name should be more than 0 characters long");
		eosio_assert(img_url.size() > 0, "not a valid image URL");
		eosio_assert(bio.size() > 0, "bio should be more than 0 characters long");
		eosio_assert(country.size() > 0, "country name should be more than 0 characters long");
		eosio_assert(telegram.size() > 4, "not a valid Telegram username");
		eosio_assert(website.size() > 0, "not a valid website URL");
		eosio_assert(linkedin.size() > 0, "not a valid linkedin URL");

		//verify that the inputs aren't too long
		eosio_assert(first_name.size() < 128, "first name should be shorter than 128 characters.");
		eosio_assert(last_name.size() < 128, "last name should be shorter than 128 characters.");
		eosio_assert(img_url.size() < 128, "image URL should be shorter than 128 characters.");
		eosio_assert(bio.size() < 256, "description should be shorter than 256 characters.");
		eosio_assert(country.size() < 64, "country name should be shorter than 64 characters.");
		eosio_assert(telegram.size() < 64, "telegram username should be shorter than 64 characters.");
		eosio_assert(website.size() < 128, "website URL should be shorter than 128 characters.");
		eosio_assert(linkedin.size() < 128, "linked URL should be shorter than 128 characters.");

		proposer_table proposers(_self, _self);

		auto itr = proposers.find(account);
		// verify that the account doesn't already exist in the table
		eosio_assert(itr != proposers.end(), "Account not found in proposer table");

		// modify value in the table
		proposers.modify(itr, account, [&](auto& proposer) {
			proposer.account = account;
			proposer.first_name = first_name;
			proposer.last_name = last_name;
			proposer.img_url = img_url;
			proposer.bio = bio;
			proposer.country = country;
			proposer.telegram = telegram;
			proposer.website = website;
			proposer.linkedin = linkedin;
		});
	}

	//@abi action
	void wps_contract::rmvproposer(account_name account) {
		// needs authority of the proposers's account
		require_auth(account);

		proposer_table proposers(_self, _self);

		// verify that the account already exists in the proposer table
		auto itr = proposers.find(account);
		eosio_assert(itr != proposers.end(), "Account not found in proposer table");

		proposers.erase(itr);
	}

	//@abi action
	void wps_contract::claimfunds(account_name proposer, uint64_t proposal_id){
		// needs authority of the proposer account
		require_auth(proposer);

		proposer_table proposers(_self, _self);
		// verify that the account already exists in the proposer table
		auto itr = proposers.find(account);
		eosio_assert(itr != proposers.end(), "Account not found in proposer table");

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::APPROVED, "Proposal::status is not PROPOSAL_STATUS::APPROVED");

		auto current_time = now();
		m_wps_env = m_wps_env_global.exists() ? m_wps_env_global.get() : wps_env();

		//check for proposal expiry
		if(current_time - (*itr_proposal).funding_start_time > duration_of_funding_seconds){
			completed_proposal_table completed_proposals(_self, _self);

			//add to the table
			completed_proposals.emplace((*itr_proposal).proposer, [&](auto& _proposal){
				_proposal = std::move(*itr_proposal);
				_proposal.status = PROPOSAL_STATUS::COMPLETED;
			});

			idx_index.erase(itr_proposal);
			//move tables
			eosio_assert(current_time - (*itr_proposal).funding_start_time < duration_of_funding_seconds, "The funding period for this proposal has expired.");
		}

		//check for iteration of claim funds
		eosio_assert((*itr_proposal).iteration_of_funding < m_wps_env.total_iteration_of_funding, "all funds for this proposal have already been claimed");

		double seconds_per_claim_interval = duration_of_funding_seconds / m_wps_env.total_iteration_of_funding;
		eosio_assert(current_time - (*itr).last_claim_time > seconds_per_claim_interval, "It has not been one month since last claim");

		double amount = ((*itr_proposal).funding_goal) / (m_wps_env.total_iteration_of_funding);

		//inline action transfer, send funds to proposer
		eosio::action(
				eosio::permission_level{ _self, N(active) },
				N(eosio.token), N(transfer),
				std::make_tuple( _self, proposer, amount, std::string("Your worker proposal has been approved."))
		).send();

		proposers.modify(itr, 0, [&](auto& _proposer){
			_proposer.last_claim_time = current_time;
		});

		if((*itr_proposal).iteration_of_funding == 0){
			idx_index.modify(itr_proposal, 0, [&](auto& _proposal){
				_proposal.fund_start_time = current_time;
			});
		}

		idx_index.modify(itr_proposal, 0, [&](auto& _proposal){
			_proposal.iteration_of_funding++;
		});

		if((*itr_proposal).iteration_of_funding >= m_wps_env.total_iteration_of_funding){
			completed_proposal_table completed_proposals(_self, _self);

			//add to the table
			completed_proposals.emplace((*itr_proposal).proposer, [&](auto& _proposal){
				_proposal = std::move(*itr_proposal);
				_proposal.status = PROPOSAL_STATUS::COMPLETED;
			});

			idx_index.erase(itr_proposal);
		}
		//if statement, change state based on count
	}
} // eosiowps
