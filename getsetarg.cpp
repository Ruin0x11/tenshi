#include "getsetarg.h"
#include "tenshi.h"
#include "regfunc.h"
#include "hsp/hsp3plugin.h"
#include <string>

int tenshi_func::tenshi_getarg_string() {
	if (curGen == NULL) throw HSPERR_ILLEGAL_FUNCTION;
	std::string* str = reinterpret_cast<std::string*>(curGen->GetArgAddress(exinfo->HspFunc_prm_geti()));
	size_t strsz = str->size();
	const char* const tmpstr = str->c_str();
	char* const tmp_sval = ref_sval; // reallocŽ¸”s‘Îô
	ref_sval = hspexpand(ref_sval, strsz + 1);
	if (ref_sval == NULL) { // realloc Ž¸”sŽž
		ref_sval = tmp_sval;
		throw HSPERR_OUT_OF_MEMORY;
	}
	strncpy(ref_sval, tmpstr, strsz + 1);
	ref_sval[strsz] = '\0'; // ˆÀ‘S‚Ì‚½‚ß
	ref_val.sval = ref_sval;
	return HSPVAR_FLAG_STR;
}

int tenshi_func::tenshi_getarg_int() {
	if (curGen == NULL) throw HSPERR_ILLEGAL_FUNCTION;
	ref_val.ival = curGen->GetArgDWord(exinfo->HspFunc_prm_geti());
	return HSPVAR_FLAG_INT;
}

int tenshi_func::tenshi_getarg_double() {
	if (curGen == NULL) throw HSPERR_ILLEGAL_FUNCTION;
	ref_val.ival = curGen->GetArgDouble(exinfo->HspFunc_prm_getd());
	return HSPVAR_FLAG_DOUBLE;
}

int tenshi_func::tenshi_setret_string() {
	if (curGen == NULL) throw HSPERR_ILLEGAL_FUNCTION;
	std::string* s = reinterpret_cast<std::string*>(curGen->GetAddressOfReturnLocation());
	*s = exinfo->HspFunc_prm_gets();
	ref_val.ival = true;
	return HSPVAR_FLAG_STR;
}

int tenshi_func::tenshi_setret_int() {
	if (curGen == NULL) throw HSPERR_ILLEGAL_FUNCTION;
	ref_val.ival = curGen->SetReturnDWord(exinfo->HspFunc_prm_geti());
	return HSPVAR_FLAG_INT;
}

int tenshi_func::tenshi_setret_double() {
	if (curGen == NULL) throw HSPERR_ILLEGAL_FUNCTION;
	ref_val.dval = curGen->SetReturnDouble(exinfo->HspFunc_prm_getd());
	return HSPVAR_FLAG_DOUBLE;
}