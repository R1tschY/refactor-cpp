include(CMakeParseArguments)
include(AddCXXFlags)

# add hardening flags for gcc / clang
# hardening([ALL]
#           [ENABLE feature ...]
#           [DISABLE feature ...]
#           [BUILD_TYPE build_type]
# )
# supported features: FORMAT FORTIFY STACKPROTECTOR PIE RELRO BINDNOW
# 
# see https://wiki.debian.org/Hardening
function(hardening)
	
  # options
	
  set(options ALL)
  set(oneValueArgs BUILD_TYPE)
  set(multiValueArgs ENABLE DISABLE)
  set(prefix _add_hardening)
  cmake_parse_arguments(${prefix} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  
  # build type
  
  set(_build_type_arg)
  if (${prefix}_VARIANT)
  	set(_build_type_arg BUILD_TYPE ${prefix}_BUILD_TYPE)
  endif()
  
  # feature list
  
  set(_features FORMAT FORTIFY STACKPROTECTOR PIE RELRO BINDNOW)
  
  # identify features to use
  
  set(_enabled )
  if (${prefix}_ALL)
  	set(_enabled ${_features})
 	endif()
 	if (${prefix}_ENABLE)
 	  string(TOUPPER "${${prefix}_ENABLE}" ${prefix}_ENABLE)
 		list(APPEND _enabled ${${prefix}_ENABLE})
 	endif()
 	list(REMOVE_DUPLICATES _enabled)
 	if (${prefix}_DISABLE)
	 	string(TOUPPER "${${prefix}_DISABLE}" ${prefix}_DISABLE)
 		list(REMOVE_ITEM _enabled ${${prefix}_DISABLE})
 	endif()
 	
 	# enable features
 	
 	foreach(_e ${_enabled})
  
		if (_e STREQUAL "FORMAT")
			add_flags(
				C "-Wformat -Wformat-security -Werror=format-security"
				CXX "-Wformat -Wformat-security -Werror=format-security"
				${_build_type_arg}
			)
			
		elseif (_e STREQUAL "FORTIFY")
			add_flags(
				CPP "-D_FORTIFY_SOURCE=2"
				${_build_type_arg}
			)
			
		elseif (_e STREQUAL "STACKPROTECTOR")
		  check_cxx_compiler_flag("-fstack-protector-strong" HAVE_STRONG_STACK_PROTECTOR)
		  if (HAVE_STRONG_STACK_PROTECTOR)
		  	add_flags(
			  	C "-fstack-protector-strong"
					CXX "-fstack-protector-strong"
					${_build_type_arg}
				)
		  else()
		  	add_flags(
			  	C "-fstack-protector --param ssp-buffer-size=4"
					CXX "-fstack-protector --param ssp-buffer-size=4"
					${_build_type_arg}
				)
		  endif()

		elseif (_e STREQUAL "PIE")
			add_flags(
				C "-fPIE"
				CXX "-fPIE"
				LD "-pie"
				${_build_type_arg}
			)

		elseif (_e STREQUAL "RELRO")
			add_flags(
				LD "-Wl,-z,relro"
				${_build_type_arg}
			)
		  
		elseif (_e STREQUAL "BINDNOW")
			add_flags(
				LD "-Wl,-z,now"
				${_build_type_arg}
			)
		
		else()
			message(WARNING "${_e} is not a known hardening feature")
		endif()  
  endforeach()	
  
  # scope
  
  move_to_parent(CMAKE_C_FLAGS${${prefix}_variant})
	move_to_parent(CMAKE_CXX_FLAGS${${prefix}_variant})
	move_to_parent(CMAKE_EXE_LINKER_FLAGS${${prefix}_variant})
	move_to_parent(CMAKE_SHARED_LINKER_FLAGS${${prefix}_variant})
	move_to_parent(CMAKE_MODULE_LINKER_FLAGS${${prefix}_variant})
endfunction()
