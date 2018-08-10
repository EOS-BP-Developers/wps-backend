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
