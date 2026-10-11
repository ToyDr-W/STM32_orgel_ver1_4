//RFIDﾓｼﾞｭｰﾙ｢MFRC522｣を制御するﾌｧｰﾑｳｪｱのｴﾝｼﾞﾝ部


//----------------------------------------------
//ｱﾌﾟﾘｹｰｼｮﾝの定数定義
//----------------------------------------------
#define	MAX_LEN		9	//転送するｺﾏﾝﾄﾞﾃﾞｰﾀの最大長


//----------------------------------------------
//MFRC522のｴﾗｰｺｰﾄﾞの定義
//----------------------------------------------
#define	MI_OK		0	//正常終了
#define	MI_NOTAG	1	//ﾀｸﾞ無し
#define	MI_ERR		2	//異常終了


//----------------------------------------------
//MFRC522のｺﾏﾝﾄﾞの定義
//----------------------------------------------
#define	RC522_IDLE	0x00	//NO action; Cancel the current command
#define	RC522_MEM	0x01	//Store 25 byte into the internal buffer.
#define	RC522_GENID	0x02	//Generates a 10 byte random ID number.
#define	RC522_CALCCRC	0x03	//CRC Calculate or selftest.
#define	RC522_TRANSMIT	0x04	//Transmit data
#define	RC522_NOCMDCH	0x07	//No command change.
#define	RC522_RECEIVE	0x08	//Receive Data
#define	RC522_TRANS	0x0C	//Transmit and receive data,
#define	RC522_AUTHENT	0x0E	//Authentication Key
#define	RC522_SOFTRESET	0x0F	//Reset


//----------------------------------------------
// Mifare_Oneﾀｸﾞのｺﾏﾝﾄﾞ
//----------------------------------------------
#define	MF1_REQIDL	0x26	//find the antenna area does not enter hibernation
#define	MF1_REQALL	0x52	//find all the tags antenna area
#define	MF1_ANTICOLL	0x93	//anti-collision
#define	MF1_SELECTTAG	0x93	//selection tag
#define	MF1_AUTHENT1A	0x60	//authentication key A
#define	MF1_AUTHENT1B	0x61	//authentication key B
#define	MF1_READ	0x30	//Read Block
#define	MF1_WRITE	0xA0	//write block
#define	MF1_DECREMENT	0xC0	//debit
#define	MF1_INCREMENT	0xC1	//recharge
#define	MF1_RESTORE	0xC2	//transfer block data to the buffer
#define	MF1_TRANSFER	0xB0	//save the data in the buffer
#define	MF1_HALT	0x50	//Sleep


//----------------------------------------------
//MFRC522のﾚｼﾞｽﾀｱﾄﾞﾚｽの定義
//----------------------------------------------
//Page 0:Command and Status
#define	Reserved00	0x00
#define	CommandReg	0x01
#define	CommIEnReg	0x02
#define	DivIEnReg	0x03
#define	CommIrqReg	0x04
#define	DivIrqReg	0x05
#define	ErrorReg	0x06
#define	Status1Reg	0x07
#define	Status2Reg	0x08
#define	FIFODataReg	0x09
#define	FIFOLevelReg	0x0A
#define	WaterLevelReg	0x0B
#define	ControlReg	0x0C
#define	BitFramingReg	0x0D
#define	CollReg		0x0E
#define	Reserved01	0x0F

//Page 1:Command
#define	Reserved10	0x10
#define	ModeReg		0x11
#define	TxModeReg	0x12
#define	RxModeReg	0x13
#define	TxControlReg	0x14
#define	TxAutoReg	0x15
#define	TxSelReg	0x16
#define	RxSelReg	0x17
#define	RxThresholdReg	0x18
#define	DemodReg	0x19
#define	Reserved11	0x1A
#define	Reserved12	0x1B
#define	MifareReg	0x1C
#define	Reserved13	0x1D
#define	Reserved14	0x1E
#define	SerialSpeedReg	0x1F

//Page 2:CFG
#define	Reserved20	0x20
#define	CRCResultRegM	0x21
#define	CRCResultRegL	0x22
#define	Reserved21	0x23
#define	ModWidthReg	0x24
#define	Reserved22	0x25
#define	RFCfgReg	0x26
#define	GsNReg		0x27
#define	CWGsPReg	0x28
#define	ModGsPReg	0x29
#define	TModeReg	0x2A
#define	TPrescalerReg	0x2B
#define	TReloadRegH	0x2C
#define	TReloadRegL	0x2D
#define	TCounterValRegH	0x2E
#define	TCounterValRegL	0x2F

