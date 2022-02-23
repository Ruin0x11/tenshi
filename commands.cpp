#include "commands.h"
#include "tenshi.h"
#include "regfunc.h"
#include "getsetvar.h"
#include "hsp/hsp3plugin.h"
#include <scriptstdstring/scriptstdstring.h>
#include <scriptany/scriptany.h>
#include <assert.h>

std::string hsp_handler_decl = "";

// Implement a simple message callback function
void message_handler(const asSMessageInfo* msg, void* param) {
	const char* type = "ERR ";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARN";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFO";
	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
	if (hsp_handler_decl != "") {
		asIScriptContext* ctx = engine->CreateContext();
		asIScriptFunction* func = engine->GetGlobalFunctionByDecl(hsp_handler_decl.c_str());
		ctx->Prepare(func);
		std::string s(msg->message);
		ctx->SetArgAddress(0, &s);
		ctx->Execute();
		ctx->Release();
	}
}

void translate_exception(asIScriptContext* ctx, void* userParam)
{
	try
	{
		// Rethrow the original exception so we can catch it again
		throw;
	}
	catch (std::exception& e)
	{
		// Tell the VM the type of exception that occurred
		ctx->SetException(e.what());
	}
	catch (...)
	{
		// The callback must not allow any exception to be thrown, but it is not necessary 
		// to explicitly set an exception string if the default exception string is sufficient
	}
}

// Print the script string to the standard output stream
void print(std::string& msg) {
	printf("%s", msg.c_str());
}

void tenshi_cmd::tenshi_init() {
	// Create the script engine
	engine = asCreateScriptEngine();

	// Set the message callback to receive information on errors in human readable form.
	int r = engine->SetMessageCallback(asFUNCTION(message_handler), 0, asCALL_CDECL); assert(r >= 0);
	r = engine->SetTranslateAppExceptionCallback(asFUNCTION(translate_exception), 0, asCALL_CDECL); assert(r >= 0);

	// AngelScript doesn't have a built-in string type, as there is no definite standard 
	// string type for C++ applications. Every developer is free to register its own string type.
	// The SDK do however provide a standard add-on for registering a string type, so it's not
	// necessary to implement the registration yourself if you don't want to.
	RegisterStdString(engine);

	RegisterScriptAny(engine);

	r = engine->SetDefaultNamespace("tenshi"); assert(r >= 0);

	// Register the function that we want the scripts to call 
	r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL); assert(r >= 0);

	r = engine->RegisterGlobalFunction("bool getvar(const string &in, ?&out)", asFUNCTION(tenshi_reg::tenshi_getvar), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterGlobalFunction("void setvar(const string &in, const string &in)", asFUNCTION(tenshi_reg::tenshi_setvar_string), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterGlobalFunction("void setvar(const string &in, int)", asFUNCTION(tenshi_reg::tenshi_setvar_int), asCALL_GENERIC); assert(r >= 0);
	r = engine->RegisterGlobalFunction("void setvar(const string &in, double)", asFUNCTION(tenshi_reg::tenshi_setvar_double), asCALL_GENERIC); assert(r >= 0);
}

void tenshi_cmd::tenshi_set_message_handler() {
	hsp_handler_decl = std::string(exinfo->HspFunc_prm_gets());
}

void tenshi_cmd::tenshi_shutdown() {
	engine->ShutDownAndRelease();
}