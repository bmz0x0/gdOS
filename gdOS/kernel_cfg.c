
/*
 *	通常コンフィギュレーターによって作成されるファイル
 *	今回は手動で作成
 */


#include "itron.h"
#include "kernel.h"
#include "kernel_id.h"
#include "gdOS.h"

//////////////////////////////////////////////////
// task objects

//////////////////////////////////////////////////
// idle task
VP			knl_idl_stkarea[128];
const VP	knl_idl_stk = (VP)knl_idl_stkarea;
const SIZE	knl_idl_stksz = sizeof(knl_idl_stkarea);

//////////////////////////////////////////////////
// user task

// stack area
VP	task1_stkarea[256];
VP	task2_stkarea[256];

const TINIB knl_tinib_tbl[TNUM_TSKID] = {
	{(ATR)(TA_ACT), (VP_INT)(0x1), (FP)(task1), (PRI)(5), (SIZE)128, (VP)task1_stkarea}
,	{(ATR)(TA_ACT), (VP_INT)(0x2), (FP)(task2), (PRI)(4), (SIZE)128, (VP)task2_stkarea}
};

TCB knl_tcb_tbl[TNUM_TSKID];

const INT knl_tcb_cnt = TNUM_TSKID;


//////////////////////////////////////////////////
// task timeout
TIM_WAI_QUE knl_tim_wai_tbl[TNUM_TSKID];
TIM_WAI_QUE *knl_tim_wai_frs;

const INT knl_tim_wai_cnt = TNUM_TSKID;


//////////////////////////////////////////////////
// event flag

const FLGINIB knl_flginib_tbl[TNUM_FLGID] = {
	{(ATR)TA_CLR, (FLGPTN)0}
};

FLGCB knl_flgcb_tbl[TNUM_FLGID];

const INT knl_flgcb_cnt = TNUM_FLGID;

FLG_WAI_QUE knl_flg_wai_tbl[TNUM_TSKID];
FLG_WAI_QUE *knl_flg_wai_frs[TNUM_FLGID];

const INT knl_flg_wai_cnt = TNUM_FLGID;


// ----- init function -----
void knl_cfg_ini()
{
	knl_dis_int();
	
	knl_tsk_ini();
	
	knl_tim_ini();
	
	knl_flg_ini();
	
	knl_ena_int();
}
