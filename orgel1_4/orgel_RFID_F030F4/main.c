//#define	SHDBS		//ﾃﾞﾊﾞｸﾞ信号を出力するときに宣言する
//#define	SHDBG		//ﾃﾞﾊﾞｸﾞ情報を出力するときに宣言する
//#define	SHBPS	115200	//ﾃﾞﾊﾞｸﾞ情報のﾎﾞｰﾚｰﾄを宣言する(ﾃﾞﾌｫﾙﾄ値は38400bps)
//#define	SHDBG_SOFT	//ﾃﾞﾊﾞｸﾞ情報のUARTをｿﾌﾄ実装するときに宣言する		

//#define	SPI_SOFT	//SPIをｿﾌﾄ実装するときに宣言する
//#define	SHSPI2		//2線SPIのときに宣言する
//#define	SHSPI3		//3線SPIのときに宣言する

//#define	MFRC_SPI3	3線SPIのときに宣言する
//#define	MFRC_SPI2	2線SPIのときに宣言する

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

・ﾀｰｹﾞｯﾄはSTM32F030F4P6(内部64MHzｸﾛｯｸ)

以下は電子ｵﾙｺﾞｰﾙとして動作するときの説明である｡

・RFIDﾀｸﾞを読んで､対応する曲を演奏する｡

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
3:WP(GNDに接続､ISPを利用する場合はVccに接続)
4:GND
5:DI
6:CLK
7:HOLD(Vccに接続)
8:Vcc


//==============================================
//RFID_RC522のﾋﾟﾝ接
//==============================================
1:NSS(基板ｼﾙｸはSDA)(SPIのSS)(2線SPIでは10k*220pの時定数回路を介してCLKに繋ぐ)
2:SCK(SPIのSCK)
3:MOSI(SPIのMOSI)
4:MISO(SPIのMISO)(3線SPIでは3.3kを介してMOSIに繋ぐ)
5:IRQ(このﾌｧｰﾑｳｪｱでは不使用)
6:GND
7:RST(このﾌｧｰﾑｳｪｱでは不使用)
8:3.3V


//==============================================
//STM32のﾋﾟﾝ接
//==============================================
STM32F030F4P6のﾎﾟｰﾄの割当て
1:BOOT0
2:(PF0)SCL
3:(PF1)SDA
4:NRST
5:VDDA
6:(PA0)BOOT0代替､MFRC_MISO(RC522のMISOに繋ぐ､3線SPI･2線SPIのときは不使用)
7:(PA1)MFRC_MOSI(RC522のMOSIに繋ぐ)
8:(PA2)MFRC_SCK(RC522のSCKに繋ぐ)
9:(PA3)ﾃﾞﾊﾞｸﾞ信号出力/(USART1_RX)ﾃﾞﾊﾞｸﾞ情報入力･UART入力
10:(PA4)MFRC_NSS/SDA(RC522のNSSに繋ぐ､2線SPIのときは不使用)
11:(PA5)SCK/(SPI1_SCK)(SDCのCLK･W25QのCLKに繋ぐ)
12:(PA6)MISO/(SPI1_MISO)(SDCのDO･W25QのDOに繋ぐ､3線SPI･2線SPIのときは不使用)
13:(PA7)MOSI/(SPI1_MOSI)(SDCのDI･W25QのDIに繋ぐ)
14:(PB1)/(TIM1_CH3N)正相
15:VSS
16:VDD
17:(PA9)/(TIM1_CH2)ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ逆相/(ISP_TX)
18:(PA10)ISP時のﾃﾞﾊﾞｸﾞ情報出力/(TIM1_CH3)ｺﾝﾌﾟﾘ逆相/(ISP_RX)
19:(PA13)SS(SDCのCS･W25QのCSに繋ぐ､2線SPIのときは不使用)/(SWDIO)
20:(PA14)ﾃﾞﾊﾞｸﾞ情報出力/(USART1_TX)ﾃﾞﾊﾞｸﾞ情報出力･UART出力/(SWCLK)
*/


//==============================================
//ﾍｯﾀﾞﾌｧｲﾙの呼込み
//==============================================
#include <stm32f0xx.h>				//ｼｽﾃﾑのﾍｯﾀﾞﾌｧｲﾙ
#include "orgel_cnf.h"				//構成情報
#include "../orgel.h"				//ｱﾌﾟﾘのﾍｯﾀﾞﾌｧｲﾙ


