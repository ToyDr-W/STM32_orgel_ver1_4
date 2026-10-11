//#define	SHDBS		//ﾃﾞﾊﾞｸﾞ信号を出力するときに宣言する
//#define	SHDBG		//ﾃﾞﾊﾞｸﾞ情報を出力するときに宣言する
//#define	SHBPS	115200	//ﾃﾞﾊﾞｸﾞ情報のﾎﾞｰﾚｰﾄを宣言する(ﾃﾞﾌｫﾙﾄ値は38400bps)
//#define	SHDBG_SOFT	//ﾃﾞﾊﾞｸﾞ情報のUARTをｿﾌﾄ実装するときに宣言する		
//#define	SHDBG_IN	//ﾃﾞﾊﾞｸﾞ情報を入力するときに宣言する(UARTのｿﾌﾄ実装は不可)	

//#define	I2C_SOFT	//I2Cをｿﾌﾄ実装するときに宣言する
//#define	SPI_SOFT	//SPIをｿﾌﾄ実装するときに宣言する
				//2線SPI又は3線SPIのときは自動的にSPIはｿﾌﾄ実装になる
//#define	SHSPI2		//2線SPIのときに宣言する
//#define	SHSPI3		//3線SPIのときに宣言する
#define		SLEEP_EN	//Sleep機能を実装するときに宣言する
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
#define		SW_EN		//SWを実装するときに宣言する
//#define	SW_PLNG		//SWをｲﾍﾞﾝﾄではなく､ﾎﾟｰﾘﾝｸﾞするときに宣言する
//#define	CdS_EN		//CdSを実装するときに宣言する
//#define	CVD_EN		//CVDを実装するときに宣言する

//ｵﾙｺﾞｰﾙ箱への応用では以下を宣言しておくこと
//#define	CdS_FUTA	//蓋を開ける(明)とｵﾝ判定する
//#define	CdS_THR	100	//判定閾値を固定の宣言値にする

#define		VOICEN_IMA	//音声NがIMA_ADPCMｴﾝｺｰﾄﾞのときに宣言する
#define		VOICEI_IMA	//音声IがIMA_ADPCMｴﾝｺｰﾄﾞのときに宣言する
#define		VOICES_IMA	//音声SがIMA_ADPCMｴﾝｺｰﾄﾞのときに宣言する


