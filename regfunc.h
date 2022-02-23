#pragma once
#include <angelscript.h>

extern asIScriptGeneric* curGen;

namespace tenshi_reg {
	void func_wrapper(asIScriptGeneric* gen);
}

namespace tenshi_func {
	int tenshi_register_function();
}