//==============================================
//ﾎﾟｰﾄｱｸｾｽのﾏｸﾛ定義
//==============================================
//GPIO_CRを配列化
typedef struct
{
  __IO uint32_t CR[2];
  __IO uint32_t IDR;
  __IO uint32_t ODR;
  __IO uint32_t BSRR;
  __IO uint32_t BRR;
  __IO uint32_t LCKR;
} GPIO_sh_TypeDef;

#define	GPIO_OUT(gpio,bit)	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]=\
	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]&~(15<<(bit&7)*4)|3<<(bit&7)*4
#define	GPIO_OUTOD(gpio,bit)	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]=\
	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]&~(15<<(bit&7)*4)|7<<(bit&7)*4
#define	GPIO_H(gpio,bit)	gpio->BSRR=1<<bit
#define	GPIO_L(gpio,bit)	gpio->BRR=1<<bit
#define	GPIO_R(gpio,bit)	(gpio->IDR&1<<bit)
#define	GPIO_IN(gpio,bit)	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]=\
	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]&~(15<<(bit&7)*4)|4<<(bit&7)*4
#define	GPIO_INPU(gpio,bit)	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]=\
	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]&~(15<<(bit&7)*4)|8<<(bit&7)*4,\
	gpio->BSRR=1<<bit
#define	GPIO_INPD(gpio,bit)	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]=\
	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]&~(15<<(bit&7)*4)|8<<(bit&7)*4,\
	gpio->BRR=1<<bit
#define	GPIO_AF(gpio,bit)	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]=\
	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]&~(15<<(bit&7)*4)|11<<(bit&7)*4
#define	GPIO_AFOD(gpio,bit)	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]=\
	((GPIO_sh_TypeDef *)gpio)->CR[bit>>3]|15<<(bit&7)*4


//==============================================
//ｼｽﾃﾑｸﾛｯｸ設定
//==============================================
void SystemInit (void)
{    

	FLASH->ACR|=FLASH_ACR_PRFTBE|FLASH_ACR_LATENCY_1;
						//ﾌﾟﾘﾌｪｯﾁ有効､ﾌﾗｯｼｭﾚｲﾃﾝｼは1wait
	RCC->CFGR=
		RCC_CFGR_PPRE1_DIV2|		//PCLK1はHCLK/2(=32MHz)
		RCC_CFGR_PPRE2_DIV1|		//PCLK2はHCLK/1(=64MHz)
		RCC_CFGR_ADCPRE_DIV6|		//ADCCLKはPCLK2/6(=10.7MHz)
		RCC_CFGR_PLLMULL16;		//PLL逓倍x16(=8/2*16=64MHz)
						//PLLｿｰｽはHSI/2(規定値)
	RCC->CR|=RCC_CR_PLLON;			//PLL有効
	while((RCC->CR&RCC_CR_PLLRDY)==0);	//PLLﾚﾃﾞｨを待つ
	RCC->CFGR|=RCC_CFGR_SW_PLL;		//SYSCLKをPLLに切替える
	while((RCC->CFGR&RCC_CFGR_SWS)!=RCC_CFGR_SWS_PLL);
						//切替え完了を待つ
}



//==============================================
//共通ｺｰﾄﾞの呼込み
//==============================================
#include "common.c"				//共通ｺｰﾄﾞを呼込む

#if VOICE_I>0				//音声Iを使うとき
#include "dev_I2C_F103.c"			//I2Cを呼込む
#endif

#if VOICE_C+VOICE_S>0			//音声Cか音声Sを使うとき
#include "dev_SPI.c"				//SPIを呼込む
#endif

#include "orgel.c"				//orgelｴﾝｼﾞﾝを呼込む

#include "dev_PWM_F103.c"			//PWMを呼込む


