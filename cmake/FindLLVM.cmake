# Find LLVM
# Copyright (c) 2012 Canal Team
#
# It defines the following variables
#  LLVM_FOUND        - True if llvm found.
#  LLVM_INCLUDE_DIRS - where to find llvm include files
#  LLVM_LIBRARY_DIRS - where to find llvm libs
#  LLVM_CFLAGS       - llvm compiler flags
#  LLVM_LDFLAGS      - llvm linker flags
#  LLVM_MODULE_LIBS  - list of llvm libs for working with modules.

include(FindPackageHandleStandardArgs)

# known versions of llvm
set(_LLVM_KNOWN_VERSIONS 
  ${LLVM_FIND_VERSION}
  ${LLVM_ADDITIONAL_VERSIONS} 
  "3.7" "3.6" "3.5" "3.4"
)

# generate names for llvm-config
set(_LLVM_CONFIG_NAMES)
foreach (_version ${_LLVM_KNOWN_VERSIONS})
  list(APPEND _LLVM_CONFIG_NAMES "llvm-config-${_version}")
endforeach()
list(APPEND _LLVM_CONFIG_NAMES "llvm-config")

# find llvm-config
find_program(LLVM_CONFIG_EXECUTABLE ${_LLVM_CONFIG_NAMES} DOC "llvm-config executable")

if (LLVM_CONFIG_EXECUTABLE)
  message(STATUS "LLVM `llvm-config` found at: ${LLVM_CONFIG_EXECUTABLE}")
else (LLVM_CONFIG_EXECUTABLE)
  message(FATAL_ERROR "Could NOT find `llvm-config` executable")
endif (LLVM_CONFIG_EXECUTABLE)

# version
execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --version
  OUTPUT_VARIABLE LLVM_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+).*" "\\1" LLVM_VERSION_MAJOR
	     "${LLVM_VERSION}")
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+).*" "\\2" LLVM_VERSION_MINOR
	     "${LLVM_VERSION}")
	     
# prefix / bindir
execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --prefix
  OUTPUT_VARIABLE LLVM_PREFIX
  OUTPUT_STRIP_TRAILING_WHITESPACE
) 
execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --bindir
  OUTPUT_VARIABLE LLVM_BINDIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)  

# includedir
execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --includedir
  OUTPUT_VARIABLE LLVM_INCLUDE_DIRS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# libdir
execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --libdir
  OUTPUT_VARIABLE LLVM_LIBRARY_DIRS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# cppflags
execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --cppflags
  OUTPUT_VARIABLE LLVM_CFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# libs
find_library(LLVM_MODULE_LIBS LLVM-${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR} ${LLVM_LIBRARY_DIRS})
if (NOT LLVM_MODULE_LIBS)
  execute_process(
    COMMAND ${LLVM_CONFIG_EXECUTABLE} --libs
    OUTPUT_VARIABLE LLVM_MODULE_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endif()

# ldflags
execute_process(
  COMMAND ${LLVM_CONFIG_EXECUTABLE} --ldflags
  OUTPUT_VARIABLE LLVM_LDFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# LLVM_FOUND
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  LLVM
  REQUIRED_VARS LLVM_CONFIG_EXECUTABLE LLVM_INCLUDE_DIRS LLVM_MODULE_LIBS LLVM_LDFLAGS
  VERSION_VAR LLVM_VERSION
)
