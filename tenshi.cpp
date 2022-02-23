
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
	//		���s���� (���ߎ��s���ɌĂ΂�܂�)
	//
	code_next();							// ���̃R�[�h���擾(�ŏ��ɕK���K�v�ł�)

	switch( cmd ) {							// �T�u�R�}���h���Ƃ̕���
		case 0x00: tenshi_cmd::tenshi_init();                break;
		case 0x01: tenshi_cmd::tenshi_set_message_handler(); break;
		case 0x02: tenshi_cmd::tenshi_shutdown();            break;
		default: puterror( HSPERR_UNSUPPORTED_FUNCTION );    break;
	}
	return RUNMODE_RUN;
}

/*------------------------------------------------------------*/

RefVal ref_val;						// �Ԓl�̂��߂̕ϐ�
char* ref_sval = NULL;

static void *reffunc( int *type_res, int cmd )
{
	//		�֐��E�V�X�e���ϐ��̎��s���� (�l�̎Q�Ǝ��ɌĂ΂�܂�)
	//

	int answerType = 0;

	//			'('�Ŏn�܂邩�𒲂ׂ�
	//
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != '(' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();

	switch( cmd ) {							// �T�u�R�}���h���Ƃ̕���
	    case 0x80: *type_res = tenshi_func::tenshi_load_module();       break;
		case 0x81: *type_res = tenshi_func::tenshi_exec_function();     break;
		case 0x82: *type_res = tenshi_func::tenshi_register_function(); break;
		case 0x83: *type_res = tenshi_func::tenshi_getarg_string();     break;
		case 0x84: *type_res = tenshi_func::tenshi_getarg_int();        break;
		case 0x85: *type_res = tenshi_func::tenshi_getarg_double();     break;
		default: puterror( HSPERR_UNSUPPORTED_FUNCTION );               break;
	}

	//			'('�ŏI��邩�𒲂ׂ�
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
	//		�I������ (�A�v���P�[�V�����I�����ɌĂ΂�܂�)
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
	//		DLL�G���g���[ (��������K�v�͂���܂���)
	//
	return TRUE;
}


EXPORT void WINAPI hsp3cmdinit( HSP3TYPEINFO *info )
{
	//		�v���O�C�������� (���s�E�I��������o�^���܂�)
	//
	hsp3sdk_init( info );		// SDK�̏�����(�ŏ��ɍs�Ȃ��ĉ�����)

    ref_sval = (char*)hspmalloc(64);

	info->cmdfunc = cmdfunc;		// ���s�֐�(cmdfunc)�̓o�^
	info->reffunc = reffunc;		// �Q�Ɗ֐�(reffunc)�̓o�^
	info->termfunc = termfunc;		// �I���֐�(termfunc)�̓o�^
}

/*------------------------------------------------------------*/

