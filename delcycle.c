/*********************************************************************
* 	AviUtl 0.98系専用 周期間引き プラグイン
* 
* [2004]
* 	01/26:	テスト完了
* 	02/05:	公開（0.01）
* 
*********************************************************************/
#include <windows.h>
#include "filter.h"


//----------------------------
//	prototype
//----------------------------


//----------------------------
//	FILTER_DLL構造体
//----------------------------
char filter_name[] = "周期間引き";
char filter_info[] = "周期間引き ver0.01 by MakKi";
#define track_N 0
#if track_N
TCHAR *track_name[]   = { 0 };	// トラックバーの名前
int   track_default[] = { 0 };	// トラックバーの初期値
int   track_s[]       = { 0 };	// トラックバーの下限値
int   track_e[]       = { 0 };	// トラックバーの上限値
#endif
#define check_N 1
#if check_N
TCHAR *check_name[]   = { "周期4を間引くようにする" };	// チェックボックス
int   check_default[] = { 0 };	// デフォルト
#endif

#define cDEL4   0


FILTER_DLL filter = {
	FILTER_FLAG_INTERLACE_FILTER |	// インターレース解除フィルタにします
	FILTER_FLAG_EX_INFORMATION,
	0,0,				// 設定ウインドウのサイズ
	filter_name,		// フィルタの名前
	track_N,        	// トラックバーの数
#if track_N
	track_name,     	// トラックバーの名前郡
	track_default,  	// トラックバーの初期値郡
	track_s,track_e,	// トラックバーの数値の下限上限
#else
	NULL,NULL,NULL,NULL,
#endif
	check_N,      	// チェックボックスの数
#if check_N
	check_name,   	// チェックボックスの名前郡
	check_default,	// チェックボックスの初期値郡
#else
	NULL,NULL,
#endif
	func_proc,   	// フィルタ処理関数
	NULL,NULL,   	// 開始時,終了時に呼ばれる関数
	NULL,        	// 設定が変更されたときに呼ばれる関数
	func_WndProc,	// 設定ウィンドウプロシージャ
	NULL,NULL,   	// システムで使用
	NULL,NULL,     	// 拡張データ領域
	filter_info,	// フィルタ情報
	NULL,			// セーブ開始直前に呼ばれる関数
	NULL,			// セーブ終了時に呼ばれる関数
	NULL,NULL,NULL,	// システムで使用
	NULL,			// 拡張領域初期値
	func_is_saveframe,	// 保存するフレームを決める時に呼ばれる関数
};

/*********************************************************************
*	DLL Export
*********************************************************************/
EXTERN_C FILTER_DLL __declspec(dllexport) * __stdcall GetFilterTable( void )
{
	return &filter;
}

/*====================================================================
*	フィルタ処理関数
*===================================================================*/
BOOL func_proc(FILTER *fp,FILTER_PROC_INFO *fpip)
{
	PIXEL_YC *ycp = (PIXEL_YC *)fp->exfunc->get_ycp_source_cache(fpip->editp,fpip->frame,0);
	memcpy(fpip->ycp_edit,ycp,fpip->max_w*fpip->h*sizeof(PIXEL_YC));
	return TRUE;
}

/*====================================================================
*	保存するフレームを決める時に呼ばれる関数
*===================================================================*/
BOOL func_is_saveframe(FILTER *fp,void *editp,int saveno,int frame,int fps,int edit_flag,int inter)
{
	FRAME_STATUS fs[5];
	int i;
	char str[32];

	fp->exfunc->get_frame_status(editp,frame,&fs[0]);

	if(fp->exfunc->is_filter_window_disp(fp)){
		wsprintf(str,"周期間引き 周期:%d",fs[0].index24fps);
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
			return TRUE;	// 前後2フレームに間引き優先があるとき
	}

	return fp->check[cDEL4] ? (fs[0].index24fps!=4) : (fs[0].index24fps!=0);
}

/*====================================================================
*	設定ウィンドウプロシージャ
*===================================================================*/
BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void *editp, FILTER *fp)
{
	switch(message){
		case WM_KEYUP:	// メインウィンドウへ送る
		case WM_KEYDOWN:
		case WM_MOUSEWHEEL:
			SendMessage(GetWindow(hwnd, GW_OWNER), message, wparam, lparam);
			break;
	}

	return FALSE;
}

