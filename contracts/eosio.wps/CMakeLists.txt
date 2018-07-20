#add_library(eosio.wps.lib
#   ${CMAKE_CURRENT_SOURCE_DIR}/src/proposal.cpp
#   ${CMAKE_CURRENT_SOURCE_DIR}/src/proposer.cpp)

#target_include_directories(eosio.wps.lib
#   PUBLIC
#   ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(eosio.wps.wasm ${CMAKE_CURRENT_SOURCE_DIR}/src/eosio.wps.cpp)
target_include_directories(eosio.wps.wasm
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

#target_link_libraries(eosio.wps.wasm eosio.wps.lib)

set_target_properties(eosio.wps.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/eosio.wps")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/abi/eosio.wps.abi" "${CMAKE_CURRENT_SOURCE_DIR}/bin/eosio.wps/eosio.wps.abi" COPYONLY)

# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${WASM_ROOT}/eosio.wasmsdk/include)
