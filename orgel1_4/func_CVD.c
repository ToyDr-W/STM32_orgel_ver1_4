//CVDを評価する機能ﾓｼﾞｭｰﾙ
//
//CVDの構成を下記の例に倣って､呼出し元にて定義しておくこと｡
//
//CVDは複数個を実装可能｡CVD毎にﾎﾟｰﾄﾚｼﾞｽﾀが違ってもよい｡
//
//CVDのｵﾝとは､ﾀｯﾁした状態｡
//CVDのｵﾌとは､ﾀｯﾁしない状態｡
//
//CVDの評価は以下の2つのﾓｰﾄﾞがある｡
//・ﾘｱﾙﾓｰﾄﾞ
//　規定期間内のﾊﾞﾀﾂｷは無視する｡
//　状態が変化したときに変化の状況を返す｡
//　戻り値は､0=変化の検知なし､1=ｵﾌからｵﾝへの変化を検知､2=ｵﾝからｵﾌへの変化を検知｡
//・ﾀｲﾏｰﾓｰﾄﾞ
//　ｵﾝの継続時間が短ｵﾝ閾値になったら短ｵﾝ一致とする｡
//　ｵﾝの継続時間が長ｵﾝ閾値になったら長ｵﾝ一致とする｡
//　ｵﾝからｵﾌへ変化したときに､ｵﾝ状態であった時間が短ｵﾝ閾値超えのときは短ｵﾝ検知とする｡
//　戻り値は､0=検知なし､1=短ｵﾝ検知､2=長ｵﾝ一致､3=短ｵﾝ一致｡
//ﾘｱﾙﾓｰﾄﾞとﾀｲﾏｰﾓｰﾄﾞは排他的で､1つのCVDに対してどちらか1つのﾓｰﾄﾞを指定できる｡
//
//CVDを評価する場合は､このﾓｼﾞｭｰﾙを#includeする｡
//ﾘｱﾙﾓｰﾄﾞで評価するときは､CVD_REALを宣言する｡
//ﾀｲﾏｰﾓｰﾄﾞで評価するときは､CVD_TIMERを宣言する｡
//Sleep関連の処理にこのﾓｼﾞｭｰﾙは関与しない｡


//==============================================
//CVD構成の定義例
//==============================================
//#define	CVD_REAL			//CVDﾘｱﾙﾓｰﾄﾞを実装するときに宣言する
//#define	CVD_TIMER			//CVDﾀｲﾏｰﾓｰﾄﾞを実装するときに宣言する

//CVDの構成--------------------------------------
//static const struct CVD_CNF
//{
//	GPIO_TypeDef *gpio;			//GPIOｱﾄﾞﾚｽ
//	unsigned short chsel;			//CHSELﾏｽｸ/ANIN番号
//	unsigned char mode;			//b7:ﾓｰﾄﾞ(0=ﾘｱﾙﾓｰﾄﾞ､1=ﾀｲﾏｰﾓｰﾄﾞ)
//						//b3-b0:ﾋﾞｯﾄ番号
//} cvd_cnf[]=
//{
//CVD0の構成-------------------------------------
//	{CVD0_GPIO,CVD0_CHSEL,CVD0_MODE|CVD0_BIT},
//CVD1の構成-------------------------------------
//	{CVD1_GPIO,CVD1_CHSEL,CVD1_MODE|CVD1_BIT},
//CVD2の構成-------------------------------------
//	{CVD2_GPIO,CVD2_CHSEL,CVD2_MODE|CVD2_BIT},
//CVD3の構成-------------------------------------
//	{CVD3_GPIO,CVD3_CHSEL,CVD3_MODE|CVD3_BIT},
//};


//==============================================
//CVD評価の閾値等
//==============================================
//閾値1は､前回がｵﾝ判定だったときにこれを超えるとｵﾝ判定
//閾値2は､前回がｵﾌ判定だったときにこれを超えるとｵﾝ判定
//つまり､閾値2-閾値1がﾋｽﾃﾘｼｽ巾になる