//Page 3:TestRegister
#define	Reserved30	0x30
#define	TestSel1Reg	0x31
#define	TestSel2Reg	0x32
#define	TestPinEnReg	0x33
#define	TestPinValueReg	0x34
#define	TestBusReg	0x35
#define	AutoTestReg	0x36
#define	VersionReg	0x37
#define	AnalogTestReg	0x38
#define	TestDAC1Reg	0x39
#define	TestDAC2Reg	0x3A
#define	TestADCReg	0x3B
#define	Reserved31	0x3C
#define	Reserved32	0x3D
#define	Reserved33	0x3E
#define	Reserved34	0x3F


//----------------------------------------------
//MFRCﾎﾟｰﾄｱｸｾｽﾏｸﾛ
//----------------------------------------------
#define	MFRC_NSS_OUT	GPIO_OUT(MFRC_NSS_GPIO,MFRC_NSS_BIT)
#define	MFRC_NSS_H	GPIO_H(MFRC_NSS_GPIO,MFRC_NSS_BIT)
#define	MFRC_NSS_L	GPIO_L(MFRC_NSS_GPIO,MFRC_NSS_BIT)
#define	MFRC_SCK_OUT	GPIO_OUT(MFRC_SCK_GPIO,MFRC_SCK_BIT)
#define	MFRC_SCK_H	GPIO_H(MFRC_SCK_GPIO,MFRC_SCK_BIT)
#define	MFRC_SCK_L	GPIO_L(MFRC_SCK_GPIO,MFRC_SCK_BIT)
#define	MFRC_MOSI_OUT	GPIO_OUT(MFRC_MOSI_GPIO,MFRC_MOSI_BIT)
#define	MFRC_MOSI_H	GPIO_H(MFRC_MOSI_GPIO,MFRC_MOSI_BIT)
#define	MFRC_MOSI_L	GPIO_L(MFRC_MOSI_GPIO,MFRC_MOSI_BIT)
#define	MFRC_MISO_IN	GPIO_IN(MFRC_MISO_GPIO,MFRC_MISO_BIT)
#define	MFRC_MISO_R	GPIO_R(MFRC_MISO_GPIO,MFRC_MISO_BIT)
#ifdef  MFRC_SPI2			//2線SPIのとき
#define	MFRC_NSS_WAIT	wait_nss(MFRC_SS_TIME)
#else					//4線SPIか3線SPIのとき
#define	MFRC_NSS_WAIT
#endif


//----------------------------------------------
//MFRC_NSSのﾈｹﾞｰﾄ待ち
//----------------------------------------------
void wait_nss(unsigned short u)
{
	volatile unsigned short s;

	while(u--)
	{
		for(s=0;s<SYS_CLK/16;s++){};
	}
}


//----------------------------------------------
//広域ﾃﾞｰﾀの宣言
//----------------------------------------------
static unsigned char MF_TaskState;		//ﾀｽｸ処理ｽﾃｰﾄ
						//　　b7:ﾀｸﾞ情報の有効/無効(0=無効､1=有効)
						//　　b6-b0:ｽﾃｰﾄﾏｼﾝ状態
static unsigned char MF_TagType[2];		//ﾀｸﾞのﾀｲﾌﾟ
						//　　0x4400=Mifare_UltraLight
						//　　0x0400=Mifare_One(S50)
						//　　0x0200=Mifare_One(S70)
						//　　0x0800=Mifare_Pro(X)
						//　　0x4403=Mifare_DESFire
static unsigned char MF_TagSerial[5];		//ﾀｸﾞのｼﾘｱﾙ番号
						//　　最終ﾊﾞｲﾄはBCC
static unsigned char data[MAX_LEN];		//転送ﾃﾞｰﾀ
static unsigned char len;			//転送ﾃﾞｰﾀ長(全体ﾊﾞｲﾄ数)
static unsigned char lenb;			//転送ﾃﾞｰﾀ長(最終ﾊﾞｲﾄのﾋﾞｯﾄ数)
static unsigned char crc[2];			//CRC計算結果
static unsigned char status;			//MFRC522のｽﾃｰﾀｽ


