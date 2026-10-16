//#define	SHDBS		//ﾃﾞﾊﾞｸﾞ信号を出力するときに宣言する
//#define	SHDBG		//ﾃﾞﾊﾞｸﾞ情報を出力するときに宣言する
//#define	SHBPS	115200	//ﾃﾞﾊﾞｸﾞ情報のﾎﾞｰﾚｰﾄを宣言する(ﾃﾞﾌｫﾙﾄ値は38400bps)

//#define	I2C_SOFT	//I2Cをｿﾌﾄ実装するときに宣言する
//#define	SPI_SOFT	//SPIをｿﾌﾄ実装するときに宣言する
				//2線SPI又は3線SPIのときは自動的にSPIはｿﾌﾄ実装になる
//#define	SHSPI2		//2線SPIのときに宣言する
//#define	SHSPI3		//3線SPIのときに宣言する
//#define	SLEEP_EN	//Sleep機能を実装するときに宣言する
//#define	KYOKU_AUTO	//ｵﾙｺﾞｰﾙ演奏を自動開始するときに宣言する
//#define	ONSEIN_AUTO	//音声N(ch0)再生を自動開始するときに宣言する
//#define	ONSEII_AUTO	//音声I再生を自動開始するときに宣言する
//#define	ONSEIC_AUTO	//音声C再生を自動開始するときに宣言する
//#define	ONSEIS_AUTO	//音声S(ch0)再生を自動開始するときに宣言する
//#define	RAND_CHSEL	0x00000001	//ﾗﾝﾀﾞﾏｲｽﾞの種にするCHSEL(PA0:ADC_IN0)
//#define	KYOKU_RAND	//曲番号をﾗﾝﾀﾞﾏｲｽﾞするときに宣言する
//#define	ONSEIN_RAND	//音声N番号をﾗﾝﾀﾞﾏｲｽﾞするときに宣言する
//#define	ONSEII_RAND	//音声I番号をﾗﾝﾀﾞﾏｲｽﾞするときに宣言する
//#define	ONSEIC_RAND	//音声C番号をﾗﾝﾀﾞﾏｲｽﾞするときに宣言する
//#define	ONSEIS_RAND	//音声S番号をﾗﾝﾀﾞﾏｲｽﾞするときに宣言する
//#define	SW_EN		//SWを実装するときに宣言する
//#define	SW_PLNG		//SWをｲﾍﾞﾝﾄではなく､ﾎﾟｰﾘﾝｸﾞするときに宣言する
//#define	CdS_EN		//CdSを実装するときに宣言する
//#define	CVD_EN		//CVDを実装するときに宣言する

//ｵﾙｺﾞｰﾙ箱への応用では以下を宣言しておくこと
//#define	CdS_FUTA	//蓋を開ける(明)とｵﾝ判定する
//#define	CdS_THR	100	//判定閾値を固定の宣言値にする

#define		VOICEN_IMA	//音声NがIMA_ADPCMｴﾝｺｰﾄﾞのときに宣言する
#define		VOICEI_IMA	//音声IがIMA_ADPCMｴﾝｺｰﾄﾞのときに宣言する
#define		VOICES_IMA	//音声SがIMA_ADPCMｴﾝｺｰﾄﾞのときに宣言する

//#define	ISP_SPIFLASH_EN	//ISP環境でSPIﾌﾗｯｼｭを書替えるときに宣言する
				//(自動的に､ﾃﾞﾊﾞｸﾞ情報のUART内蔵ﾓｼﾞｭｰﾙを230400bpsで使用する)
				//(通常のｱﾌﾟﾘは実行されない)


