# Sources
file(GLOB_RECURSE RefactorCppLib_SOURCES "*.cpp")

# Clang
find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)

# Target
add_library(RefactorCppLib SHARED ${RefactorCppLib_SOURCES})
target_include_directories(RefactorCppLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(RefactorCppLib
  PROPERTIES
    OUTPUT_NAME refactor-cpp
    CXX_VISIBILITY_PRESET "hidden"
    C_VISIBILITY_PRESET "hidden"
    VISIBILITY_INLINES_HIDDEN ON)

# Includes / Libs
target_include_directories(RefactorCppLib SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})

# Boost
target_link_libraries(RefactorCppLib ${Boost_LIBRARIES})

# Clang
target_link_libraries(RefactorCppLib PRIVATE ${CLANG_LIBS})
target_include_directories(RefactorCppLib SYSTEM PRIVATE ${CLANG_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${CLANG_CFLAGS})
