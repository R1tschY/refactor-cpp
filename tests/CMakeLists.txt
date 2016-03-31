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

set(unit_test_command ${CMAKE_CURRENT_BINARY_DIR}/refactor-unit-tests -i -s --detect_fp_exceptions=y -l message)
add_test(NAME unit-tests COMMAND ${unit_test_command})
add_dependencies(check RefactorUnitTests)

# coverage

set(GENHTML_FLAGS "${GENHTML_FLAGS} --title ${PROJECT_NAME} --prefix ${CMAKE_SOURCE_DIR} --num-spaces 2")
get_filename_component(_libdir ${CMAKE_CURRENT_SOURCE_DIR}/../lib REALPATH)
add_coverage_target(
  TARGET coverage
  COMMAND ${unit_test_command}
  OUTPUT ${CMAKE_BINARY_DIR}/coverage
  INCLUDE '${_libdir}/*'
)
