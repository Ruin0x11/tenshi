#include "regfunc.h"
#include "tenshi.h"
#include "hsp/hsp3plugin.h"
#include <scriptany/scriptany.h>
#include <string>

asIScriptGeneric* curGen = NULL;

// The function has been registered with signature:
//  any @func(int)
void tenshi_reg::func_wrapper(asIScriptGeneric* gen)
{
    std::string s;
    curGen = gen;

	// Extract the arguments
    unsigned short* label = (unsigned short*)gen->GetAuxiliary();

	// Call the real function
    code_call(label);
    if (ctx->retval_level == (ctx->sublev + 1)) {
        PVal* pvResult = *(exinfo->mpval);
        switch (pvResult->flag) {
        case HSPVAR_FLAG_INT:
            gen->SetReturnDWord(*(int*)pvResult->pt);
            break;
        case HSPVAR_FLAG_DOUBLE:
            gen->SetReturnDouble(*(double*)pvResult->pt);
            break;
        case HSPVAR_FLAG_STR:
            s = std::string((char*)pvResult->pt);
            gen->SetReturnObject(&s);
            break;
        default:
            gen->SetReturnObject(new CScriptAny(engine));
            break;
        }
    }

    curGen = NULL;
}

int tenshi_func::tenshi_register_function() {
    const char* sig = exinfo->HspFunc_prm_gets();
    unsigned short* label = exinfo->HspFunc_prm_getlb();
    ref_val.ival = engine->RegisterGlobalFunction(sig, asFUNCTION(tenshi_reg::func_wrapper), asCALL_GENERIC, label);
    return HSPVAR_FLAG_INT;
}