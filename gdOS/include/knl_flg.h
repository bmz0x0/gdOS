
/*
 *	�C�x���g�t���O�֘A�w�b�_�[
 */

#ifndef _KNL_FLG_H
#define _KNL_FLG_H


#include "itron.h"
#include "knl_tsk.h"


// �\���̌^��`
typedef struct _flginib {
	ATR			flgatr;		// �C�x���g�t���O����
	FLGPTN		iflgptn;	// �r�b�g�p�^�[�������l
} FLGINIB;

typedef struct _flgcb {
	FLGINIB		*flginib;	// �C�x���g�t���O�����l�ւ̃|�C���^
	FLGPTN		flgptn;		// ���݂̃r�b�g�p�^�[���l
} FLGCB;

typedef struct _flg_wai_que {
	TCB						*tcb;		// �C�x���g�҂��^�X�N
	FLGPTN					waiptn;		// �҂��r�b�g�p�^�[��
	MODE					wfmode;		// �҂����[�h
	struct _flg_wai_que		*next;
} FLG_WAI_QUE;


// �C�x���g�t���OID����z��ԍ����v�Z
#define FLGID_TO_ARYNO(a) (a-TMIN_FLGID)
// �C�x���g�t���OID����FLGCB��Ԃ�
#define FLGID_TO_FLGCB(a) (&(knl_flgcb_tbl[a-TMIN_FLGID]))


// �T�[�r�X�R�[����`
ER set_flg(ID flgid, FLGPTN setptn);
ER wai_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn);
ER clr_flg(ID flgid, FLGPTN clrptn);


// �J�[�l�����֐���`
void knl_flg_ini(void);
BOOL knl_flg_srh_tsk(FLGCB *wait_flgcb, ID flgid, FLGPTN waiptn);
void knl_flg_wai_add(TCB *tcb, ID flgid, FLGPTN waiflg, MODE wfmode);
BOOL knl_flg_chk_flg(FLGPTN set_flg, FLGPTN wait_flg, MODE wfmode);


#endif // _KNL_FLG_H

