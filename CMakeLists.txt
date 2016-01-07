cmake_minimum_required (VERSION 3.0.0)
project (RefactorCpp)

# include extra local modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# C++14
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y")

# Debug mode
set(DEBUG_FLAGS "-g -Og -ffast-math -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DEBUG_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${DEBUG_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${DEBUG_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} ${DEBUG_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG} ${DEBUG_FLAGS}")

# Profile mode	
set(PROFILE_FLAGS "-g -fno-omit-frame-pointer -Ofast -ffast-math -flto -DNDEBUG")
set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELEASE} ${PROFILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${PROFILE_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_PROFILE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${PROFILE_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS_PROFILE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} ${PROFILE_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS_PROFILE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} ${PROFILE_FLAGS}")

# Sanitize mode
set(SAN_FLAGS "-g -O2 -fsanitize=thread -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_SAN "${CMAKE_CXX_FLAGS_RELEASE} ${SAN_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_SAN "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${SAN_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_SAN "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${SAN_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS_SAN "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} ${SAN_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS_SAN "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} ${SAN_FLAGS}")
if (CMAKE_BUILD_TYPE STREQUAL "San")
	link_libraries(-ltsan)
endif()

# Release mode
set(RELEASE_FLAGS "-s -Ofast -ffast-math -funroll-all-loops -flto -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${RELEASE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${RELEASE_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${RELEASE_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} ${RELEASE_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} ${RELEASE_FLAGS}")

# Warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
  -Wall \
  -Wextra \
  -Wshadow \
  -Wnon-virtual-dtor \
  -Wcast-align \
  -Wunused \
  -Woverloaded-virtual \
  -Wcast-qual \
  -Wconversion \
  -Wno-unused-parameter\
")

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

#
add_subdirectory(lib)
add_subdirectory(cli)
add_subdirectory(tests)

# utils
add_custom_target(update COMMAND cmake .)

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
  message(STATUS "  LLVM:     ${LLVM_VERSION}")
else()
  message(STATUS "  LLVM:     not found") 
endif()
message(STATUS "")
message(STATUS "==============================================================")
message(STATUS "")



