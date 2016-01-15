add_test(NAME integration-tests COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run_cli_tests.py)
add_dependencies(check RefactorCppCli)
