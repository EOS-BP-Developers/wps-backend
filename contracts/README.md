# eosio.wps
## Version : 1.0.0

The design of the EOSIO blockchain calls for a number of smart contracts that are run at a privileged permission level in order to support functions such as working proposal.  These smart contracts are referred to as the wps contract.

Dependencies:
* [eosio v1.1.4](https://github.com/eosio/eos/tree/v1.1.4)
* [eosio.wasmsdk v1.1.1](https://github.com/eosio/eosio.wasmsdk/tree/v1.1.1)

To build the contracts and the unit tests:
* First, ensure that your __eosio__ is compiled to the core symbol for the EOSIO blockchain that intend to deploy to.
* Second, make sure that you have ```sudo make install```ed __eosio__.
The naming convention is codeaccount::actionname followed by a list of paramters.

After build:
* The unit tests executable is placed in the _build/tests_ and is named __unit_test__.
* The contracts are built into a _bin/\<contract name\>_ folder in their respective directories.
* Finally, simply use __cleos__ to _set contract_ by pointing to the previously mentioned directory.

## Actions

Note: When the required authority is `_self`, it means that the authority of the contract account is required. When deployed onto the `eosio.wps` account on mainnet, this authority will be given to `eosio`, which is owned by `eosio.prods`. We also expect that most committees will be "owned" by its reviewers, where the committee has an `msig` permission set up and controlled by its members.

#### setwpsenv

Required authority: `_self`

Parameters: `uint32_t total_voting_percent, uint32_t duration_of_voting, uint32_t max_duration_of_funding, uint32_t total_iteration_of_funding`

#### regproposer

Required authority: Account owner

Parameters: `account_name account, const string& first_name, const string& last_name,
                            const string& img_url, const string& bio, const string& country, const string& telegram,
                            const string& website, const string& linkedin`

#### editproposer

Required authority: Account owner

Parameters: `account_name account, const string& first_name, const string& last_name,
                            const string& img_url, const string& bio, const string& country, const string& telegram,
                            const string& website, const string& linkedin`

#### rmvproposer

Required authority: Account owner

Parameters: `account_name account`

#### regproposal

Required authority: Proposer

Parameters: `account_name proposer,
                    account_name committee,
                    uint16_t subcategory,
                    const string& title,
                    const string& summary,
                    const string& project_img_url,
                    const string& description,
                    const string& roadmap,
                    uint64_t duration,
                    const vector<string>& members,
                    const asset& funding_goal`

#### editproposal

Required authority: Proposer

Parameters: `account_name proposer,
                    account_name committee,
                    uint16_t subcategory,
                    const string& title,
                    const string& summary,
                    const string& project_img_url,
                    const string& description,
                    const string& roadmap,
                    uint64_t duration,
                    const vector<string>& members,
                    const asset& funding_goal`

#### rmvproposal

Required authority: Proposer

Parameters: 

#### regcommittee

Required authority: `_self`

#### editcommittee

Required authority: `_self`

#### rmvcommittee

Required authority: `_self`

#### regreviewer

Required authority: Committee

#### editreviewer

Required authority: Committee

#### rmvreviewer

Required authority: Committee

#### acceptprop

Required authority: Reviewer

#### rejectprop

Required authority: Reviewer

#### checkvote

Required authority: Reviewer

#### approve

Required authority: Reviewer

#### claimfunds

Required authority: Proposer

#### rmvreject

Required authority: Reviewer

#### rmvcompleted

Required authority: Reviewer

#### vote

Required authority: Account owner

#### unvote

Required authority: Account owner

#### rejectfund

Required authority: Committee (oversight)

#### commitvote

Required authority: Committee (oversight)

#### rollbackvote

Required authority: Committee (oversight)

#### checkexpire

Required authority: None
