
/*
 * CPU依存部（C言語）
 */

#include "iodefine.h"
#include "knl_h8.h"
#include "knl_tim.h"

// H8/36079のタイマーB1初期化
void knl_cpu_tim_ini(void)
{
	TB1.TMB1.BYTE = 0x02;		// クロック周波数/512でTCB1のカウントアップ
	TB1.TCB1 = 0x00;			// カウンタークリア
	IENR2.BIT.IENTB1 = 1;		// タイマB1割り込み許可
}

// H8/36079のタイマーB1割り込み
void knl_cpu_tim_int(void)
{
	IRR2.BIT.IRRTB1 = 0;
	
	// カーネルのタイマー割り込み処理を呼び出す
	knl_tim_int();
}


