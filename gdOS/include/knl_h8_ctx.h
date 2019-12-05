
/*
 *	CPU�ˑ������֌W�̒�`
 */


#ifndef _KNL_H8_CTX_H
#define _KNL_H8_CTX_H


#include "itron.h"


// �R���e�L�X�g�u���b�N�iCPU�ˑ��j
typedef struct task_context_block {
	VP			sp;			// �X�^�b�N�|�C���^
} TCTXB;


//////////////////////////////////////////////////
// kernel_h8.src
// ���荞�݋֎~
void knl_dis_int(void);
// ���荞�݋���
void knl_ena_int(void);
// �R���e�L�X�g�쐬
void knl_cre_ctx(TCTXB *ctx, VP sp, void (*task)(VP_INT), VP_INT exinf);
// �^�X�N�X�C�b�`
void knl_swi_ctx(TCTXB *pre_ctx,	TCTXB *nxt_ctx);


#endif // _KNL_H8_CTX_H