//----------------------------------------------
//SPIで1ﾊﾞｲﾄを送受信する
//----------------------------------------------
unsigned char MFRC_spi_trans(			//受信したﾃﾞｰﾀを返す
	unsigned char data)			//送信するﾃﾞｰﾀ
{
	unsigned char c=8; 
	unsigned short s;

//DBG_C('<');
//DBG_B(data);

	s=data;					//送信するﾃﾞｰﾀを取り込む
	while(c--)
	{
		s<<=1;				//ﾃﾞｰﾀを左ｼﾌﾄしておく
		if(MFRC_MISO_R) s|=1;		//MISOをﾃﾞｰﾀに反映する
		if(s&0x0100) MFRC_MOSI_H;	//MSBからMOSIに
		else MFRC_MOSI_L;		//　反映する
#if defined(MFRC_SPI3)||defined(MFRC_SPI2)
					//3線SPIか2線SPIのとき
		MFRC_MOSI_OUT;			//MOSIを出力ﾓｰﾄﾞ
#endif
#ifdef MFRC_SPI2			//2線SPIのとき
		__disable_irq();		//割込み禁止
#endif
		MFRC_SCK_H;			//SCKをH
		MFRC_SCK_L;			//SCKをL
#ifdef MFRC_SPI2			//2線SPIのとき
		__enable_irq();			//割込み許可
#endif
#if defined(MFRC_SPI3)||defined(MFRC_SPI2)
					//3線SPIか2線SPIのとき
		MFRC_MISO_IN;			//MISOを入力ﾓｰﾄﾞ
#endif
	}

//DBG_S(s);
//DBG_C('>');

	return((unsigned char)s);		//受信ﾃﾞｰﾀを返す
}


//----------------------------------------------
//MFRC522のﾚｼﾞｽﾀを読込む
//----------------------------------------------
unsigned char MFRC_read_reg(			//ﾚｼﾞｽﾀ値を返す
	unsigned char reg)			//ﾚｼﾞｽﾀｱﾄﾞﾚｽ
{
	unsigned char value;

//DBG_C('r');
//DBG_B(reg);

#if !defined(MFRC_SPI3)&&!defined(MFRC_SPI2)
					//4線SPIのとき
	MFRC_MOSI_OUT;				//MOSIを出力ﾓｰﾄﾞ
#endif
#ifndef MFRC_SPI2			//4線SPIか3線SPIのとき
	MFRC_SCK_L;				//SCKをL
	MFRC_SCK_OUT;				//SCKを出力ﾓｰﾄﾞ
#endif
	MFRC_NSS_L;				//ｺﾏﾝﾄﾞを開始
	MFRC_spi_trans(((reg<<1)&0x7E)|0x80);	//ﾚｼﾞｽﾀｱﾄﾞﾚｽを送る
	value=MFRC_spi_trans(0);		//ﾚｼﾞｽﾀ値を得る
	MFRC_NSS_H;				//ｺﾏﾝﾄﾞを終了
	MFRC_NSS_WAIT;				//NSSのﾈｹﾞｰﾄを待つ

//DBG_B(value);
//DBG_C('.');

	return(value);				//ﾚｼﾞｽﾀ値を返す
}


//----------------------------------------------
//MFRC522のﾚｼﾞｽﾀを書込む
//----------------------------------------------
void MFRC_write_reg(
	unsigned char reg,			//ﾚｼﾞｽﾀｱﾄﾞﾚｽ
	unsigned char value)			//ﾚｼﾞｽﾀ設定値
{

//DBG_C('w');
//DBG_B(reg);
//DBG_B(value);

#if !defined(MFRC_SPI3)&&!defined(MFRC_SPI2)
					//4線SPIのとき
	MFRC_MOSI_OUT;				//MOSIを出力ﾓｰﾄﾞ
#endif
#ifndef MFRC_SPI2			//4線SPIか3線SPIのとき
	MFRC_SCK_L;				//SCKをL
	MFRC_SCK_OUT;				//SCKを出力ﾓｰﾄﾞ
#endif
	MFRC_NSS_L;				//ｺﾏﾝﾄﾞを開始
	MFRC_spi_trans((reg<<1)&0x7E);		//ﾚｼﾞｽﾀｱﾄﾞﾚｽを送る
	MFRC_spi_trans(value);			//ﾚｼﾞｽﾀ設定値を送る
	MFRC_NSS_H;				//ｺﾏﾝﾄﾞを終了
	MFRC_NSS_WAIT;				//NSSのﾈｹﾞｰﾄを待つ

//DBG_C('.');

}


