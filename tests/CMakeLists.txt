# Boost Test
find_package(Boost QUIET COMPONENTS unit_test_framework REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_TEST_DYN_LINK)

# Sources
file(GLOB_RECURSE RefactorCppTests_SOURCES "*.cpp")

# Target
add_executable(RefactorCppTests EXCLUDE_FROM_ALL ${RefactorCppTests_SOURCES})
target_link_libraries(RefactorCppTests ${Boost_LIBRARIES})
add_test(NAME RefactorCppTests COMMAND RefactorCppTests)
add_dependencies(check RefactorCppTests)


