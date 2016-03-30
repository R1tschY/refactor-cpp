

macro(append _var _arg)
	set(_var "${_var} ${_arg}" ${ARGN})
endmacro()
