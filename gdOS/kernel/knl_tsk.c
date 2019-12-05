
/*
 *	タスク関連処理
 */


#include "kernel_id.h"
#include "knl_tsk.h"
#include "knl_tim.h"


// タスク管理
extern TINIB	knl_tinib_tbl[];
extern TCB		knl_tcb_tbl[];
extern INT		knl_tcb_cnt;

// アイドルタスク系
extern VP		knl_idl_stkarea[];
extern VP		knl_idl_stk;
extern SIZE		knl_idl_stksz;

// グローバル変数
STAT	knl_ctx_sts;
TCB		*knl_run_tsk = NULL;
TCTXB	knl_idl_ctx;


// 自タスクをdlytim時間待ち状態にする
ER dly_tsk(RELTIM dlytim)
{
	knl_dis_int();
	
	knl_wai_tsk(knl_run_tsk);
	knl_tim_wai_add(knl_run_tsk, dlytim);
	
	knl_exe_dsp();

	knl_ena_int();
}

// タスクの初期化
void knl_tsk_ini(void)
{
	TCB *ptcb;
	int i;
	
	for ( i = 0; i < knl_tcb_cnt; i++ ) {
		ptcb = &knl_tcb_tbl[i];
		
		// タスク初期情報をTCBテーブルに入れる
		ptcb->tinib = &knl_tinib_tbl[i];
		
		ptcb->stat = TTS_DMT;		// タスクステータスの設定
		
		if ( ptcb->tinib->tskatr & TA_ACT ) {
			// タスクのコンテキストを作成
			knl_cre_ctx( &(ptcb->ctxb)
						, (VP)((UB *)ptcb->tinib->stk + (ptcb->tinib->stksz & ~(0x3)))
						, (VP)knl_tsk_ent
						, (VP_INT)ptcb );
						
			ptcb->stat = TTS_RDY;		// 実行可能状態に設定;
		}
	}
	
	// アイドルタスクのコンテキストを作成
	knl_cre_ctx( &knl_idl_ctx
				, (VP)((UB *)knl_idl_stk + (knl_idl_stksz & ~(0x3)))
				, (VP)knl_idl_tsk
				, (VP_INT)0 );
	
	// コンテキスト状態を初期化
	knl_ctx_sts = CTS_NTSK;
}

// タスク起動時の処理
void knl_tsk_ent(TCB *tcb)
{
	knl_ena_int();
	
	tcb->tinib->task(tcb->tinib->exinf);
	
	// タスク終了処理
	tcb->stat = TTS_DMT;
	knl_exe_dsp();
}

// アイドルタスク本体
void knl_idl_tsk(void)
{
	knl_ena_int();
	
	while(1);
}

// 
void knl_sta_knl(void)
{
	TCTXB	dmy_ctxb;
	TCB		*ptcb;
	
	ptcb = knl_srh_act_tsk();
	
	knl_ctx_sts = CTS_TSK;
	
	if ( NULL == ptcb ) {
		// アイドルタスク開始
		knl_run_tsk = NULL;
		knl_swi_ctx(&dmy_ctxb, &knl_idl_ctx);
	} else {
		knl_run_tsk = ptcb;
		knl_swi_ctx(&dmy_ctxb, &ptcb->ctxb);
	}
}

// 要修正：リターンをとりあえずBOOL型にしてある。
// ディスパッチ実行でTRUE
// ディスパッチ実行無しでFALSE
BOOL knl_exe_dsp(void)
{
	TCB		*tcb_top;		// 一番優先度の高いタスク
	TCB		*tcb_run;		// 現在実行中タスク
	TCTXB	*pre_ctxb;		// タスクスイッチされる側のコンテキスト
	TCTXB	*nxt_ctxb;		// タスクスイッチする側のコンテキスト
	
	// ディスパッチしない条件群
	if ( 0 != (CTS_NTSK & knl_ctx_sts) ) {
		// 遅延ディスパッチ要求にします
		knl_ctx_sts |= CTS_DLYD;
		
		return FALSE;
	}
	
	tcb_top = knl_srh_act_tsk();
	tcb_run = knl_run_tsk;
	
	if ( tcb_top == tcb_run ) {
		return FALSE;
	}
	
	// ディスパッチ確定
	if ( NULL == tcb_run ) {
		pre_ctxb = &knl_idl_ctx;
	} else {
		pre_ctxb = &tcb_run->ctxb;
	}
		
	if ( NULL == tcb_top ) {
		knl_run_tsk = NULL;
		nxt_ctxb = &knl_idl_ctx;
	} else {
		knl_run_tsk = tcb_top;
		nxt_ctxb = &(tcb_top->ctxb);
	}
		
	knl_swi_ctx(pre_ctxb, nxt_ctxb);
	
	return TRUE;
}

// 実行可能かつ優先度が一番高いタスクを探す
TCB *knl_srh_act_tsk(void)
{
	TCB *ptcb;
	TCB *ret_tcb = NULL;
	UINT pri = 0;
	int i;
	
	for ( i = 0; i < knl_tcb_cnt; i++ ) {
		ptcb = &knl_tcb_tbl[i];
		
		if ( TTS_RDY == ptcb->stat && (0 == pri || pri > ptcb->tinib->pri) ) {
			pri = ptcb->tinib->pri;
			ret_tcb = ptcb;
		}
	}
	
	return ret_tcb;
}


void knl_wup_tsk(TCB *tcb)
{
	tcb->stat = TTS_RDY;
}

void knl_wai_tsk(TCB *tcb)
{
	tcb->stat = TTS_WAI;
} 