#ifdef DEV_F030				//F030ﾃﾞﾊﾞｲｽのとき
#ifndef	CVD_CNT				//評価の回数が宣言されていないとき
#define	CVD_CNT		8			//　評価の回数を宣言する
#endif
#ifndef	CVD_THR1			//評価の閾値1が宣言されていないとき
#define	CVD_THR1	20			//　閾値1を宣言する
#endif
#ifndef	CVD_THR2			//評価の閾値2が宣言されていないとき
#define	CVD_THR2	40			//　閾値2を宣言する
#endif
#ifndef	CVD_INC				//移動平均計算の増分値が宣言されていないとき
#define	CVD_INC		2			//　増分値を宣言する
#endif
#endif

#ifdef DEV_F103				//F103のとき
#ifndef	CVD_CNT				//評価の回数が宣言されていないとき
#define	CVD_CNT		8			//　評価の回数を宣言する
#endif
#ifndef	CVD_THR1			//評価の閾値1が宣言されていないとき
#define	CVD_THR1	30			//　閾値1を宣言する
#endif
#ifndef	CVD_THR2			//評価の閾値2が宣言されていないとき
#define	CVD_THR2	60			//　閾値2を宣言する
#endif
#ifndef	CVD_INC				//移動平均計算の増分値が宣言されていないとき
#define	CVD_INC		2			//　増分値を宣言する
#endif
#endif

#ifdef DEV_F302				//F302のとき
#ifndef	CVD_CNT				//評価の回数が宣言されていないとき
#define	CVD_CNT		8			//　評価の回数を宣言する
#endif
#ifndef	CVD_THR1			//評価の閾値1が宣言されていないとき
#define	CVD_THR1	20			//　閾値1を宣言する
#endif
#ifndef	CVD_THR2			//評価の閾値2が宣言されていないとき
#define	CVD_THR2	40			//　閾値2を宣言する
#endif
#ifndef	CVD_INC				//移動平均計算の増分値が宣言されていないとき
#define	CVD_INC		2			//　増分値を宣言する
#endif
#endif

#ifdef DEV_G03x				//G03xのとき
#ifndef	CVD_CNT				//評価の回数が宣言されていないとき
#define	CVD_CNT		8			//　評価の回数を宣言する
#endif
#ifndef	CVD_THR1			//評価の閾値1が宣言されていないとき
#define	CVD_THR1	20			//　閾値1を宣言する
#endif
#ifndef	CVD_THR2			//評価の閾値2が宣言されていないとき
#define	CVD_THR2	40			//　閾値2を宣言する
#endif
#ifndef	CVD_INC				//移動平均計算の増分値が宣言されていないとき
#define	CVD_INC		2			//　増分値を宣言する
#endif
#endif

#ifdef DEV_L010				//L010ﾃﾞﾊﾞｲｽのとき
#ifndef	CVD_CNT				//評価の回数が宣言されていないとき
#define	CVD_CNT		8			//　評価の回数を宣言する
#endif
#ifndef	CVD_THR1			//評価の閾値1が宣言されていないとき
#define	CVD_THR1	20			//　閾値1を宣言する
#endif
#ifndef	CVD_THR2			//評価の閾値2が宣言されていないとき
#define	CVD_THR2	40			//　閾値2を宣言する
#endif
#ifndef	CVD_INC				//移動平均計算の増分値が宣言されていないとき
#define	CVD_INC		2			//　増分値を宣言する
#endif
#endif

//ﾘｱﾙﾓｰﾄﾞの設定値
#ifndef	CVD_HST_BIT			//ﾊﾞﾀﾂｷ除去期間[評価周期](1～7の範囲)が
					//　宣言されていないとき
#define	CVD_HST_BIT	3			//　ﾊﾞﾀﾂｷ除去期間を宣言する
#endif
#define	CVD_HST_MSK	((1<<(CVD_HST_BIT+1))-1)//履歴判定ﾋﾞｯﾄﾏｽｸ
#define	CVD_HST_ON	((1<<CVD_HST_BIT)-1)	//履歴ｵﾝ変化判定値
#define	CVD_HST_OFF	((1<<(CVD_HST_BIT+1))-2)//履歴ｵﾌ変化判定値

