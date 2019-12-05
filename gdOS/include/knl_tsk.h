
/*
 *	�^�X�N�֘A�����̒�`
 */

#ifndef _KNL_TSK_H
#define _KNL_TSK_H

#include "itron.h"
#include "knl_h8_ctx.h"


// �^�X�N�������
typedef struct _tinib {
	ATR		tskatr;				// �^�X�N����
	VP_INT	exinf;				// �^�X�N�̊g�����
	FP		task;				// �^�X�N�̋N���Ԓn
	UINT	pri;				// �^�X�N�̗D��x(���s���ύX�Ȃ�)
	SIZE	stksz;				// �X�^�b�N�̈�̃T�C�Y
	VP		stk;				// �X�^�b�N�̈�̐擪�Ԓn
} TINIB;

// �^�X�N�R���g���[���u���b�N
typedef struct _tcb {
	const TINIB		*tinib;		// �^�X�N�������ւ̃|�C���^
	UINT			stat;		// �^�X�N���
	TCTXB			ctxb;		// �^�X�N�R���e�L�X�g
} TCB;


// �f�B�X�p�b�`���
#define CTS_TSK			0x01		// �^�X�N�����s��
#define CTS_NTSK		0x02		// ��^�X�N�����s��
#define CTS_DLYD		0x80		// �x���f�B�X�p�b�`�v��

// �}�N����`
#define TSKID_TO_TCB(a) (&(knl_tcb_tbl[a-TMIN_TSKID]))

// �R���e�L�X�g���
extern TCB	*knl_run_tsk;
extern STAT knl_ctx_sts;


// �T�[�r�X�R�[����`
ER dly_tsk(RELTIM dlytim);


// �J�[�l�����֐���`
void knl_tsk_ini(void);
void knl_tsk_ent(TCB *tcb);
void knl_idl_tsk(void);
void knl_sta_knl(void);
BOOL knl_exe_dsp(void);
TCB *knl_srh_act_tsk(void);
void knl_wup_tsk(TCB *tcb);
void knl_wai_tsk(TCB *tcb);

#endif // _KNL_TSK_H


