add_executable(RefactorCppCli "main.cpp")
target_link_libraries(RefactorCppCli RefactorCppLib)
set_target_properties(RefactorCppCli
  PROPERTIES
    OUTPUT_NAME refactor-cpp)