//ﾀｲﾏｰﾓｰﾄﾞの設定値
#ifndef	CVD_SHORT			//短ｵﾝ時間閾値[ms]が宣言されていないとき
#define	CVD_SHORT	100			//ﾃﾞﾌｫﾙﾄ値[ms]を設定する
#endif
#ifndef	CVD_LONG			//長ｵﾝ時間閾値[ms]が宣言されていないとき
#define	CVD_LONG	1000			//ﾃﾞﾌｫﾙﾄ値[ms]を設定する
#endif

//初期安定ﾀｲﾏｰ
#define	CVD_STB_TMR	8			//初期時の安定時間[ｺｰﾙ回数]
 


//==============================================
//作業域
//==============================================
static unsigned char cvd_stb_tmr;		//初期安定ﾀｲﾏｰ

static struct HIST				//履歴情報
{
	unsigned short cvd_ave;			//ADC移動平均(記録)
	unsigned char cvd_hst;			//CVDの検査履歴(記録)
						//ﾘｱﾙﾓｰﾄﾞではｵﾝ/ｵﾌ状態履歴(0=ｵﾌ､1=ｵﾝ)
						//　b0が最新状態で評価周期毎に左ｼﾌﾄ
						//ﾀｲﾏｰﾓｰﾄﾞではｵﾝ状態経過時間[評価周期]
}cvd_hist[CVD_SU];


