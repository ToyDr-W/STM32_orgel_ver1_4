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
//
//SRM32F030F4のﾋﾟﾝ割当ての事情
//SPI内蔵ﾓｼﾞｭｰﾙはPA5(SPI1_SCK)･PA6(SPI1_MISO)･PA7(SPI1_MOSI)しかない｡
//I2C内蔵ﾓｼﾞｭｰﾙはPA8(I2C1_SCL)･PA10(I2C1_SDA)しかない｡
//残るﾋﾟﾝでPWM出力が可能なﾋﾟﾝはPB1(TIM1_CH3N/TIM3_CH4)しかない。
//TIM3はｺﾝﾌﾟﾘ出力が無くいので､TIM1_CH3Nを正相に割当てる｡
//逆相はSPI又はI2Cをｿﾌﾄ実装して､PWMﾋﾟﾝを確保する｡
//
//ﾋﾟﾝ割当てﾊﾟﾀｰﾝ選択の指針
//I2Cをｿﾌﾄ実装とすれば全出力ﾀｲﾌﾟが可能になる
//I2Cは内蔵ﾓｼﾞｭｰﾙを使用し､SPIをｿﾌﾄ実装すればｺﾝﾌﾟﾘ以外の出力ﾀｲﾌﾟが可能になる｡
//I2CとSPIを共に内蔵ﾓｼﾞｭｰﾙを使用するとｼﾝｸﾞﾙ出力のみが可能になる｡
//2線SPIと3線SPIでは､SPIはｿﾌﾄ実装になることも考慮して決める｡
//==============================================
STM32F030F4P6のﾎﾟｰﾄの割当て

//SPI=内蔵ﾓｼﾞｭｰﾙ､I2C=内蔵ﾓｼﾞｭｰﾙのとき､PWMはｼﾝｸﾞﾙ出力のみ可能
1:BOOT0
2:(PF0)
3:(PF1)
4:NRST
5:VDDA
6:(PA0)BOOT0代替､SW2/(ADC_IN0)CdS･CVD2
7:(PA1)SW1/(ADC_IN1)CVD1
8:(PA2)SW0/(ADC_IN2)CVD0
9:(PA3)ﾃﾞﾊﾞｸﾞ信号出力/(USART1_RX)ﾃﾞﾊﾞｸﾞ情報入力
10:(PA4)SS(SDCのCS･W25QのCSに繋ぐ)
11:(PA5)/(SPI1_SCK)(SDCのCLK･W25QのCLKに繋ぐ)
12:(PA6)/(SPI1_MISO)(SDCのDO･W25QのDOに繋ぐ)
13:(PA7)/(SPI1_MOSI)(SDCのDI･W25QのDIに繋ぐ)
14:(PB1)/(TIM1_CH3N)ｼﾝｸﾞﾙ正相出力
15:VSS
16:VDD
17:(PA9)/(I2C1_SCL)SCL/(ISP_TX)
18:(PA10)/(I2C1_SDA)SDA/(ISP_RX)
19:(PA13)LED0/(SWDIO)
20:(PA14)LED1･ﾃﾞﾊﾞｸﾞ情報出力/(USART1_TX)ﾃﾞﾊﾞｸﾞ情報出力/(SWCLK)

//SPI=内蔵ﾓｼﾞｭｰﾙ､I2C=ｿﾌﾄ実装のとき､PWMは全出力ﾀｲﾌﾟが可能
1:BOOT0
2:(PF0)SCL
3:(PF1)SDA
4:NRST
5:VDDA
6:(PA0)BOOT0代替､SW2/(ADC_IN0)CdS
7:(PA1)SW1/(ADC_IN1)CVD1
8:(PA2)SW0/(ADC_IN2)CVD0
9:(PA3)ﾃﾞﾊﾞｸﾞ信号出力/(USART1_RX)ﾃﾞﾊﾞｸﾞ情報入力
10:(PA4)SS(SDCのCS･W25QのCSに繋ぐ)
11:(PA5)/(SPI1_SCK)(SDCのCLK･W25QのCLKに繋ぐ)
12:(PA6)/(SPI1_MISO)(SDCのDO･W25QのDOに繋ぐ)
13:(PA7)/(SPI1_MOSI)(SDCのDI･W25QのDIに繋ぐ)
14:(PB1)/(TIM1_CH3N)正相出力
15:VSS
16:VDD
17:(PA9)/(TIM1_CH2)ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ逆相出力/(ISP_TX)
18:(PA10)/(TIM1_CH3)ｺﾝﾌﾟﾘ逆相出力/(ISP_RX)
19:(PA13)LED0/(SWDIO)
20:(PA14)LED1･ﾃﾞﾊﾞｸﾞ情報出力/(USART1_TX)ﾃﾞﾊﾞｸﾞ情報出力/(SWCLK)

