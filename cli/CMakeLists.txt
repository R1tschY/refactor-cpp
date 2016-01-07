file(GLOB_RECURSE RefactorCppCli_SOURCES "*.cpp")

add_executable(RefactorCppCli ${RefactorCppCli_SOURCES})
target_include_directories(RefactorCppCli SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(RefactorCppCli RefactorCppLib ${Boost_LIBRARIES})
set_target_properties(RefactorCppCli
  PROPERTIES
    OUTPUT_NAME refactor-cpp)
