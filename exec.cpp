#include "exec.h"
#include "tenshi.h"
#include "hsp/hsp3plugin.h"
#include <scriptbuilder/scriptbuilder.h>

int tenshi_func::tenshi_load_module() {

    // The CScriptBuilder helper is an add-on that loads the file,
    // performs a pre-processing pass if necessary, and then tells
    // the engine to build a script module.
    CScriptBuilder builder;
    const char* modname = exinfo->HspFunc_prm_gets();
    ref_val.ival = builder.StartNewModule(engine, modname);
    if (ref_val.ival < 0)
    {
        // If the code fails here it is usually because there
        // is no more memory to allocate the module
        printf("Unrecoverable error while starting a new module.\n");
        goto end;
    }

    const char* filename = exinfo->HspFunc_prm_gets();
    ref_val.ival = builder.AddSectionFromFile(filename);
    if (ref_val.ival < 0)
    {
        // The builder wasn't able to load the file. Maybe the file
        // has been removed, or the wrong name was given, or some
        // preprocessing commands are incorrectly written.
        printf("Please correct the errors in the script and try again.\n");
        goto end;
    }

    ref_val.ival = builder.BuildModule();
    if (ref_val.ival < 0)
    {
        // An error occurred. Instruct the script writer to fix the 
        // compilation errors that were listed in the output stream.
        printf("Please correct the errors in the script and try again.\n");
        goto end;
    }

end:
    return HSPVAR_FLAG_INT;
}

int tenshi_func::tenshi_exec_function() {
    std::string modname(exinfo->HspFunc_prm_gets());
    std::string funcdecl(exinfo->HspFunc_prm_gets());

    // Find the function that is to be called. 
    asIScriptModule* mod = engine->GetModule(modname.c_str());
    asIScriptFunction* func = mod->GetFunctionByDecl(funcdecl.c_str());
    if (func == 0)
    {
        // The function couldn't be found. Instruct the script writer
        // to include the expected function in the script.
        printf("The script module '%s' must have the function '%s'. Please add it and try again.\n", modname.c_str(), funcdecl.c_str());
        ref_val.ival = -1;
        return HSPVAR_FLAG_INT;
    }

    // Create our context, prepare it, and then execute
    asIScriptContext* ctx = engine->CreateContext();
    ctx->Prepare(func);
    ref_val.ival = ctx->Execute();
    if (ref_val.ival != asEXECUTION_FINISHED)
    {
        // The execution didn't complete as expected. Determine what happened.
        if (ref_val.ival == asEXECUTION_EXCEPTION)
        {
            // An exception occurred, let the script writer know what happened so it can be corrected.
            printf("%s (%s): An exception '%s' occurred. Please correct the code and try again.\n", funcdecl.c_str(), modname.c_str(), ctx->GetExceptionString());
        }
    }
    ctx->Release();
    return HSPVAR_FLAG_INT;
}