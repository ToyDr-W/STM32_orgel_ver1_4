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

	FLASH->ACR|=FLASH_ACR_PRFTEN|FLASH_ACR_LATENCY;
						//ﾌﾟﾘﾌｪｯﾁ有効､ﾌﾗｯｼｭﾚｲﾃﾝｼは1wait
	RCC->APB1ENR|=RCC_APB1ENR_PWREN;	//PWR有効
	PWR->CR=(PWR->CR&~PWR_CR_VOS)|PWR_CR_VOS_0;
						//電圧ﾚﾝｼﾞ1設定

	RCC->CR|=RCC_CR_HSION;			//HSI有効
	while(!(RCC->CR&RCC_CR_HSIRDY)){};	//HSIﾚﾃﾞｨを待つ
#if SYS_CLK==48				//ｼｽﾃﾑｸﾛｯｸを48MHzにするとき
	RCC->CFGR|=RCC_CFGR_PLLSRC_HSI|RCC_CFGR_PLLMUL6|RCC_CFGR_PLLDIV2;
						//PLL入力はHSI､6逓倍､2分周
#else					//ｼｽﾃﾑｸﾛｯｸを32MHzにするとき
	RCC->CFGR|=RCC_CFGR_PLLSRC_HSI|RCC_CFGR_PLLMUL4|RCC_CFGR_PLLDIV2;
						//PLL入力はHSI､4逓倍､2分周
#endif
	RCC->CR|=RCC_CR_PLLON;			//PLLｵﾝ
	while(!(RCC->CR&RCC_CR_PLLRDY)){};	//PLLﾚﾃﾞｨを待つ
	RCC->CFGR|=RCC_CFGR_SW_PLL;		//ｼｽﾃﾑｸﾛｯｸをPLL
	while(!(RCC->CFGR&RCC_CFGR_SWS_PLL)){}	//PLL切替えを待つ
}


//==============================================
//共通ｺｰﾄﾞの呼込み
//==============================================
#include "common.c"				//共通ｺｰﾄﾞを呼込む

#if VOICE_I>0				//音声Iを使うとき
#include "dev_I2C_F030.c"			//I2Cを呼込む(F030用を共用)
#endif

#if VOICE_C+VOICE_S>0			//音声Cか音声Sを使うとき
#include "dev_SPI.c"				//SPIを呼込む
#endif

#include "orgel.c"				//orgelｴﾝｼﾞﾝを呼込む

#include "dev_PWM_L010.c"			//PWMを呼込む


//==============================================
//固有の初期設定
//==============================================
static void CB_INIT(void)
{

	//低電力設定
	RCC->APB1ENR|=RCC_APB1ENR_PWREN;	//電源ｲﾝﾀﾌｪｰｽにｸﾛｯｸ供給
	SCB->SCR|=SCB_SCR_SLEEPDEEP_Msk;	//SLEEPDEEP設定
	//PWR_CRのPDDSは0(STOPﾓｰﾄﾞ)初期値
	PWR->CR|=PWR_CR_LPSDSR;			//STOPﾓｰﾄﾞ時電圧ﾚｷﾞｭﾚｰﾀを低電力ﾓｰﾄﾞ
	RCC->APB1ENR&=~RCC_APB1ENR_PWREN;	//電源ｲﾝﾀﾌｪｰｽのｸﾛｯｸ停止

	//GPIO設定
 	RCC->IOPENR|=
		RCC_IOPENR_IOPAEN|		//GPIOA､GPIOB､GPIOCにｸﾛｯｸ供給
		RCC_IOPENR_IOPBEN|
		RCC_IOPENR_IOPCEN;

	//PA0ﾁｪｯｸ(PB9-BOOT0はﾌﾟﾙｱｯﾌﾟ/ﾀﾞｳﾝ機能が無いので､代わりにPA0をﾁｪｯｸする)
	GPIO_INPD(GPIOA,0);			//PA0をﾌﾟﾙﾀﾞｳﾝ入力ﾓｰﾄﾞ
	wait_ms(1);				//電圧安定時間
	if(GPIO_R(GPIOA,0))			//PA0がHだったら
	{
		wait_ms(2000);			//2秒待つ(この間にﾃﾞﾊﾞｶﾞを繋ぐ)
	}

	//GPIO設定の続き
	GPIOA->MODER=GPIOB->MODER=GPIOC->MODER=0xffffffff;
						//全ﾎﾟｰﾄをｱﾅﾛｸﾞﾓｰﾄﾞにしておく
	GPIOA->PUPDR=GPIOB->PUPDR=GPIOC->PUPDR=0x00000000;
						//全ﾎﾟｰﾄのﾌﾟﾙｱｯﾌﾟ/ﾀﾞｳﾝを無効にしておく
	GPIOA->OSPEEDR=GPIOB->OSPEEDR=GPIOC->OSPEEDR=0xaaaaaaaa;
						//GPIOA､GPIOB､GPIOC出力高速

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
#define	ADC_EN					//ADCを実装が必要
#endif
#if defined(KYOKU_RAND)||defined(ONSEIN_RAND)||defined(ONSEII_RAND)||defined(ONSEIC_RAND)||defined(ONSEIS_RAND)
					//どれかでﾗﾝﾀﾞﾑが必要なとき
#define	ADC_EN					//ADCを実装が必要
#endif

#ifdef ADC_EN				//ADCの実装が必要なとき
	//ADC設定(ADC1)
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;	//ADC1にｸﾛｯｸ供給
//DBG_C('a');
	ADC1->CFGR2|=ADC_CFGR2_CKMODE_1;	//ADCｸﾛｯｸはPCLK/4
//DBG_C('r');
	ADC1->CR|=ADC_CR_ADCAL;			//ｷｬﾘﾌﾞﾚｰﾄ開始
	while(ADC1->CR&ADC_CR_ADCAL){};		//ｷｬﾘﾌﾞﾚｰﾄ終了を待つ
//DBG_C('b');
#endif

	//ｱﾌﾟﾘの初期設定(LED設定はｱﾌﾟﾘの初期化で実施)
//DBG_C('p');
	apl_init();

//DBG_C('!');
}