/*
STM32電子ｵﾙｺﾞｰﾙ演奏+音声再生ver1_4
これは根のｿｰｽｺｰﾄﾞ

・ﾀｰｹﾞｯﾄはSTM32F103C6T6A(内部64MHzｸﾛｯｸ)

・複数のSW･単数のCdS･複数のCVDを実装可能｡但しﾎﾟｰﾄ割当てが可能なもの｡
・それぞれをｵﾙｺﾞｰﾙ演奏･ﾌﾗｯｼｭ音声再生･ﾌﾟﾛｸﾞﾗﾑ音声再生の開始･中断に充てることができる｡
・短ｵﾝで開始､長ｵﾝで中断する｡
・曲･音声はﾗｳﾝﾄﾞﾛﾋﾞﾝ･ﾗﾝﾀﾞﾑが可能｡

・SWはﾎﾟｰﾄ-GND間に設置し､負論理(導通=L)で入力する｡

・CdSはﾎﾟｰﾄ-GND間に設置し､正論理(暗=H)で入力する｡

・CVDはCVDﾊﾟｯﾄﾞﾎﾟｰﾄから10kΩの抵抗を介してﾎﾟｰﾄに入力する｡
　10kΩはﾎﾟｰﾄ保護のためであり､必須ではない｡

・xxxx_AUTOを宣言することで､自動的に曲を演奏開始､及び音声を再生開始する｡

・音声ﾃﾞｰﾀはﾌﾟﾛｸﾞﾗﾑﾒﾓﾘ､I2Cﾌﾗｯｼｭ､SDｶｰﾄﾞ､SPIﾌﾗｯｼｭに格納しておく｡


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
3:WP(GNDに接続)
4:GND
5:DI
6:CLK
7:HOLD(Vccに接続)
8:Vcc


//==============================================
//STM32のﾋﾟﾝ接
//ｼﾘｺﾝﾊﾞｸﾞを回避するためI2C1のﾘﾏｯﾌﾟは避ける
//PWM1もﾘﾏｯﾌﾟする特段の理由が無いのでﾃﾞﾌｫﾙﾄ設定とする
//SPI1はADCﾋﾟﾝを残したいのでﾘﾏｯﾌﾟする
//==============================================
STM32F103C6T6Aのﾎﾟｰﾄの割当て
1:VBAT
2:(PC13)MFRC_NSS/SDA
3:(PC14)MFRC_SCK
4:(PC15)MFRC_MOSI
5:(PD0)MFRC_MISO
6:(PD1)MFRC_RST
7:NRST
8:VSSA
9:VDDA
10:(PA0)BOOT0代替
11:(PA1)
12:(PA2)
13:(PA3)
14:(PA4)/(ADC12_IN4)CdS
15:(PA5)/(ADC12_IN5)CVD0
16:(PA6)/(ADC12_IN6)CVD1
17:(PA7)/(ADC12_IN7)CVD2
18:(PB0)SW0
19:(PB1)SW1
20:(PB2)/BOOT1(外部ﾌﾟﾙﾀﾞｳﾝ)
21:(PB10)SW2
22:(PB11)SW3
23:VSS
24:VDD
25:(PB12)
26:(PB13)/(TIM1_CH1N)PWMｺﾝﾌﾟﾘ逆相
27:(PB14)
28:(PB15)
29:(PA8)/(TIM1_CH1)PWM正相
30:(PA9)ﾃﾞﾊﾞｸﾞ情報出力/(USART1_TX)ﾃﾞﾊﾞｸﾞ情報出力/(ISP_TX)
31:(PA10)ﾃﾞﾊﾞｸﾞ信号/(USART1_RX)ﾃﾞﾊﾞｸﾞ情報入力/(ISP_RX)
32:(PA11)/(TIM1_CH4)PWMﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ逆相
33:(PA12)空き
34:(PA13)/(SWDIO)
35:VSS
36:VDD
37:(PA14)/(SWCLK)
38:(PA15)SS(SDCのCS･W25QのCSに繋ぐ､2線SPIのときは不使用)
39:(PB3)SCK/(SPI1_SCK)(SDCのCLK･W25QのCLKに繋ぐ)
40:(PB4)MISO/(SPI1_MISO)(SDCのDO･W25QのDOに繋ぐ､3線SPI･2線SPIのときは不使用)
41:(PB5)MOSI/(SPI1_MOSI)(SDCのDI･W25QのDIに繋ぐ)
42:(PB6)SCL/(I2C1_SCL)(24FCのSCLに繋ぐ)
43:(PB7)SDA/(I2C1_SDA)(24FCのSDAに繋ぐ)
44:BOOT0(外部ﾌﾟﾙﾀﾞｳﾝ)
45:(PB8)
46:(PB9)
47:VSS
48:VDD
*/


//==============================================
//ﾍｯﾀﾞﾌｧｲﾙの呼込み
//==============================================
#include <stm32f10x.h>				//ｼｽﾃﾑのﾍｯﾀﾞﾌｧｲﾙ
#include "orgel_cnf.h"				//構成情報
#include "../orgel.h"				//ｱﾌﾟﾘのﾍｯﾀﾞﾌｧｲﾙ


//==============================================
//構成情報(ｱﾌﾟﾘｹｰｼｮﾝとﾀｰｹﾞｯﾄﾃﾞﾊﾞｲｽに関わるもの)
//==============================================
//ﾃﾞﾊﾞｸﾞ信号のﾎﾟｰﾄ定義
#define	DBS_GPIO	GPIOB			//ﾃﾞﾊﾞｸﾞ信号のﾎﾟｰﾄ
#define	DBS_BIT		7			//ﾃﾞﾊﾞｸﾞ信号のﾋﾞｯﾄ

//ﾃﾞﾊﾞｸﾞ情報のﾎﾟｰﾄ定義
//#define	DBG_REMAP	AFIO_MAPR_USART1_REMAP	//ﾃﾞﾊﾞｸﾞ情報USART1をﾘﾏｯﾌﾟ(RB6､RB7)する
#define	DBG_GPIO	GPIOA			//ﾃﾞﾊﾞｸﾞ情報出力のﾎﾟｰﾄ
#define	DBG_BIT		9			//ﾃﾞﾊﾞｸﾞ情報出力のﾋﾞｯﾄ
#define	DBG_IN_GPIO	GPIOA			//ﾃﾞﾊﾞｸﾞ情報入力のﾎﾟｰﾄ
#define	DBG_IN_BIT	10			//ﾃﾞﾊﾞｸﾞ情報入力のﾋﾞｯﾄ