//----------------------------------------------
//MFRC522のﾚｼﾞｽﾀをﾋﾞｯﾄｾｯﾄする
//----------------------------------------------
void MFRC_set_reg(
	unsigned char reg,			//ﾚｼﾞｽﾀｱﾄﾞﾚｽ
	unsigned char mask)			//ﾋﾞｯﾄﾏｽｸ
{
	unsigned char value;

	value=MFRC_read_reg(reg);		//現在値を読込む
	value|=mask;				//ﾋﾞｯﾄｾｯﾄする
	MFRC_write_reg(reg,value);		//書き戻す
}


//----------------------------------------------
//MFRC522のﾚｼﾞｽﾀをﾋﾞｯﾄｸﾘｱする
//----------------------------------------------
void MFRC_clr_reg(
	unsigned char reg,			//ﾚｼﾞｽﾀｱﾄﾞﾚｽ
	unsigned char mask)			//ﾋﾞｯﾄﾏｽｸ
{
	unsigned char value;

	value=MFRC_read_reg(reg);		//現在値を読込む
	value&=(~mask);				//ﾋﾞｯﾄｸﾘｱする
	MFRC_write_reg(reg,value);		//書き戻す
}


//----------------------------------------------
//MFRC522からﾊﾞｰｼﾞｮﾝを取得する
//----------------------------------------------
unsigned char MFRC_get_version(void)
{
	unsigned char value;

	value=MFRC_read_reg(VersionReg);	//ﾊﾞｰｼﾞｮﾝﾚｼﾞｽﾀを読込む
	return(value);				//ﾊﾞｰｼﾞｮﾝを返す
}


//----------------------------------------------
//MFRC522を初期化する
//----------------------------------------------
void MFRC_init(void)
{
	unsigned char value;

//DBG_C('i');

	MFRC_write_reg(CommandReg,RC522_SOFTRESET);
						//ｿﾌﾄﾘｾｯﾄする
	do
	{
		wait_ms(50);			//50ms待つ
		value=MFRC_read_reg(CommandReg);//ｺﾏﾝﾄﾞﾚｼﾞｽﾀを取得する
	}
	while(value&0x10);			//ﾊﾟﾜｰﾀﾞｳﾝﾋﾞｯﾄがｸﾘｱされるまで待つ

	MFRC_write_reg(TxModeReg,0x00);		//ﾎﾞｰﾚｰﾄをﾘｾｯﾄ
	MFRC_write_reg(RxModeReg,0x00);
	MFRC_write_reg(ModWidthReg,0x26);	//ModWidthをﾘｾｯﾄ
	MFRC_write_reg(TModeReg,0x80);		//TAuto=1; timer starts automatically at the
						//　end of the transmission in all
						//　communication
						//　modes at all speeds
        MFRC_write_reg(TPrescalerReg,0xA9);	//TPreScaler=TModeReg[3..0]:TPrescalerReg,
						//　ie 0x0A9 = 169 => f_timer=40kHz,
						//　ie a timer period of 25μs.
        MFRC_write_reg(TReloadRegH,0x03);	//Reload timer with 0x3E8=1000,
        MFRC_write_reg(TReloadRegL,0xE8);	//　ie 25ms before timeout.
        MFRC_write_reg(TxAutoReg,0x40);		//Default 0x00.Force a 100%ASK modulation
						//　independent of the ModGsPReg register
						//　setting
        MFRC_write_reg(ModeReg,0x3D);		//Default 0x3F.Set the preset value for
						//　the CRC coprocessor for the CalcCRC
						//　command to
						//　0x6363(ISO14443-3 part 6.2.4)
	MFRC_set_reg(TxControlReg,0x03);	//ｱﾝﾃﾅをｵﾝ

//DBG_C('.');
}


