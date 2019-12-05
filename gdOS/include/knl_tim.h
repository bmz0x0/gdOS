
/*
 *	タイマ関連処理の定義
 */

#ifndef _KNL_TIM_H
#define _KNL_TIM_H

#include "itron.h"
#include "knl_tsk.h"


// タイムアウトキュー
typedef struct timeout_que {
	TCB		*tcb;
	RELTIM	diff;
	struct	timeout_que *next;
} TIM_WAI_QUE;


// サービスコール定義
ER isig_tim(void);
ER set_tim(SYSTIM *p_systim);
ER get_tim(SYSTIM *p_systim);


// カーネル内関数定義
void knl_tim_ini(void);
void knl_tim_int(void);
void knl_tic_tsk(RELTIM tmout);
void knl_tim_wai_add(TCB *tcb, RELTIM tmout);

#endif // _KNL_TIM_H

