//RFIDﾓｼﾞｭｰﾙ｢MFRC522｣を使ってRFIDﾀｸﾞからｼﾘｱﾙ番号を取得して
//その5ﾊﾞｲﾄ目の値に対応する曲､音声を再生する


//==============================================
//MFRC522制御の機能ﾓｼﾞｭｰﾙの呼込み
//==============================================
#include "func_RFID.c"


//==============================================
//ｱﾌﾟﾘの初期設定
//==============================================
static void apl_init(void)
{

DBG_C('\n');
DBG_C('i');
	//MFRC522を繋ぐﾎﾟｰﾄの設定
	MFRC_NSS_H;				//MFRC_NSSをH
#ifndef MFRC_SPI2			//2線SPIでないとき
	MFRC_NSS_OUT;				//MFRC_NSSを出力
	MFRC_SCK_L;				//MFRC_SCKをL
#endif
	MFRC_SCK_OUT;				//MFRC_SCKを出力
#if !defined(MFRC_SPI2)&&!defined(MFRC_SPI3)
					//4線SPIのとき
	MFRC_MOSI_L;				//MFRC__MOSIをL
	MFRC_MOSI_OUT;				//MFRC__MOSIを出力
	MFRC_MISO_IN;				//MFRC_MISOを入力
#endif

	//電圧安定時間
	wait_ms(100);				//100ms待つ

	//MFRC522を初期化
#ifdef SHDBG
	unsigned char version=MFRC_get_version();
						//ﾊﾞｰｼﾞｮﾝを取得する
DBG_B(version);
#endif
	MFRC_init();				//MFRCを初期化
}


//==============================================
//固有の処理(ｺｰﾙﾊﾞｯｸ関数)
//==============================================
static void CB_KOYUU(void)
{

//DBG_C('\n');
DBG_C('m');
DBG_B(MF_TaskState);
	if(!MFRC_TASK()) return;		//ﾀｸﾞ情報を取得できなかったら戻る

DBG_C('\n');
DBG_B(MF_TagType[0]);
DBG_B(MF_TagType[1]);
DBG_C('-');
DBG_B(MF_TagSerial[0]);
DBG_B(MF_TagSerial[1]);
DBG_B(MF_TagSerial[2]);
DBG_B(MF_TagSerial[3]);
DBG_B(MF_TagSerial[4]);

	if(MF_TagSerial[4]==TAG_NO1)		//赤ﾀｸﾞのときは
	{
#if PART_N>0				//ｵﾙｺﾞｰﾙ演奏を実装するとき
		SONG_SEL(1);			//赤とんぼ
#endif
#if VOICE_N>0				//音声再生を実装するとき
		VOICEN_SEL(0,1);		//赤
#endif
	}
	else if(MF_TagSerial[4]==TAG_NO2)	//青ﾀｸﾞのときは
	{
#if PART_N>0				//ｵﾙｺﾞｰﾙ演奏を実装するとき
		SONG_SEL(2);			//青い目の
#endif
#if VOICE_N>0				//音声再生を実装するとき
		VOICEN_SEL(0,2);		//青
#endif
	}
	else if(MF_TagSerial[4]==TAG_NO3)	//黄ﾀｸﾞのときは
	{
#if PART_N>0				//ｵﾙｺﾞｰﾙ演奏を実装するとき
		SONG_SEL(3);			//汽車ポッポ
#endif
#if VOICE_N>0				//音声再生を実装するとき
		VOICEN_SEL(0,3);		//黄色
#endif
	}
	else if(MF_TagSerial[4]==TAG_NO0)	//白ﾀｸﾞのときは
	{
#if PART_N>0				//ｵﾙｺﾞｰﾙ演奏を実装するとき
		SONG_SEL(0);			//ｵﾙｺﾞｰﾙ演奏を停止する
#endif
#if VOICE_N>0				//音声再生を実装するとき
		VOICEN_SEL(0,0);		//音声再生を停止する
#endif
	}
}


#if	PART_N>0			//ｵﾙｺﾞｰﾙ演奏を実装するとき
//==============================================
//演奏終了時の処理(ｺｰﾙﾊﾞｯｸ関数)
//==============================================
static void CB_SONG_END(void)
{
}
#endif


#if	VOICE_N>0			//音声N再生を実装するとき
//==============================================
//音声0再生終了時の処理(ｺｰﾙﾊﾞｯｸ関数)
//==============================================
static void CB_VOICEN_END(
	unsigned char ch)			//音声Nのch番号
{
}
#endif


#if	VOICE_I>0			//音声I再生を実装するとき
//==============================================
//音声I再生終了時の処理(ｺｰﾙﾊﾞｯｸ関数)
//==============================================
static void CB_VOICEI_END(void)
{
}
#endif


#if	VOICE_C>0			//音声C再生を実装するとき
//==============================================
//音声C再生終了時の処理(ｺｰﾙﾊﾞｯｸ関数)
//==============================================
static void CB_VOICEC_END(void)
{
}
#endif


#if	VOICE_S>0			//音声S再生を実装するとき
//==============================================
//音声S再生終了時の処理(ｺｰﾙﾊﾞｯｸ関数)
//==============================================
static void CB_VOICES_END((
	unsigned char ch)			//音声Sのch番号
{
}
#endif
