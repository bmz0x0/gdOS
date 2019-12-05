
/*
 *	�^�C�}�֘A�����@
 */

#include "kernel.h"
#include "kernel_id.h"
#include "knl_tim.h"
#include "knl_tsk.h"
#include "knl_h8.h"

// �R���e�L�X�g���(�{���͂��̕ϐ����Q�Ƃ���kernel_task.c���ł��ׂ�)

// �V�X�e�������v�Z�Ŏg�p����ϐ�
SYSTIM	knl_sys_tim;			// �V�X�e������
UW		knl_tic_rem = 0;		// �^�C���e�B�b�N�p�v�Z�̗]��

// �^�C���A�E�g�v�Z�Ŏg�p����ϐ�
extern TIM_WAI_QUE	knl_tim_wai_tbl[];
extern INT			knl_tim_wai_cnt;
extern TIM_WAI_QUE	*knl_tim_wai_frs;


// �V�X�e�������Ƀ^�C���e�B�b�N����������
ER isig_tim(void)
{
	UW add_time;		// ���ɉ��Z���鎞��(ms)
	UW pre_ltime;		// ���Z����O�̎���
	
	// ���Z���鎞�Ԃ̌v�Z
	add_time = TIM_TIC_DIV;
	knl_tic_rem += TIM_TIC_REM;
	if ( knl_tic_rem >= TIM_TIC_NUME ) {
		add_time++;
		knl_tic_rem -= TIM_TIC_NUME;
	}
	
	// ����32bit�������ӂꂵ���ꍇ�̏���
	pre_ltime = knl_sys_tim.ltime;
	knl_sys_tim.ltime += add_time;
	
	if ( knl_sys_tim.ltime < pre_ltime ) {
		knl_sys_tim.utime++;
	}
	
	// �^�X�N�̃^�C���A�E�g�v�Z
	knl_tic_tsk(add_time);
	
	return E_OK;
}

// �V�X�e���������Z�b�g����
ER set_tim(SYSTIM *p_systim)
{
	knl_sys_tim.ltime = p_systim->ltime;
	knl_sys_tim.utime = p_systim->utime;
	
	return E_OK;
}

// �V�X�e���������擾����
ER get_tim(SYSTIM *p_systim)
{
	p_systim->ltime = knl_sys_tim.ltime;
	p_systim->utime = knl_sys_tim.utime;
	
	return E_OK;
}

// �^�C�}�[�̏�����
void knl_tim_ini(void)
{
	int i;
	
	knl_cpu_tim_ini();
	
	// �ϐ��̏�����
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

// �^�C�}���荞�ݎ��̏���
void knl_tim_int(void)
{
	// �R���e�L�X�g��Ԃ��^�X�N���ֈڍs
	knl_ctx_sts &= (~CTS_TSK);
	knl_ctx_sts |= CTS_NTSK;
	
	isig_tim();
	
	// �R���e�L�X�g��Ԃ��^�X�N���֖߂�
	knl_ctx_sts &= (~CTS_NTSK);
	knl_ctx_sts |= (CTS_TSK);
	
	// �x���f�B�X�p�b�`�v���̏���
	if ( 0 != (knl_ctx_sts & CTS_TSK)
	  && 0 != (knl_ctx_sts & CTS_DLYD) ) {
		// �x���t���O�N���A
		knl_ctx_sts &= (~CTS_DLYD);
		
		knl_exe_dsp();
	}
	 
	knl_ena_int();
}

// �^�C���A�E�g�҂��^�X�N�Ɍo�ߎ��Ԃ����Z
void knl_tic_tsk(RELTIM tmout)
{
	TCB *wup_tsk;
	TIM_WAI_QUE *tmp_que;
	
	while ( NULL != knl_tim_wai_frs && knl_tim_wai_frs->diff <= tmout ) {
		tmout -= knl_tim_wai_frs->diff;
		
		wup_tsk = knl_tim_wai_frs->tcb;
		
		knl_wup_tsk(wup_tsk);
		// �x���f�B�X�p�b�`�v��
		knl_exe_dsp();
		
		tmp_que = knl_tim_wai_frs;
		knl_tim_wai_frs = knl_tim_wai_frs->next;
		
		// �g���I������珉�������Ă����܂��B
		tmp_que->tcb = NULL;
		tmp_que->diff = 0;
		tmp_que->next = NULL;
	}
	
	knl_tim_wai_frs->diff -= tmout;
}


// �^�C���A�E�g�L���[�Ƀ^�X�N��ǉ�
void knl_tim_wai_add(TCB *tcb, RELTIM tmout)
{
	TIM_WAI_QUE *add_que;
	TIM_WAI_QUE *pre_que;
	TIM_WAI_QUE *tmp_que;
 	int i;
	
	// �f�[�^��������̂�T��
	for ( i = 0; i < knl_tim_wai_cnt; i++ ) {
		if ( NULL == knl_tim_wai_tbl[i].tcb ) {
			knl_tim_wai_tbl[i].tcb = tcb;
			add_que = &knl_tim_wai_tbl[i];
			break;
		}
	}
	
	tmp_que = knl_tim_wai_frs;
	pre_que = NULL;

	// �L���[�̓r���ɒǉ�����ꍇ		
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

