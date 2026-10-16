//==============================================
//ﾎﾟｰﾄｱｸｾｽのﾏｸﾛ定義
//==============================================
#define	GPIO_OUT(gpio,bit)	gpio->MODER=gpio->MODER&~(3<<bit*2)|1<<bit*2
#define	GPIO_OUTOD(gpio,bit)	gpio->OTYPER|=1<<bit,\
				gpio->MODER=gpio->MODER&~(3<<bit*2)|1<<bit*2
#define	GPIO_H(gpio,bit)	gpio->BSRR=1<<bit
#define	GPIO_L(gpio,bit)	gpio->BRR=1<<bit
#define	GPIO_R(gpio,bit)	(gpio->IDR&1<<bit)
#define	GPIO_IN(gpio,bit)	gpio->MODER&=~(3<<bit*2),\
				gpio->PUPDR&=~(3<<bit*2)
#define	GPIO_INPU(gpio,bit)	gpio->MODER&=~(3<<bit*2),\
				gpio->PUPDR=gpio->PUPDR&~(3<<bit*2)|1<<bit*2
#define	GPIO_INPD(gpio,bit)	gpio->MODER&=~(3<<bit*2),\
				gpio->PUPDR=gpio->PUPDR&~(3<<bit*2)|2<<bit*2
#define	GPIO_AF(gpio,bit,af)	gpio->MODER=gpio->MODER&~(3<<bit*2)|2<<bit*2,\
				gpio->AFR[bit>>3]|=af<<(bit&7)*4

//==============================================
//ISP用のSYSCLK設定
//==============================================
#ifdef ISP_SPIFLASH_EN			//ISP環境でSPIﾌﾗｯｼｭを書替えるとき
#undef SYS_CLK					//SYS_CLKを
						//　64MHzに強制する
#define	SYS_CLK	64
#endif


//==============================================
//ｼｽﾃﾑｸﾛｯｸ設定
//==============================================
void SystemInit (void)
{    

	FLASH->ACR|=FLASH_ACR_PRFTEN|FLASH_ACR_LATENCY_1;
						//ﾌﾟﾘﾌｪｯﾁ有効､ﾌﾗｯｼｭﾚｲﾃﾝｼは2wait
	RCC->CFGR|=(4<<RCC_CFGR_PPRE_Pos);	//APBﾌﾟﾘｽｹは1/2
	RCC->PLLCFGR=
		(2<<RCC_PLLCFGR_PLLSRC_Pos)|	//PLL入力はHSI16
#if SYS_CLK==96				//ｼｽﾃﾑｸﾛｯｸを96MHzにするとき
		(12<<RCC_PLLCFGR_PLLN_Pos)|	//12逓倍
#else					//ｼｽﾃﾑｸﾛｯｸを64MHzにするとき
		(8<<RCC_PLLCFGR_PLLN_Pos)|	//8逓倍
#endif
		(1<<RCC_PLLCFGR_PLLR_Pos)|	//PLLRは1/2
		RCC_PLLCFGR_PLLREN;		//PLLRを出力
	RCC->CR|=RCC_CR_PLLON;			//PLL有効
	while(!(RCC->CR&RCC_CR_PLLRDY));	//PLLﾚﾃﾞｨを待つ
	RCC->CFGR|=(2<<RCC_CFGR_SW_Pos);	//SYSCLKをPLLRに切替える
	while((RCC->CFGR&RCC_CFGR_SWS_Msk)!=(2<<RCC_CFGR_SWS_Pos));
						//切替え完了を待つ
}


//==============================================
//共通ｺｰﾄﾞの呼込み
//==============================================
#include "common.c"				//共通ｺｰﾄﾞを呼込む

#ifdef ISP_SPIFLASH_EN			//ISP環境でSPIﾌﾗｯｼｭを書替えるとき
#undef UARTBPS					//UARTのﾎﾞｰﾚｰﾄを
#define	UARTBPS	230400				//　230400bpsに強制する
#include "dev_UART.c"				//UARTを呼込む
#undef	VOICE_S					//SPI音声を
#define	VOICE_S	1				//　使用する設定にする
#include "dev_SPI.c"				//SPIを呼込む

#else					//電子ｵﾙｺﾞｰﾙを実行するとき
#if VOICE_I>0				//音声Iを使うとき
#include "dev_I2C_F030.c"			//I2Cを呼込む
#endif
#if VOICE_C+VOICE_S>0			//音声Cか音声Sを使うとき
#include "dev_SPI.c"				//SPIを呼込む
#endif
#include "orgel.c"				//orgelｴﾝｼﾞﾝを呼込む
#include "dev_PWM_G03x.c"			//PWMを呼込む
#endif


