# CMake generated Testfile for 
# Source directory: /Users/yepp4you/Work/blockchain/wps-backend/contracts/tests
# Build directory: /Users/yepp4you/Work/blockchain/wps-backend/contracts/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(unit_test_binaryen "/Users/yepp4you/Work/blockchain/wps-backend/contracts/build/tests/unit_test" "--report_level=detailed" "--color_output" "--" "--binaryen")
add_test(unit_test_wavm "/Users/yepp4you/Work/blockchain/wps-backend/contracts/build/tests/unit_test" "--report_level=detailed" "--color_output" "--catch_system_errors=no" "--" "--wavm")
