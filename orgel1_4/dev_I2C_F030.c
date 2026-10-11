//STM32G03x､STM32L010のI2CはSTM32F030と近しいので､ｿｰｽｺｰﾄﾞを共用し､
//相違点は #if で書き分けている｡
//STM32F103とSTM32F302は相違点が多いので､それぞれ別のｿｰｽｺｰﾄﾞにしている｡


#ifdef I2C_SOFT				//I2Cをｿﾌﾄ実装するとき
#include "dev_I2C_SOFT.c"			//ｿﾌﾄ実装のｺｰﾄﾞを呼込む

#else					//内蔵I2Cﾓｼﾞｭｰﾙを使うとき
//==============================================
//I2Cﾌﾗｯｼｭ開始
//==============================================
static void i2c_begin(
	unsigned char s_ad,
	unsigned short r_ad)
{

DBG_C('<');
DBG_C('S');
	I2Cx->CR2=(2<<16)|s_ad;			//ﾘﾛｰﾄﾞしない､2ﾊﾞｲﾄ､ｽﾚｰﾌﾞｱﾄﾞﾚｽ
	I2Cx->CR2|=I2C_CR2_START;		//START
DBG_C('a');
	while(!(I2Cx->ISR&I2C_ISR_TXIS)){};	//TXISｾｯﾄを待つ
DBG_C('b');
	I2Cx->TXDR=r_ad>>8;			//ｱﾄﾞﾚｽ上位
DBG_C('c');
	while(!(I2Cx->ISR&I2C_ISR_TXIS)){};	//TXISｾｯﾄを待つ
DBG_C('d');
	I2Cx->TXDR=r_ad;			//ｱﾄﾞﾚｽ下位
DBG_C('e');
	while(!(I2Cx->ISR&I2C_ISR_TC)){};	//TCｾｯﾄを待つ
DBG_C('f');
	I2Cx->CR2=I2C_CR2_RELOAD|(1<<16)|I2C_CR2_RD_WRN|s_ad;
						//ﾘﾛｰﾄﾞする､1ﾊﾞｲﾄ､ﾏｽﾀﾘｰﾄﾞ､ｽﾚｰﾌﾞｱﾄﾞﾚｽ
	I2Cx->CR2|=I2C_CR2_START;		//START
DBG_C('>');
}


//==============================================
//I2Cﾌﾗｯｼｭ読込み
//==============================================
static unsigned char i2c_read(void)
{
	unsigned char data;

DBG_C('<');
DBG_C('R');
	while(!(I2Cx->ISR&I2C_ISR_RXNE)){};	//RXNEｾｯﾄを待つ
DBG_C('a');
	data=I2Cx->RXDR;			//受信ﾃﾞｰﾀを取込む
DBG_B(data);
	I2Cx->CR2=I2C_CR2_RELOAD|(1<<16)|I2C_CR2_RD_WRN;
						//ﾘﾛｰﾄﾞする､1ﾊﾞｲﾄ､ﾏｽﾀﾘｰﾄﾞ
DBG_C('>');
	return(data);
}


//==============================================
//I2Cﾌﾗｯｼｭ読込み(最後のﾃﾞｰﾀ)
//==============================================
static unsigned char i2c_read_last(void)
{
	unsigned char data;

DBG_C('<');
DBG_C('L');
	while(!(I2Cx->ISR&I2C_ISR_RXNE)){};	//RXNEｾｯﾄを待つ
DBG_C('a');
	data=I2Cx->RXDR;			//受信ﾃﾞｰﾀを取込む
DBG_B(data);
	I2Cx->CR2=(1<<16)|I2C_CR2_RD_WRN;	//ﾘﾛｰﾄﾞしない､1ﾊﾞｲﾄ､ﾏｽﾀﾘｰﾄﾞ
DBG_C('>');
	return(data);
}


//==============================================
//I2Cﾌﾗｯｼｭ終了
//==============================================
static void i2c_end(void)
{

DBG_C('<');
DBG_C('P');
	while(!(I2Cx->ISR&I2C_ISR_RXNE)){};	//RXNEｾｯﾄを待つ
DBG_C('a');
	I2Cx->RXDR;				//ﾃﾞｰﾀを空読み
	I2Cx->CR2|=I2C_CR2_STOP;		//STOPする
DBG_C('>');
}
#endif


//ここからはｿﾌﾄ実装と内臓ﾓｼﾞｭｰﾙ使用で共通の処理
//==============================================
//I2初期設定
//==============================================
static void i2c_init(void)
{

DBG_C('<');
DBG_C('I');
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH
	GPIO_OUTOD(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをOD出力
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをH
	GPIO_OUTOD(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをOD出力
	for(;;)					//I2Cｽﾚｰﾌﾞを同期する
	{
DBG_C('j');
		wait_us(1);
		if(GPIO_R(I2C_SDA_GPIO,I2C_SDA_BIT)) break;
						//SDAがHになったら抜ける
		GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをH
		wait_us(1);
		GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをL
	}
DBG_C('k');
	wait_us(1);
	GPIO_L(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをL
	wait_us(1);
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH
	wait_us(1);
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをH
	wait_us(1);

#ifndef I2C_SOFT			//内蔵I2Cﾓｼﾞｭｰﾙを使うとき
#ifdef DEV_F030				//F030のとき
DBG_C('a');
	RCC->APB1ENR|=RCC_APB1ENR_I2C1EN;	//I2C1にｸﾛｯｸ供給
	I2Cx->TIMINGR=0x00100209;		//400kbps@8MHz
#endif
#ifdef DEV_G03x				//G03xのとき
#ifdef DEV_I2C1				//I2C1を使うとき
DBG_C('b');
	RCC->CCIPR|=RCC_CCIPR_I2C1SEL_1;	//I2C1ｸﾛｯｸはHSI16
	RCC->APBENR1|=RCC_APBENR1_I2C1EN;	//I2C1にｸﾛｯｸ供給
	I2Cx->TIMINGR=0x00300716;		//400kbps@16MHz
#endif
#ifdef DEV_I2C2				//I2C2を使うとき
DBG_C('c');
						//I2C2ｸﾛｯｸはPCLKに固定されている
	RCC->APBENR1|=RCC_APBENR1_I2C2EN;	//I2C2にｸﾛｯｸ供給
	I2Cx->TIMINGR=0x20300b15;		//400kbps@48MHz
#endif
DBG_C('d');
#endif
#ifdef DEV_L010				//L010のとき
DBG_C('e');
	RCC->CCIPR|=RCC_CCIPR_I2C1SEL_1;	//I2C1ｸﾛｯｸはHSI16
	RCC->APB1ENR|=RCC_APB1ENR_I2C1EN;	//I2C1にｸﾛｯｸ供給
	I2Cx->TIMINGR=0x00000914;		//400kbps@16MHz
#endif
DBG_C('f');
	I2Cx->CR1=I2C_CR1_PE;			//I2C有効
	GPIO_AF(I2C_SCL_GPIO,I2C_SCL_BIT,I2C_SCL_AF);
						//SCLを交代機能に設定
	GPIO_AF(I2C_SDA_GPIO,I2C_SDA_BIT,I2C_SDA_AF);
						//SDAを交代機能に設定
DBG_C('>');
#endif
}