//----------------------------------------------
//MFRC522でﾀｸﾞへｺﾏﾝﾄﾞﾃﾞｰﾀを転送する(前半)
//
//BjarteJohansenのお手本では最終送信ﾃﾞｰﾀのﾋﾞｯﾄ数を呼び出し元で設定させているが､
//ここでは大域変数｢lenb｣で引き継ぎ､この関数で｢BitFramingReg｣に設定するやり方にした
//----------------------------------------------
void MFRC_trans_tag1(void)
{
	unsigned char n;

//DBG_C('t');
//DBG_B(*data);
//DBG_B(*(data+1));
//DBG_B(len);

	MFRC_write_reg(CommIEnReg,0xf7);	//割込み設定を初期化
	MFRC_clr_reg(CommIrqReg,0x80);		//割込み許可を初期化
	MFRC_set_reg(FIFOLevelReg,0x80);	//FIFOをｸﾘｱしておく
	MFRC_write_reg(CommandReg,RC522_IDLE);	//動作停止しておく

	for(n=0;n<len;n++) MFRC_write_reg(FIFODataReg,data[n]);
						//ｺﾏﾝﾄﾞﾃﾞｰﾀをFIFOへ送る

	MFRC_write_reg(CommandReg,RC522_TRANS);	//ｺﾏﾝﾄﾞﾃﾞｰﾀの転送指示

	MFRC_write_reg(BitFramingReg,lenb|0x80);//最終ﾊﾞｲﾄのﾋﾞｯﾄ数を設定して転送を開始する

//DBG_C('.');

}


//----------------------------------------------
//MFRC522でﾀｸﾞへｺﾏﾝﾄﾞﾃﾞｰﾀを転送する(後半)
//----------------------------------------------
unsigned char MFRC_trans_tag2(void)		//処理結果を返す
						//　MI_ERR:異常終了
						//　MI_NOTAG:ﾀｸﾞ無し
						//　MI_OK:正常終了
{
	unsigned char value;
	unsigned char lastBits,n;

//DBG_C('a');

	value=MFRC_read_reg(CommIrqReg);	//実行状態を取得する

//DBG_B(value);

	MFRC_clr_reg(BitFramingReg,0x80);	//転送を止める

	if(value&0x31)				//受信完了､ｺﾏﾝﾄﾞ終了､ﾀｲﾑｱｳﾄのいずれかのとき
	{
		if(!(MFRC_read_reg(ErrorReg)&0x1D))
						//ﾊﾞｯﾌｧｵｰﾊﾞﾌﾛｰ､ｺﾘｼﾞｮﾝ､CRC､ﾌﾟﾛﾄｺﾙの
						//　いずれのｴﾗｰも無いとき
		{
			if(value&0x01) return(MI_NOTAG);
						//ﾀｲﾑｱｳﾄのときはMI_NOTAGを返す

			value=MFRC_read_reg(FIFOLevelReg);
						//応答ﾃﾞｰﾀのﾊﾞｲﾄ数を得る

//DBG_B(value);

			lastBits=MFRC_read_reg(ControlReg)&0x07;
						//最終ﾃﾞｰﾀのﾋﾞｯﾄ数を得る

//DBG_B(lastBits);

						//ﾋﾞｯﾄ数に変換する
			if(lastBits) len=(value-1)*8+lastBits;
			else len=value*8;

//DBG_B(len);

			if(value==0) value=1;
			if(value>MAX_LEN) value=MAX_LEN;
						//許容する最大長以内に規制する

//DBG_B(value);

			for(n=0;n<value;n++) data[n]=MFRC_read_reg(FIFODataReg);
						//応答ﾃﾞｰﾀを取出す
			return(MI_OK);
		}
		else return(MI_ERR);
	}
	else return(MI_ERR);
}


//----------------------------------------------
//MFRC522でCRCを計算する(前半)
//----------------------------------------------
void MFRC_calculateCRC1(void)
{
	unsigned char n;

//DBG_C('c');
//DBG_B(*data);
//DBG_B(*(data+1));
//DBG_B(len);

	MFRC_clr_reg(DivIrqReg,0x04);		//CRC計算の割込みをｸﾘｱしておく
	MFRC_set_reg(FIFOLevelReg,0x80);	//FIFOをｸﾘｱしておく

	for(n=0;n<len;n++) MFRC_write_reg(FIFODataReg,data[n]);
						//ｺﾏﾝﾄﾞﾃﾞｰﾀをFIFOへ送る

	MFRC_write_reg(CommandReg,RC522_CALCCRC);
						//CRC計算を開始する

//DBG_C('.');

}


