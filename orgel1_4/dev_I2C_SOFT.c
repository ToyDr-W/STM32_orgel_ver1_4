//==============================================
//I2Cｽﾀｰﾄｺﾝﾃﾞｨｼｮﾝ
//==============================================
static void i2c_start(void)
{

//DBG_C('s');
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH(High-Z)
	wait_1us();
	GPIO_L(I2C_SDA_GPIO,I2C_SDA_BIT);	//SCLがHのままSDAをL
	wait_1us();
	GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをL
	wait_1us();
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをH(High-Z)
	wait_1us();
}


//==============================================
//I2Cｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝ
//==============================================
static void i2c_stop(void)
{

//DBG_C('p');
	GPIO_L(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをL
	wait_1us();
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH(High-Z)
	wait_1us();
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SCLがHのままSDAをH(High-Z)
	wait_1us();
}


//==============================================
//I2C送信
//==============================================
static void i2c_send(
	unsigned char data)			//送信ﾃﾞｰﾀ
{
	unsigned char n=8;

//DBG_C('d');
	while(n--)				//8ﾋﾞｯﾄ分繰返す
	{
		if(data&0x80) GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);
						//送信ﾋﾞｯﾄを設定する
		else GPIO_L(I2C_SDA_GPIO,I2C_SDA_BIT);
		data<<=1;			//ﾋﾞｯﾄを進める
		wait_1us();
		GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをH(High-Z)
		wait_1us();
		GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをL
		wait_1us();
	}
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをHigh-Z(ACK受信)
	wait_1us();
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH(High-Z)
	wait_1us();
	GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをL
	wait_1us();
}


//==============================================
//I2Cｱﾄﾞﾚｽ送信
//==============================================
#define	i2c_addr(addr)	i2c_send(addr)		//i2c_addr()はi2c_send()と同じ


//==============================================
//I2C受信+ACK応答
//==============================================
static unsigned char i2c_recv_ack(void)		//受信ﾃﾞｰﾀを返す
{
	unsigned char n=8,data=0;

//DBG_C('k');
	while(n--)				//8ﾋﾞｯﾄ分繰返す
	{
		GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをH(High-Z)
		wait_1us();
		data<<=1;			//ﾋﾞｯﾄを進める
		if(GPIO_R(I2C_SDA_GPIO,I2C_SDA_BIT)) data++;
						//受信ﾋﾞｯﾄを取込む
		GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをL
		wait_1us();
	}
	GPIO_L(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをL(ACK)
	wait_1us();
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH(High-Z)
	wait_1us();
	GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをL
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをH(High-Z)
	wait_1us();
	return(data);				//受信ﾃﾞｰﾀを返す
}


//==============================================
//I2C受信+NOACK応答
//==============================================
static unsigned char i2c_recv_noack(void)	//受信ﾃﾞｰﾀを返す
{
	unsigned char n=8,data=0;

//DBG_C('n');
	while(n--)				//8ﾋﾞｯﾄ分繰返す
	{
		GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをH(High-Z)
		wait_1us();
		data<<=1;			//ﾋﾞｯﾄを進める
		if(GPIO_R(I2C_SDA_GPIO,I2C_SDA_BIT)) data++;
						//受信ﾋﾞｯﾄを取込む
		GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをL
		wait_1us();
	}
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをH(High-Z)(NOACK)
	wait_1us();
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH(High-Z)
	wait_1us();
	GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをL
	wait_1us();
	return(data);				//受信ﾃﾞｰﾀを返す
}


//==============================================
//I2Cﾌﾗｯｼｭ開始
//==============================================
static void i2c_begin(
	unsigned char s_ad,
	unsigned short r_ad)
{

//DBG_C('b');
	i2c_start();				//ｽﾀｰﾄｺﾝﾃﾞｨｼｮﾝ
	i2c_addr(s_ad);				//ｽﾚｰﾌﾞｱﾄﾞﾚｽ+書込み指定送信
	i2c_send(r_ad>>8);			//ｱﾄﾞﾚｽ上位送信
	i2c_send(r_ad);				//ｱﾄﾞﾚｽ下位送信
	i2c_start();				//ｽﾀｰﾄｺﾝﾃﾞｨｼｮﾝ
	i2c_addr(s_ad|1);			//ｽﾚｰﾌﾞｱﾄﾞﾚｽ+読込み指定送信
}


//==============================================
//I2Cﾌﾗｯｼｭ読込み
//==============================================
#define	i2c_read()	i2c_recv_ack()


//==============================================
//I2Cﾌﾗｯｼｭ読込み(最後のﾃﾞｰﾀ)
//==============================================
#define	i2c_read_last()	i2c_recv_noack()


//==============================================
//I2Cﾌﾗｯｼｭ終了
//==============================================
#define	i2c_end()	i2c_stop()
