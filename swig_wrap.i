%module c

%typemap(in) uint8 {
	$1 = (uint8)lua_tonumber(L,$input);
}

%typemap(in) uint16 {
	$1 = (uint16)lua_tonumber(L,$input);
}

%typemap(in) uint32 {
	$1 = (uint32)lua_tonumber(L,$input);
}

%typemap(in) uint64 {
	$1 = (uint64)lua_tonumber(L,$input);
}

%typemap(out) uint8 {
	lua_pushinteger(L,$1);SWIG_arg++;
}

%typemap(out) uint16 {
	lua_pushinteger(L,$1);SWIG_arg++;
}

%typemap(out) uint32 {
	lua_pushinteger(L,$1);SWIG_arg++;
}

%typemap(out) uint64 {
	lua_pushinteger(L,$1);SWIG_arg++;
}

%{
#include "script_test.h"
%}

%include "std_string.i"
%include "script_test.h"