//PWMのﾎﾟｰﾄ定義
//#define	TIM1_REMAP	AFIO_MAPR_TIM1_REMAP_FULLREMAP
						//TIM1ﾘﾏｯﾌﾟ
#define	PWM_F_GPIO	GPIOA			//PWM正相のﾎﾟｰﾄ
#define	PWM_F_BIT	8			//PWM正相のﾋﾞｯﾄ
#define	PWM_R_GPIO	GPIOA			//PWM逆相のﾎﾟｰﾄ
#define	PWM_R_BIT	11			//PWM逆相のﾋﾞｯﾄ
#define	PWM_C_GPIO	GPIOB			//PWM相補のﾎﾟｰﾄ
#define	PWM_C_BIT	13			//PWM相補のﾋﾞｯﾄ

//I2Cのﾎﾟｰﾄ定義
//#define	I2C_REMAP	AFIO_MAPR_I2C1_REMAP	//I2C1_REMAPﾏｽｸ
#define I2C_SCL_GPIO	GPIOB			//SCLのﾎﾟｰﾄ
#define I2C_SCL_BIT	6			//SCLのﾋﾞｯﾄ
#define I2C_SDA_GPIO	GPIOB			//SDAのﾎﾟｰﾄ
#define I2C_SDA_BIT	7			//SDAのﾋﾞｯﾄ

//SPIのﾎﾟｰﾄ定義
#if defined(SHSPI2)||defined(SHSPI3)	//2線SPI又は3線SPIのとき
#define	SPI_SOFT				//SPIはｿﾌﾄ実装になる
#endif
#define	SPI_REMAP	AFIO_MAPR_SPI1_REMAP	//SPI1_REMAPﾏｽｸ
#define SPI_SCK_GPIO	GPIOB			//SCKのﾎﾟｰﾄ
#define SPI_SCK_BIT	3			//SCKのﾋﾞｯﾄ
#define SPI_MOSI_GPIO	GPIOB			//MOSIのﾎﾟｰﾄ
#define SPI_MOSI_BIT	5			//MOSIのﾋﾞｯﾄ
#ifdef SHSPI2				//2線SPIのとき
#define	SPI_SS_TIME	5			//SSﾈｹﾞｰﾄ待ち時間[us]
#define SPI_SS_GPIO	SPI_SCK_GPIO		//SSのﾎﾟｰﾄ
#define SPI_SS_BIT	SPI_SCK_BIT		//SSのﾋﾞｯﾄ
#else					//3線SPIまたは4線SPIのとき
#define SPI_SS_GPIO	GPIOA			//SSのﾎﾟｰﾄ
#define SPI_SS_BIT	15			//SSのﾋﾞｯﾄ
#endif
#if defined(SHSPI2)||defined(SHSPI3)	//2線SPIまたは3線SPIのとき
#define	SPI_SOFT				//SPIはｿﾌﾄ実装する
#define SPI_MISO_GPIO	SPI_MOSI_GPIO		//MISOのﾎﾟｰﾄ
#define SPI_MISO_BIT	SPI_MOSI_BIT		//MISOのﾋﾞｯﾄ
#else					//4線SPIのとき
#define SPI_MISO_GPIO	GPIOB			//MISOのﾎﾟｰﾄ
#define SPI_MISO_BIT	4			//MISOのﾋﾞｯﾄ
#endif

//SPIのﾌﾟﾙｱｯﾌﾟ/ﾀﾞｳﾝ定義			//SWのﾎﾟｰﾄ定義と合わせること
#define	SPI_SCK_PU				//SPI_SCKをﾌﾟﾙｱｯﾌﾟする
//#define	SPI_SCK_PD			//SPI_SCKをﾌﾟﾙﾀﾞｳﾝする
#define	SPI_MOSI_PU				//SPI_MOSIをﾌﾟﾙｱｯﾌﾟする
//#define	SPI_MOSI_PD			//SPI_MOSIをﾌﾟﾙﾀﾞｳﾝする
#define	SPI_MISO_PU				//SPI_MISOをﾌﾟﾙｱｯﾌﾟする
//#define	SPI_MISO_PD			//SPI_MISOをﾌﾟﾙﾀﾞｳﾝする