/*
STM32電子ｵﾙｺﾞｰﾙ演奏+音声再生ver1_4
これは根のｿｰｽｺｰﾄﾞ

・ISP形態でSPIﾌﾗｯｼｭを書替える機能(ISP_SPIFLASH)に対応している｡

・ﾀｰｹﾞｯﾄはSTM32F030CCT6(内部64MHzｸﾛｯｸ)

以下は電子ｵﾙｺﾞｰﾙとして動作するときの説明である｡

・UARTから演奏ｺｰﾄﾞを受信して､該当する音符ﾃﾞｰﾀをｴﾝｼﾞﾝへ送って演奏する｡

・UARTの通信ﾊﾟﾗﾒｰﾀは230400bps､8ﾃﾞｰﾀﾋﾞｯﾄ､ﾊﾟﾘﾃｨ無し､1ｽﾄｯﾌﾟﾋﾞｯﾄ､ﾌﾛｰ制御無し｡


//==============================================
//I2Cﾌﾗｯｼｭﾒﾓﾘのﾋﾟﾝ接
//==============================================
1:A0(Vssに繋ぐ)
2:A1(Vssに繋ぐ)
3:A2(Vssに繋ぐ)
4:Vss
5:SDA(1.5kで外部ﾌﾟﾙｱｯﾌﾟ)
6:SCL(1.5kで外部ﾌﾟﾙｱｯﾌﾟ)
7:WP(Vccに繋ぐ)
8:Vcc


//==============================================
//SDCのﾋﾟﾝ接(SPIﾓｰﾄﾞ)
//==============================================
9:空き(不使用)
1:CS(2線SPIでは10k*220pの時定数回路を介してCLKに繋ぐ)
2:DI
3:Vss1
4:Vdd
5:CLK
6:Vss2
7:DO(2線SPI･3線SPIでは1.5kを介してDIに繋ぐ)
8:IRQ(不使用)


//==============================================
//SPIﾌﾗｯｼｭﾒﾓﾘのﾋﾟﾝ接
//==============================================
W25Qのﾋﾟﾝ接(標準SPIﾓｰﾄﾞ)
1:CS(2線SPIでは10k*220pの時定数回路を介してCLKに繋ぐ)
2:DO(2線SPI･3線SPIではDIに繋ぐ)
3:WP(GNDに接続､ISPを利用する場合はVccに接続)
4:GND
5:DI
6:CLK
7:HOLD(Vccに接続)
8:Vcc


//==============================================
//STM32のﾋﾟﾝ接
//==============================================
STM32F030CCT6のﾎﾟｰﾄの割当て
1:VDD
2:(PC13)空き
3:(PC14)空き
4:(PC15)空き
5:(PF0)ﾃﾞﾊﾞｸﾞ信号出力
6:(PF1)空き
7:NRST
8:VSSA
9:VDDA
10:(PA0)BOOT0代替/(ADC_IN0)CVD3
11:(PA1)SW0/(ADC_IN1)CVD2
12:(PA2)SW1/(ADC_IN2)CVD1
13:(PA3)SW2/(ADC_IN3)CVD0
14:(PA4)/(ADC_IN4)CdS
15:(PA5)SCK/(SPI1_SCK)(SDCのCLK･W25QのCLKに繋ぐ)
16:(PA6)MISO/(SPI1_MISO)(SDCのDO･W25QのDOに繋ぐ､3線SPI･2線SPIのときは不使用)
17:(PA7)MOSI/(SPI1_MOSI)(SDCのDI･W25QのDIに繋ぐ)
18:(PB0)空き
19:(PB1)空き
20:(PB2)空き
21:(PB10)空き
22:(PB11)空き
23:VSS
24:VDD
25:(PB12)空き
26:(PB13)/(TIM1_CH1N)ｺﾝﾌﾟﾘ出力
27:(PB14)空き
28:(PB15)空き
29:(PA8)/(TIM1_CH1)正相出力
30:(PA9)/(USART1_TX)ﾃﾞﾊﾞｸﾞ情報出力･UART出力/(ISP_TX)
31:(PA10)/(USART1_RX)ﾃﾞﾊﾞｸﾞ情報入力･UART入力/(ISP_RX)
32:(PA11)/(TIM1_CH4)ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ逆相出力
33:(PA12)空き
34:(PA13)SS(SDCのCS･W25QのCSに繋ぐ､2線SPIのときは不使用)/(SWDIO)
35:(PF6)空き
36:(PF7)空き
37:(PA14)/(SWCLK)
38:(PA15)
39:(PB3)空き
40:(PB4)空き
41:(PB5)空き
42:(PB6)SCL/(I2C1_SCL)(24FCのSCLに繋ぐ)
43:(PB7)SDA/(I2C1_SDA)(24FCのSDAに繋ぐ)
44:BOOT0(外部ﾌﾟﾙﾀﾞｳﾝ)
45:(PB8)ﾃﾞﾊﾞｸﾞ信号出力
46:(PB9)ISP時のﾃﾞﾊﾞｸﾞ情報出力
47:VSS
48:VDD
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
//UARTのﾎﾟｰﾄ定義
#define	UART_GPIO	GPIOA			//UART出力のﾎﾟｰﾄ
#define	UART_BIT	9			//UART出力のﾋﾞｯﾄ
#define	UART_AF		1			//UART出力のUART交代機能番号
#define	UART_IN_GPIO	GPIOA			//UART入力のﾎﾟｰﾄ
#define	UART_IN_BIT	10			//UART入力のﾋﾞｯﾄ
#define	UART_IN_AF	1			//UART入力のUART交代機能番号

//ﾃﾞﾊﾞｸﾞ信号のﾎﾟｰﾄ定義
#define	DBS_GPIO	GPIOB			//ﾃﾞﾊﾞｸﾞ信号のﾎﾟｰﾄ
#define	DBS_BIT		8			//ﾃﾞﾊﾞｸﾞ信号のﾋﾞｯﾄ

//ﾃﾞﾊﾞｸﾞ情報のﾎﾟｰﾄ定義
#ifdef	ISP_SPIFLASH_EN			//ISP環境でSPIﾌﾗｯｼｭを書替えるとき
#define	SHDBG_SOFT				//ﾃﾞﾊﾞｸﾞ情報のUARTをｿﾌﾄ実装する
#define	DBG_GPIO	GPIOB			//ﾃﾞﾊﾞｸﾞ情報出力のﾎﾟｰﾄ
#define	DBG_BIT		9			//ﾃﾞﾊﾞｸﾞ情報出力のﾋﾞｯﾄ
#else					//電子ｵﾙｺﾞｰﾙ動作のとき
#define	DBG_GPIO	GPIOA			//ﾃﾞﾊﾞｸﾞ情報出力のﾎﾟｰﾄ
#define	DBG_BIT		9			//ﾃﾞﾊﾞｸﾞ情報出力のﾋﾞｯﾄ
#define	DBG_AF		1			//ﾃﾞﾊﾞｸﾞ情報出力のUART_TX交代機能番号
#define	DBG_IN_GPIO	GPIOA			//ﾃﾞﾊﾞｸﾞ情報入力のﾎﾟｰﾄ
#define	DBG_IN_BIT	10			//ﾃﾞﾊﾞｸﾞ情報入力のﾋﾞｯﾄ
#define	DBG_IN_AF	1			//ﾃﾞﾊﾞｸﾞ情報入力のUART_RX交代機能番号
#endif

//PWMのﾎﾟｰﾄ定義
#define	PWM_F_GPIO	GPIOA			//PWM正相のﾎﾟｰﾄ
#define	PWM_F_BIT	8			//PWM正相のﾋﾞｯﾄ
#define	PWM_F_AF	2			//PWM正相の交代機能番号
#define	PWM_R_GPIO	GPIOA			//PWM逆相のﾎﾟｰﾄ
#define	PWM_R_BIT	11			//PWM逆相のﾋﾞｯﾄ
#define	PWM_R_AF	2			//PWM逆相の交代機能番号
#define	PWM_C_GPIO	GPIOB			//PWM相補のﾎﾟｰﾄ
#define	PWM_C_BIT	13			//PWM相補のﾋﾞｯﾄ
#define	PWM_C_AF	2			//PWM相補の交代機能番号

//I2Cのﾎﾟｰﾄ定義
#define I2C_SCL_GPIO	GPIOB			//SCLのﾎﾟｰﾄ
#define I2C_SCL_BIT	6			//SCLのﾋﾞｯﾄ
#define	I2C_SCL_AF	1			//SCLの交代機能番号
#define I2C_SDA_GPIO	GPIOB			//SDAのﾎﾟｰﾄ
#define I2C_SDA_BIT	7			//SDAのﾋﾞｯﾄ
#define	I2C_SDA_AF	1			//SDAの交代機能番号

//SPIのﾎﾟｰﾄ定義
#define SPI_SCK_GPIO	GPIOA			//SCKのﾎﾟｰﾄ
#define SPI_SCK_BIT	5			//SCKのﾋﾞｯﾄ
#define	SPI_SCK_AF	0			//SCKの交代機能番号
#define SPI_MOSI_GPIO	GPIOA			//MOSIのﾎﾟｰﾄ
#define SPI_MOSI_BIT	7			//MOSIのﾋﾞｯﾄ
#define	SPI_MOSI_AF	0			//MOSIの交代機能番号
#ifdef SHSPI2				//2線SPIのとき
#define	SPI_SS_TIME	5			//SSﾈｹﾞｰﾄ待ち時間[us]
#define SPI_SS_GPIO	GPIOA			//SSのﾎﾟｰﾄ
#define SPI_SS_BIT	5			//SSのﾋﾞｯﾄ
#else					//3線SPIまたは4線SPIのとき
#define SPI_SS_GPIO	GPIOA			//SSのﾎﾟｰﾄ
#define SPI_SS_BIT	13			//SSのﾋﾞｯﾄ
#endif
#if defined(SHSPI2)||defined(SHSPI3)	//2線SPIまたは3線SPIのとき
#define	SPI_SOFT				//SPIはｿﾌﾄ実装する
#define SPI_MISO_GPIO	GPIOA			//MISOのﾎﾟｰﾄ
#define SPI_MISO_BIT	7			//MISOのﾋﾞｯﾄ
#else					//4線SPIのとき
#define SPI_MISO_GPIO	GPIOA			//MISOのﾎﾟｰﾄ
#define SPI_MISO_BIT	6			//MISOのﾋﾞｯﾄ
#define	SPI_MISO_AF	0			//MISOの交代機能番号
#endif

//SPIのﾌﾟﾙｱｯﾌﾟ/ﾀﾞｳﾝ定義			//SWのﾎﾟｰﾄ定義と合わせること
#define	SPI_SCK_PU				//SPI_SCKをﾌﾟﾙｱｯﾌﾟする
//#define	SPI_SCK_PD			//SPI_SCKをﾌﾟﾙﾀﾞｳﾝする
#define	SPI_MOSI_PU				//SPI_MOSIをﾌﾟﾙｱｯﾌﾟする
//#define	SPI_MOSI_PD			//SPI_MOSIをﾌﾟﾙﾀﾞｳﾝする
#define	SPI_MISO_PU				//SPI_MISOをﾌﾟﾙｱｯﾌﾟする
//#define	SPI_MISO_PD			//SPI_MISOをﾌﾟﾙﾀﾞｳﾝする

//SW0のﾎﾟｰﾄ定義
#define	SW0_GPIO	GPIOA			//SW0のﾎﾟｰﾄ
#define	SW0_EXTI_PORT	0			//SW0のEXTIﾎﾟｰﾄ(0=PA､1=PB､2=PC､3=PD､5=PF)
#define	SW0_BIT		1			//SW0のﾋﾞｯﾄ
#define	SW0_INV					//SW0入力は負論理
					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
#define SW0_TIMER				//SW0はﾀｲﾏｰﾓｰﾄﾞ
//#define	SW0_REAL			//SW0はﾘｱﾙﾓｰﾄﾞ
#define	SW0_CNT		ORGEL_CNT|VOICEN_CNT|VOICEI_CNT|VOICEC_CNT|VOICES_CNT
						//SW0の操作対象

//SW1のﾎﾟｰﾄ定義
#define	SW1_GPIO	GPIOA			//SW1のﾎﾟｰﾄ
#define	SW1_EXTI_PORT	0			//SW1のEXTIﾎﾟｰﾄ(0=PA､1=PB､2=PC､3=PD､5=PF)
#define	SW1_BIT		2			//SW1のﾋﾞｯﾄ
#define	SW1_INV					//SW1入力は負論理
					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
#define SW1_TIMER				//SW1はﾀｲﾏｰﾓｰﾄﾞ
//#define	SW1_REAL			//SW1はﾘｱﾙﾓｰﾄﾞ
#define	SW1_CNT		ORGEL_CNT|VOICEN_CNT|VOICEI_CNT|VOICEC_CNT|VOICES_CNT
						//SW1の操作対象

//SW2のﾎﾟｰﾄ定義
//#define	SW2_GPIO	GPIOA		//SW2のﾎﾟｰﾄ
//#define	SW2_EXTI_PORT	0		//SW2のEXTIﾎﾟｰﾄ(0=PA､1=PB､2=PC､3=PD､5=PF)
//#define	SW2_BIT		3		//SW2のﾋﾞｯﾄ
//#define	SW2_INV				//SW2入力は負論理
//					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
//#define SW2_TIMER				//SW2はﾀｲﾏｰﾓｰﾄﾞ
//#define	SW2_REAL			//SW2はﾘｱﾙﾓｰﾄﾞ
//#define	SW2_CNT		ORGEL_CNT|VOICEN_CNT|VOICEI_CNT|VOICEC_CNT|VOICES_CNT
						//SW2の操作対象

//CdSのﾎﾟｰﾄ定義
#define	CdS_GPIO	GPIOA			//CdSのﾎﾟｰﾄ
#define	CdS_BIT		4			//CdSのﾋﾞｯﾄ
#define	CdS_CHSEL	0x0010			//CdSのADCCHSEL
//#define	CdS_INV				//CdS入力は負論理(暗=L)
					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
#define CdS_TIMER				//CdSはﾀｲﾏｰﾓｰﾄﾞ
//#define	CdS_REAL			//CdSはﾘｱﾙﾓｰﾄﾞ
#define	CdS_CNT	ORGEL_CNT|VOICEN_CNT|VOICEI_CNT|VOICEC_CNT|VOICES_CNT
						//CdSの操作対象

//CVD0のﾎﾟｰﾄ定義
#define	CVD0_GPIO	GPIOA			//CVD0のﾎﾟｰﾄ
#define	CVD0_BIT	3			//CVD0のﾋﾞｯﾄ
#define	CVD0_CHSEL	0x0008			//CVD0のADCCHSEL
					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
#define CVD0_TIMER				//CVD0はﾀｲﾏｰﾓｰﾄﾞ
//#define	CVD0_REAL			//CVD0はﾘｱﾙﾓｰﾄﾞ
#define	CVD0_CNT	ORGEL_CNT|VOICEN_CNT|VOICEI_CNT|VOICEC_CNT|VOICES_CNT
						//CVD0の操作対象

//CVD1のﾎﾟｰﾄ定義
//#define	CVD1_GPIO	GPIOA		//CVD1のﾎﾟｰﾄ
//#define	CVD1_BIT	2		//CVD1のﾋﾞｯﾄ
//#define	CVD1_CHSEL	0x0004		//CVD1のADCCHSEL
//					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
//#define CVD1_TIMER				//CVD1はﾀｲﾏｰﾓｰﾄﾞ
//#define	CVD1_REAL			//CVD1はﾘｱﾙﾓｰﾄﾞ
//#define	CVD1_CNT	NON_CNT		//CVD1の操作対象

//CVD2のﾎﾟｰﾄ定義
//#define	CVD2_GPIO	GPIOA		//CVD2のﾎﾟｰﾄ
//#define	CVD2_BIT	1		//CVD2のﾋﾞｯﾄ
//#define	CVD2_CHSEL	0x0002		//CVD2のADCCHSEL
//					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
//#define CVD2_TIMER				//CVD2はﾀｲﾏｰﾓｰﾄﾞ
//#define	CVD2_REAL			//CVD2はﾘｱﾙﾓｰﾄﾞ
//#define	CVD2_CNT	NON_CNT		//CVD2の操作対象

//CVD3のﾎﾟｰﾄ定義
//#define	CVD3_GPIO	GPIOA		//CVD3のﾎﾟｰﾄ
//#define	CVD3_BIT	0		//CVD3のﾋﾞｯﾄ
//#define	CVD3_CHSEL	0x0001		//CVD3のADCCHSEL
//					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
//#define CVD3_TIMER				//CVD3はﾀｲﾏｰﾓｰﾄﾞ
//#define	CVD3_REAL			//CVD3はﾘｱﾙﾓｰﾄﾞ
//#define	CVD3_CNT	NON_CNT		//CVD3の操作対象

//LEDのﾎﾟｰﾄ定義
//#define	LED0_GPIO	GPIOC		//LED0のﾎﾟｰﾄ
//#define	LED0_BIT	0		//LED0のﾋﾞｯﾄ
//#define	LED1_GPIO	GPIOD		//LED1のﾎﾟｰﾄ
//#define	LED1_BIT	2		//LED1のﾋﾞｯﾄ
//#define	LED2_GPIO	GPIOD		//LED2のﾎﾟｰﾄ
//#define	LED2_BIT	3		//LED2のﾋﾞｯﾄ
//#define	LED3_GPIO	GPIOC		//LED3のﾎﾟｰﾄ
//#define	LED3_BIT	2		//LED3のﾋﾞｯﾄ


#if	KB_N>0
//==============================================
//KB演奏で使う音源波形とｴﾝﾍﾞﾛｰﾌﾟ波形のextern
//song_idx.sで実体をincludeしておくこと
//==============================================
extern const struct ENVE_HDR enve_EXP;
extern const struct ENVE_HDR enve_RSN;
extern const struct ENVE_HDR enve_VIB;
extern const struct ENVE_HDR enve_TREMOLO;
extern const struct ENVE_HDR enve_ELEGANTE;
extern const struct WAVE_HDR wave_SIN14;
extern const struct WAVE_HDR wave_SHEPARD;
extern const struct WAVE_HDR wave_RING4;
#endif


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
#ifdef VOICEN_IMA			//IMA-ADPCMのとき
#include	"../voice_WAN_IMA.c"
#include	"../voice_DAME_IMA.c"
#include	"../voice_CHIKO_IMA.c"

#else					//8ﾋﾞｯﾄPCMのとき
#include	"../voice_WAN.c"
#include	"../voice_DAME.c"
#include	"../voice_CHIKO.c"

#endif
//==============================================
//音声ﾃﾞｰﾀ(ﾌﾟﾛｸﾞﾗﾑﾌﾗｯｼｭ)のｲﾝﾃﾞｯｸｽ
//==============================================
static const struct VOICEN_HDR voiceN_idx[]=
{
	von_mac(voice_WAN,sizeof(voice_WAN),8000)
	von_mac(voice_DAME,sizeof(voice_DAME),8000)
	von_mac(voice_CHIKO,sizeof(voice_CHIKO),8000)
};
#define VOICEN_SU	(sizeof(voiceN_idx)/sizeof(voiceN_idx[0]))
#endif


#if	VOICE_I>0
//==============================================
//音声ﾃﾞｰﾀ(I2Cﾌﾗｯｼｭ)のｲﾝﾃﾞｯｸｽ
//==============================================
static const struct VOICEI_HDR voiceI_idx[]=
{

#ifdef VOICEI_IMA			//IMA-ADPCMのとき
	//voice_DAMEのW25Q_IMAを使う
	voi_mac(0x000000,0x0007df,8000,0xa0)	//1:WAN_1ch16bit16ksps.wav
	voi_mac(0x0007df,0x000797,8000,0xa0)	//2:DAME_1ch16bit16ksps.wav
	voi_mac(0x000f76,0x001d0b,8000,0xa0)	//3:CHIKO_1ch16bit16ksps.wav

#else					//8ﾋﾞｯﾄPCMのとき
	//voice_DAMEのW25Qを使う
	voi_mac(0x000000,0x000fbe,8000,0xa0)	//1:WAN_1ch16bit16ksps.wav
	voi_mac(0x000fbe,0x000f2f,8000,0xa0)	//2:DAME_1ch16bit16ksps.wav
	voi_mac(0x001eed,0x003a17,8000,0xa0)	//3:CHIKO_1ch16bit16ksps.wav

/*
	//voice_Mirrorna\W25Q16.hexを使う
	voi_mac(0x000000,0x002401,8000,0xa0)	//1:sharin.wav
	voi_mac(0x002401,0x004575,8000,0xa0)	//2:kirakiraomeme.wav
	voi_mac(0x006976,0x001956,8000,0xa0)	//3:aidorumitai.wav
	voi_mac(0x0082cc,0x002b18,8000,0xa0)	//4:kyounooshare.wav
	voi_mac(0x00ade4,0x001d61,8000,0xa0)	//5:egaogasuteki.wav
	voi_mac(0x00cb45,0x0033a5,8000,0xa0)	//6:kirarin.wav
*/
#endif

};
#define VOICEI_SU	(sizeof(voiceI_idx)/sizeof(voiceI_idx[0]))
#endif


