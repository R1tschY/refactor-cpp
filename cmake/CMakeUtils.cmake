

macro(append _var _arg)
	set(${_var} "${${_var}}${_arg}" ${ARGN})
endmacro()

macro(move_to_parent _var)
	set(${_var} "${${_var}}" PARENT_SCOPE)
endmacro()
