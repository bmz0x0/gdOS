
/*
 * �^�X�N�A�C�x���g�t���O�\���ɂ��ҏW����t�@�C��
 * kernel_cfg.c�̃w�b�_�[�ł̂悤�ȕ�
 */

#ifndef _KERNEL_ID_H
#define _KERNEL_ID_H


// �^�C���e�B�b�N�������̌v�Z�Ɏg�p����萔
#define TIM_TIC_NUME	4096							// ���q
#define TIM_TIC_DENO	625								// ����
#define TIM_TIC_DIV		(TIM_TIC_NUME/TIM_TIC_DENO)		// ����������
#define TIM_TIC_REM		(TIM_TIC_NUME%TIM_TIC_DENO)		// �]��

// �^�X�NID
#define TSKID_TASK1		1			// TASK1�̃^�X�NID
#define TSKID_TASK2		2			// TASK2�̃^�X�NID

#define TMIN_TSKID		1			// �^�X�NID�̍ŏ��l
#define TMAX_TSKID		2			// �^�X�NID�̍ő�l
#define TNUM_TSKID		2			// �^�X�N���v��


// �C�x���g�t���OID
#define FLGID_CHG_TSK	1			// CHG_TSK�̃t���OID

#define TMIN_FLGID		1			// �C�x���g�t���OID�̍ŏ��l
#define TMAX_FLGID		1			// �C�x���g�t���OID�̍ő�l
#define TNUM_FLGID		1			// �C�x���g�t���O���v��


#endif // _KERNEL_ID_H