#if	VOICE_C>0
//==============================================
//音声ﾃﾞｰﾀ(SDｶｰﾄﾞ)のｲﾝﾃﾞｯｸｽ
//==============================================
static const struct VOICEC_HDR voiceC_idx[]=
{

	//｢ｱﾝﾊﾟﾝﾏﾝｼｮｰ｣の音声ﾃﾞｰﾀ(SDのANPANMAN.wav)を使う
	voc_mac(0x00029e,0x00e367,8000)		//anpan
	voc_mac(0x00029e,0x00e367,8000)		//anpan
//	voc_mac(0x002000,0x000020,8000)		//anpanの一部(ﾃｽﾄ用)
//	voc_mac(0x004000,0x000020,8000)		//anpanの一部(ﾃｽﾄ用)
//	voc_mac(0x006000,0x000020,8000)		//anpanの一部(ﾃｽﾄ用)
//	voc_mac(0x008000,0x000020,8000)		//anpanの一部(ﾃｽﾄ用)

};
#define VOICEC_SU	(sizeof(voiceC_idx)/sizeof(voiceC_idx[0]))
#endif


#if	VOICE_S>0
//==============================================
//音声ﾃﾞｰﾀ(SPIﾌﾗｯｼｭ)のｲﾝﾃﾞｯｸｽ
//==============================================
static const struct VOICES_HDR voiceS_idx[]=
{

#ifdef VOICES_IMA			//IMA-ADPCMのとき
	//｢ｷﾃｨﾐﾗｰﾅ｣の音声ﾃﾞｰﾀ(voice_Mirrorna\W25Q_IMA.hex)を使う
	vos_mac(0x000000,0x001200,8000)	//1:sharin.wav
	vos_mac(0x001200,0x0022ba,8000)	//2:kirakiraomeme.wav
	vos_mac(0x0034ba,0x000cab,8000)	//3:aidorumitai.wav
	vos_mac(0x004165,0x00158c,8000)	//4:kyounooshare.wav
	vos_mac(0x0056f1,0x000eb0,8000)	//5:egaogasuteki.wav
	vos_mac(0x0065a1,0x0019d2,8000)	//6:kirarin.wav

#else
	//｢ｷﾃｨﾐﾗｰﾅ｣の音声ﾃﾞｰﾀ(voice_Mirrorna\W25Q.hex)を使う
	vos_mac(0x000000,0x002401,8000)	//sharin.wav
	vos_mac(0x002401,0x004575,8000)	//kirakiraomeme.wav
	vos_mac(0x006976,0x001956,8000)	//aidorumitai.wav
	vos_mac(0x0082cc,0x002b18,8000)	//kyounooshare.wav
	vos_mac(0x00ade4,0x001d61,8000)	//egaogasuteki.wav
	vos_mac(0x00cb45,0x0033a5,8000)	//kirarin.wav

/*
	//voice_ToyStory\W25Q16_12ksps8bit.hexを使う
	vos_mac(0x000000,0x0afc83,12000)	//ToyStory_12ksps8bit
*/

#endif
};
#define VOICES_SU	(sizeof(voiceS_idx)/sizeof(voiceS_idx[0]))
#endif


