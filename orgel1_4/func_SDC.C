//==============================================
//SDからｺﾏﾝﾄﾞﾚｽﾎﾟﾝｽを受信する
//==============================================
static unsigned char mmsd_resp(void)		//ﾚｽﾎﾟﾝｽを返す(0=OK､0x11=ﾀｲﾑｱｳﾄ)
{
	unsigned char n=255,res;

//DBG_C('!');
	while(n--)
	{
		res=spi_trans(0xff);
//DBG_B(res);
		if((res&0x80)==0) break;	//b7が0のときは抜ける
	}
	return(res);				//ﾚｽﾎﾟﾝｽを返す
}


//==============================================
//SDｱｸｾｽを初期化する
//==============================================
static void mmsd_init(void)			//ｴﾗｰのときはこの中でﾘﾄﾗｲする
{
	unsigned char res;

	//ﾘﾄﾗｲ用ｴﾝﾄﾘ
mmsd_init_retry:

	//SDを非選択にする
//DBG_C('\n');
//DBG_C('u');
	GPIO_H(SPI_SS_GPIO,SPI_SS_BIT);		//SSをﾈｹﾞｰﾄする
	SPI_SS_WAIT();				//SSﾈｹﾞｰﾄ時間を待つ
	wait_ms(10);				//安定化のおまじない(5ms以上)

	//ﾀﾞﾐｰｸﾛｯｸを送信する
//DBG_C('d');
#ifndef SHSPI2				//2線SPIでないときは
	spi_trans(0xff);			//おまじない
#endif

	//SDを選択する
//DBG_C('s');
	GPIO_L(SPI_SS_GPIO,SPI_SS_BIT);		//ﾁｯﾌﾟｾﾚｸﾄ

//DBG_C('r');
	spi_trans(CMD0);			//CMD0を送信する
	spi_trans(0);
	spi_trans(0);
	spi_trans(0);
	spi_trans(0);
	spi_trans(CMD0CRC);
	res=mmsd_resp();			//ﾚｽﾎﾟﾝｽを得る
//DBG_B(res);
	if(res!=0x01) goto mmsd_init_retry;	//ｱｲﾄﾞﾙでなかったらﾘﾄﾗｲする

	//初期化の完了を待つ
//DBG_C('i');
	do
	{
		spi_trans(0xff);		//ﾀﾞﾐｰ転送しないとCMD1が0x05になる
		spi_trans(CMD1);		//CMD1を送信する
		spi_trans(0);
		spi_trans(0);
		spi_trans(0);
		spi_trans(0);
		spi_trans(1);
		res=mmsd_resp();		//ﾚｽﾎﾟﾝｽを得る
//DBG_B(res);
		if(res&0x80) goto mmsd_init_retry;
						//ﾚｽﾎﾟﾝｽのMSBが1のときはﾘﾄﾗｲする
	}
	while(res==0x01);			//ｱｲﾄﾞﾙのときは繰返す
	if(res) goto mmsd_init_retry;		//ﾚｽﾎﾟﾝｽが0でなかったらﾘﾄﾗｲする

	//ﾌﾞﾛｯｸ長(512[ﾊﾞｲﾄ]設定)を送信する
//DBG_C('l');
	
	spi_trans(0xff);			//ﾀﾞﾐｰ転送しないとCMD16が0x01になる
	spi_trans(CMD16);			//CMD16を送信する
	spi_trans(0);
	spi_trans(0);
	spi_trans(2);
	spi_trans(0);
	spi_trans(1);
	res=mmsd_resp();			//ﾚｽﾎﾟﾝｽを得る
//DBG_B(res);
	if(res) goto mmsd_init_retry;		//ﾚｽﾎﾟﾝｽが0でなかったらﾘﾄﾗｲする
}


//==============================================
//SD読込み開始
//==============================================
static void mmsd_read(void)
{

	//ﾚﾃﾞｨﾁｪｯｸ
//DBG_C('\n');
//DBG_C('r');
//DBG_L(voiceC.begin);
	do
	{
		while(spi_trans(0xff)!=0xff){};	//0xffが返るまでﾀﾞﾐｰﾃﾞｰﾀを送る

	//ｼﾝｸﾞﾙﾌﾞﾛｯｸﾘｰﾄﾞ設定
//DBG_C('b');
		spi_trans(CMD17);		//CMD17を送信する
		spi_trans(voiceC.begin>>16);	//SDｶｰﾄﾞのｱﾄﾞﾚｽ(MSB)
		spi_trans(voiceC.begin>>8);	//SDｶｰﾄﾞのｱﾄﾞﾚｽ
		spi_trans(voiceC.begin);	//SDｶｰﾄﾞのｱﾄﾞﾚｽ
		spi_trans(0);			//SDｶｰﾄﾞのｱﾄﾞﾚｽ(LSB)
		spi_trans(1);			//CRC
	}
	while(mmsd_resp());			//ﾚｽﾎﾟﾝｽが0になるまで繰返す

	//ﾃﾞｰﾀﾄｰｸﾝ開始待ち
//DBG_C('t');
	while(spi_trans(0xff)!=0xfe){};		//ﾃﾞｰﾀ受信開始まで繰返す
//DBG_C('o');
}
