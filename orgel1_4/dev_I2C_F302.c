#ifdef I2C_SOFT				//I2Cをｿﾌﾄ実装するとき
#include "dev_I2C_SOFT.c"			//ｿﾌﾄ実装ｺｰﾄﾞを呼込む

#else					//内蔵I2Cﾓｼﾞｭｰﾙを使うとき
//==============================================
//I2Cﾌﾗｯｼｭ開始
//==============================================
static void i2c_begin(
	unsigned char s_ad,
	unsigned short r_ad)
{

		//ｱﾄﾞﾚｽ設定
//DBG_C('a');
		while(I2Cx->ISR&I2C_ISR_BUSY);	//BUSY解除待ち
		I2Cx->CR2=0;			//ｸﾘｱしておく
		I2Cx->CR2|=s_ad;		//ｽﾚｰﾌﾞｱﾄﾞﾚｽ
		I2Cx->CR2|=(2<<16);		//NBYTES=2
		I2Cx->CR2&=~I2C_CR2_RD_WRN;	//書込み
		I2Cx->CR2|=I2C_CR2_AUTOEND;	//自動STOP
		I2Cx->CR2|=I2C_CR2_START;	//START
//DBG_C('b');
		while(!(I2Cx->ISR&I2C_ISR_TXIS));
						//送信可を待つ
		I2Cx->TXDR=r_ad>>8;		//ｱﾄﾞﾚｽ上位
//DBG_C('c');
		while(!(I2Cx->ISR&I2C_ISR_TXIS));
						//送信可を待つ
		I2Cx->TXDR=r_ad;		//ｱﾄﾞﾚｽ下位
//DBG_C('d');
		while(!(I2Cx->ISR&I2C_ISR_STOPF));
						//STOPを待つ
		I2Cx->ICR|=I2C_ICR_STOPCF;	//STOP検知をｸﾘｱしておく

		//連続受信開始
//DBG_C('e');
		while(I2Cx->ISR&I2C_ISR_BUSY);	//BUSY解除待ち
		I2Cx->CR2=0;			//ｸﾘｱしておく
		I2Cx->CR2|=s_ad;		//ｽﾚｰﾌﾞｱﾄﾞﾚｽ
		I2Cx->CR2|=I2C_CR2_RD_WRN;	//Read
		I2Cx->CR2|=(255<<16);		//NBYTES最大
		I2Cx->CR2|=I2C_CR2_RELOAD;	//連続受信
		I2Cx->CR2|=I2C_CR2_START;	//START
//DBG_C('f');
}


//==============================================
//I2Cﾌﾗｯｼｭ読込み
//==============================================
static unsigned char i2c_read(void)
{
	unsigned char data;

//DBG_C('g');
	while(!(I2Cx->ISR&I2C_ISR_RXNE)){};	//RXNEｾｯﾄを待つ
//DBG_C('h');
	data=I2Cx->RXDR;			//ﾃﾞｰﾀを取出す
//DBG_B(data);
	if(I2Cx->ISR&I2C_ISR_TCR)		//受信ﾌﾞﾛｯｸ満了のとき
	{
//DBG_C('i');
						//次の255バイト設定(無限継続)
		I2Cx->CR2&=~I2C_CR2_NBYTES;
		I2Cx->CR2|=(255<<16);
		I2Cx->CR2|=I2C_CR2_RELOAD;
	}
	return(data);
}


//==============================================
//I2Cﾌﾗｯｼｭ読込み(最後のﾃﾞｰﾀ)
//==============================================
static unsigned char i2c_read_last(void)
{
	unsigned char data;

//DBG_C('j');
	while(!(I2Cx->ISR&I2C_ISR_RXNE)){};	//RXNEｾｯﾄを待つ
//DBG_C('k');
	data=I2Cx->RXDR;			//ﾃﾞｰﾀを取出す
//DBG_B(data);
	return(data);
}


//==============================================
//I2Cﾌﾗｯｼｭ終了
//==============================================
static void i2c_end(void)
{

//DBG_C('n');
	I2Cx->CR2|=I2C_CR2_STOP;		//STOPを予約
	while(!(I2Cx->ISR&I2C_ISR_STOPF))	//STOP検知するまでは
	{
//DBG_C('o');
		if(I2Cx->ISR&I2C_ISR_RXNE) I2Cx->RXDR;
						//受信ﾃﾞｰﾀを刈り取る
	}
//DBG_C('p');
	I2Cx->ICR|=I2C_ICR_STOPCF;		//STOP検知をｸﾘｱしておく
//DBG_C('q');
}
#endif


//==============================================
//I2C初期設定
//==============================================
static void i2c_init(void)
{

	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH
	GPIO_OUTOD(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをOD出力
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをH
	GPIO_OUTOD(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをOD出力
	for(;;)					//I2Cｽﾚｰﾌﾞを同期する
	{
		wait_us(1);
		if(GPIO_R(I2C_SDA_GPIO,I2C_SDA_BIT)) break;
						//SDAがHになったら抜ける
		GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをH
		wait_us(1);
		GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをL
	}
	wait_us(1);
	GPIO_L(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをL
	wait_us(1);
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH
	wait_us(1);
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをH
	wait_us(1);
#ifndef I2C_SOFT			//内蔵I2Cﾓｼﾞｭｰﾙを使うとき
//DBG_C('r');
	RCC->CFGR3|=RCC_CFGR3_I2C2SW;		//I2C2ｸﾛｯｸはPCLK1
	RCC->APB1ENR|=RCC_APB1ENR_I2C2EN;	//I2C2にｸﾛｯｸ供給
	GPIO_AF(I2C_SCL_GPIO,I2C_SCL_BIT,I2C_SCL_AF);
						//SCLを交代機能に設定
	GPIO_AF(I2C_SDA_GPIO,I2C_SDA_BIT,I2C_SDA_AF);
						//SDAを交代機能に設定
	I2Cx->TIMINGR=0x30000b16;		//400kbps@64MHz
	I2Cx->CR1=I2C_CR1_PE;			//I2C有効
#endif
//DBG_C('s');
}