//==============================================
//CVDを評価する
//ﾊﾟﾗﾒｰﾀ
//　評価対象のCVD番号[0起算]
//　稼働状態(0=通常稼働中､1=Sleep中)
//戻り値
//　ﾘｱﾙﾓｰﾄﾞのとき W:評価結果(0=変化なし､1=ｵﾌからｵﾝへ変化検知､2=ｵﾝからｵﾌへ変化検知)
//　ﾀｲﾏｰﾓｰﾄﾞのとき W:評価結果(0=検知なし､1=短ｵﾝ検知､2=長ｵﾝ一致､3=短ｵﾝ一致)
//Sleep中の検査のときは､操作を検知しても履歴を保持する
//==============================================
unsigned char cvd_read(				//ﾀｯﾁ評価結果を返す
	unsigned char no,			//評価対象のCVD番号(0起算)
	unsigned char sleep)			//Sleep中の検査のときは1を指定する
{
	unsigned short value;			//CVDの評価値(小：ﾀｯﾁ無し､大:ﾀｯﾁ有り)
	unsigned char res;			//CVD評価戻り値
	const struct CVD_CNF *p;		//CVD構成表へのﾎﾟｲﾝﾀ
	struct HIST *q;				//履歴情報へのﾎﾟｲﾝ
	unsigned char n;

if(no) DBG_C('-');
else DBG_C('\n');

//DBG_C('\n');
//DBG_C('v');
DBG_B(no);
//DBG_B(sleep);
	//構成表へのﾎﾟｲﾝﾀを設定
	if(no>=CVD_SU) return(0);
	p=cvd_cnf+no;
//DBG_L(p);
//DBG_D(p,8);

	//履歴情報へのﾎﾟｲﾝﾀ
	q=cvd_hist+no;
//DBG_L(q);
//DBG_D(q,4);

	//ADC設定
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef DEV_F030				//F030のとき(ここから)
	ADC1->CFGR1=ADC1->CFGR1&~ADC_CFGR1_RES_Msk|ADC_CFGR1_RES_0;
						//分解能を10ﾋﾞｯﾄにする
	ADC1->SMPR=0;				//ｻﾝﾌﾟﾘﾝｸﾞ時間は1/14MHz*1.5=0.1us)
	ADC1->CHSELR=(uint32_t)p->chsel;	//CHSELを設定する
#endif					//F030のとき(ここまで)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef DEV_F103				//F103のとき(ここから)
	ADC1->SMPR2=0x00000000;			//ｻﾝﾌﾟﾘﾝｸﾞ時間は1/10.7MHz*1.5=0.14us)
	ADC1->SQR3=p->chsel;			//ADIN番号を設定する
#endif					//F103のとき(ここまで)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef DEV_F302				//F302のとき(ここから)
	ADC1->CFGR=ADC1->CFGR&~ADC_CFGR_RES_Msk|ADC_CFGR_RES_0;
						//分解能を10ﾋﾞｯﾄにする
	ADC1->SMPR1=0x00000010;			//ｻﾝﾌﾟﾘﾝｸﾞ時間は1/64MHz*4.5=0.07us)
	ADC1->SQR1=p->chsel;			//ADIN番号を設定する
#endif					//F302のとき(ここまで)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef DEV_G03x				//G03xのとき(ここから)
	ADC1->CFGR1=ADC1->CFGR1&~ADC_CFGR1_RES_Msk|ADC_CFGR1_RES_0;
						//分解能を10ﾋﾞｯﾄにする
	ADC1->SMPR=0;				//ｻﾝﾌﾟﾘﾝｸﾞ時間は1/16MHz*1.5=0.1us)
	ADC1->CHSELR=(uint32_t)p->chsel;	//CHSELを設定する
#endif					//G03xのとき(ここまで)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef DEV_L010				//L010のとき(ここから)
	ADC1->CFGR1=ADC1->CFGR1&~ADC_CFGR1_RES_Msk|ADC_CFGR1_RES_0;
						//分解能を10ﾋﾞｯﾄにする
	ADC1->SMPR=0;				//ｻﾝﾌﾟﾘﾝｸﾞ時間は1/16MHz*1.5=0.1us)
	ADC1->CHSELR=(uint32_t)p->chsel;	//CHSELを設定する
#endif					//L010のとき(ここまで)

	//CVD計測
DBG_C('-');
DBG_S(q->cvd_ave);
	value=0;				//評価値を初期化しておく
	GPIO_L(p->gpio,(p->mode&0x000f));	//ﾋﾟﾝをL
	GPIO_OUT(p->gpio,(p->mode&0x000f));	//ﾋﾟﾝを放電する
	for(n=0;n<CVD_CNT;n++)
	{

		//ADC実行
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if defined(DEV_F030)			//F030のとき(ここから)
DBS_H;
		ADC1->CR|=ADC_CR_ADEN;		//ADC1有効
		while(!(ADC1->ISR&ADC_ISR_ADRDY)){};
						//ADC1有効の完了を待つ
		__disable_irq();		//割込み禁止
		GPIO_INPU(p->gpio,(p->mode&0x000f));
						//充電を開始する
						//ｱﾅﾛｸﾞﾓｰﾄﾞにするとﾌﾟﾙｱｯﾌﾟが無効になるのでﾀﾞﾒ
		GPIO_IN(p->gpio,(p->mode&0x000f));
						//充電を終了する
		ADC1->CR|=ADC_CR_ADSTART;	//変換開始
		__enable_irq();			//割込み許可
		while(!(ADC1->ISR&ADC_ISR_EOC)){};
						//変換完了を待つ
DBS_L;
		GPIO_L(p->gpio,(p->mode&0x000f));
						//ﾋﾟﾝをL
		GPIO_OUT(p->gpio,(p->mode&0x000f));
						//ﾋﾟﾝを放電する
DBG_C('+');
DBG_S(ADC1->DR);
		value+=1024-ADC1->DR;		//変換結果10ﾋﾞｯﾄを評価値に累積する
		ADC1->CR|=ADC_CR_ADDIS;		//ADC1無効
		while(ADC1->CR&ADC_CR_ADEN){};	//ADC1無効を待つ
#endif					//F030のとき(ここまで)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if defined(DEV_F103)			//F103のとき(ここから)
		ADC1->CR2|=ADC_CR2_ADON;	//ADC1有効
		wait_1us();			//起動時間待ち
DBS_H;
		__disable_irq();		//割込み禁止
		GPIO_INPU(p->gpio,(p->mode&0x000f));
						//充電を開始する
						//ｱﾅﾛｸﾞﾓｰﾄﾞにするとﾌﾟﾙｱｯﾌﾟが無効になるのでﾀﾞﾒ
		GPIO_IN(p->gpio,(p->mode&0x000f));
						//充電を終了する
		ADC1->CR2|=ADC_CR2_ADON;	//変換開始
		__enable_irq();			//割込み許可
		while(!(ADC1->SR&ADC_SR_EOC)){};//変換完了を待つ
DBS_L;
		ADC1->SR&=~ADC_SR_EOC;		//EOCﾌﾗｸﾞをｸﾘｱしておく
		GPIO_L(p->gpio,(p->mode&0x000f));
						//ﾋﾟﾝをL
		GPIO_OUT(p->gpio,(p->mode&0x000f));
						//ﾋﾟﾝを放電する
DBG_C('+');
DBG_S(ADC1->DR);
		value+=1024-(ADC1->DR>>2);	//変換結果を10ﾋﾞｯﾄ精度で評価値に累積する
		ADC1->CR2&=~ADC_CR2_ADON;	//ADC1無効
		wait_1us();			//ADC1無効時間待ち
#endif					//F103のとき(ここまで)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if defined(DEV_F302)			//F302のとき(ここから)
DBS_H;
		ADC1->CR|=ADC_CR_ADEN;		//ADC1有効
		while(!(ADC1->ISR&ADC_ISR_ADRDY)){};
						//ADC1有効の完了を待つ
		__disable_irq();		//割込み禁止
		GPIO_INPU(p->gpio,(p->mode&0x000f));
						//充電を開始する
						//ｱﾅﾛｸﾞﾓｰﾄﾞにするとﾌﾟﾙｱｯﾌﾟが無効になるのでﾀﾞﾒ

		GPIO_INPU(p->gpio,(p->mode&0x000f));	//F302では充電時間を長めにする
		GPIO_INPU(p->gpio,(p->mode&0x000f));

		GPIO_IN(p->gpio,(p->mode&0x000f));
						//充電を終了する
		ADC1->CR|=ADC_CR_ADSTART;	//変換開始
		__enable_irq();			//割込み許可
		while(!(ADC1->ISR&ADC_ISR_EOC)){};
						//変換完了を待つ
DBS_L;
		GPIO_L(p->gpio,(p->mode&0x000f));
						//ﾋﾟﾝをL
		GPIO_OUT(p->gpio,(p->mode&0x000f));
						//ﾋﾟﾝを放電する
DBG_C('+');
DBG_S(ADC1->DR);
		value+=1024-(ADC1->DR);		//変換結果10ﾋﾞｯﾄを評価値に累積する
		ADC1->CR|=ADC_CR_ADDIS;		//ADC1無効
		while(ADC1->CR&ADC_CR_ADEN){};	//ADC1無効を待つ
#endif					//F302のとき(ここまで)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if defined(DEV_G03x)			//G03xのとき(ここから)
DBS_H;
		ADC1->CR|=ADC_CR_ADEN;		//ADC1有効
		while(!(ADC1->ISR&ADC_ISR_ADRDY)){};
						//ADC1有効の完了を待つ
		__disable_irq();		//割込み禁止
		GPIO_INPU(p->gpio,(p->mode&0x000f));
						//充電を開始する
						//ｱﾅﾛｸﾞﾓｰﾄﾞにするとﾌﾟﾙｱｯﾌﾟが無効になるのでﾀﾞﾒ

		wait_1us();			//G03xでは充電時間を長め(1us)にする

		GPIO_IN(p->gpio,(p->mode&0x000f));
						//充電を終了する
		ADC1->CR|=ADC_CR_ADSTART;	//変換開始
		__enable_irq();			//割込み許可
		while(!(ADC1->ISR&ADC_ISR_EOC)){};
						//変換完了を待つ
DBS_L;
		GPIO_L(p->gpio,(p->mode&0x000f));
						//ﾋﾟﾝをL
		GPIO_OUT(p->gpio,(p->mode&0x000f));
						//ﾋﾟﾝを放電する
DBG_C('+');
DBG_S(ADC1->DR);
		value+=1024-ADC1->DR;		//変換結果10ﾋﾞｯﾄを評価値に累積する
		ADC1->CR|=ADC_CR_ADDIS;		//ADC1無効
		while(ADC1->CR&ADC_CR_ADEN){};	//ADC1無効を待つ
#endif					//G03xのとき(ここまで)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if defined(DEV_L010)			//L010のとき(ここから)
DBS_H;
		ADC1->CR|=ADC_CR_ADEN;		//ADC1有効
		while(!(ADC1->ISR&ADC_ISR_ADRDY)){};
						//ADC1有効の完了を待つ
		__disable_irq();		//割込み禁止
		GPIO_INPU(p->gpio,(p->mode&0x000f));
		GPIO_INPU(p->gpio,(p->mode&0x000f));	//時間稼ぎ
		GPIO_INPU(p->gpio,(p->mode&0x000f));	//時間稼ぎ
		GPIO_INPU(p->gpio,(p->mode&0x000f));	//時間稼ぎ
						//充電を開始する
						//ｱﾅﾛｸﾞﾓｰﾄﾞにするとﾌﾟﾙｱｯﾌﾟが無効になるのでﾀﾞﾒ
		GPIO_IN(p->gpio,(p->mode&0x000f));
						//充電を終了する
		ADC1->CR|=ADC_CR_ADSTART;	//変換開始
		__enable_irq();			//割込み許可
		while(!(ADC1->ISR&ADC_ISR_EOC)){};
						//変換完了を待つ
DBS_L;
		GPIO_L(p->gpio,(p->mode&0x000f));
						//ﾋﾟﾝをL
		GPIO_OUT(p->gpio,(p->mode&0x000f));
						//ﾋﾟﾝを放電する
DBG_C('+');
DBG_S(ADC1->DR);
		value+=1024-ADC1->DR;		//変換結果10ﾋﾞｯﾄを評価値に累積する
		ADC1->CR|=ADC_CR_ADDIS;		//ADC1無効
		while(ADC1->CR&ADC_CR_ADEN){};	//ADC1無効を待つ
#endif					//L010のとき(ここまで)
	}
DBG_C('=');
DBG_S(value);

	//定常動作待ち
	if(cvd_stb_tmr<CVD_STB_TMR)		//初期安定ﾀｲﾏｰが未満了のときは
	{
		cvd_stb_tmr++;			//初期安定ﾀｲﾏｰをｲﾝｸﾘする
		return(0);			//何もせず戻る
	}

	//移動平均(簡易計算)
	if(q->cvd_ave)				//定常動作のとき
	{
		if(value>q->cvd_ave) q->cvd_ave+=CVD_INC;
						//評価値が移動平均を超えたら
						//　移動平均を増加させる
		else q->cvd_ave=value;		//超えてなければ
						//　評価値を移動平均にする
	//	else q->cvd_ave-=CVD_INC*2;	//超えてなければ
						//　評価値を速めに減少させる
	}
	else					//初回動作のとき
	{
		q->cvd_ave=value;		//移動平均を初期化する
	}
//DBG_C('-');
//DBG_S(q->cvd_ave);

	//ﾀｲﾏｰﾓｰﾄﾞの処理
	if(p->mode&CVD_MODE)			//ﾀｲﾏｰﾓｰﾄﾞのとき
	{
#ifdef CVD_TIMER			//ﾀｲﾏｰﾓｰﾄﾞを実装するとき
//DBG_C('t');

		//ﾀｯﾁを判定
//DBG_B(q->cvd_hst);
		res=0;				//仮にｵﾌ判定にしておく
		if(q->cvd_hst)			//前回がｵﾝ判定だったとき
		{
			if(value>q->cvd_ave+CVD_THR1*CVD_CNT) res=1;
						//評価値>(移動平均値+閾値1)のときｵﾝ判定
		}
		else				//前回がｵﾌ判定だったとき
		{
			if(value>q->cvd_ave+CVD_THR2*CVD_CNT) res=1;
						//評価値>(移動平均値+閾値2)のときｵﾝ判定
		}
//DBG_B(res);

		//履歴を評価
		if(res)				//ｵﾝ判定のとき
		{
//DBG_C('+');
			if((++(q->cvd_hst)==0)) q->cvd_hst--;
						//ｵﾝ継続時間をｲﾝｸﾘして
						//　ｵｰﾊﾞﾌﾛｰしたら戻す
//DBG_B(q->cvd_hst);
			if(q->cvd_hst==CVD_LONG/KOYUU_PR)
						//長ｵﾝ閾値に一致したら
			{
				res=2;		//　｢2｣を返す
				if(sleep) q->cvd_hst--;
						//　Sleep中は履歴を巻戻す
			}
			else if(q->cvd_hst==CVD_SHORT/KOYUU_PR)
						//短ｵﾝ閾値に一致したら
			{
				res=3;		//　｢3｣を返す
				if(sleep) q->cvd_hst--;
						//　Sleep中は履歴を巻戻す
			}
			else res=0;		//その他は｢0｣を返す
		}
		else				//ｵﾌ判定のとき
		{
//DBG_C('-');
//DBG_B(q->cvd_hst);
			if(q->cvd_hst>CVD_LONG/KOYUU_PR) res=0;
						//長ｵﾝ閾値を超えていたら｢0｣を返す
			else if(q->cvd_hst>CVD_SHORT/KOYUU_PR) res=1;
						//短ｵﾝ閾値を超えていたら｢1｣を返す
			else res=0;		//その他は｢0｣を返す
			if(!sleep||!res) q->cvd_hst=0;
						//稼働中か検知無しのときは履歴をｸﾘｱする
		}

//DBG_B(res);
#else					//ﾀｲﾏｰﾓｰﾄﾞを実装しないとき
		return(0);
#endif
	}

	//ﾘｱﾙﾓｰﾄﾞの処理
	else
	{
#ifdef CVD_REAL				//ﾘｱﾙﾓｰﾄﾞを実装するとき
//DBG_C('r');
		q->cvd_hst<<=1;			//履歴をｼﾌﾄしておく
//DBG_B(q->cvd_hst);
//DBG_B(value);
		if(q->cvd_hst&0x02)		//前回がｵﾝ判定だったとき
		{
			if(value>(short)q->cvd_ave+CVD_THR1*CVD_CNT) q->cvd_hst|=1;
						//評価値>(移動平均値+閾値1)のときｵﾝ判定
		}
		else				//前回がｵﾌ判定だったとき
		{
			if(value>(short)q->cvd_ave+CVD_THR2*CVD_CNT) q->cvd_hst|=1;
						//評価値>(移動平均値+閾値1)のときｵﾝ判定
		}
//DBG_B(q->cvd_hst);
		switch(q->cvd_hst&CVD_HST_MSK)	//履歴で判定する
		{
		case CVD_HST_ON:		//ｵﾝ判定ﾊﾟﾀｰﾝのときは
			res=1;			//　｢1｣を返す
			break;
		case CVD_HST_OFF:		//ｵﾌ判定ﾊﾟﾀｰﾝのときは
			res=2;			//　｢2｣を返す
			break;
		default:
			res=0;			//その他は｢0｣を返す
		}
//DBG_B(res);
		if(res&&sleep) q->cvd_hst>>=1;	//Sleep中に操作を検知したら履歴を巻戻す
#else					//ﾘｱﾙﾓｰﾄﾞを実装しないとき
		return(0);
#endif
	}
DBG_C('-');
DBG_B(res);
	return(res);				//評価結果を返す
}