//SPI=ｿﾌﾄ実装､I2C=内蔵ﾓｼﾞｭｰﾙのとき､PWMはｼﾝｸﾞﾙ･ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ出力が可能､ｺﾝﾌﾟﾘ出力は不可
1:BOOT0
2:(PF0)MOSI(SDCのDI･W25QのDIに繋ぐ)
3:(PF1)
4:NRST
5:VDDA
6:(PA0)BOOT0代替､SW2/(ADC_IN0)CdS
7:(PA1)SW1/(ADC_IN1)CVD1
8:(PA2)SW0/(ADC_IN2)CVD0
9:(PA3)ﾃﾞﾊﾞｸﾞ信号出力/(USART1_RX)ﾃﾞﾊﾞｸﾞ情報入力
10:(PA4)SS(SDCのCS･W25QのCSに繋ぐ)
11:(PA5)SCK(SDCのCLK･W25QのCLKに繋ぐ)
12:(PA6)MISO(SDCのDO･W25QのDOに繋ぐ､3線SPI･2線SPIのときは不使用)
13:(PA7)/(TIM1_CH1N)ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ逆相出力
14:(PB1)/(TIM1_CH3N)ｼﾝｸﾞﾙ･ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ正相出力
15:VSS
16:VDD
17:(PA9)/(I2C1_SCL)SCL/(ISP_TX)
18:(PA10)/(I2C1_SDA)SDA/(ISP_RX)
19:(PA13)LED0/(SWDIO)
20:(PA14)LED1･ﾃﾞﾊﾞｸﾞ情報出力/(USART1_TX)ﾃﾞﾊﾞｸﾞ情報出力/(SWCLK)

//SPI=ｿﾌﾄ実装､I2C=ｿﾌﾄ実装のとき､PWMは全出力ﾀｲﾌﾟが可能
1:BOOT0
2:(PF0)SCL
3:(PF1)SDA
4:NRST
5:VDDA
6:(PA0)BOOT0代替､SW2/(ADC_IN0)CdS
7:(PA1)SW1/(ADC_IN1)CVD1
8:(PA2)SW0/(ADC_IN2)CVD0
9:(PA3)ﾃﾞﾊﾞｸﾞ信号出力/(USART1_RX)ﾃﾞﾊﾞｸﾞ情報入力
10:(PA4)SS(SDCのCS･W25QのCSに繋ぐ)
11:(PA5)SCK(SDCのCLK･W25QのCLKに繋ぐ)
12:(PA6)MISO(SDCのDO･W25QのDOに繋ぐ､3線SPI･2線SPIのときは不使用)
13:(PA7)MOSI)(SDCのDI･W25QのDIに繋ぐ)
14:(PB1)/(TIM1_CH3N)正相出力
15:VSS
16:VDD
17:(PA9)(TIM1_CH2)ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ逆相出力/(ISP_TX)
18:(PA10)/(TIM1_CH3)ｺﾝﾌﾟﾘ逆相出力/(ISP_RX)
19:(PA13)LED0/(SWDIO)
20:(PA14)LED1･ﾃﾞﾊﾞｸﾞ情報出力/(USART1_TX)ﾃﾞﾊﾞｸﾞ情報出力/(SWCLK)
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
#define	UART_BIT	14			//UART出力のﾋﾞｯﾄ
#define	UART_AF		1			//UART出力のUART_TX交代機能番号
#define	UART_IN_GPIO	GPIOA			//UART入力のﾎﾟｰﾄ
#define	UART_IN_BIT	3			//UART入力のﾋﾞｯﾄ
#define	UART_IN_AF	1			//UART入力のUART_RX交代機能番号

