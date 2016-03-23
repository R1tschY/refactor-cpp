include(CMakeParseArguments)

function(add_config _name)
  set(oneValueArgs COPY_FROM CXX_FLAGS LINKER_FLAGS EXE_LINKER_FLAGS SHARED_LINKER_FLAGS MODULE_LINKER_FLAGS STATIC_LINKER_FLAGS)
  set(prefix _add_config)
  cmake_parse_arguments(${prefix} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  
  string(TOUPPER "${_name}" _nameUpper)
  
  # copy flags from config from argument
  if (${prefix}_COPY_FROM)
    string(TOUPPER "${${prefix}_COPY_FROM}" _copyFrom)
		set(CMAKE_CXX_FLAGS_${_nameUpper} "${CMAKE_CXX_FLAGS_${_copyFrom}}" PARENT_SCOPE)
		set(CMAKE_EXE_LINKER_FLAGS_${_nameUpper} "${CMAKE_EXE_LINKER_FLAGS_${_copyFrom}}" PARENT_SCOPE)
		set(CMAKE_SHARED_LINKER_FLAGS_${_nameUpper} "${CMAKE_SHARED_LINKER_FLAGS_${_copyFrom}}" PARENT_SCOPE)
		set(CMAKE_MODULE_LINKER_FLAGS_${_nameUpper} "${CMAKE_MODULE_LINKER_FLAGS_${_copyFrom}}" PARENT_SCOPE)
		set(CMAKE_STATIC_LINKER_FLAGS_${_nameUpper} "${CMAKE_STATIC_LINKER_FLAGS_${_copyFrom}}" PARENT_SCOPE) 
	else()
		set(CMAKE_CXX_FLAGS_${_nameUpper} "" PARENT_SCOPE)
		set(CMAKE_EXE_LINKER_FLAGS_${_nameUpper} "" PARENT_SCOPE)
		set(CMAKE_SHARED_LINKER_FLAGS_${_nameUpper} "" PARENT_SCOPE)
		set(CMAKE_MODULE_LINKER_FLAGS_${_nameUpper} "" PARENT_SCOPE)
		set(CMAKE_STATIC_LINKER_FLAGS_${_nameUpper} "" PARENT_SCOPE) 
	endif()
  
  # set flags from arguments
  macro(_add_flags _flagsName _flags)
  	set(CMAKE_${_flagsName}_${_nameUpper} "${CMAKE_${_flagsName}_${_nameUpper}} ${_flags}" PARENT_SCOPE)
  endmacro()
  macro(_optional_add_flags _flagsName)
  	if (${prefix}_${_flagsName})
			_add_flags(${_flagsName} "${${prefix}_${_flagsName}}")
		endif()
  endmacro()
  
  _optional_add_flags(CXX_FLAGS)
  _optional_add_flags(EXE_LINKER_FLAGS)
  _optional_add_flags(SHARED_LINKER_FLAGS)
  _optional_add_flags(MODULE_LINKER_FLAGS)
  _optional_add_flags(STATIC_LINKER_FLAGS)
  
  if (${prefix}_LINKER_FLAGS)
  	_add_flags(EXE_LINKER_FLAGS "${${prefix}_LINKER_FLAGS}")
  	_add_flags(SHARED_LINKER_FLAGS "${${prefix}_LINKER_FLAGS}")
  	_add_flags(MODULE_LINKER_FLAGS "${${prefix}_LINKER_FLAGS}")
  endif()
	
endfunction()
