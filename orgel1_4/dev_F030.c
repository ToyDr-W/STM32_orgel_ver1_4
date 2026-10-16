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
//ｼｽﾃﾑｸﾛｯｸ設定
//==============================================
void SystemInit (void)
{    

	FLASH->ACR|=FLASH_ACR_PRFTBE|FLASH_ACR_LATENCY;
						//ﾌﾟﾘﾌｪｯﾁ有効､ﾌﾗｯｼｭﾚｲﾃﾝｼは1wait
	RCC->CFGR|=(uint32_t)RCC_CFGR_HPRE_DIV1;//HCLKはSYSCLK分周無し
	RCC->CFGR|=(uint32_t)RCC_CFGR_PPRE_DIV1;//PCLKはHCLK分周無し
#if SYS_CLK==64				//ｼｽﾃﾑｸﾛｯｸを64MHzにするとき
	RCC->CFGR|=(uint32_t)RCC_CFGR_PLLMUL16;	//PLL入力はHSI/2(規定値)､16逓倍
#else					//ｼｽﾃﾑｸﾛｯｸを48MHzにするとき
	RCC->CFGR|=(uint32_t)RCC_CFGR_PLLMUL12;	//PLL入力はHSI/2(規定値)､12逓倍
#endif
	RCC->CR|=RCC_CR_PLLON;			//PLL有効
	while((RCC->CR&RCC_CR_PLLRDY)==0);	//PLLﾚﾃﾞｨを待つ
	RCC->CFGR|=(uint32_t)RCC_CFGR_SW_PLL;	//SYSCLKをPLLに切替える
	while((RCC->CFGR&(uint32_t)RCC_CFGR_SWS)!=(uint32_t)RCC_CFGR_SWS_PLL);
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
#ifdef DEV_F030F4
#include "dev_PWM_F030F4.c"			//F030F4用PWMを呼込む
#endif
#ifdef DEV_F030Kx
#include "dev_PWM_F030Kx.c"			//F030Kx用PWMを呼込む
#endif
#ifdef DEV_F030Cx
#include "dev_PWM_F030Cx.c"			//F030Cx用PWMを呼込む
#endif
#endif


//==============================================
//固有の初期設定
//==============================================
static void CB_INIT(void)
{

	//ﾀﾞｳﾝﾛｰﾄﾞ猶予時間
	wait_ms(1000);				//絶対に取るな!

	//低電力設定
	RCC->APB1ENR|=RCC_APB1ENR_PWREN;	//電源ｲﾝﾀﾌｪｰｽにｸﾛｯｸ供給
	SCB->SCR|=SCB_SCR_SLEEPDEEP_Msk;	//SLEEPDEEP設定
	//PWR_CRのPDDSは0(STOPﾓｰﾄﾞ)初期値
	PWR->CR|=PWR_CR_LPDS;			//電圧ﾚｷﾞｭﾚｰﾀを低電力ﾓｰﾄﾞ
	RCC->APB1ENR&=~RCC_APB1ENR_PWREN;	//電源ｲﾝﾀﾌｪｰｽのｸﾛｯｸ停止

	//GPIO設定
 	RCC->AHBENR|=
		RCC_AHBENR_GPIOAEN|		//GPIOA､GPIOB､GPIOC､GPIOFにｸﾛｯｸ供給
		RCC_AHBENR_GPIOBEN|
		RCC_AHBENR_GPIOCEN|
		RCC_AHBENR_GPIOFEN;

	//PA0ﾁｪｯｸ(BOOT0ﾋﾟﾝはﾌﾟﾛｸﾞﾗﾑから確認できないので､代わりにPA0をﾁｪｯｸする)
	GPIO_INPD(GPIOA,0);			//PA0をﾌﾟﾙﾀﾞｳﾝ入力ﾓｰﾄﾞ
	wait_ms(1);				//電圧安定時間
	if(GPIO_R(GPIOA,0))			//PA0がHだったら
	{
		wait_ms(2000);			//2秒待つ(この間にﾃﾞﾊﾞｶﾞを繋ぐ)
	}

	//GPIO設定の続き
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
	RCC->APB2ENR|=RCC_APB2ENR_ADCEN;	//ADCにｸﾛｯｸ供給
	RCC->CR2|=RCC_CR2_HSI14ON;		//HSI14RCｵｼﾚｰﾀを起動
	while(!(RCC->CR2&RCC_CR2_HSI14RDY)){};	//HSI14RCｵｼﾚｰﾀのﾚﾃﾞｨを待つ
	//ADC1->CFGR2のCKMODEは000(HSI14RC)初期値
//DBG_C('r');
	ADC1->CR|=ADC_CR_ADCAL;			//ｷｬﾘﾌﾞﾚｰﾄ開始
	while(ADC1->CR&ADC_CR_ADCAL);		//ｷｬﾘﾌﾞﾚｰﾄ終了を待つ
//DBG_C('a');
#endif
#endif

	//ｱﾌﾟﾘｹｰｼｮﾝの初期設定
	apl_init();

//DBG_C('!');
}