//----------------------------------------------
//MFRC522でCRCを計算する(後半)
//----------------------------------------------
void MFRC_calculateCRC2(void)
{

//DBG_C('d');

	crc[0]=MFRC_read_reg(CRCResultRegL);	//CRC計算結果を
	crc[1]=MFRC_read_reg(CRCResultRegM);	//　取出す

//DBG_B(crc[0]);
//DBG_B(crc[1]);
//DBG_C('.');

}


//----------------------------------------------
//MFRC522でﾀｸﾞからｼﾘｱﾙ番号を取得する(ﾀｽｸ処理)
//----------------------------------------------
unsigned char MFRC_TASK(void)			//ﾀｸﾞ情報の有無を返す(0=｢無し｣､1=｢有り｣)
{

//DBG_C('+');
//DBG_B(MF_TaskState);

	switch(MF_TaskState&0x7f)		//ﾀｽｸｽﾃｰﾄで分岐する
	{
	case 0:
		//ﾀｸﾞを検出する
		data[0]=MF1_REQIDL;		//ﾀｸﾞを検出する
		len=1;				//ｺﾏﾝﾄﾞﾃﾞｰﾀ長は
		lenb=7;				//　7ﾋﾞｯﾄ
		MFRC_trans_tag1();		//ｺﾏﾝﾄﾞﾃﾞｰﾀを転送する
		MF_TaskState++;			//ｽﾃｰﾄを進める
		break;

	case 1:
		//応答を得る
		status=MFRC_trans_tag2();	//応答を得る
//DBG_B(status);
//DBG_B(len);
		if(len!=16) status=MI_ERR;	//応答が16ﾋﾞｯﾄでなかったらｴﾗｰとする
//DBG_B(data[0]);
//DBG_B(data[1]);

		if(status!=MI_OK)		//ｴﾗｰのときは
		{
			MF_TagSerial[0]=0;	//ﾀｸﾞのｼﾘｱﾙ番号をｸﾘｱしておく
			MF_TagSerial[1]=0;
			MF_TagSerial[2]=0;
			MF_TagSerial[3]=0;
			MF_TagSerial[4]=0;	//最終ﾊﾞｲﾄはBCC1
			MF_TaskState=0;		//ﾀｸﾞ情報を｢無効｣にしてｽﾃｰﾄを最初に戻す
			break;
		}
		MF_TagType[0]=data[0];		//ﾀｸﾞのﾀｲﾌﾟを
		MF_TagType[1]=data[1];		//　取得する
		MF_TaskState++;			//ｽﾃｰﾄを進める
		break;

	case 2:
		//ｺﾘｼﾞｮﾝを回避する
		data[0]=MF1_ANTICOLL;		//ｺﾘｼﾞｮﾝ回避する
		data[1]=0x20;			//ｺﾘｼﾞｮﾝ回避の指定
		len=2;				//ｺﾏﾝﾄﾞﾃﾞｰﾀ長は2ﾊﾞｲﾄ
		lenb=0;
		MFRC_trans_tag1();		//ｺﾏﾝﾄﾞﾃﾞｰﾀを転送する
		MF_TaskState++;			//ｽﾃｰﾄを進める
		break;

	case 3:
		//応答を得る
		status=MFRC_trans_tag2();	//応答を得る
//DBG_B(status);
//DBG_B(len);
		if(len!=40) status=MI_ERR;	//応答が40ﾋﾞｯﾄでなかったらｴﾗｰとする

		if(!(status==MI_OK))		//ｴﾗｰのときは
		{
			MF_TaskState=0x08;	//　ﾀｸﾞ情報｢無し｣にしてｽﾃｰﾄをHALTへ飛ばす
			break;
		}
//DBG_B(data[0]);
//DBG_B(data[1]);
//DBG_B(data[2]);
//DBG_B(data[3]);
//DBG_B(data[4]);

		if((MF_TagSerial[0]!=data[0])||	//前回のﾀｸﾞとｼﾘｱﾙ番号が違うときは
			(MF_TagSerial[1]!=data[1])||
			(MF_TagSerial[2]!=data[2])||
			(MF_TagSerial[3]!=data[3]))
		{
			MF_TagSerial[0]=data[0];//ｼﾘｱﾙ番号を取込む
			MF_TagSerial[1]=data[1];
			MF_TagSerial[2]=data[2];
			MF_TagSerial[3]=data[3];
			MF_TagSerial[4]=data[4];//最終ﾊﾞｲﾄはBCC1
			MF_TaskState|=0x80;	//ﾀｸﾞ情報｢有効｣に設定する
			MF_TaskState++;		//ｽﾃｰﾄを進める
		}
		break;

	case 4:
		//ﾀｸﾞを選択する
		data[0]=MF1_SELECTTAG;		//ﾀｸﾞを選択する
		data[1]=0x70;			//ﾀｸﾞ選択の指定
		data[2]=MF_TagSerial[0];	//ｼﾘｱﾙ番号
		data[3]=MF_TagSerial[1];
		data[4]=MF_TagSerial[2];
		data[5]=MF_TagSerial[3];
		data[6]=MF_TagSerial[4];	//最終ﾊﾞｲﾄはBCC1
		len=7;
		MFRC_calculateCRC1();		//CRCを計算する(前半)
		MF_TaskState++;			//ｽﾃｰﾄを進める
		break;

	case 5:
		//CRCを取得する
		MFRC_calculateCRC2();		//CRCを計算する(後半)
		MF_TaskState++;			//ｽﾃｰﾄを進める
		break;

	case 6:
		//CRCを転送する
		data[7]=crc[0];			//CRC0
		data[8]=crc[1];			//CRC1
		len=9;				//ｺﾏﾝﾄﾞﾃﾞｰﾀ長は9ﾊﾞｲﾄ
		lenb=0;
		MFRC_trans_tag1();		//ｺﾏﾝﾄﾞﾃﾞｰﾀを転送する
		MF_TaskState++;			//ｽﾃｰﾄを進める
		break;

	case 7:
		//応答を得る
		MFRC_trans_tag2();		//応答を得る
//DBG_B(status);
		MF_TaskState++;			//ｽﾃｰﾄを進める
		break;

	case 8:
		//ﾀｸﾞ処理を終了する
		data[0]=MF1_HALT;		//ﾀｸﾞ処理を終了する
		data[1]=0;			//ADR
		len=2;				//ｺﾏﾝﾄﾞﾃﾞｰﾀ長は2ﾊﾞｲﾄ
		lenb=0;
		MFRC_calculateCRC1();		//CRCを計算する(前半)		
		MF_TaskState++;			//ｽﾃｰﾄを進める
		break;

	case 9:
		MFRC_calculateCRC2();		//CRCを計算する(後半)
		MF_TaskState++;			//ｽﾃｰﾄを進める
		break;

	case 10:
		//CRCを転送する
		data[2]=crc[0];			//CRC0
		data[3]=crc[1];			//CRC1
		len=4;				//ｺﾏﾝﾄﾞﾃﾞｰﾀ長は4ﾊﾞｲﾄ
		lenb=0;
		MFRC_trans_tag1();		//ｺﾏﾝﾄﾞﾃﾞｰﾀを転送する
		MF_TaskState++;			//ｽﾃｰﾄを進める
		break;

	case 11:
		//応答を得る
		MFRC_trans_tag2();		//応答を得る
		MF_TaskState++;			//ｽﾃｰﾄを進める
		break;

	case 12:
		//終了する
		MFRC_clr_reg(Status2Reg,0x08);	//暗号化をｵﾌ
		if(MF_TaskState&0x80)		//ﾀｸﾞ情報gが｢有効｣のときは
		{
			MF_TaskState=0;		//ｽﾃｰﾄを最初に戻す
			return(1);		//ﾀｸﾞ情報｢有り｣を返す
		}
		else				//ﾀｸﾞ情報gが｢無効｣のときは
		{
			MF_TaskState=0;		//ｽﾃｰﾄを最初に戻す
			break;			//ﾀｸﾞ情報｢無し｣を返す
		}
	}
	return(0);
}