//==============================================
//共通ｺｰﾄﾞの呼込み
//==============================================
#define	DEV_F030				//ﾃﾞﾊﾞｲｽはF030
#define	DEV_F030Cx				//ﾃﾞﾊﾞｲｽはF030Cx
#define	DEV_I2C1				//I2C1を使う
#define	I2Cx	I2C1				//I2CﾚｼﾞｽﾀはI2C1を指す
#define	DEV_SPI1				//SPI1を使う
#define	SPIx	SPI1				//SPIﾚｼﾞｽﾀはSPI1を指す
#define	DEV_UART1				//UART1を使う
#define	UARTx	USART1				//USARTﾚｼﾞｽﾀはUSART1を指す
//#define	UARTBPS	115200			//UARTのﾎﾞｰﾚｰﾄ(ﾃﾞﾌｫﾙﾄ値は38400bps)
#include "../dev_F030.c"			//F030ﾃﾞﾊﾞｲｽｺｰﾄﾞ
#ifdef ISP_SPIFLASH_EN			//ISP環境でSPIﾌﾗｯｼｭを書替えるとき
#include "../apl_ISP_SPIFLASH.c"		//ISP実行ｱﾌﾟﾘｹｰｼｮﾝ
#else					//電子ｵﾙｺﾞｰﾙ動作のとき
#include "../apl_KB.c"				//KB操作ｱﾌﾟﾘｹｰｼｮﾝ
#endif
