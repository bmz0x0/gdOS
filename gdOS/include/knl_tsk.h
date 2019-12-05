
/*
 *	タスク関連処理の定義
 */

#ifndef _KNL_TSK_H
#define _KNL_TSK_H

#include "itron.h"
#include "knl_h8_ctx.h"


// タスク初期情報
typedef struct _tinib {
	ATR		tskatr;				// タスク属性
	VP_INT	exinf;				// タスクの拡張情報
	FP		task;				// タスクの起動番地
	UINT	pri;				// タスクの優先度(実行中変更なし)
	SIZE	stksz;				// スタック領域のサイズ
	VP		stk;				// スタック領域の先頭番地
} TINIB;

// タスクコントロールブロック
typedef struct _tcb {
	const TINIB		*tinib;		// タスク初期情報へのポインタ
	UINT			stat;		// タスク状態
	TCTXB			ctxb;		// タスクコンテキスト
} TCB;


// ディスパッチ状態
#define CTS_TSK			0x01		// タスク部実行中
#define CTS_NTSK		0x02		// 非タスク部実行中
#define CTS_DLYD		0x80		// 遅延ディスパッチ要求

// マクロ定義
#define TSKID_TO_TCB(a) (&(knl_tcb_tbl[a-TMIN_TSKID]))

// コンテキスト状態
extern TCB	*knl_run_tsk;
extern STAT knl_ctx_sts;


// サービスコール定義
ER dly_tsk(RELTIM dlytim);


// カーネル内関数定義
void knl_tsk_ini(void);
void knl_tsk_ent(TCB *tcb);
void knl_idl_tsk(void);
void knl_sta_knl(void);
BOOL knl_exe_dsp(void);
TCB *knl_srh_act_tsk(void);
void knl_wup_tsk(TCB *tcb);
void knl_wai_tsk(TCB *tcb);

#endif // _KNL_TSK_H


