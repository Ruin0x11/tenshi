#include "getsetvar.h"
#include "tenshi.h"
#include "hsp/hsp3plugin.h"
#include <string>
#include <scriptany/scriptany.h>
#include <scriptstdstring/scriptstdstring.h>

char buffer[1024];

// bool @tenshi_getvar(const string &in, ?&out) const
void tenshi_reg::tenshi_getvar(asIScriptGeneric* gen) {
    std::string* varname = static_cast<std::string*>(gen->GetArgAddress(0));
    void* ref = (void*)gen->GetArgAddress(1);
    int refTypeId = gen->GetArgTypeId(1);
    bool* ret = (bool*)gen->GetAddressOfReturnLocation();
    std::string* s;
    int* i;
    double* d;

    *ret = false;

    int index = exinfo->HspFunc_seekvar(varname->c_str());
    if (index == -1) {
        return;
    }

    PVal* val = &ctx->mem_var[index];
    switch (val->flag) {
    case HSPVAR_FLAG_STR:
        if (refTypeId == engine->GetTypeIdByDecl("string")) {
            *ret = true;
            s = reinterpret_cast<std::string*>(ref);
            *s = std::string(val->pt);
        }
        break;
    case HSPVAR_FLAG_INT:
        if (refTypeId & asTYPEID_INT32) { 
            *ret = true;
            i = reinterpret_cast<int*>(ref);
            *i = *(int*)val->pt;
        }
        break;
    case HSPVAR_FLAG_DOUBLE:
        if (refTypeId & asTYPEID_DOUBLE) {
            *ret = true;
            d = reinterpret_cast<double*>(ref);
            *d = *(double*)val->pt;
        }
        break;
    case HSPVAR_FLAG_LABEL:
        if (refTypeId & asTYPEID_INT32) {
            *ret = true;
            unsigned short label = *(unsigned short*)val->pt;
            i = reinterpret_cast<int*>(ref);
            *i = label;
        }
        break;
    default:
        break;
    }

    return;
}

// void tn_setvar(const string &in, const string &in)
void tenshi_reg::tenshi_setvar_string(asIScriptGeneric* gen) {
   std::string* varname = static_cast<std::string*>(gen->GetArgAddress(0));
   std::string* val = static_cast<std::string*>(gen->GetArgAddress(1));
   char* c = NULL;

   int index = exinfo->HspFunc_seekvar(varname->c_str());
   if (index == -1) {
       sprintf_s(buffer, 1024, "Could not find variable with name '%s'", varname->c_str());
       asGetActiveContext()->SetException(buffer, true);
       return;
   }

    PVal* pvResult = &ctx->mem_var[index];
    switch (pvResult->flag) {
    case HSPVAR_FLAG_STR:
        c = new char[val->length() + 1];
        strcpy(c, val->c_str());
        pvResult->pt = c;
        break;
    default:
        sprintf_s(buffer, 1024, "Could not set HSP type %d to value of type 'string'", pvResult->flag);
        asGetActiveContext()->SetException(buffer, true);
        break;
    }
}

// void tn_setvar(const string &in, int)
void tenshi_reg::tenshi_setvar_int(asIScriptGeneric* gen) {
    std::string* varname = static_cast<std::string*>(gen->GetArgAddress(0));
    int val = gen->GetArgDWord(1);

    int index = exinfo->HspFunc_seekvar(varname->c_str());
    if (index == -1) {
        sprintf_s(buffer, 1024, "Could not find variable with name '%s'", varname->c_str());
        asGetActiveContext()->SetException(buffer, true);
        return;
    }

    PVal* pvResult = &ctx->mem_var[index];
    switch (pvResult->flag) {
    case HSPVAR_FLAG_INT:
        *(int*)pvResult->pt = val;
        break;
    default:
        sprintf_s(buffer, 1024, "Could not set HSP type %d to value of type 'int32'", pvResult->flag);
        asGetActiveContext()->SetException(buffer, true);
        break;
    }
}

// void tn_setvar(const string &in, double)
void tenshi_reg::tenshi_setvar_double(asIScriptGeneric* gen) {
    std::string* varname = static_cast<std::string*>(gen->GetArgAddress(0));
    double val = gen->GetArgDouble(1);

    int index = exinfo->HspFunc_seekvar(varname->c_str());
    if (index == -1) {
        sprintf_s(buffer, 1024, "Could not find variable with name '%s'", varname->c_str());
        asGetActiveContext()->SetException(buffer, true);
        return;
    }

    PVal* pvResult = &ctx->mem_var[index];
    switch (pvResult->flag) {
    case HSPVAR_FLAG_DOUBLE:
        *(double*)pvResult->pt = val;
        break;
    default:
        sprintf_s(buffer, 1024, "Could not set HSP type %d to value of type 'double'", pvResult->flag);
        asGetActiveContext()->SetException(buffer, true);
        break;
    }
}