//==============================================
//固有の初期設定
//==============================================
static void CB_INIT(void)
{

	//低電力設定
	RCC->APB1ENR|=RCC_APB1ENR_PWREN;	//電源ｲﾝﾀﾌｪｰｽにｸﾛｯｸ供給
	SCB->SCR|=SCB_SCR_SLEEPDEEP_Msk;	//SLEEPDEEP設定
	//PWR_CRのPDDSは0(STOPﾓｰﾄﾞ)初期値
	PWR->CR|=PWR_CR_LPDS;			//電圧ﾚｷﾞｭﾚｰﾀを低電力ﾓｰﾄﾞ
	RCC->APB1ENR&=~RCC_APB1ENR_PWREN;	//電源ｲﾝﾀﾌｪｰｽのｸﾛｯｸ停止

	//GPIO設定
 	RCC->APB2ENR|=
		RCC_APB2ENR_IOPAEN|		//GPIOAにｸﾛｯｸ供給
 		RCC_APB2ENR_IOPBEN|		//GPIOBにｸﾛｯｸ供給
		RCC_APB2ENR_IOPCEN|		//GPIOCにｸﾛｯｸ供給
		RCC_APB2ENR_IOPDEN|		//GPIODにｸﾛｯｸ供給
		RCC_APB2ENR_AFIOEN;		//AFIOにｸﾛｯｸ供給

	//PA0ﾁｪｯｸ(BOOT0ﾋﾟﾝはﾌﾟﾛｸﾞﾗﾑから確認できないので､代わりにPA0をﾁｪｯｸする)
	GPIO_INPD(GPIOA,0);			//PA0をﾌﾟﾙﾀﾞｳﾝ入力ﾓｰﾄﾞ
	wait_ms(1);				//電圧安定時間
	if(GPIO_R(GPIOA,0))			//PA0がHだったら
	{
		wait_ms(2000);			//2秒待つ(この間にﾃﾞﾊﾞｶﾞを繋ぐ)
	}

	//GPIO設定の続き
	AFIO->MAPR=0x04008000;			//JTAG_DP無効､SW_DP無効､PD0とPD1有効
	GPIOA->CRL=GPIOA->CRH=GPIOB->CRL=GPIOB->CRH=
	GPIOC->CRL=GPIOC->CRH=GPIOD->CRL=GPIOD->CRH=0x00000000;
						//全ﾎﾟｰﾄをｱﾅﾛｸﾞﾓｰﾄﾞにしておく

	//ﾃﾞﾊﾞｸﾞ信号設定
#ifdef SHDBS				//ﾃﾞﾊﾞｸﾞ信号を出力するとき
	dbs_init();				//ﾃﾞﾊﾞｸﾞ情報を初期化する
//待ち時間のﾃｽﾄ
//for(;;){wait_1us();DBS_H;wait_1us();DBS_L;}
//for(;;){wait_us(10);DBS_H;wait_us(10);DBS_L;}
#endif

	//ﾃﾞﾊﾞｸﾞ情報設定
#if defined(SHDBG)||defined(SHDBG_IN)	//ﾃﾞﾊﾞｸﾞ情報出力か入力するとき
	dbg_init();				//ﾃﾞﾊﾞｸﾞ情報を初期化する
#ifdef SHDBG				//ﾃﾞﾊﾞｸﾞ情報を出力するとき
//ﾃﾞﾊﾞｸﾞ情報出力のﾃｽﾄ
//for(;;){DBG_C('U');}
#endif
#endif

//DBG_C('\n');
//DBG_C('i');

	//I2Cﾎﾟｰﾄ設定
#if VOICE_I>0
	i2c_init();				//I2C初期設定
#endif

	//SPIﾎﾟｰﾄ設定
#if VOICE_C+VOICE_S>0
	spi_init();				//SPI初期設定
#endif

	//PWM設定
	pwm_init();				//PWM初期設定

	//ADCの実装要否
#if defined(CdS_EN)||defined(CVD_EN)	//CdSかCVDを実装するとき
#define	ADC_EN					//ADCの実装が必要
#endif
#if defined(KYOKU_RAND)||defined(ONSEIN_RAND)||defined(ONSEII_RAND)||defined(ONSEIC_RAND)||defined(ONSEIS_RAND)
					//どれかでﾗﾝﾀﾞﾑが必要なとき
#define	ADC_EN					//ADCを実装が必要
#endif

#ifdef ADC_EN				//ADCの実装が必要なとき
	//ADC設定(ADC1)
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;	//ADC1にｸﾛｯｸ供給
DBG_C('r');
//	wait_1us();				//ｷｬﾘﾌﾞﾚｰﾄ前の待ち時間
//	ADC1->CR2|=ADC_CR2_CAL;			//ｷｬﾘﾌﾞﾚｰﾄ開始
//	while(ADC1->CR2&ADC_CR2_CAL);		//ｷｬﾘﾌﾞﾚｰﾄ終了を待つ
//DBG_C('a');
#endif

	//ｱﾌﾟﾘの初期設定(LED設定はｱﾌﾟﾘの初期化で実施)
//DBG_C('p');
	apl_init();

//DBG_C('!');
}
