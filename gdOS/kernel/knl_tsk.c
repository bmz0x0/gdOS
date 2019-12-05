
/*
 *	�^�X�N�֘A����
 */


#include "kernel_id.h"
#include "knl_tsk.h"
#include "knl_tim.h"


// �^�X�N�Ǘ�
extern TINIB	knl_tinib_tbl[];
extern TCB		knl_tcb_tbl[];
extern INT		knl_tcb_cnt;

// �A�C�h���^�X�N�n
extern VP		knl_idl_stkarea[];
extern VP		knl_idl_stk;
extern SIZE		knl_idl_stksz;

// �O���[�o���ϐ�
STAT	knl_ctx_sts;
TCB		*knl_run_tsk = NULL;
TCTXB	knl_idl_ctx;


// ���^�X�N��dlytim���ԑ҂���Ԃɂ���
ER dly_tsk(RELTIM dlytim)
{
	knl_dis_int();
	
	knl_wai_tsk(knl_run_tsk);
	knl_tim_wai_add(knl_run_tsk, dlytim);
	
	knl_exe_dsp();

	knl_ena_int();
}

// �^�X�N�̏�����
void knl_tsk_ini(void)
{
	TCB *ptcb;
	int i;
	
	for ( i = 0; i < knl_tcb_cnt; i++ ) {
		ptcb = &knl_tcb_tbl[i];
		
		// �^�X�N��������TCB�e�[�u���ɓ����
		ptcb->tinib = &knl_tinib_tbl[i];
		
		ptcb->stat = TTS_DMT;		// �^�X�N�X�e�[�^�X�̐ݒ�
		
		if ( ptcb->tinib->tskatr & TA_ACT ) {
			// �^�X�N�̃R���e�L�X�g���쐬
			knl_cre_ctx( &(ptcb->ctxb)
						, (VP)((UB *)ptcb->tinib->stk + (ptcb->tinib->stksz & ~(0x3)))
						, (VP)knl_tsk_ent
						, (VP_INT)ptcb );
						
			ptcb->stat = TTS_RDY;		// ���s�\��Ԃɐݒ�;
		}
	}
	
	// �A�C�h���^�X�N�̃R���e�L�X�g���쐬
	knl_cre_ctx( &knl_idl_ctx
				, (VP)((UB *)knl_idl_stk + (knl_idl_stksz & ~(0x3)))
				, (VP)knl_idl_tsk
				, (VP_INT)0 );
	
	// �R���e�L�X�g��Ԃ�������
	knl_ctx_sts = CTS_NTSK;
}

// �^�X�N�N�����̏���
void knl_tsk_ent(TCB *tcb)
{
	knl_ena_int();
	
	tcb->tinib->task(tcb->tinib->exinf);
	
	// �^�X�N�I������
	tcb->stat = TTS_DMT;
	knl_exe_dsp();
}

// �A�C�h���^�X�N�{��
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
		// �A�C�h���^�X�N�J�n
		knl_run_tsk = NULL;
		knl_swi_ctx(&dmy_ctxb, &knl_idl_ctx);
	} else {
		knl_run_tsk = ptcb;
		knl_swi_ctx(&dmy_ctxb, &ptcb->ctxb);
	}
}

// �v�C���F���^�[�����Ƃ肠����BOOL�^�ɂ��Ă���B
// �f�B�X�p�b�`���s��TRUE
// �f�B�X�p�b�`���s������FALSE
BOOL knl_exe_dsp(void)
{
	TCB		*tcb_top;		// ��ԗD��x�̍����^�X�N
	TCB		*tcb_run;		// ���ݎ��s���^�X�N
	TCTXB	*pre_ctxb;		// �^�X�N�X�C�b�`����鑤�̃R���e�L�X�g
	TCTXB	*nxt_ctxb;		// �^�X�N�X�C�b�`���鑤�̃R���e�L�X�g
	
	// �f�B�X�p�b�`���Ȃ������Q
	if ( 0 != (CTS_NTSK & knl_ctx_sts) ) {
		// �x���f�B�X�p�b�`�v���ɂ��܂�
		knl_ctx_sts |= CTS_DLYD;
		
		return FALSE;
	}
	
	tcb_top = knl_srh_act_tsk();
	tcb_run = knl_run_tsk;
	
	if ( tcb_top == tcb_run ) {
		return FALSE;
	}
	
	// �f�B�X�p�b�`�m��
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

// ���s�\���D��x����ԍ����^�X�N��T��
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
