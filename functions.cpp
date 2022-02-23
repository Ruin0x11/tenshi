#include "functions.h"
#include "tenshi.h"
#include "hsp/hsp3plugin.h"

int tenshi_func::tenshi_set_default_namespace() {
	ref_val.ival = engine->SetDefaultNamespace(exinfo->HspFunc_prm_gets());
	return HSPVAR_FLAG_INT;
}