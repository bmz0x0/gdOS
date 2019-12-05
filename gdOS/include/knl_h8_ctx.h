
/*
 *	CPU依存処理関係の定義
 */


#ifndef _KNL_H8_CTX_H
#define _KNL_H8_CTX_H


#include "itron.h"


// コンテキストブロック（CPU依存）
typedef struct task_context_block {
	VP			sp;			// スタックポインタ
} TCTXB;


//////////////////////////////////////////////////
// kernel_h8.src
// 割り込み禁止
void knl_dis_int(void);
// 割り込み許可
void knl_ena_int(void);
// コンテキスト作成
void knl_cre_ctx(TCTXB *ctx, VP sp, void (*task)(VP_INT), VP_INT exinf);
// タスクスイッチ
void knl_swi_ctx(TCTXB *pre_ctx,	TCTXB *nxt_ctx);


#endif // _KNL_H8_CTX_H

