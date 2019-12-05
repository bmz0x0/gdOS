
/*
 *	タイマ関連処理　
 */

#include "kernel.h"
#include "kernel_id.h"
#include "knl_tim.h"
#include "knl_tsk.h"
#include "knl_h8.h"

// コンテキスト状態(本来はこの変数を参照せずkernel_task.c内でやるべき)

// システム時刻計算で使用する変数
SYSTIM	knl_sys_tim;			// システム時刻
UW		knl_tic_rem = 0;		// タイムティック用計算の余り

// タイムアウト計算で使用する変数
extern TIM_WAI_QUE	knl_tim_wai_tbl[];
extern INT			knl_tim_wai_cnt;
extern TIM_WAI_QUE	*knl_tim_wai_frs;


// システム時刻にタイムティックを供給する
ER isig_tim(void)
{
	UW add_time;		// 一回に加算する時間(ms)
	UW pre_ltime;		// 加算する前の時間
	
	// 加算する時間の計算
	add_time = TIM_TIC_DIV;
	knl_tic_rem += TIM_TIC_REM;
	if ( knl_tic_rem >= TIM_TIC_NUME ) {
		add_time++;
		knl_tic_rem -= TIM_TIC_NUME;
	}
	
	// 下位32bitが桁あふれした場合の処理
	pre_ltime = knl_sys_tim.ltime;
	knl_sys_tim.ltime += add_time;
	
	if ( knl_sys_tim.ltime < pre_ltime ) {
		knl_sys_tim.utime++;
	}
	
	// タスクのタイムアウト計算
	knl_tic_tsk(add_time);
	
	return E_OK;
}

// システム時刻をセットする
ER set_tim(SYSTIM *p_systim)
{
	knl_sys_tim.ltime = p_systim->ltime;
	knl_sys_tim.utime = p_systim->utime;
	
	return E_OK;
}

// システム時刻を取得する
ER get_tim(SYSTIM *p_systim)
{
	p_systim->ltime = knl_sys_tim.ltime;
	p_systim->utime = knl_sys_tim.utime;
	
	return E_OK;
}

// タイマーの初期化
void knl_tim_ini(void)
{
	int i;
	
	knl_cpu_tim_ini();
	
	// 変数の初期化
	knl_sys_tim.ltime = 0;
	knl_sys_tim.utime = 0;
	knl_tic_rem = 0;
		
	for ( i = 0; i < knl_tim_wai_cnt; i++ ) {
		knl_tim_wai_tbl[i].tcb = NULL;
		knl_tim_wai_tbl[i].diff = 0;
		knl_tim_wai_tbl[i].next = NULL;
	}
	
	knl_tim_wai_frs = NULL;
}

// タイマ割り込み時の処理
void knl_tim_int(void)
{
	// コンテキスト状態を非タスク部へ移行
	knl_ctx_sts &= (~CTS_TSK);
	knl_ctx_sts |= CTS_NTSK;
	
	isig_tim();
	
	// コンテキスト状態をタスク部へ戻す
	knl_ctx_sts &= (~CTS_NTSK);
	knl_ctx_sts |= (CTS_TSK);
	
	// 遅延ディスパッチ要求の処理
	if ( 0 != (knl_ctx_sts & CTS_TSK)
	  && 0 != (knl_ctx_sts & CTS_DLYD) ) {
		// 遅延フラグクリア
		knl_ctx_sts &= (~CTS_DLYD);
		
		knl_exe_dsp();
	}
	 
	knl_ena_int();
}

// タイムアウト待ちタスクに経過時間を加算
void knl_tic_tsk(RELTIM tmout)
{
	TCB *wup_tsk;
	TIM_WAI_QUE *tmp_que;
	
	while ( NULL != knl_tim_wai_frs && knl_tim_wai_frs->diff <= tmout ) {
		tmout -= knl_tim_wai_frs->diff;
		
		wup_tsk = knl_tim_wai_frs->tcb;
		
		knl_wup_tsk(wup_tsk);
		// 遅延ディスパッチ要求
		knl_exe_dsp();
		
		tmp_que = knl_tim_wai_frs;
		knl_tim_wai_frs = knl_tim_wai_frs->next;
		
		// 使い終わったら初期化しておきます。
		tmp_que->tcb = NULL;
		tmp_que->diff = 0;
		tmp_que->next = NULL;
	}
	
	knl_tim_wai_frs->diff -= tmout;
}


// タイムアウトキューにタスクを追加
void knl_tim_wai_add(TCB *tcb, RELTIM tmout)
{
	TIM_WAI_QUE *add_que;
	TIM_WAI_QUE *pre_que;
	TIM_WAI_QUE *tmp_que;
 	int i;
	
	// データを入れる実体を探す
	for ( i = 0; i < knl_tim_wai_cnt; i++ ) {
		if ( NULL == knl_tim_wai_tbl[i].tcb ) {
			knl_tim_wai_tbl[i].tcb = tcb;
			add_que = &knl_tim_wai_tbl[i];
			break;
		}
	}
	
	tmp_que = knl_tim_wai_frs;
	pre_que = NULL;

	// キューの途中に追加する場合		
	while ( NULL != tmp_que ) {
		if ( tmout < tmp_que->diff ) {
			add_que->diff = tmout;
			tmp_que->diff -= tmout;
				
			add_que->next = tmp_que;
			if ( NULL == pre_que ) {
				knl_tim_wai_frs = add_que;
			} else {
				pre_que->next = add_que;
			}
			break;
		}
		tmout -= tmp_que->diff;
		
		pre_que = tmp_que;
		tmp_que = tmp_que->next;
	}
		
	if ( NULL == tmp_que ) {
		add_que->diff = tmout;
		add_que->next = NULL;

		if ( NULL == pre_que ) {
			knl_tim_wai_frs = add_que;
		} else {
			pre_que->next = add_que;
		}
	}
}