//ﾃﾞﾊﾞｸﾞ信号のﾎﾟｰﾄ定義
#define	DBS_GPIO	GPIOA			//ﾃﾞﾊﾞｸﾞ信号のﾎﾟｰﾄ
#define	DBS_BIT		3			//ﾃﾞﾊﾞｸﾞ信号のﾋﾞｯﾄ

//ﾃﾞﾊﾞｸﾞ情報のﾎﾟｰﾄ定義
#ifdef	ISP_SPIFLASH_EN			//ISP環境でSPIﾌﾗｯｼｭを書替えるとき
#define	SHDBG_SOFT				//ﾃﾞﾊﾞｸﾞ情報のUARTをｿﾌﾄ実装する
#define	DBG_GPIO	GPIOA			//ﾃﾞﾊﾞｸﾞ情報出力のﾎﾟｰﾄ
#define	DBG_BIT		13			//ﾃﾞﾊﾞｸﾞ情報出力のﾋﾞｯﾄ
#else					//電子ｵﾙｺﾞｰﾙ動作のとき
#define	DBG_GPIO	GPIOA			//ﾃﾞﾊﾞｸﾞ情報出力のﾎﾟｰﾄ
#define	DBG_BIT		14			//ﾃﾞﾊﾞｸﾞ情報出力のﾋﾞｯﾄ
#define	DBG_AF		1			//ﾃﾞﾊﾞｸﾞ情報出力のUART_TX交代機能番号
#define	DBG_IN_GPIO	GPIOA			//ﾃﾞﾊﾞｸﾞ情報入力のﾎﾟｰﾄ
#define	DBG_IN_BIT	3			//ﾃﾞﾊﾞｸﾞ情報入力のﾋﾞｯﾄ
#define	DBG_IN_AF	1			//ﾃﾞﾊﾞｸﾞ情報入力のUART_RX交代機能番号
#endif

#if defined(SHSPI2)||defined(SHSPI3)	//2線SPI又は3線SPIのとき
#define	SPI_SOFT				//SPIはｿﾌﾄ実装する
#endif

//以下のPWM･I2C･SPIのﾎﾟｰﾄ定義はI2C･SPIのｿﾌﾄ実装/内蔵ﾓｼﾞｭｰﾙ使用の別で異なる
#if !defined(SPI_SOFT)&&!defined(I2C_SOFT)	////SPI=内蔵ﾓｼﾞｭｰﾙ､I2C=内蔵ﾓｼﾞｭｰﾙのとき
//PWMのﾎﾟｰﾄ定義
#define	PWM_F_GPIO	GPIOB			//PWM正相のﾎﾟｰﾄ
#define	PWM_F_BIT	1			//PWM正相のﾋﾞｯﾄ
#define	PWM_F_AF	2			//PWM正相の交代機能番号
#define	PWM_3N					//ｼﾝｸﾞﾙ3N出力のみ

//I2Cのﾎﾟｰﾄ定義
#define I2C_SCL_GPIO	GPIOA			//SCLのﾎﾟｰﾄ
#define I2C_SCL_BIT	9			//SCLのﾋﾞｯﾄ
#define I2C_SCL_AF	4			//SCLの交代機能番号
#define I2C_SDA_GPIO	GPIOA			//SDAのﾎﾟｰﾄ
#define I2C_SDA_BIT	10			//SDAのﾋﾞｯﾄ
#define I2C_SDA_AF	4			//SDAの交代機能番号

//SPIのﾎﾟｰﾄ定義
#define SPI_SCK_GPIO	GPIOA			//SCKのﾎﾟｰﾄ
#define SPI_SCK_BIT	5			//SCKのﾋﾞｯﾄ
#define	SPI_SCK_AF	0			//SCKの交代機能番号
#define SPI_MOSI_GPIO	GPIOA			//MOSIのﾎﾟｰﾄ
#define SPI_MOSI_BIT	7			//MOSIのﾋﾞｯﾄ
#define	SPI_MOSI_AF	0			//MOSIの交代機能番号
#define SPI_SS_GPIO	GPIOA			//SSのﾎﾟｰﾄ
#define SPI_SS_BIT	4			//SSのﾋﾞｯﾄ
#define SPI_MISO_GPIO	GPIOA			//MISOのﾎﾟｰﾄ
#define SPI_MISO_BIT	6			//MISOのﾋﾞｯﾄ
#define	SPI_MISO_AF	0			//MISOの交代機能番号
#endif

