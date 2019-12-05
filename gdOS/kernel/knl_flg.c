
/*
 *	�C�x���g�t���O�֌W����
 */

#include "kernel_id.h"
#include "knl_flg.h"

// �^�C���A�E�g�v�Z�Ŏg�p����ϐ�
extern FLGINIB	knl_flginib_tbl[];
extern FLGCB	knl_flgcb_tbl[];
extern INT		knl_flgcb_cnt;

extern FLG_WAI_QUE	knl_flg_wai_tbl[];
extern FLG_WAI_QUE	*knl_flg_wai_frs[];
extern INT			knl_flg_wai_cnt;


// �w�肳�ꂽID�̃t���O���Z�b�g����
ER set_flg(ID flgid, FLGPTN setptn)
{
	FLGCB *pflgcb;
	FLG_WAI_QUE *pwait_que;
	
	if ( flgid < TMIN_FLGID || flgid > TMAX_FLGID ) {
		return E_ID;
	}
	
	knl_dis_int();
	
	pflgcb = FLGID_TO_FLGCB(flgid);
	
	pflgcb->flgptn |= setptn;
	
	if ( TRUE == knl_flg_srh_tsk(pflgcb, flgid, setptn) ) {
		knl_exe_dsp();
	}
	knl_ena_int();
	
	return E_OK;
}

// �w�肳�ꂽID�̃t���O��҂�
ER wai_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
	TCB	*ptcb;
	FLGCB *pflgcb;
	
	if ( flgid < TMIN_FLGID || flgid > TMAX_FLGID ) {
		return E_ID;
	}
	
	if ( 0 == waiptn || (wfmode != TWF_ANDW && wfmode != TWF_ORW) ) {
		return E_PAR;
	}
	
	knl_dis_int();
	
	if ( 0 == (CTS_TSK & knl_ctx_sts) ) {
		knl_ena_int();
		return E_CTX;
	}
	
	pflgcb = FLGID_TO_FLGCB(flgid);
	
	if ( TRUE == knl_flg_chk_flg( pflgcb->flgptn, waiptn, wfmode ) ) {
		// ���łɏ����𖞂����Ă���
		*p_flgptn = pflgcb->flgptn;
		if ( 0 != (pflgcb->flginib->flgatr & TA_CLR) ) {
			pflgcb->flgptn = 0;
		}
	} else {
		// �����𖞂����Ă��Ȃ��̂ő҂��ɓ���܂�
		ptcb = knl_run_tsk;
		
		knl_flg_wai_add(ptcb, flgid, waiptn, wfmode);
		knl_wai_tsk(ptcb);
		
		knl_exe_dsp();
	}
	knl_ena_int();
	
	return E_OK;
}

// �w�肳�ꂽID�̃t���O���N���A����
ER clr_flg(ID flgid, FLGPTN clrptn)
{
	FLGCB *pflgcb;

	pflgcb = FLGID_TO_FLGCB(flgid);

	pflgcb->flgptn &= clrptn;
	
	return E_OK;
}

// �C�x���g�t���O�̏�����
void knl_flg_ini(void)
{
	int i;
	
	for ( i = 0; i < knl_flgcb_cnt; i++ ) {
		knl_flgcb_tbl[i].flginib = &knl_flginib_tbl[i];
		knl_flgcb_tbl[i].flgptn = knl_flginib_tbl[i].iflgptn;
		
		knl_flg_wai_frs[i] = NULL;
	}
	
	for ( i = 0; i < knl_flgcb_cnt; i++ ) {
		knl_flg_wai_tbl[i].tcb = NULL;
		knl_flg_wai_tbl[i].waiptn = 0;
		knl_flg_wai_tbl[i].wfmode = 0;
		knl_flg_wai_tbl[i].next = NULL;
	}
}

// �C�x���g�t���O�҂��̃^�X�N��T��
BOOL knl_flg_srh_tsk(FLGCB *wait_flgcb, ID flgid, FLGPTN waiptn)
{
	FLG_WAI_QUE *pre_que = NULL;
	FLG_WAI_QUE *wait_que;
	FLG_WAI_QUE *first_que;
	BOOL ret = FALSE;
	
	first_que = wait_que = knl_flg_wai_frs[FLGID_TO_ARYNO(flgid)];
	
	while( NULL != wait_que ) {
		if ( TRUE == knl_flg_chk_flg(wait_que->waiptn, waiptn, wait_que->wfmode) ) {
			wait_que->waiptn = waiptn;
			
			if ( 0 != (wait_flgcb->flginib->flgatr & TA_CLR) ) {
				wait_flgcb->flgptn = 0;
			}
			
			// �^�X�N���N������
			knl_wup_tsk(wait_que->tcb);
			
			// �C�x���g�҂��L���[����҂��^�X�N���폜
			wait_que->tcb = NULL;
			wait_que->waiptn = 0;
			wait_que->wfmode = 0;

			if ( NULL == pre_que ) {
				wait_que->next = NULL;
				first_que = NULL;
				wait_que = NULL;
			} else {
				pre_que->next = wait_que->next;
				wait_que->next = NULL;
				wait_que = pre_que->next;
			}
			
			ret = TRUE;
		} else {
			pre_que = wait_que;
			wait_que = wait_que->next;
		}
	}
	
	return TRUE;
}

// �C�x���g�t���O�҂��̃^�X�N��ǉ�
void knl_flg_wai_add(TCB *tcb, ID flgid, FLGPTN waiflg, MODE wfmode)
{
	FLG_WAI_QUE **flg_que_first;
	FLG_WAI_QUE *wait_flg_que;
	int i;
	
	// �f�[�^�i�[�ꏊ��T��
	for ( i = 0; i < knl_flg_wai_cnt; i++ ) {
		if ( NULL == knl_flg_wai_tbl[i].tcb ) {
			knl_flg_wai_tbl[i].tcb = tcb;
			knl_flg_wai_tbl[i].waiptn = waiflg;
			knl_flg_wai_tbl[i].wfmode = wfmode;
			knl_flg_wai_tbl[i].next = NULL;
			
			wait_flg_que = &knl_flg_wai_tbl[i];
			break;
		}
	}
	// �i�[�ꏊ���Ȃ��Ƃ������Ƃ͌v�Z��Ȃ�
	
	flg_que_first = &(knl_flg_wai_frs[FLGID_TO_ARYNO(flgid)]);
	
	if ( NULL == *flg_que_first ) {
		*flg_que_first = wait_flg_que;
	} else {
		while( NULL != (*flg_que_first)->next ) {
			*flg_que_first = (*flg_que_first)->next;
		}
		(*flg_que_first)->next = wait_flg_que;
	}
}

// �^�X�N�҂������t���O�����̃`�F�b�N
BOOL knl_flg_chk_flg(FLGPTN set_flg, FLGPTN wait_flg, MODE wfmode)
{
	BOOL ret = FALSE;
	
	if ( TWF_ANDW == wfmode ) {
		if ( (set_flg & wait_flg) == wait_flg ) {
			ret = TRUE;
		}
	} else {
		if ( 0 != (set_flg & wait_flg) ) {
			ret = TRUE;
		}
	}
	
	return ret;
}
