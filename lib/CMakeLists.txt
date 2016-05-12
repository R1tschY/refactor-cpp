# Sources
file(GLOB_RECURSE RefactorCppLib_SOURCES "*.cpp")

# Target
add_library(RefactorCppLib SHARED ${RefactorCppLib_SOURCES})
target_include_directories(RefactorCppLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(RefactorCppLib
  PROPERTIES
    OUTPUT_NAME refactor-cpp
    CXX_VISIBILITY_PRESET "hidden"
    C_VISIBILITY_PRESET "hidden"
    VISIBILITY_INLINES_HIDDEN ON)
    
# config.h
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)
include_directories("${PROJECT_BINARY_DIR}")

# Includes / Libs
target_include_directories(RefactorCppLib SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})

# Clang
target_link_libraries(RefactorCppLib PRIVATE ${LLVM_MODULE_LIBS} ${Clang_LIBS})
target_include_directories(RefactorCppLib SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS} ${Clang_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_CFLAGS} ${Clang_CFLAGS})
