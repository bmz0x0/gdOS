
/*
 *	ITRON�Œ�`����Ă������
 *	�K�v�Ȃ��̂����̒�`
 */


#ifndef _ITRON_H
#define _ITRON_H

//////////////////////////////////////////////////
// �f�[�^�̌^��`
typedef signed int		B;			// �����t��8�r�b�g����
typedef signed char		H;			// �����t��16�r�b�g����
typedef signed long		W;			// �����t��32�r�b�g����

typedef unsigned char	UB;			// ��������8�r�b�g����
typedef unsigned short	UH;			// ��������16�r�b�g����
typedef unsigned long	UW;			// ��������32�r�b�g����

typedef signed char		VB;			// �f�[�^�^�C�v����܂�Ȃ�8�r�b�g�̒l
typedef signed short	VH;			// �f�[�^�^�C�v����܂�Ȃ�16�r�b�g�̒l
typedef signed long		VW;			// �f�[�^�^�C�v����܂�Ȃ�32�r�b�g�̒l

typedef void			*VP;		// �f�[�^�^�C�v����܂�Ȃ����̂ւ̃|�C���^
typedef void			(*FP)();	// �v���O�����̋N���Ԓn

typedef signed int		INT;		// CPU�Ɏ��R�ȃT�C�Y�̕����t������(16bit�ȏ�)
typedef unsigned int	UINT;		// CPU�Ɏ��R�ȃT�C�Y�̕�����������(16bit�ȏ�)

typedef INT				BOOL;		// �^�U�l

typedef INT				FN;			// �@�\�R�[�h
typedef INT				ER;			// �G���[�R�[�h
typedef INT				ID;			// �I�u�W�F�N�g��ID�ԍ�
typedef UH				ATR;		// �I�u�W�F�N�g�̑���
typedef UH				STAT;		// �I�u�W�F�N�g�̏��
typedef UH				MODE;		// �T�[�r�X�R�[���̓��샂�[�h
typedef INT				PRI;		// �D��x
typedef UW				SIZE;		// �������̈�̃T�C�Y
typedef W				TMO;		// �^�C���A�E�g�w��
typedef UW				RELTIM;		// ���Ύ���

typedef struct t_systim {			// �V�X�e������
	UW ltime;						// ����32bit
	UH utime;						// ���16bit
} SYSTIM;

typedef long			VP_INT;		// VP�܂���INT
typedef ER				ER_BOOL;	// ER�܂���BOOL
typedef W				ER_UINT;	// ER�܂���UINT
typedef INT				ER_ID;		// ER�܂���ID
typedef UH				FLGPTN;		// �t���O�p�^�[��
typedef INT				INTNO;		// ���荞�ݔԍ�


//////////////////////////////////////////////////
// �萔��`
#ifdef NULL		// C����W���w�b�_�Ƃ̋������
#undef NULL
#endif

#define NULL		((void *)0)	// �����|�C���^

#define TRUE		1			// �^
#define FALSE		0			// �U

#define E_OK		0			// ����I��


// �I�u�W�F�N�g����
#define TA_NULL		0			// �I�u�W�F�N�g�������w�肵�Ȃ�
#define TA_ACT		0x02		// �^�X�N���N�����ꂽ��ԂŐ���

#define TA_WSGL		0x00		// �C�x���g�t���O�𕡐��̃^�X�N���҂��Ƃ������Ȃ�
//#define TA_WMUL		0x02		// �C�x���g�t���O�𕡐��̃^�X�N���҂��Ƃ�����(���T�|�[�g)
#define TA_CLR		0x04		// �҂��������ɃC�x���g�t���O���N���A


// �T�[�r�X�R�[���̓��샂�[�h
#define TWF_ANDW	0x00		// �C�x���g�t���O��AND�҂�
#define TWF_ORW		0x01		// �C�x���g�t���O��OR�҂�


// �I�u�W�F�N�g�̏��
#define TTS_RUN		0x01		// ���s���
#define TTS_RDY		0x02		// ���s�\���
#define TTS_WAI		0x04		// �҂����
#define TTS_DMT		0x10		// �x�~���

// ���̑��̒萔
#define TSK_SELF	0			// ���^�X�N�w��

// �G���[�R�[�h
#define E_PAR		(-17)		// �p�����[�^�G���[
#define E_ID		(-18)		// �s��ID�ԍ�
#define E_CTX		(-25)		// �R���e�L�X�g�G���[


#endif // _ITRON_H
