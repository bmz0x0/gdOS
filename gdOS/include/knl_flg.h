
/*
 *	イベントフラグ関連ヘッダー
 */

#ifndef _KNL_FLG_H
#define _KNL_FLG_H


#include "itron.h"
#include "knl_tsk.h"


// 構造体型定義
typedef struct _flginib {
	ATR			flgatr;		// イベントフラグ属性
	FLGPTN		iflgptn;	// ビットパターン初期値
} FLGINIB;

typedef struct _flgcb {
	FLGINIB		*flginib;	// イベントフラグ初期値へのポインタ
	FLGPTN		flgptn;		// 現在のビットパターン値
} FLGCB;

typedef struct _flg_wai_que {
	TCB						*tcb;		// イベント待ちタスク
	FLGPTN					waiptn;		// 待ちビットパターン
	MODE					wfmode;		// 待ちモード
	struct _flg_wai_que		*next;
} FLG_WAI_QUE;


// イベントフラグIDから配列番号を計算
#define FLGID_TO_ARYNO(a) (a-TMIN_FLGID)
// イベントフラグIDからFLGCBを返す
#define FLGID_TO_FLGCB(a) (&(knl_flgcb_tbl[a-TMIN_FLGID]))


// サービスコール定義
ER set_flg(ID flgid, FLGPTN setptn);
ER wai_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn);
ER clr_flg(ID flgid, FLGPTN clrptn);


// カーネル内関数定義
void knl_flg_ini(void);
BOOL knl_flg_srh_tsk(FLGCB *wait_flgcb, ID flgid, FLGPTN waiptn);
void knl_flg_wai_add(TCB *tcb, ID flgid, FLGPTN waiflg, MODE wfmode);
BOOL knl_flg_chk_flg(FLGPTN set_flg, FLGPTN wait_flg, MODE wfmode);


#endif // _KNL_FLG_H

