
/*
 *	CPU依存処理関係の定義
 */


#ifndef _KNL_H8_H
#define _KNL_H8_H


#include "itron.h"


// CPUに依存するタイマー用関数
void knl_cpu_tim_ini(void);
void knl_cpu_tim_int(void);


#endif // _KNL_H8_H

