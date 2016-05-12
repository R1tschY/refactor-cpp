cmake_minimum_required (VERSION 3.0.0)
project (RefactorCpp VERSION 0.1)

# include extra local modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_SOURCE_DIR}/cmake)

# C++14
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y")

include(AddConfig)

# Debug mode
add_config(
	Debug
	CXX_FLAGS "-g -Og -DDEBUG -D_DEBUG"
	LINKER_FLAGS "-Og"
)

# Profile mode	
add_config(
	Profile
	CXX_FLAGS "-g -fno-omit-frame-pointer -Ofast -flto -DNDEBUG"
	LINKER_FLAGS "-g -fno-omit-frame-pointer -Ofast -flto"
)

# Sanitize mode
add_config(
	San
	CXX_FLAGS "-g -O2 -fsanitize=thread -fno-omit-frame-pointer -DNDEBUG"
	LINKER_FLAGS "-g -O2 -fsanitize=thread -ltsan -fno-omit-frame-pointer"
)

# Release mode
add_config(
	Release
	CXX_FLAGS "-s -O3 -funroll-all-loops -flto -DNDEBUG"
	LINKER_FLAGS "-s -O3 -funroll-all-loops -flto"
)

# Warnings
include(AddCXXFlags)
add_cxx_flags("-Wall -Wextra -Wshadow -Wcast-align -Wunused -Wcast-qual -Wconversion -Wno-unused-parameter")
add_cxx_flag_checked("-Wnon-virtual-dtor")
add_cxx_flag_checked("-Woverloaded-virtual")
add_cxx_flag_checked("-Wold-style-cast")     
add_cxx_flag_checked("-Wsuggest-override")
add_cxx_flag_checked("-Wundef")
add_cxx_flag_checked("-Wconditionally-supported")
add_cxx_flag_checked("-Wzero-as-null-pointer-constant")
add_cxx_flag_checked("-Wnull-dereference") # gcc 6
add_cxx_flag_checked("-Wduplicated-cond") # gcc 6
add_cxx_flag_checked("-Wshift-negative-value") # gcc 6
#add_cxx_flag_checked("-Wsuggest-final-methods")
#add_cxx_flag_checked("-Wsuggest-final-types")

# Eclipse
add_flags(CXX "-fmessage-length=0" C "-fmessage-length=0" LD "-fmessage-length=0")

# hardening
include(CodeHardening)
hardening(ALL)

# Coverage
include(CodeCoverage)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Testing
enable_testing ()
set(CMAKE_CTEST_COMMAND ctest --output-on-failure)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doxyfile @ONLY)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
	)
endif(DOXYGEN_FOUND)

# Boost
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON) 
find_package(Boost QUIET REQUIRED)

# Clang
find_package(LLVM REQUIRED)
find_package(Clang 
  REQUIRED 
  COMPONENTS Frontend Serialization Driver Tooling ToolingCore Parse Sema 
             Analysis RewriteFrontend Rewrite Edit AST ASTMatchers Lex Basic
             # CodeGen  Index StaticAnalyzerFrontend StaticAnalyzerCheckers StaticAnalyzerCore Format
)

#
add_subdirectory(lib)
add_subdirectory(cli)
add_subdirectory(tests)

# utils
add_custom_target(update COMMAND cmake .)

# debug output
string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)
message(STATUS "")
message(STATUS "==============================================================")
message(STATUS "")
message(STATUS "BUILD TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "COMPILER FLAGS:")
message(STATUS "  ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${BUILD_TYPE}}")
message(STATUS "LINKER FLAGS:")
message(STATUS "  ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_${BUILD_TYPE}}")
message(STATUS "")
message(STATUS "REQUIRED 3TH-PARTY LIBRARIES")
if (Boost_FOUND)
  message(STATUS "  Boost:    ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")
else()
  message(STATUS "  Boost:    not found") 
endif()
if (LLVM_FOUND)
  message(STATUS "  Clang:    ${LLVM_VERSION}")
else()
  message(STATUS "  Clang:    not found") 
endif()
message(STATUS "")
message(STATUS "==============================================================")
message(STATUS "")