#if !defined(SPI_SOFT)&&defined(I2C_SOFT)	////SPI=内蔵ﾓｼﾞｭｰﾙ､I2C=ｿﾌﾄ実装のとき
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
#define	PWM_3N_2_3				//正相3N､ﾌﾞﾘｯｼﾞ逆相2､ｺﾝﾌﾟﾘ逆相3

//I2Cのﾎﾟｰﾄ定義
#define I2C_SCL_GPIO	GPIOF			//SCLのﾎﾟｰﾄ
#define I2C_SCL_BIT	0			//SCLのﾋﾞｯﾄ
#define I2C_SDA_GPIO	GPIOF			//SDAのﾎﾟｰﾄ
#define I2C_SDA_BIT	1			//SDAのﾋﾞｯﾄ

//SPIのﾎﾟｰﾄ定義
#define SPI_SCK_GPIO	GPIOA			//SCKのﾎﾟｰﾄ
#define SPI_SCK_BIT	5			//SCKのﾋﾞｯﾄ
#define	SPI_SCK_AF	0			//SCKの交代機能番号
#define SPI_MOSI_GPIO	GPIOA			//MOSIのﾎﾟｰﾄ
#define SPI_MOSI_BIT	7			//MOSIのﾋﾞｯﾄ
#define	SPI_MOSI_AF	0			//MOSIの交代機能番号
#define SPI_SS_GPIO	GPIOA			//SSのﾎﾟｰﾄ
#define SPI_SS_BIT	4			//SSのﾋﾞｯﾄ
#define SPI_MISO_GPIO	GPIOA			//MISOのﾎﾟｰﾄ
#define SPI_MISO_BIT	6			//MISOのﾋﾞｯﾄ
#define	SPI_MISO_AF	0			//MISOの交代機能番号
#endif

#if defined(SPI_SOFT)&&!defined(I2C_SOFT)	////SPI=ｿﾌﾄ実装､I2C=内蔵ﾓｼﾞｭｰﾙのとき
//PWMのﾎﾟｰﾄ定義
#define	PWM_F_GPIO	GPIOB			//PWM正相のﾎﾟｰﾄ
#define	PWM_F_BIT	1			//PWM正相のﾋﾞｯﾄ
#define	PWM_F_AF	2			//PWM正相の交代機能番号
#define	PWM_R_GPIO	GPIOA			//PWM逆相のﾎﾟｰﾄ
#define	PWM_R_BIT	7			//PWM逆相のﾋﾞｯﾄ
#define	PWM_R_AF	2			//PWM逆相の交代機能番号
#define	PWM_3N_1N				//正相3N､ﾌﾞﾘｯｼﾞ逆相1N

//I2Cのﾎﾟｰﾄ定義
#define I2C_SCL_GPIO	GPIOA			//SCLのﾎﾟｰﾄ
#define I2C_SCL_BIT	9			//SCLのﾋﾞｯﾄ
#define I2C_SCL_AF	4			//SCLの交代機能番号
#define I2C_SDA_GPIO	GPIOA			//SDAのﾎﾟｰﾄ
#define I2C_SDA_BIT	10			//SDAのﾋﾞｯﾄ
#define I2C_SDA_AF	4			//SDAの交代機能番号

//SPIのﾎﾟｰﾄ定義
#define SPI_SCK_GPIO	GPIOA			//SCKのﾎﾟｰﾄ
#define SPI_SCK_BIT	5			//SCKのﾋﾞｯﾄ
#define SPI_MOSI_GPIO	GPIOF			//MOSIのﾎﾟｰﾄ
#define SPI_MOSI_BIT	0			//MOSIのﾋﾞｯﾄ
#ifdef SHSPI2				//2線SPIのとき
#define	SPI_SS_TIME	5			//SSﾈｹﾞｰﾄ待ち時間[us]
#define SPI_SS_GPIO	SPI_SCK_GPIO		//SSのﾎﾟｰﾄ
#define SPI_SS_BIT	SPI_SCK_BIT		//SSのﾋﾞｯﾄ
#else					//3線SPIまたは4線SPIのとき
#define SPI_SS_GPIO	GPIOA			//SSのﾎﾟｰﾄ
#define SPI_SS_BIT	4			//SSのﾋﾞｯﾄ
#endif
#if defined(SHSPI2)||defined(SHSPI3)	//2線SPIまたは3線SPIのとき
#define SPI_MISO_GPIO	SPI_MOSI_GPIO		//MISOのﾎﾟｰﾄ
#define SPI_MISO_BIT	SPI_MOSI_BIT		//MISOのﾋﾞｯﾄ
#else					//4線SPIのとき
#define SPI_MISO_GPIO	GPIOA			//MISOのﾎﾟｰﾄ
#define SPI_MISO_BIT	6			//MISOのﾋﾞｯﾄ
#endif
#endif

