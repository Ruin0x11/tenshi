
//
//		TENSHI
//

#include <windows.h>
#include "tenshi.h"
#include "hsp/hsp3plugin.h"
#include "hsp/hspvar_core.h"
#include "commands.h"
#include "exec.h"
#include "regfunc.h"
#include "getarg.h"
#include <cstdlib>

 /*------------------------------------------------------------*/
/*
		controller
*/
/*------------------------------------------------------------*/

asIScriptEngine* engine = NULL;

static int cmdfunc( int cmd )
{
	//		実行処理 (命令実行時に呼ばれます)
	//
	code_next();							// 次のコードを取得(最初に必ず必要です)

	switch( cmd ) {							// サブコマンドごとの分岐
		case 0x00: tenshi_cmd::tenshi_init();                break;
		case 0x01: tenshi_cmd::tenshi_set_message_handler(); break;
		case 0x02: tenshi_cmd::tenshi_shutdown();            break;
		default: puterror( HSPERR_UNSUPPORTED_FUNCTION );    break;
	}
	return RUNMODE_RUN;
}

/*------------------------------------------------------------*/

RefVal ref_val;						// 返値のための変数
char* ref_sval = NULL;

static void *reffunc( int *type_res, int cmd )
{
	//		関数・システム変数の実行処理 (値の参照時に呼ばれます)
	//

	int answerType = 0;

	//			'('で始まるかを調べる
	//
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != '(' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();

	switch( cmd ) {							// サブコマンドごとの分岐
	    case 0x80: *type_res = tenshi_func::tenshi_load_module();       break;
		case 0x81: *type_res = tenshi_func::tenshi_exec_function();     break;
		case 0x82: *type_res = tenshi_func::tenshi_register_function(); break;
		case 0x83: *type_res = tenshi_func::tenshi_getarg_string();     break;
		case 0x84: *type_res = tenshi_func::tenshi_getarg_int();        break;
		case 0x85: *type_res = tenshi_func::tenshi_getarg_double();     break;
		default: puterror( HSPERR_UNSUPPORTED_FUNCTION );               break;
	}

	//			'('で終わるかを調べる
	//
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != ')' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();

    if (*type_res == HSPVAR_FLAG_STR) {
        return (void*)ref_val.sval;
    }
    else {
        return (void *)&ref_val;
    }
}

/*------------------------------------------------------------*/

static int termfunc( int option )
{
	//		終了処理 (アプリケーション終了時に呼ばれます)
	//
	return 0;
}

/*------------------------------------------------------------*/
/*
		interface
*/
/*------------------------------------------------------------*/

int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	//		DLLエントリー (何もする必要はありません)
	//
	return TRUE;
}


EXPORT void WINAPI hsp3cmdinit( HSP3TYPEINFO *info )
{
	//		プラグイン初期化 (実行・終了処理を登録します)
	//
	hsp3sdk_init( info );		// SDKの初期化(最初に行なって下さい)

    ref_sval = (char*)hspmalloc(64);

	info->cmdfunc = cmdfunc;		// 実行関数(cmdfunc)の登録
	info->reffunc = reffunc;		// 参照関数(reffunc)の登録
	info->termfunc = termfunc;		// 終了関数(termfunc)の登録
}

/*------------------------------------------------------------*/

