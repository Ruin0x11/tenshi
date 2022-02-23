#ifndef __TENSHI_H
#define __TENSHI_H

#include <angelscript.h>
#include <vector>

extern asIScriptEngine* engine;

extern union RefVal {
	int ival;
	double dval;
	char* sval;
} ref_val;
extern char* ref_sval;

#endif
