#pragma once
#include <angelscript.h>

namespace tenshi_reg {
	void tenshi_getvar(asIScriptGeneric* gen);
	void tenshi_setvar_string(asIScriptGeneric* gen);
	void tenshi_setvar_int(asIScriptGeneric* gen);
	void tenshi_setvar_double(asIScriptGeneric* gen);
}