//==============================================
//ﾀｸﾞのｼﾘｱﾙ番号(5ﾊﾞｲﾄ目)
//==============================================
#define	TAG_NO0	0x33	//ﾀｸﾞ1(白ｶｰﾄﾞ)
#define	TAG_NO1	0x98	//ﾀｸﾞ2(赤ｺｲﾝ)
#define	TAG_NO2	0xa2	//ﾀｸﾞ3(青ｺｲﾝ)
#define	TAG_NO3	0x04	//ﾀｸﾞ4(黄ｺｲﾝ)


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
#define	DBG_BIT		10			//ﾃﾞﾊﾞｸﾞ情報出力のﾋﾞｯﾄ
#else					//電子ｵﾙｺﾞｰﾙ動作のとき
#define	DBG_GPIO	GPIOA			//ﾃﾞﾊﾞｸﾞ情報出力のﾎﾟｰﾄ
#define	DBG_BIT		14			//ﾃﾞﾊﾞｸﾞ情報出力のﾋﾞｯﾄ
#define	DBG_AF		1			//ﾃﾞﾊﾞｸﾞ情報出力のUART_TX交代機能番号
#define	DBG_IN_GPIO	GPIOA			//ﾃﾞﾊﾞｸﾞ情報入力のﾎﾟｰﾄ
#define	DBG_IN_BIT	3			//ﾃﾞﾊﾞｸﾞ情報入力のﾋﾞｯﾄ
#define	DBG_IN_AF	1			//ﾃﾞﾊﾞｸﾞ情報入力のUART_RX交代機能番号
#endif

//PWMのﾎﾟｰﾄ定義
#define	PWM_F_GPIO	GPIOA			//PWM正相のﾎﾟｰﾄ
#define	PWM_F_BIT	10			//PWM正相のﾋﾞｯﾄ
#define	PWM_F_AF	2			//PWM正相の交代機能番号
#define	PWM_R_GPIO	GPIOA			//PWM逆相のﾎﾟｰﾄ
#define	PWM_R_BIT	9			//PWM逆相のﾋﾞｯﾄ
#define	PWM_R_AF	2			//PWM逆相の交代機能番号
#define	PWM_C_GPIO	GPIOB			//PWM相補のﾎﾟｰﾄ
#define	PWM_C_BIT	1			//PWM相補のﾋﾞｯﾄ
#define	PWM_C_AF	2			//PWM相補の交代機能番号

//I2Cのﾎﾟｰﾄ定義
#define	I2C_SOFT				//I2Cをｿﾌﾄ実装する
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
#ifdef SHSPI2				//2線SPIのとき
#define	SPI_SS_TIME	15			//SSﾈｹﾞｰﾄ待ち時間[us]
#define SPI_SS_GPIO	SPI_SCK_GPIO		//SSのﾎﾟｰﾄ
#define SPI_SS_BIT	SPI_SCK_BIT		//SSのﾋﾞｯﾄ
#else					//3線SPIまたは4線SPIのとき
#define SPI_SS_GPIO	GPIOA			//SSのﾎﾟｰﾄ
#define SPI_SS_BIT	13			//SSのﾋﾞｯﾄ
#endif
#if defined(SHSPI2)||defined(SHSPI3)	//2線SPIまたは3線SPIのとき
#define	SPI_SOFT				//SPIをｿﾌﾄ実装する
#define SPI_MISO_GPIO	SPI_MOSI_GPIO		//MISOのﾎﾟｰﾄ
#define SPI_MISO_BIT	SPI_MOSI_BIT		//MISOのﾋﾞｯﾄ
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

//MFRCのﾎﾟｰﾄ定義
#define	MFRC_SCK_GPIO	GPIOA			//MFRC_SCKのﾎﾟｰﾄ
#define	MFRC_SCK_BIT	2			//MFRC_SCKのﾋﾞｯﾄ
#define	MFRC_MOSI_GPIO	GPIOA			//MFRC_MOSIのﾎﾟｰﾄ
#define	MFRC_MOSI_BIT	1			//MFRC_MOSIのﾋﾞｯﾄ
#if defined(MFRC_SPI3)||defined(MFRC_SPI2)
					//3線SPIか2線SPIのとき
#define MFRC_MISO_GPIO	MFRC_MOSI_GPIO		//RC522_MISOのﾎﾟｰﾄ
#define MFRC_MISO_BIT	MFRC_MOSI_BIT		//RC522_MISOのﾋﾞｯﾄ
#else					//4線SPIのとき
#define	MFRC_MISO_GPIO	GPIOA			//MFRC_MISOのﾎﾟｰﾄ
#define	MFRC_MISO_BIT	0			//MFRC_MISOのﾋﾞｯﾄ
#endif
#ifdef  MFRC_SPI2			//2線SPIのとき
#define MFRC_SS_TIME	15			//SSﾈｹﾞｰﾄ待ち時間[us]
#define MFRC_NSS_GPIO	MFRC_SCK_GPIO		//RC522_NSSのﾎﾟｰﾄ
#define MFRC_NSS_BIT	MFRC_SCK_BIT		//RC522_NSSのﾋﾞｯﾄ
#else					//4線SPIか3線SPIのとき
#define	MFRC_NSS_GPIO	GPIOA			//MFRC_NSSのﾎﾟｰﾄ
#define	MFRC_NSS_BIT	4			//MFRC_NSSのﾋﾞｯﾄ
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
#include "../apl_RFID.c"			//RFID操作ｱﾌﾟﾘｹｰｼｮﾝ
#endif
