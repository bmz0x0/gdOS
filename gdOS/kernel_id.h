
/*
 * タスク、イベントフラグ構成により編集するファイル
 * kernel_cfg.cのヘッダー版のような物
 */

#ifndef _KERNEL_ID_H
#define _KERNEL_ID_H


// タイムティック供給時の計算に使用する定数
#define TIM_TIC_NUME	4096							// 分子
#define TIM_TIC_DENO	625								// 分母
#define TIM_TIC_DIV		(TIM_TIC_NUME/TIM_TIC_DENO)		// 割った結果
#define TIM_TIC_REM		(TIM_TIC_NUME%TIM_TIC_DENO)		// 余り

// タスクID
#define TSKID_TASK1		1			// TASK1のタスクID
#define TSKID_TASK2		2			// TASK2のタスクID

#define TMIN_TSKID		1			// タスクIDの最小値
#define TMAX_TSKID		2			// タスクIDの最大値
#define TNUM_TSKID		2			// タスク合計数


// イベントフラグID
#define FLGID_CHG_TSK	1			// CHG_TSKのフラグID

#define TMIN_FLGID		1			// イベントフラグIDの最小値
#define TMAX_FLGID		1			// イベントフラグIDの最大値
#define TNUM_FLGID		1			// イベントフラグ合計数


#endif // _KERNEL_ID_H


