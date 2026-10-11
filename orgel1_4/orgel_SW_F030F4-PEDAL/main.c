//#define	SHDBS		//ﾃﾞﾊﾞｸﾞ信号を出力するときに宣言する
//#define	SHDBG		//ﾃﾞﾊﾞｸﾞ情報を出力するときに宣言する
//#define	SHBPS	115200	//ﾃﾞﾊﾞｸﾞ情報のﾎﾞｰﾚｰﾄを宣言する(ﾃﾞﾌｫﾙﾄ値は38400bps)
//#define	SHDBG_SOFT	//ﾃﾞﾊﾞｸﾞ情報のUARTをｿﾌﾄ実装するときに宣言する		

//#define		SLEEP_EN	//Sleep機能を実装するときに宣言する


/*
PUYA32電子ｵﾙｺﾞｰﾙ演奏+音声再生ver1_4をﾍﾞｰｽに
ｱﾝﾊﾟﾝﾏﾝﾄﾞﾗｲﾌﾞﾍﾟﾀﾞﾙ(ｱｶﾞﾂﾏ)旧版のﾏｲｺﾝ換装用にｶｽﾀﾏｲｽﾞした｡
これは根のｿｰｽｺｰﾄﾞ

・ﾀｰｹﾞｯﾄはSTM32F003F4P6(内部64MHzｸﾛｯｸ)

・ｱｸｾﾙﾍﾟﾀﾞﾙSWでｴﾝｼﾞﾝ音を低速～高速に変える｡｡


//==============================================
//STM32のﾋﾟﾝ接
//==============================================
STM32F030F4P6のﾎﾟｰﾄの割当て
1:BOOT0
2:(PF0)
3:(PF1)
4:NRST
5:VDDA
6:(PA0)SW0(ｱｸｾﾙﾍﾟﾀﾞﾙ)(負論理､内部ﾌﾟﾙｯﾌﾟ)
7:(PA1)
8:(PA2)
9:(PA3)/(USART1_RX)ﾃﾞﾊﾞｸﾞ情報入力
10:(PA4)
11:(PA5)
12:(PA6)
13:(PA7)
14:(PB1)ﾃﾞﾊﾞｸﾞ信号出力/(TIM1_CH3N)正相出力
15:VSS
16:VDD
17:(PA9)/(TIM1_CH2)ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ逆相出力/(ISP_TX)
18:(PA10)/(TIM1_CH3)ｺﾝﾌﾟﾘ逆相出力/(ISP_RX)
19:(PA13)/(SWDIO)
20:(PA14)ﾃﾞﾊﾞｸﾞ情報出力/(USART1_TX)ﾃﾞﾊﾞｸﾞ情報出力/(SWCLK)
*/


//==============================================
//ﾍｯﾀﾞﾌｧｲﾙの呼込み
//==============================================
#include <stm32f0xx.h>				//ｼｽﾃﾑのﾍｯﾀﾞﾌｧｲﾙ
#include "orgel_cnf.h"				//構成情報
#include "../orgel.h"				//ｱﾌﾟﾘのﾍｯﾀﾞﾌｧｲﾙ


//==============================================
//構成情報(ｱﾌﾟﾘｹｰｼｮﾝとﾀｰｹﾞｯﾄﾃﾞﾊﾞｲｽに関わるもの)
//==============================================
//ﾃﾞﾊﾞｸﾞ信号のﾎﾟｰﾄ定義
#define	DBS_GPIO	GPIOB			//ﾃﾞﾊﾞｸﾞ信号のﾎﾟｰﾄ
#define	DBS_BIT		1			//ﾃﾞﾊﾞｸﾞ信号のﾋﾞｯﾄ

//ﾃﾞﾊﾞｸﾞ情報のﾎﾟｰﾄ定義
#define	DBG_GPIO	GPIOA			//ﾃﾞﾊﾞｸﾞ情報出力のﾎﾟｰﾄ
#define	DBG_BIT		14			//ﾃﾞﾊﾞｸﾞ情報出力のﾋﾞｯﾄ
#define	DBG_AF		1			//ﾃﾞﾊﾞｸﾞ情報出力のUART交代機能番号
#define	DBG_IN_GPIO	GPIOA			//ﾃﾞﾊﾞｸﾞ情報入力のﾎﾟｰﾄ
#define	DBG_IN_BIT	3			//ﾃﾞﾊﾞｸﾞ情報入力のﾋﾞｯﾄ
#define	DBG_IN_AF	1			//ﾃﾞﾊﾞｸﾞ情報入力のUART交代機能番号

//PWMのﾎﾟｰﾄ定義
#define	PWM_F_GPIO	GPIOB			//PWM正相のﾎﾟｰﾄ
#define	PWM_F_BIT	1			//PWM正相のﾋﾞｯﾄ
#define	PWM_F_AF	2			//PWM正相の交代機能番号
#define	PWM_R_GPIO	GPIOA			//PWM逆相のﾎﾟｰﾄ
#define	PWM_R_BIT	9			//PWM逆相のﾋﾞｯﾄ
#define	PWM_R_AF	2			//PWM逆相の交代機能番号
#define	PWM_C_GPIO	GPIOA			//PWM相補のﾎﾟｰﾄ
#define	PWM_C_BIT	10			//PWM相補のﾋﾞｯﾄ
#define	PWM_C_AF	2			//PWM相補の交代機能番号

//SW_のﾎﾟｰﾄ定義
#define	SW0_GPIO	GPIOA			//SW0のﾎﾟｰﾄ
#define	SW0_BIT		0			//SW0のﾋﾞｯﾄ


#if	PART_N+KB_N>0
//==============================================
//ｿﾝｸﾞﾃﾞｰﾀのｲﾝﾃﾞｯｸｽ
//==============================================
extern const struct SONG_IDX SONG_IDX;		//曲ｲﾝﾃﾞｯｸｽ実体
#endif


#if	VOICE_N>0
//==============================================
//音声ﾃﾞｰﾀ
//==============================================
#include	"..\voice_ENGINE2.c"


//==============================================
//音声ﾃﾞｰﾀ(ﾌﾟﾛｸﾞﾗﾑﾌﾗｯｼｭ)のｲﾝﾃﾞｯｸｽ
//==============================================
static const struct VOICEN_HDR voiceN_idx[]=
{
	von_mac(voice_ENGINE2,sizeof(voice_ENGINE2),4000)
};
#define VOICEN_SU	(sizeof(voiceN_idx)/sizeof(voiceN_idx[0]))
#endif


//==============================================
//共通ｺｰﾄﾞの呼込み
//==============================================
#define	DEV_F030				//ﾃﾞﾊﾞｲｽはF030
#define	DEV_F030F4				//ﾃﾞﾊﾞｲｽはF030F4
#define	I2Cx	I2C1				//I2C1を使う
#define	SPIx	SPI1				//SPI1を使う
#include "../dev_F030.c"			//F030ﾃﾞﾊﾞｲｽｺｰﾄﾞ
#include "../apl_SW-PEDAL.c"			//ﾄﾞﾗｲﾌﾞﾊﾝﾄﾞﾙのｱﾌﾟﾘｹｰｼｮﾝ
