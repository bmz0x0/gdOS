
/*
 *	ITRONで定義されているもの
 *	必要なものだけの定義
 */


#ifndef _ITRON_H
#define _ITRON_H

//////////////////////////////////////////////////
// データの型定義
typedef signed int		B;			// 符号付き8ビット整数
typedef signed char		H;			// 符号付き16ビット整数
typedef signed long		W;			// 符号付き32ビット整数

typedef unsigned char	UB;			// 符号無し8ビット整数
typedef unsigned short	UH;			// 符号無し16ビット整数
typedef unsigned long	UW;			// 符号無し32ビット整数

typedef signed char		VB;			// データタイプが定まらない8ビットの値
typedef signed short	VH;			// データタイプが定まらない16ビットの値
typedef signed long		VW;			// データタイプが定まらない32ビットの値

typedef void			*VP;		// データタイプが定まらないものへのポインタ
typedef void			(*FP)();	// プログラムの起動番地

typedef signed int		INT;		// CPUに自然なサイズの符号付き整数(16bit以上)
typedef unsigned int	UINT;		// CPUに自然なサイズの符号無し整数(16bit以上)

typedef INT				BOOL;		// 真偽値

typedef INT				FN;			// 機能コード
typedef INT				ER;			// エラーコード
typedef INT				ID;			// オブジェクトのID番号
typedef UH				ATR;		// オブジェクトの属性
typedef UH				STAT;		// オブジェクトの状態
typedef UH				MODE;		// サービスコールの動作モード
typedef INT				PRI;		// 優先度
typedef UW				SIZE;		// メモリ領域のサイズ
typedef W				TMO;		// タイムアウト指定
typedef UW				RELTIM;		// 相対時間

typedef struct t_systim {			// システム時刻
	UW ltime;						// 下位32bit
	UH utime;						// 上位16bit
} SYSTIM;

typedef long			VP_INT;		// VPまたはINT
typedef ER				ER_BOOL;	// ERまたはBOOL
typedef W				ER_UINT;	// ERまたはUINT
typedef INT				ER_ID;		// ERまたはID
typedef UH				FLGPTN;		// フラグパターン
typedef INT				INTNO;		// 割り込み番号


//////////////////////////////////////////////////
// 定数定義
#ifdef NULL		// C言語標準ヘッダとの競合回避
#undef NULL
#endif

#define NULL		((void *)0)	// 無効ポインタ

#define TRUE		1			// 真
#define FALSE		0			// 偽

#define E_OK		0			// 正常終了


// オブジェクト属性
#define TA_NULL		0			// オブジェクト属性を指定しない
#define TA_ACT		0x02		// タスクを起動された状態で生成

#define TA_WSGL		0x00		// イベントフラグを複数のタスクが待つことを許さない
//#define TA_WMUL		0x02		// イベントフラグを複数のタスクが待つことを許す(未サポート)
#define TA_CLR		0x04		// 待ち解除時にイベントフラグをクリア


// サービスコールの動作モード
#define TWF_ANDW	0x00		// イベントフラグのAND待ち
#define TWF_ORW		0x01		// イベントフラグのOR待ち


// オブジェクトの状態
#define TTS_RUN		0x01		// 実行状態
#define TTS_RDY		0x02		// 実行可能状態
#define TTS_WAI		0x04		// 待ち状態
#define TTS_DMT		0x10		// 休止状態

// その他の定数
#define TSK_SELF	0			// 自タスク指定

// エラーコード
#define E_PAR		(-17)		// パラメータエラー
#define E_ID		(-18)		// 不正ID番号
#define E_CTX		(-25)		// コンテキストエラー


#endif // _ITRON_H
