#define	I2Cx	I2C1


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

//DBG_C('g');
		I2Cx->CR1|=I2C_CR1_START;	//START
//DBG_C('h');
		while(!(I2Cx->SR1&I2C_SR1_SB)){};
						//STARTを待つ
//DBG_C('i');
		I2Cx->DR=s_ad;			//ｽﾚｰﾌﾞｱﾄﾞﾚｽ(W)
//DBG_C('j');
		while(!(I2Cx->SR1&I2C_SR1_ADDR)){};
						//ｱﾄﾞﾚｽ送信を待つ
//DBG_C('k');
		I2Cx->SR2;			//SR2を空読み
//DBG_C('l');
		I2Cx->DR=r_ad>>8;		//ｱﾄﾞﾚｽ上位
//DBG_C('m');
		while(!(I2Cx->SR1&I2C_SR1_TXE)){};
						//ﾃﾞｰﾀﾚｼﾞｽﾀ空きを待つ
//DBG_C('n');
		I2Cx->DR=r_ad;			//ｱﾄﾞﾚｽ下位
//DBG_C('o');
		while(!(I2Cx->SR1&I2C_SR1_TXE)){};
						//ﾃﾞｰﾀﾚｼﾞｽﾀ空きを待つ
//DBG_C('p');
		I2Cx->CR1|=I2C_CR1_ACK;		//自動ACKする

		I2Cx->CR1|=I2C_CR1_START;	//START
//DBG_C('q');
		while(!(I2Cx->SR1&I2C_SR1_SB)){};
						//STARTを待つ
//DBG_C('r');
		I2Cx->DR=s_ad|1;		//ｽﾚｰﾌﾞｱﾄﾞﾚｽ(R)
//DBG_C('s');
		while(!(I2Cx->SR1&I2C_SR1_ADDR)){};
						//ｱﾄﾞﾚｽ送信を待つ
//DBG_C('t');
		I2Cx->SR2;			//SR2を空読み
//DBG_C('u');
}


//==============================================
//I2Cﾌﾗｯｼｭ読込み
//==============================================
static unsigned char i2c_read(void)
{
	unsigned char data;

	while(!(I2Cx->SR1&I2C_SR1_RXNE)){};	//RXNEｾｯﾄを待つ
//DBG_C('b');
	data=I2Cx->DR;				//受信ﾃﾞｰﾀを取込む
//DBG_B(data);
	return(data);
}


//==============================================
//I2Cﾌﾗｯｼｭ読込み(最後のﾃﾞｰﾀ)
//==============================================
static unsigned char i2c_read_last(void)
{
	unsigned char data;

	while(!(I2Cx->SR1&I2C_SR1_RXNE)){};	//RXNEｾｯﾄを待つ
//DBG_C('b');
//DBG_C('c');
	I2Cx->CR1&=~I2C_CR1_ACK;		//自動ACKしない
	data=I2Cx->DR;				//ﾃﾞｰﾀを取出す
	I2Cx->CR1|=I2C_CR1_STOP;		//STOPを予約
	return(data);
}


//==============================================
//I2Cﾌﾗｯｼｭ終了
//==============================================
static void i2c_end(void)
{

//DBG_C('a');
	if(I2Cx->CR1&I2C_CR1_ACK)		//自動ACKが設定されていたら
	{
//DBG_C('b');
		while(!(I2Cx->SR1&I2C_SR1_RXNE)){};
						//受信を待つ
		I2Cx->CR1&=~I2C_CR1_ACK;
						//自動ACKしない
		I2Cx->DR;			//ﾃﾞｰﾀを空読み
		I2Cx->CR1|=I2C_CR1_STOP;	//STOPを予約
	}
//DBG_C('c');
	while(!(I2Cx->SR1&I2C_SR1_RXNE)){};	//受信を待つ
	I2Cx->DR;				//ﾃﾞｰﾀを空読み
//DBG_C('d');
}
#endif


//==============================================
//I2初期設定
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
	RCC->APB1ENR|=RCC_APB1ENR_I2C1EN;	//I2C1にｸﾛｯｸ供給
#ifdef I2C_REMAP			//I2Cをﾘﾏｯﾌﾟするとき
	AFIO->MAPR|=I2C_REMAP;			//I2Cをﾘﾏｯﾌﾟする
#endif
//DBS_H;
	GPIO_AFOD(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLを交代機能OD出力に設定
	GPIO_AFOD(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAを交代機能OD出力に設定
//DBS_L;
	I2Cx->CR2=32;				//PCLK1は32MHz
	I2Cx->CCR=0x801b;			//高速､DUTY0､CCR=32MHz/400kbps/3=27
	I2Cx->TRISE=9;				//SCL立上り=1000ns
	I2Cx->CR1=I2C_CR1_PE;			//I2C有効
#endif
}