//SW0のﾎﾟｰﾄ定義
#define	SW0_GPIO	GPIOB			//SW0のﾎﾟｰﾄ
#define	SW0_EXTI_PORT	1			//SW0のEXTIﾎﾟｰﾄ(0=PA､1=PB､2=PC)
#define	SW0_BIT		0			//SW0のﾋﾞｯﾄ
#define	SW0_INV					//SW0入力は負論理
					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
#define SW0_TIMER				//SW0はﾀｲﾏｰﾓｰﾄﾞ
//#define	SW0_REAL			//SW0はﾘｱﾙﾓｰﾄﾞ
#define	SW0_CNT		ORGEL_CNT|VOICEN_CNT|VOICEI_CNT|VOICEC_CNT|VOICES_CNT
					//SW0の操作対象 

//SW1のﾎﾟｰﾄ定義
#define	SW1_GPIO	GPIOB			//SW1のﾎﾟｰﾄ
#define	SW1_EXTI_PORT	1			//SW1のEXTIﾎﾟｰﾄ(0=PA､1=PB､5=PF)
#define	SW1_BIT		1			//SW1のﾋﾞｯﾄ
#define	SW1_INV					//SW1入力は負論理
					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
#define SW1_TIMER				//SW1はﾀｲﾏｰﾓｰﾄﾞ
//#define	SW1_REAL			//SW1はﾘｱﾙﾓｰﾄﾞ
#define	SW1_CNT		ORGEL_CNT|VOICEN_CNT|VOICEI_CNT|VOICEC_CNT|VOICES_CNT
						//SW1の操作対象

//SW2のﾎﾟｰﾄ定義
#define	SW2_GPIO	GPIOB			//SW2のﾎﾟｰﾄ
#define	SW2_EXTI_PORT	1			//SW2のEXTIﾎﾟｰﾄ(0=PA､1=PB､5=PF)
#define	SW2_BIT		10			//SW2のﾋﾞｯﾄ
#define	SW2_INV					//SW2入力は負論理
//					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
#define SW2_TIMER				//SW2はﾀｲﾏｰﾓｰﾄﾞ
//#define	SW2_REAL			//SW2はﾘｱﾙﾓｰﾄﾞ
#define	SW2_CNT		ORGEL_CNT|VOICEN_CNT|VOICEI_CNT|VOICEC_CNT|VOICES_CNT
						//SW2の操作対象

//SW3のﾎﾟｰﾄ定義
#define	SW3_GPIO	GPIOB			//SW3のﾎﾟｰﾄ
#define	SW3_EXTI_PORT	1			//SW3のEXTIﾎﾟｰﾄ(0=PA､1=PB､5=PF)
#define	SW3_BIT		11			//SW3のﾋﾞｯﾄ
#define	SW3_INV					//SW3入力は負論理
//					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
#define SW3_TIMER				//SW3はﾀｲﾏｰﾓｰﾄﾞ
//#define	SW3_REAL			//SW3はﾘｱﾙﾓｰﾄﾞ
#define	SW3_CNT		ORGEL_CNT|VOICEN_CNT|VOICEI_CNT|VOICEC_CNT|VOICES_CNT
						//SW3の操作対象

//CdSのﾎﾟｰﾄ定義
#define	CdS_GPIO	GPIOA			//CdSのﾎﾟｰﾄ
#define	CdS_BIT		4			//CdSのﾋﾞｯﾄ
#define	CdS_CHSEL	4			//CdSのADIN
//#define	CdS_INV				//CdS入力は負論理(暗=L)
					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
#define CdS_TIMER				//CdSはﾀｲﾏｰﾓｰﾄﾞ
//#define	CdS_REAL			//CdSはﾘｱﾙﾓｰﾄﾞ
#define	CdS_CNT		ORGEL_CNT|VOICEN_CNT|VOICEI_CNT|VOICEC_CNT|VOICES_CNT
						//CdSの操作対象

//CVD0のﾎﾟｰﾄ定義
#define	CVD0_GPIO	GPIOA			//CVD0のﾎﾟｰﾄ
#define	CVD0_BIT	5			//CVD0のﾋﾞｯﾄ
#define	CVD0_CHSEL	5			//CVD0のADIN
					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
#define CVD0_TIMER				//CVD0はﾀｲﾏｰﾓｰﾄﾞ
//#define	CVD0_REAL			//CVD0はﾘｱﾙﾓｰﾄﾞ
#define	CVD0_CNT	ORGEL_CNT|VOICEN_CNT|VOICEI_CNT|VOICEC_CNT|VOICES_CNT
						//CVD0の操作対象

