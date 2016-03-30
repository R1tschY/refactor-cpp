include(CheckCXXCompilerFlag)
include(CMakeParseArguments)
include(CMakeUtils)

# add_cxx_flags("flag ..." [variant])
# adds flags to CMAKE_CXX_FLAGS or CMAKE_CXX_FLAGS_VARIANT
function(add_cxx_flags _flags)
	set(_variant ${ARGV1})
  if(_variant)
    string(TOUPPER "_${_variant}" _variant)
  endif()
  set(CMAKE_CXX_FLAGS${_variant} "${CMAKE_CXX_FLAGS${_variant}} ${_flags}" PARENT_SCOPE)
endfunction()

# add_cxx_flag_checked("flag ..." [build_type] [REQUIRED])
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

# add_flags(
#   [CXX "--flag ..."] 
#   [C "--flag ..."] 
#   [CPP "-DDEFINE=0 ..."]
#   [BUILD_TYPE build_type]
#   [PARENT_SCOPE]
# )
function(add_flags)

	# options
	
  set(options )
  set(oneValueArgs CXX C CPP LD BUILD_TYPE)
  set(multiValueArgs )
  set(prefix _add_flags)
  cmake_parse_arguments(${prefix} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	# build type
	
	set(${prefix}_variant ${${prefix}_BUILD_TYPE})
  if(${prefix}_variant)
    string(TOUPPER "_${${prefix}_variant}" ${prefix}_variant)
  endif()
  
  # flags
  
  if (${prefix}_C)
	  append(CMAKE_C_FLAGS${${prefix}_variant} " ${${prefix}_C}")
	endif()
	
	if (${prefix}_CXX)
	  append(CMAKE_CXX_FLAGS${${prefix}_variant} " ${${prefix}_CXX}")
	endif()
	
	if (${prefix}_CPP)
  	append(CMAKE_C_FLAGS${${prefix}_variant} " ${${prefix}_CPP}")
  	append(CMAKE_CXX_FLAGS${${prefix}_variant} " ${${prefix}_CPP}")
  endif()
  
  if (${prefix}_LD)
  	append(CMAKE_EXE_LINKER_FLAGS${${prefix}_variant} " ${${prefix}_LD}")
  	append(CMAKE_SHARED_LINKER_FLAGS${${prefix}_variant} " ${${prefix}_LD}")
  	append(CMAKE_MODULE_LINKER_FLAGS${${prefix}_variant} " ${${prefix}_LD}")
  endif()
  
  # scope
  
  move_to_parent(CMAKE_C_FLAGS${${prefix}_variant})
	move_to_parent(CMAKE_CXX_FLAGS${${prefix}_variant})
	move_to_parent(CMAKE_EXE_LINKER_FLAGS${${prefix}_variant})
	move_to_parent(CMAKE_SHARED_LINKER_FLAGS${${prefix}_variant})
	move_to_parent(CMAKE_MODULE_LINKER_FLAGS${${prefix}_variant})
	
endfunction()



