
/*
 * CPU�ˑ����iC����j
 */

#include "iodefine.h"
#include "knl_h8.h"
#include "knl_tim.h"

// H8/36079�̃^�C�}�[B1������
void knl_cpu_tim_ini(void)
{
	TB1.TMB1.BYTE = 0x02;		// �N���b�N���g��/512��TCB1�̃J�E���g�A�b�v
	TB1.TCB1 = 0x00;			// �J�E���^�[�N���A
	IENR2.BIT.IENTB1 = 1;		// �^�C�}B1���荞�݋���
}

// H8/36079�̃^�C�}�[B1���荞��
void knl_cpu_tim_int(void)
{
	IRR2.BIT.IRRTB1 = 0;
	
	// �J�[�l���̃^�C�}�[���荞�ݏ������Ăяo��
	knl_tim_int();
}


