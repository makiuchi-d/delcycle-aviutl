/*********************************************************************
* 	AviUtl 0.98�n��p �����Ԉ��� �v���O�C��
* 
* [2004]
* 	01/26:	�e�X�g����
* 	02/05:	���J�i0.01�j
* 
*********************************************************************/
#include <windows.h>
#include "filter.h"


//----------------------------
//	prototype
//----------------------------


//----------------------------
//	FILTER_DLL�\����
//----------------------------
char filter_name[] = "�����Ԉ���";
char filter_info[] = "�����Ԉ��� ver0.01 by MakKi";
#define track_N 0
#if track_N
TCHAR *track_name[]   = { 0 };	// �g���b�N�o�[�̖��O
int   track_default[] = { 0 };	// �g���b�N�o�[�̏����l
int   track_s[]       = { 0 };	// �g���b�N�o�[�̉����l
int   track_e[]       = { 0 };	// �g���b�N�o�[�̏���l
#endif
#define check_N 1
#if check_N
TCHAR *check_name[]   = { "����4���Ԉ����悤�ɂ���" };	// �`�F�b�N�{�b�N�X
int   check_default[] = { 0 };	// �f�t�H���g
#endif

#define cDEL4   0


FILTER_DLL filter = {
	FILTER_FLAG_INTERLACE_FILTER |	// �C���^�[���[�X�����t�B���^�ɂ��܂�
	FILTER_FLAG_EX_INFORMATION,
	0,0,				// �ݒ�E�C���h�E�̃T�C�Y
	filter_name,		// �t�B���^�̖��O
	track_N,        	// �g���b�N�o�[�̐�
#if track_N
	track_name,     	// �g���b�N�o�[�̖��O�S
	track_default,  	// �g���b�N�o�[�̏����l�S
	track_s,track_e,	// �g���b�N�o�[�̐��l�̉������
#else
	NULL,NULL,NULL,NULL,
#endif
	check_N,      	// �`�F�b�N�{�b�N�X�̐�
#if check_N
	check_name,   	// �`�F�b�N�{�b�N�X�̖��O�S
	check_default,	// �`�F�b�N�{�b�N�X�̏����l�S
#else
	NULL,NULL,
#endif
	func_proc,   	// �t�B���^�����֐�
	NULL,NULL,   	// �J�n��,�I�����ɌĂ΂��֐�
	NULL,        	// �ݒ肪�ύX���ꂽ�Ƃ��ɌĂ΂��֐�
	func_WndProc,	// �ݒ�E�B���h�E�v���V�[�W��
	NULL,NULL,   	// �V�X�e���Ŏg�p
	NULL,NULL,     	// �g���f�[�^�̈�
	filter_info,	// �t�B���^���
	NULL,			// �Z�[�u�J�n���O�ɌĂ΂��֐�
	NULL,			// �Z�[�u�I�����ɌĂ΂��֐�
	NULL,NULL,NULL,	// �V�X�e���Ŏg�p
	NULL,			// �g���̈揉���l
	func_is_saveframe,	// �ۑ�����t���[�������߂鎞�ɌĂ΂��֐�
};

/*********************************************************************
*	DLL Export
*********************************************************************/
EXTERN_C FILTER_DLL __declspec(dllexport) * __stdcall GetFilterTable( void )
{
	return &filter;
}

/*====================================================================
*	�t�B���^�����֐�
*===================================================================*/
BOOL func_proc(FILTER *fp,FILTER_PROC_INFO *fpip)
{
	PIXEL_YC *ycp = (PIXEL_YC *)fp->exfunc->get_ycp_source_cache(fpip->editp,fpip->frame,0);
	memcpy(fpip->ycp_edit,ycp,fpip->max_w*fpip->h*sizeof(PIXEL_YC));
	return TRUE;
}

/*====================================================================
*	�ۑ�����t���[�������߂鎞�ɌĂ΂��֐�
*===================================================================*/
BOOL func_is_saveframe(FILTER *fp,void *editp,int saveno,int frame,int fps,int edit_flag,int inter)
{
	FRAME_STATUS fs[5];
	int i;
	char str[32];

	fp->exfunc->get_frame_status(editp,frame,&fs[0]);

	if(fp->exfunc->is_filter_window_disp(fp)){
		wsprintf(str,"�����Ԉ��� ����:%d",fs[0].index24fps);
		SetWindowText(fp->hwnd,str);
	}

	if(fps!=24) return !!(frame*fps/30 - (frame+1)*fps/30);

	if(edit_flag & EDIT_FRAME_EDIT_FLAG_DELFRAME) return FALSE;

	fp->exfunc->get_frame_status(editp,frame+1,&fs[1]);
	fp->exfunc->get_frame_status(editp,frame+2,&fs[2]);
	fp->exfunc->get_frame_status(editp,frame-1,&fs[3]);
	fp->exfunc->get_frame_status(editp,frame-2,&fs[4]);

	for(i=4;i;--i){
		if(fs[i].edit_flag & EDIT_FRAME_EDIT_FLAG_DELFRAME)
			return TRUE;	// �O��2�t���[���ɊԈ����D�悪����Ƃ�
	}

	return fp->check[cDEL4] ? (fs[0].index24fps!=4) : (fs[0].index24fps!=0);
}

/*====================================================================
*	�ݒ�E�B���h�E�v���V�[�W��
*===================================================================*/
BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void *editp, FILTER *fp)
{
	switch(message){
		case WM_KEYUP:	// ���C���E�B���h�E�֑���
		case WM_KEYDOWN:
		case WM_MOUSEWHEEL:
			SendMessage(GetWindow(hwnd, GW_OWNER), message, wparam, lparam);
			break;
	}

	return FALSE;
}
