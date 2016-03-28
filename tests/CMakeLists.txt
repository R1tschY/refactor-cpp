add_test(NAME integration-tests COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run_cli_tests.py)
add_dependencies(check RefactorCppCli)

# Unit tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB_RECURSE RefactorUnitTests_SOURCES "units/*.cpp")
add_executable(RefactorUnitTests ${RefactorUnitTests_SOURCES})
set_target_properties(RefactorUnitTests
  PROPERTIES
    OUTPUT_NAME refactor-unit-tests)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
target_link_libraries(RefactorUnitTests RefactorCppLib ${Boost_LIBRARIES} ${LLVM_MODULE_LIBS} ${CLANG_LIBS})
target_include_directories(RefactorUnitTests SYSTEM PRIVATE ${Boost_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_CFLAGS} ${CLANG_CFLAGS})
add_definitions(-DBOOST_TEST_DYN_LINK)

add_custom_command(TARGET RefactorUnitTests
                   POST_BUILD
                   COMMAND ./refactor-unit-tests -i -s --detect_fp_exceptions=y -l message
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Running refactor-unit-tests ..." VERBATIM)
add_dependencies(check RefactorUnitTests)
