#include <eosiolib/eosio.hpp>
#include <string>

using eosio::indexed_by;
using eosio::const_mem_fun;
using std::string;

class wps : public eosio::contract {
	public:
		explicit wps(action_name self)
			: contract(self){
		}

		// @abi action
		void regproposer(const account_name account, const string& first_name, const string& last_name,
						 const string& img_url, const string& bio, const string& country, const string& telegram,
						 const string& website, const string& linkedin){
			
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

			//verify that the inputs are not too long
			eosio_assert(first_name.size() < 128, "first name should be shorter than 128 characters.");
			eosio_assert(last_name.size() < 128, "last name should be shorter than 128 characters.");
			eosio_assert(img_url.size() < 128, "image URL should be shorter than 128 characters.");
			eosio_assert(bio.size() < 256, "description should be shorter than 256 characters.");
			eosio_assert(country.size() < 64, "country name should be shorter than 64 characters.");
			eosio_assert(telegram.size() < 64, "telegram username should be shorter than 64 characters.");
			eosio_assert(website.size() < 128, "website URL should be shorter than 128 characters.");
			eosio_assert(linkedin.size() < 128, "linked URL should be shorter than 128 characters.");


			// creates the proposers table if there isn't one already
			proposer_index proposers(_self, _self);

			auto itr = proposers.find(account);
			// verify that the account doesn't already exist in the table
			eosio_assert(itr == proposers.end(), "This account has already been registered as a proposer");

			// add to the table
			// storage is billed to the contract account
			proposers.emplace(account, [&](auto& proposer){
				proposer.account_name = account;
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
		void editproposer(const account_name account, const string& first_name, const string& last_name,
						 const string& img_url, const string& bio, const string& country, const string& telegram,
						 const string& website, const string& linkedin){
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

			//verify that the inputs are not too long
			eosio_assert(first_name.size() < 128, "first name should be shorter than 128 characters.");
			eosio_assert(last_name.size() < 128, "last name should be shorter than 128 characters.");
			eosio_assert(img_url.size() < 128, "image URL should be shorter than 128 characters.");
			eosio_assert(bio.size() < 256, "description should be shorter than 256 characters.");
			eosio_assert(country.size() < 64, "country name should be shorter than 64 characters.");
			eosio_assert(telegram.size() < 64, "telegram username should be shorter than 64 characters.");
			eosio_assert(website.size() < 128, "website URL should be shorter than 128 characters.");
			eosio_assert(linkedin.size() < 128, "linked URL should be shorter than 128 characters.");


			proposer_index proposers(_self, _self);

			auto itr = proposers.find(account);
			// verify that the account doesn't already exist in the table
			eosio_assert(itr != proposers.end(), "Account not found in proposer table");

			// modify value in the table
			proposers.modify(account, [&](auto& proposer){
				proposer.account_name = account;
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
		void rmvproposer(const account_name account){
			// needs authority of the proposers's account
			require_auth(account);

			proposer_index addresses(_self, _self);

			// verify that the account already exists in the proposer table
			auto itr = proposers.find(account);
			eosio_assert(itr != addresses.end(), "Account not found in proposers table");

			proposers.erase( itr );
		}

		//@abi action
		void setreviewer(account_name account, const string& first_name, const string& last_name, const string& committee){

			//Require permission of contract account
			require_auth(_self);

			//verify that the account exists
			eosio_assert(is_account(account), "The account does not exist");

			//verify that the inputs are not too short
			eosio_assert(first_name.size() > 0, "first name should be more than 0 characters long");
			eosio_assert(last_name.size() > 0, "last name should be more than 0 characters long");
			eosio_assert(committee.size() > 0, "committee name should be more than 0 characters long");

			//verify that the inputs are not too long
			eosio_assert(first_name.size() < 128, "first name should be shorter than 128 characters.");
			eosio_assert(last_name.size() < 128, "last name should be shorter than 128 characters.");
			eosio_assert(committee.size() < 64, "committee name should be shorter than 64 charactrs.");

			//creates the reviewers table if it there isn't one already
			reviewer_index reviewers(_self, _self);

			auto itr = reviewers.find(account);
			// verify that the account doesn't already exist in the table
			eosio_assert(itr == reviewers.end(), "This account has already been registered as a reviewer");

			//add to the table
			reviewers.emplace(account, [&](auto& reviewer){
				reviewer.account_name = account;
				reviewer.first_name = first_name;
				reviewer.last_name = last_name;
				reviewer.committee = committee;
			});
		}

		//@abi action
		void editreviewer(){
			//Require permission of contract account
			require_auth(_self);

			//verify that the account exists
			eosio_assert(is_account(account), "The account does not exist");

			//verify that the inputs are not too short
			eosio_assert(first_name.size() > 0, "first name should be more than 0 characters long");
			eosio_assert(last_name.size() > 0, "last name should be more than 0 characters long");
			eosio_assert(committee.size() > 0, "committee name should be more than 0 characters long");

			//verify that the inputs are not too long
			eosio_assert(first_name.size() < 128, "first name should be shorter than 128 characters.");
			eosio_assert(last_name.size() < 128, "last name should be shorter than 128 characters.");
			eosio_assert(committee.size() < 64, "committee name should be shorter than 64 charactrs.");

			//creates the reviewers table if it there isn't one already
			reviewer_index reviewers(_self, _self);

			auto itr = reviewers.find(account);
			// verify that the account already exists in the table
			eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

			//add to the table
			reviewers.modify(account, [&](auto& reviewer){
				reviewer.account_name = account;
				reviewer.first_name = first_name;
				reviewer.last_name = last_name;
				reviewer.committee = committee;
			});
		}

		//@abi action
		void rmvreviewer(const account_name account){
			// needs authority of the contract account
			require_auth(_self);

			//verify that the account exists
			eosio_assert(is_account(account), "The account does not exist");

			reviewer_index reviewers(_self, _self);

			// verify that the account already exists in the reviewers table
			auto itr = reviewers.find(account);
			eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

			reviewers.erase( itr );
		}

	private:

		struct proposer {
			uint64_t account_name;
			string first_name;
			string last_name;
			string img_url;
			string bio;
			string country;
			string telegram;
			string website;
			string linkedin;

			uint64_t primary_key() const { return account_name; }

			EOSLIB_SERIALIZE( proposer, (account_name)(first_name)(last_name)(img_url)(bio)(country)(telegram)(website)(linkedin) )
		};

		typedef eosio::multi_index< N(proposer), proposer> proposer_index;

		struct reviewer {
			uint64_t account_name;
			string first_name;
			string last_name;
			string committee;

			uint64_t primary_key() const { return account_name; }

			EOSLIB_SERIALIZE( reviewer, (account_name)(first_name)(last_name)(committee) )
		};

		typedef eosio::multi_index< N(reviewer), reviewer> reviewer_index;
};

EOSIO_ABI( proposers, (regproposer)(editproposer)(removeproposer)(setreviewer)(editreviewer)(rmvreviewer) )