//CVD1のﾎﾟｰﾄ定義
#define	CVD1_GPIO	GPIOA			//CVD1のﾎﾟｰﾄ
#define	CVD1_BIT	6			//CVD1のﾋﾞｯﾄ
#define	CVD1_CHSEL	6			//CVD1のADIN
//					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
#define CVD1_TIMER				//CVD1はﾀｲﾏｰﾓｰﾄﾞ
//#define	CVD1_REAL			//CVD1はﾘｱﾙﾓｰﾄﾞ
#define	CVD1_CNT	ORGEL_CNT|VOICEN_CNT|VOICEI_CNT|VOICEC_CNT|VOICES_CNT
						//CVD1の操作対象

//CVD2のﾎﾟｰﾄ定義
//#define	CVD2_GPIO	GPIOA		//CVD2のﾎﾟｰﾄ
//#define	CVD2_BIT	7		//CVD2のﾋﾞｯﾄ
//#define	CVD2_CHSEL	7		//CVD2のADIN
//					//以下のﾀｲﾏｰﾓｰﾄﾞとﾘｱﾙﾓｰﾄﾞは択一
//#define CVD2_TIMER				//CVD2はﾀｲﾏｰﾓｰﾄﾞ
//#define	CVD2_REAL			//CVD2はﾘｱﾙﾓｰﾄﾞ
//#define	CVD2_CNT	NON_CNT		//CVD2の操作対象

//LEDのﾎﾟｰﾄ定義
//#define	LED0_GPIO	GPIOB		//LED0のﾎﾟｰﾄ
//#define	LED0_BIT	1		//LED0のﾋﾞｯﾄ
//#define	LED1_GPIO	GPIOD		//LED1のﾎﾟｰﾄ
//#define	LED1_BIT	2		//LED1のﾋﾞｯﾄ
//#define	LED2_GPIO	GPIOD		//LED2のﾎﾟｰﾄ
//#define	LED2_BIT	3		//LED2のﾋﾞｯﾄ
//#define	LED3_GPIO	GPIOC		//LED3のﾎﾟｰﾄ
//#define	LED3_BIT	2		//LED3のﾋﾞｯﾄ

//MFRCのﾎﾟｰﾄ定義
#define	MFRC_RST_GPIO	GPIOA			//MFRC_RSTのﾎﾟｰﾄ
#define	MFRC_RST_BIT	8			//MFRC_RSTのﾋﾞｯﾄ
#define	MFRC_SCK_GPIO	GPIOB			//MFRC_SCKのﾎﾟｰﾄ
#define	MFRC_SCK_BIT	14			//MFRC_SCKのﾋﾞｯﾄ
#define	MFRC_NSS_GPIO	GPIOB			//MFRC_NSSのﾎﾟｰﾄ
#define	MFRC_NSS_BIT	15			//MFRC_NSSのﾋﾞｯﾄ
#define	MFRC_MOSI_GPIO	GPIOB			//MFRC_MOSIのﾎﾟｰﾄ
#define	MFRC_MOSI_BIT	13			//MFRC_MOSIのﾋﾞｯﾄ
#define	MFRC_MISO_GPIO	GPIOB			//MFRC_MISOのﾎﾟｰﾄ
#define	MFRC_MISO_BIT	12			//MFRC_MISOのﾋﾞｯﾄ


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
//	voc_mac(0x002000,0x000080,8000)		//anpanの一部(ﾃｽﾄ用)
//	voc_mac(0x004000,0x000080,8000)		//anpanの一部(ﾃｽﾄ用)

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
	vos_mac(0x000000,0x002401,8000)		//sharin.wav
	vos_mac(0x002401,0x004575,8000)		//kirakiraomeme.wav
	vos_mac(0x006976,0x001956,8000)		//aidorumitai.wav
	vos_mac(0x0082cc,0x002b18,8000)		//kyounooshare.wav
	vos_mac(0x00ade4,0x001d61,8000)		//egaogasuteki.wav
	vos_mac(0x00cb45,0x0033a5,8000)		//kirarin.wav

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
#define	DEV_F103				//ﾃﾞﾊﾞｲｽはF103
#define	I2Cx	I2C1				//I2C1を使う
#define	SPIx	SPI1				//SPI1を使う
#include "../dev_F103.c"			//F103ﾃﾞﾊﾞｲｽｺｰﾄﾞ
#include "../apl_SW.c"				//SW操作ｱﾌﾟﾘｹｰｼｮﾝ