#if defined(SPI_SOFT)&&defined(I2C_SOFT)	////SPI=ｿﾌﾄ実装､I2C=ｿﾌﾄ実装のとき
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
#define	PWM_3N_2_3				//正相3N､ﾌﾞﾘｯｼﾞ逆相2､ｺﾝﾌﾟﾘ逆相3

//I2Cのﾎﾟｰﾄ定義
#define I2C_SCL_GPIO	GPIOF			//SCLのﾎﾟｰﾄ
#define I2C_SCL_BIT	0			//SCLのﾋﾞｯﾄ
#define I2C_SDA_GPIO	GPIOF			//SDAのﾎﾟｰﾄ
#define I2C_SDA_BIT	1			//SDAのﾋﾞｯﾄ

//SPIのﾎﾟｰﾄ定義
#define SPI_SCK_GPIO	GPIOA			//SCKのﾎﾟｰﾄ
#define SPI_SCK_BIT	5			//SCKのﾋﾞｯﾄ
#define SPI_MOSI_GPIO	GPIOA			//MOSIのﾎﾟｰﾄ
#define SPI_MOSI_BIT	7			//MOSIのﾋﾞｯﾄ
#ifdef SHSPI2				//2線SPIのとき
#define	SPI_SS_TIME	5			//SSﾈｹﾞｰﾄ待ち時間[us]
#define SPI_SS_GPIO	SPI_SCK_GPIO		//SSのﾎﾟｰﾄ
#define SPI_SS_BIT	SPI_SCK_BIT		//SSのﾋﾞｯﾄ
#else					//3線SPIまたは4線SPIのとき
#define SPI_SS_GPIO	GPIOA			//SSのﾎﾟｰﾄ
#define SPI_SS_BIT	4			//SSのﾋﾞｯﾄ
#endif
#if defined(SHSPI2)||defined(SHSPI3)	//2線SPIまたは3線SPIのとき
#define SPI_MISO_GPIO	SPI_MOSI_GPIO		//MISOのﾎﾟｰﾄ
#define SPI_MISO_BIT	SPI_MOSI_BIT		//MISOのﾋﾞｯﾄ
#else					//4線SPIのとき
#define SPI_MISO_GPIO	GPIOA			//MISOのﾎﾟｰﾄ
#define SPI_MISO_BIT	6			//MISOのﾋﾞｯﾄ
#endif
#endif

//SPIのﾌﾟﾙｱｯﾌﾟ/ﾀﾞｳﾝ定義			//SWのﾎﾟｰﾄ定義と合わせること
#define	SPI_SCK_PU				//SPI_SCKをﾌﾟﾙｱｯﾌﾟする
//#define	SPI_SCK_PD			//SPI_SCKをﾌﾟﾙﾀﾞｳﾝする
#define	SPI_MOSI_PU				//SPI_MOSIをﾌﾟﾙｱｯﾌﾟする
//#define	SPI_MOSI_PD			//SPI_MOSIをﾌﾟﾙﾀﾞｳﾝする
#define	SPI_MISO_PU				//SPI_MISOをﾌﾟﾙｱｯﾌﾟする
//#define	SPI_MISO_PD			//SPI_MISOをﾌﾟﾙﾀﾞｳﾝする

//SW_のﾎﾟｰﾄ定義
#define	SW0_GPIO	GPIOA			//SW0のﾎﾟｰﾄ
#define	SW0_BIT		2			//SW0のﾋﾞｯﾄ


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
#include "../apl_SW-PEDAL.c"			//ﾄﾞﾗｲﾌﾞﾊﾝﾄﾞﾙのｱﾌﾟﾘｹｰｼｮﾝ
#endif
