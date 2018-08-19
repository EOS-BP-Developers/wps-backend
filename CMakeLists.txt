cmake_minimum_required(VERSION 3.10)
project(wps_backend)

set(CMAKE_CXX_STANDARD 11)

include_directories(contracts/eosio.wps/include)
include_directories(contracts/eosio.wps/include/eosio.wps)
include_directories(contracts/tests)

add_executable(wps_backend
        contracts/eosio.wps/include/eosio.wps/eosio.wps.hpp
        contracts/eosio.wps/include/eosio.wps/proposal.hpp
        contracts/eosio.wps/include/eosio.wps/proposer.hpp
        contracts/eosio.wps/include/eosio.wps/reviewer.hpp
        contracts/eosio.wps/src/eosio.wps.cpp
        contracts/eosio.wps/src/proposal.cpp
        contracts/eosio.wps/src/proposer.cpp
        contracts/eosio.wps/src/reviewer.cpp
        contracts/tests/contracts.hpp
        contracts/tests/eosio.wps_tests.cpp
        contracts/build.sh
        contracts/CMakeLists.txt
        contracts/LICENSE
        contracts/README.md
        contracts/UnitTestsExternalProject.txt
        LICENSE
        README.md contracts/eosio.wps/src/committee.cpp contracts/eosio.wps/include/eosio.wps/committee.hpp)