//==============================================
//固有の初期設定
//==============================================
static void CB_INIT(void)
{

	//ﾀﾞｳﾝﾛｰﾄﾞ猶予時間
	wait_ms(1000);				//絶対に取るな!

	//低電力設定
	RCC->APBENR1|=RCC_APBENR1_PWREN;	//電源ｲﾝﾀﾌｪｰｽにｸﾛｯｸ供給
	PWR->CR1|=PWR_CR1_LPMS_0;		//STOP1ﾓｰﾄﾞ
	SCB->SCR|=SCB_SCR_SLEEPDEEP_Msk;	//SLEEPDEEP設定
	RCC->APBENR1&=~RCC_APBENR1_PWREN;	//電源ｲﾝﾀﾌｪｰｽのｸﾛｯｸ停止

	//GPIO設定
 	RCC->IOPENR|=RCC_IOPENR_GPIOAEN|RCC_IOPENR_GPIOBEN|RCC_IOPENR_GPIOCEN;
						//GPIOA､GPIOB､GPIOCにｸﾛｯｸ供給

	//PA0ﾁｪｯｸ(PA14-BOOT0はSWCKLと被るので､代わりにPA0をﾁｪｯｸする)
	GPIO_INPD(GPIOA,0);			//PA0をﾌﾟﾙﾀﾞｳﾝ入力ﾓｰﾄﾞ
	wait_ms(1);				//電圧安定時間
	if(GPIO_R(GPIOA,0))			//PA0がHだったら
	{
		wait_ms(2000);			//2秒待つ(この間にﾃﾞﾊﾞｶﾞを繋ぐ)
	}

	//GPIO設定の続き
	GPIOA->OSPEEDR=GPIOB->OSPEEDR=GPIOC->OSPEEDR=GPIOF->OSPEEDR=0xaaaaaaaa;
						//全ﾎﾟｰﾄを高速
	GPIOA->MODER=GPIOB->MODER=GPIOC->MODER=GPIOF->MODER=0xffffffff;
						//全ﾎﾟｰﾄをｱﾅﾛｸﾞﾓｰﾄﾞにしておく
	GPIOA->PUPDR=GPIOB->PUPDR=GPIOC->PUPDR=GPIOF->PUPDR=0x00000000;
						//全ﾎﾟｰﾄのﾌﾟﾙｱｯﾌﾟ/ﾀﾞｳﾝを無効にしておく

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

	//PWMﾎﾟｰﾄ設定/UARTﾎﾟｰﾄ設定
#ifdef	ISP_SPIFLASH_EN			//ISP環境でSPIﾌﾗｯｼｭを書替えるとき
	uart_init();				//UART初期設定
	spi_init();				//SPI初期設定

#else					//電子ｵﾙｺﾞｰﾙを実行するとき
#if VOICE_I>0
	i2c_init();				//I2C初期設定
#endif
#if VOICE_C+VOICE_S>0
	spi_init();				//SPI初期設定
#endif
	pwm_init();				//PWM初期設定

	//ADCの実装要否
#if defined(CdS_EN)||defined(CVD_EN)	//CdSかCVDを実装するとき
#define	ADC_EN					//ADCを実装が必要
#endif
#if defined(KYOKU_RAND)||defined(ONSEIN_RAND)||defined(ONSEII_RAND)||defined(ONSEIC_RAND)||defined(ONSEIS_RAND)
					//どれかでﾗﾝﾀﾞﾑが必要なとき
#define	ADC_EN					//ADCを実装が必要
#endif

#ifdef ADC_EN				//ADCの実装が必要なとき
	//ADC設定(ADC1)
	RCC->APBENR2|=RCC_APBENR2_ADCEN;	//ADCにｸﾛｯｸ供給
	ADC1->CFGR2|=ADC_CFGR2_CKMODE_1;	//ADCｸﾛｯｸはPCLK/4
	ADC1->CR|=ADC_CR_ADVREGEN;		//ADCﾚｷﾞｭﾚｰﾀ有効
//DBG_C('r');
//	wait_1us();				//起動時間待ち
//	ADC1->CR|=ADC_CR_ADCAL;			//ｷｬﾘﾌﾞﾚｰﾄ開始
//	while(ADC1->CR&ADC_CR_ADCAL);		//ｷｬﾘﾌﾞﾚｰﾄ終了を待つ
//DBG_C('a');
#endif
#endif

	//ｱﾌﾟﾘの初期設定(LED設定はｱﾌﾟﾘの初期化で実施)
//DBG_C('p');
	apl_init();

//DBG_C('!');
}
