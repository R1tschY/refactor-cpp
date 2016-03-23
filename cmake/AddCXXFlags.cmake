include(CheckCXXCompilerFlag)
include(CMakeParseArguments)

# add_cxx_flags("flag ..." [variant])
# adds flags to CMAKE_CXX_FLAGS or CMAKE_CXX_FLAGS_VARIANT
function(add_cxx_flags _flags)
	set(_variant ${ARGV1})
  if(_variant)
    string(TOUPPER "_${_variant}" _variant)
  endif()
  set(CMAKE_CXX_FLAGS${_variant} "${CMAKE_CXX_FLAGS${_variant}} ${_flag}" PARENT_SCOPE)
endfunction()

# add_cxx_flag_checked("flag ..." [variant] [REQUIRED])
# add c++ compiler flag if compiler support it.
#
# if the flag is supported CMAKE_CXX_FLAGS is updated and HAVE_CXX_FLAG_<flag ...> is set
# if variant is set CMAKE_CXX_FLAGS_VARIANT is updated.
# if REQUIRED is set: an error is show when the flag is not supported
function(add_cxx_flag_checked _flag)
  string(TOUPPER "HAVE_CXX_FLAG_${_flag}" _haveFlagDef)
  string(REPLACE "+" "X" _haveFlagDef ${_haveFlagDef})
  string(REGEX REPLACE "[^A-Za-z0-9]+" "_" _haveFlagDef ${_haveFlagDef})
  
  set(prefix _add_cxx_flag_checked)
  cmake_parse_arguments(${prefix} "REQUIRED" "" "" ${ARGN})

  check_cxx_compiler_flag("${_flag}" ${_haveFlagDef})
  if(${_haveFlagDef})
    set(VARIANT ${ARGV1})
    if(ARGV1)
      string(TOUPPER "_${VARIANT}" VARIANT)
    endif()
    set(CMAKE_CXX_FLAGS${VARIANT} "${CMAKE_CXX_FLAGS${VARIANT}} ${_flag}" PARENT_SCOPE)
    
  elseif(${prefix}_REQUIRED)
		message(ERROR "required flag `${_flag}' is not supported by c++ compiler")
  endif()
endfunction()
