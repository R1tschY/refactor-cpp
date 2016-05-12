include(FindPackageHandleStandardArgs)

if (NOT LLVM_FOUND)
   message(FATAL_ERROR "Clang support requires LLVM and no LLVM Support")
   return()
endif()

# components
if (Clang_FIND_COMPONENTS)
  foreach (_comp ${Clang_FIND_COMPONENTS})
    set(_libname_ "clang${_comp}")
    find_library(Clang_${_libname_}_LIB ${_libname_} ${LLVM_LIBRARY_DIRS} ${Clang_LIBRARY_DIRS})
    if (Clang_${_libname_}_LIB)
       set(Clang_LIBS ${Clang_LIBS} ${Clang_${_libname_}_LIB})
       set(Clang_${_comp}_FOUND TRUE)
    endif()
  endforeach()
else()
  message(ERROR "No Clang components given")
endif()

# Clang_INCLUDE_DIRS
find_path(Clang_INCLUDE_DIRS clang/Basic/Version.h HINTS ${LLVM_INCLUDE_DIRS})

# FIND_PACKAGE_HANDLE_STANDARD_ARGS
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  Clang
  REQUIRED_VARS Clang_INCLUDE_DIRS Clang_LIBS
  VERSION_VAR LLVM_VERSION
  HANDLE_COMPONENTS
)
