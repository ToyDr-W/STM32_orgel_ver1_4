//SW･CdS･CVDでSTM32電子ｵﾙｺﾞｰﾙを制御するｱﾌﾟﾘｹｰｼｮﾝの共通機能部

//複数のSW･１個のCdS･複数のCVDを実装可能｡但しﾎﾟｰﾄ割当てが可能なもの｡
//それぞれをｵﾙｺﾞｰﾙ演奏･ﾌﾗｯｼｭ音声再生･ﾌﾟﾛｸﾞﾗﾑ音声再生の開始･中断に充てることができる｡
//短ｵﾝで開始､長ｵﾝで中断する｡
//曲･音声はﾗｳﾝﾄﾞﾛﾋﾞﾝ･ﾗﾝﾀﾞﾑが可能｡

//SWﾎﾟｰﾄの内部ﾌﾟﾙｱｯﾌﾟ/ﾀﾞｳﾝはこのｺｰﾄﾞ内で行っている｡
//CdSﾎﾟｰﾄの内部ﾌﾟﾙｱｯﾌﾟ/ﾀﾞｳﾝはfunc_CdS.cで行っている｡
//CVDﾎﾟｰﾄの内部ﾌﾟﾙﾀﾞｳﾝはfunc_CVD.cで行っている｡

//下記のｵﾌﾟｼｮﾝ設定を呼出し元で宣言しておくこと｡
//#define	SLEEP_EN	//Sleep機能を実装するときに宣言する
//#define	KYOKU_AUTO	//ｵﾙｺﾞｰﾙ演奏を自動開始するときに宣言する
//#define	ONSEIN_AUTO	//音声0再生を自動開始するときに宣言する
//#define	ONSEII_AUTO	//音声I再生を自動開始するときに宣言する
//#define	ONSEIC_AUTO	//音声C再生を自動開始するときに宣言する
//#define	ONSEIS_AUTO	//音声S再生を自動開始するときに宣言する
//#define	RAND_CHSEL	0x00000001	//ﾗﾝﾀﾞﾏｲｽﾞの種にするCHSEL(ﾃﾞﾌｫﾙﾄはCH0:PA0)
//#define	KYOKU_RAND	//曲番号をﾗﾝﾀﾞﾏｲｽﾞするときに宣言する
//#define	ONSEIN_RAND	//音声N番号をﾗﾝﾀﾞﾏｲｽﾞするときに宣言する
//#define	ONSEII_RAND	//音声I番号をﾗﾝﾀﾞﾏｲｽﾞするときに宣言する
//#define	ONSEIC_RAND	//音声C番号をﾗﾝﾀﾞﾏｲｽﾞするときに宣言する
//#define	ONSEIS_RAND	//音声S番号をﾗﾝﾀﾞﾏｲｽﾞするときに宣言する
//#define	SW_EN		//SWを実装するときに宣言する
//#define	SW_PLNG		//SWをｲﾍﾞﾝﾄではなく､ﾎﾟｰﾘﾝｸﾞするときに宣言する
//#define	CdS_EN		//CdSを実装するときに宣言する
//#define	CVD_EN		//CVDを実装するときに宣言する

//ｵﾙｺﾞｰﾙ箱への応用では以下を宣言しておくこと
//#define	CdS_FUTA	//蓋を開ける(明)とｵﾝ判定する
//#define	CdS_THR	100	//判定閾値を固定の宣言値にする


//==============================================
//構成情報(個別ｱﾌﾟﾘｹｰｼｮﾝだけで使うもの)
//==============================================
#define	SLEEP_TMR	1000			//Sleepに入る前の時限[ms]
						//　操作の評価時間よりも長くすること
#ifndef	AWU_PR				//AWU周期が宣言されていないとき
#define	AWU_PR		KOYUU_PR		//固有処理のｺｰﾙﾊﾞｯｸ周期に合わせる
#endif


//==============================================
//操作のﾋﾞｯﾄ定義
//==============================================
#define	ORGEL_CNT	0x80		//ｵﾙｺﾞｰﾙ操作
#define	VOICEN_CNT	0x40		//音声N操作
#define	VOICEI_CNT	0x20		//音声I操作
#define	VOICEC_CNT	0x10		//音声C操作
#define	VOICES_CNT	0x08		//音声S操作
#define	NON_CNT		0x00		//操作無し


#ifdef SW_EN				//SWを実装するとき
//==============================================
//SW構成の定義
//==============================================
#define	SW_MODE	0x80			//ﾓｰﾄﾞ設定ﾏｽｸ
#define	SW_POL	0x40			//極性設定ﾏｽｸ
static const struct SW_CNF
{
	GPIO_TypeDef *gpio;		//GPIOｱﾄﾞﾚｽ
	unsigned short msk;		//ﾋﾞｯﾄﾏｽｸ
	unsigned char mode;		//b7:ﾓｰﾄﾞ(0=ﾘｱﾙﾓｰﾄﾞ､1=ﾀｲﾏｰﾓｰﾄﾞ)
					//b6:入力極性(0=負論理､1=正論理)
}sw_cnf[]=
{

//EXTICRﾚｼﾞｽﾀの仕様差を吸収---------------------
#if defined(DEV_F030)||defined(DEV_F103)||defined(DEV_F302)||defined(DEV_L010)
					//F030､F103､L010のとき
#define	EXTICR_BIT	4			//4ﾋﾞｯﾄ単位
#endif
#ifdef DEV_G03x				//G03xのとき
#define	EXTICR_BIT	8			//8ﾋﾞｯﾄ単位
#endif
#if defined(DEV_F030)||defined(DEV_F103)
					//F030､F103のとき
#define	EXTICR_REG	AFIO
#endif
#if defined(DEV_G03x)			//G03xのとき
#define	EXTICR_REG	EXTI
#endif
#if defined(DEV_L010)||defined(DEV_F302)
					//L010､F302のとき
#define	EXTICR_REG	SYSCFG
#endif

//SW0の構成-------------------------------------
#ifdef SW0_BIT				//SW0を実装するとき
#ifdef SW0_TIMER
#define SW0_MODE SW_MODE
#define SW_TIMER
#endif
#ifdef SW0_REAL
#define SW0_MODE 0x00
#define SW_REAL
#endif
#ifdef SW0_INV
#define SW0_POL 0x00
#else
#define SW0_POL SW_POL
#endif
	{SW0_GPIO,1<<SW0_BIT,SW0_MODE|SW0_POL},
#if SW0_BIT<4
#define SW0_EXTICR1	(SW0_EXTI_PORT<<SW0_BIT*EXTICR_BIT)
#define SW0_EXTICR2	0x00000000
#define SW0_EXTICR3	0x00000000
#define SW0_EXTICR4	0x00000000
#elif SW0_BIT<8
#define SW0_EXTICR1	0x00000000
#define SW0_EXTICR2	(SW0_EXTI_PORT<<(SW0_BIT-4)*EXTICR_BIT)
#define SW0_EXTICR3	0x00000000
#define SW0_EXTICR4	0x00000000
#elif SW0_BIT<12
#define SW0_EXTICR1	0x00000000
#define SW0_EXTICR2	0x00000000
#define SW0_EXTICR3	(SW0_EXTI_PORT<<(SW0_BIT-8)*EXTICR_BIT)
#define SW0_EXTICR4	0x00000000
#else
#define SW0_EXTICR1	0x00000000
#define SW0_EXTICR2	0x00000000
#define SW0_EXTICR3	0x00000000
#define SW0_EXTICR4	(SW0_EXTI_PORT<<(SW0_BIT-12)*EXTICR_BIT)
#endif
#define SW0_EXTIMSK	(1<<SW0_BIT)
#else					//SW0を実装しないとき
#define SW0_EXTICR1	0x00000000
#define SW0_EXTICR2	0x00000000
#define SW0_EXTICR3	0x00000000
#define SW0_EXTICR4	0x00000000
#define SW0_EXTIMSK	0x00000000
#endif

//SW1の構成-------------------------------------
#ifdef SW1_BIT				//SW1を実装するとき
#ifdef SW1_TIMER
#define SW1_MODE SW_MODE
#define SW_TIMER
#endif
#ifdef SW1_REAL
#define SW1_MODE 0x00
#define SW_REAL
#endif
#ifdef SW1_INV
#define SW1_POL 0x00
#else
#define SW1_POL SW_POL
#endif
	{SW1_GPIO,1<<SW1_BIT,SW1_MODE|SW1_POL},
#if SW1_BIT<4
#define SW1_EXTICR1	(SW1_EXTI_PORT<<SW1_BIT*EXTICR_BIT)
#define SW1_EXTICR2	0x00000000
#define SW1_EXTICR3	0x00000000
#define SW1_EXTICR4	0x00000000
#elif SW1_BIT<8
#define SW1_EXTICR1	0x00000000
#define SW1_EXTICR2	(SW1_EXTI_PORT<<(SW1_BIT-4)*EXTICR_BIT)
#define SW1_EXTICR3	0x00000000
#define SW1_EXTICR4	0x00000000
#elif SW1_BIT<12
#define SW1_EXTICR1	0x00000000
#define SW1_EXTICR2	0x00000000
#define SW1_EXTICR3	(SW1_EXTI_PORT<<(SW1_BIT-8)*EXTICR_BIT)
#define SW1_EXTICR4	0x00000000
#else
#define SW1_EXTICR1	0x00000000
#define SW1_EXTICR2	0x00000000
#define SW1_EXTICR3	0x00000000
#define SW1_EXTICR4	(SW1_EXTI_PORT<<(SW1_BIT-12)*EXTICR_BIT)
#endif
#define SW1_EXTIMSK	(1<<SW1_BIT)
#else					//SW1を実装しないとき
#define SW1_EXTICR1	0x00000000
#define SW1_EXTICR2	0x00000000
#define SW1_EXTICR3	0x00000000
#define SW1_EXTICR4	0x00000000
#define SW1_EXTIMSK	0x00000000
#endif

//SW2の構成-------------------------------------
#ifdef SW2_BIT				//SW2を実装するとき
#ifdef SW2_TIMER
#define SW2_MODE SW_MODE
#define SW_TIMER
#endif
#ifdef SW2_REAL
#define SW2_MODE 0x00
#define SW_REAL
#endif
#ifdef SW2_INV
#define SW2_POL 0x00
#else
#define SW2_POL SW_POL
#endif
	{SW2_GPIO,1<<SW2_BIT,SW2_MODE|SW2_POL},
#if SW2_BIT<4
#define SW2_EXTICR1	(SW2_EXTI_PORT<<SW2_BIT*EXTICR_BIT)
#define SW2_EXTICR2	0x00000000
#define SW2_EXTICR3	0x00000000
#define SW2_EXTICR4	0x00000000
#elif SW2_BIT<8
#define SW2_EXTICR1	0x00000000
#define SW2_EXTICR2	(SW2_EXTI_PORT<<(SW2_BIT-4)*EXTICR_BIT)
#define SW2_EXTICR3	0x00000000
#define SW2_EXTICR4	0x00000000
#elif SW2_BIT<12
#define SW2_EXTICR1	0x00000000
#define SW2_EXTICR2	0x00000000
#define SW2_EXTICR3	(SW2_EXTI_PORT<<(SW2_BIT-8)*EXTICR_BIT)
#define SW2_EXTICR4	0x00000000
#else
#define SW2_EXTICR1	0x00000000
#define SW2_EXTICR2	0x00000000
#define SW2_EXTICR3	0x00000000
#define SW2_EXTICR4	(SW2_EXTI_PORT<<(SW2_BIT-12)*EXTICR_BIT)
#endif
#define SW2_EXTIMSK	(1<<SW2_BIT)
#else					//SW2を実装しないとき
#define SW2_EXTICR1	0x00000000
#define SW2_EXTICR2	0x00000000
#define SW2_EXTICR3	0x00000000
#define SW2_EXTICR4	0x00000000
#define SW2_EXTIMSK	0x00000000
#endif

//SW3の構成-------------------------------------
#ifdef SW3_BIT				//SW3を実装するとき
#ifdef SW3_TIMER
#define SW3_MODE SW_MODE
#define SW_TIMER
#endif
#ifdef SW3_REAL
#define SW3_MODE 0x00
#define SW_REAL
#endif
#ifdef SW3_INV
#define SW3_POL 0x00
#else
#define SW3_POL SW_POL
#endif
	{SW3_GPIO,1<<SW3_BIT,SW3_MODE|SW3_POL},
#if SW3_BIT<4
#define SW3_EXTICR1	(SW3_EXTI_PORT<<SW3_BIT*EXTICR_BIT)
#define SW3_EXTICR2	0x00000000
#define SW3_EXTICR3	0x00000000
#define SW3_EXTICR4	0x00000000
#elif SW3_BIT<8
#define SW3_EXTICR1	0x00000000
#define SW3_EXTICR2	(SW3_EXTI_PORT<<(SW3_BIT-4)*EXTICR_BIT)
#define SW3_EXTICR3	0x00000000
#define SW3_EXTICR4	0x00000000
#elif SW3_BIT<12
#define SW3_EXTICR1	0x00000000
#define SW3_EXTICR2	0x00000000
#define SW3_EXTICR3	(SW3_EXTI_PORT<<(SW3_BIT-8)*EXTICR_BIT)
#define SW3_EXTICR4	0x00000000
#else
#define SW3_EXTICR1	0x00000000
#define SW3_EXTICR2	0x00000000
#define SW3_EXTICR3	0x00000000
#define SW3_EXTICR4	(SW3_EXTI_PORT<<(SW3_BIT-12)*EXTICR_BIT)
#endif
#define SW3_EXTIMSK	(1<<SW3_BIT)
#else					//SW3を実装しないとき
#define SW3_EXTICR1	0x00000000
#define SW3_EXTICR2	0x00000000
#define SW3_EXTICR3	0x00000000
#define SW3_EXTICR4	0x00000000
#define SW3_EXTIMSK	0x00000000
#endif

//SW4の構成-------------------------------------
#ifdef SW4_BIT				//SW4を実装するとき
#ifdef SW4_TIMER
#define SW4_MODE SW_MODE
#define SW_TIMER
#endif
#ifdef SW4_REAL
#define SW4_MODE 0x00
#define SW_REAL
#endif
#ifdef SW4_INV
#define SW4_POL 0x00
#else
#define SW4_POL SW_POL
#endif
	{SW4_GPIO,1<<SW4_BIT,SW4_MODE|SW4_POL},
#if SW4_BIT<4
#define SW4_EXTICR1	(SW4_EXTI_PORT<<SW4_BIT*EXTICR_BIT)
#define SW4_EXTICR2	0x00000000
#define SW4_EXTICR3	0x00000000
#define SW4_EXTICR4	0x00000000
#elif SW4_BIT<8
#define SW4_EXTICR1	0x00000000
#define SW4_EXTICR2	(SW4_EXTI_PORT<<(SW4_BIT-4)*EXTICR_BIT)
#define SW4_EXTICR3	0x00000000
#define SW4_EXTICR4	0x00000000
#elif SW4_BIT<12
#define SW4_EXTICR1	0x00000000
#define SW4_EXTICR2	0x00000000
#define SW4_EXTICR3	(SW4_EXTI_PORT<<(SW4_BIT-8)*EXTICR_BIT)
#define SW4_EXTICR4	0x00000000
#else
#define SW4_EXTICR1	0x00000000
#define SW4_EXTICR2	0x00000000
#define SW4_EXTICR3	0x00000000
#define SW4_EXTICR4	(SW4_EXTI_PORT<<(SW4_BIT-12)*EXTICR_BIT)
#endif
#define SW4_EXTIMSK	(1<<SW4_BIT)
#else					//SW4を実装しないとき
#define SW4_EXTICR1	0x00000000
#define SW4_EXTICR2	0x00000000
#define SW4_EXTICR3	0x00000000
#define SW4_EXTICR4	0x00000000
#define SW4_EXTIMSK	0x00000000
#endif

//SW5の構成-------------------------------------
#ifdef SW5_BIT				//SW5を実装するとき
#ifdef SW5_TIMER
#define SW5_MODE SW_MODE
#define SW_TIMER
#endif
#ifdef SW5_REAL
#define SW5_MODE 0x00
#define SW_REAL
#endif
#ifdef SW5_INV
#define SW5_POL 0x00
#else
#define SW5_POL SW_POL
#endif
	{SW5_GPIO,1<<SW5_BIT,SW5_MODE|SW5_POL},
#if SW5_BIT<4
#define SW5_EXTICR1	(SW5_EXTI_PORT<<SW5_BIT*EXTICR_BIT)
#define SW5_EXTICR2	0x00000000
#define SW5_EXTICR3	0x00000000
#define SW5_EXTICR4	0x00000000
#elif SW5_BIT<8
#define SW5_EXTICR1	0x00000000
#define SW5_EXTICR2	(SW5_EXTI_PORT<<(SW5_BIT-4)*EXTICR_BIT)
#define SW5_EXTICR3	0x00000000
#define SW5_EXTICR4	0x00000000
#elif SW5_BIT<12
#define SW5_EXTICR1	0x00000000
#define SW5_EXTICR2	0x00000000
#define SW5_EXTICR3	(SW5_EXTI_PORT<<(SW5_BIT-8)*EXTICR_BIT)
#define SW5_EXTICR4	0x00000000
#else
#define SW5_EXTICR1	0x00000000
#define SW5_EXTICR2	0x00000000
#define SW5_EXTICR3	0x00000000
#define SW5_EXTICR4	(SW5_EXTI_PORT<<(SW5_BIT-12)*EXTICR_BIT)
#endif
#define SW5_EXTIMSK	(1<<SW5_BIT)
#else					//SW5を実装しないとき
#define SW5_EXTICR1	0x00000000
#define SW5_EXTICR2	0x00000000
#define SW5_EXTICR3	0x00000000
#define SW5_EXTICR4	0x00000000
#define SW5_EXTIMSK	0x00000000
#endif

//SW6の構成-------------------------------------
#ifdef SW6_BIT				//SW6を実装するとき
#ifdef SW6_TIMER
#define SW6_MODE SW_MODE
#define SW_TIMER
#endif
#ifdef SW6_REAL
#define SW6_MODE 0x00
#define SW_REAL
#endif
#ifdef SW6_INV
#define SW6_POL 0x00
#else
#define SW6_POL SW_POL
#endif
	{SW6_GPIO,1<<SW6_BIT,SW6_MODE|SW6_POL},
#if SW6_BIT<4
#define SW6_EXTICR1	(SW6_EXTI_PORT<<SW6_BIT*EXTICR_BIT)
#define SW6_EXTICR2	0x00000000
#define SW6_EXTICR3	0x00000000
#define SW6_EXTICR4	0x00000000
#elif SW6_BIT<8
#define SW6_EXTICR1	0x00000000
#define SW6_EXTICR2	(SW6_EXTI_PORT<<(SW6_BIT-4)*EXTICR_BIT)
#define SW6_EXTICR3	0x00000000
#define SW6_EXTICR4	0x00000000
#elif SW6_BIT<12
#define SW6_EXTICR1	0x00000000
#define SW6_EXTICR2	0x00000000
#define SW6_EXTICR3	(SW6_EXTI_PORT<<(SW6_BIT-8)*EXTICR_BIT)
#define SW6_EXTICR4	0x00000000
#else
#define SW6_EXTICR1	0x00000000
#define SW6_EXTICR2	0x00000000
#define SW6_EXTICR3	0x00000000
#define SW6_EXTICR4	(SW6_EXTI_PORT<<(SW6_BIT-12)*EXTICR_BIT)
#endif
#define SW6_EXTIMSK	(1<<SW6_BIT)
#else					//SW6を実装しないとき
#define SW6_EXTICR1	0x00000000
#define SW6_EXTICR2	0x00000000
#define SW6_EXTICR3	0x00000000
#define SW6_EXTICR4	0x00000000
#define SW6_EXTIMSK	0x00000000
#endif

//SW7の構成-------------------------------------
#ifdef SW7_BIT				//SW7を実装するとき
#ifdef SW7_TIMER
#define SW7_MODE SW_MODE
#define SW_TIMER
#endif
#ifdef SW7_REAL
#define SW7_MODE 0x00
#define SW_REAL
#endif
#ifdef SW7_INV
#define SW7_POL 0x00
#else
#define SW7_POL SW_POL
#endif
	{SW7_GPIO,1<<SW7_BIT,SW7_MODE|SW7_POL},
#if SW7_BIT<4
#define SW7_EXTICR1	(SW7_EXTI_PORT<<SW7_BIT*EXTICR_BIT)
#define SW7_EXTICR2	0x00000000
#define SW7_EXTICR3	0x00000000
#define SW7_EXTICR4	0x00000000
#elif SW7_BIT<8
#define SW7_EXTICR1	0x00000000
#define SW7_EXTICR2	(SW7_EXTI_PORT<<(SW7_BIT-4)*EXTICR_BIT)
#define SW7_EXTICR3	0x00000000
#define SW7_EXTICR4	0x00000000
#elif SW7_BIT<12
#define SW7_EXTICR1	0x00000000
#define SW7_EXTICR2	0x00000000
#define SW7_EXTICR3	(SW7_EXTI_PORT<<(SW7_BIT-8)*EXTICR_BIT)
#define SW7_EXTICR4	0x00000000
#else
#define SW7_EXTICR1	0x00000000
#define SW7_EXTICR2	0x00000000
#define SW7_EXTICR3	0x00000000
#define SW7_EXTICR4	(SW7_EXTI_PORT<<(SW7_BIT-12)*EXTICR_BIT)
#endif
#define SW7_EXTIMSK	(1<<SW7_BIT)
#else					//SW7を実装しないとき
#define SW7_EXTICR1	0x00000000
#define SW7_EXTICR2	0x00000000
#define SW7_EXTICR3	0x00000000
#define SW7_EXTICR4	0x00000000
#define SW7_EXTIMSK	0x00000000
#endif
};

//EXTICR設定値
#define SW_EXTICR1	(SW0_EXTICR1|SW1_EXTICR1|SW2_EXTICR1|SW3_EXTICR1|\
			SW4_EXTICR1|SW5_EXTICR1|SW6_EXTICR1|SW7_EXTICR1)
#define SW_EXTICR2	(SW0_EXTICR2|SW1_EXTICR2|SW2_EXTICR2|SW3_EXTICR2|\
			SW4_EXTICR2|SW5_EXTICR2|SW6_EXTICR2|SW7_EXTICR2)
#define SW_EXTICR3	(SW0_EXTICR3|SW1_EXTICR3|SW2_EXTICR3|SW3_EXTICR3|\
			SW4_EXTICR3|SW5_EXTICR3|SW6_EXTICR1|SW7_EXTICR3)
#define SW_EXTICR4	(SW0_EXTICR4|SW1_EXTICR4|SW2_EXTICR4|SW3_EXTICR4|\
			SW4_EXTICR4|SW5_EXTICR4|SW6_EXTICR4|SW7_EXTICR4)
#define SW_EXTIMSK	(SW0_EXTIMSK|SW1_EXTIMSK|SW2_EXTIMSK|SW3_EXTIMSK|\
			SW4_EXTIMSK|SW5_EXTIMSK|SW6_EXTIMSK|SW7_EXTIMSK)

//SWの実装数
#define	SW_SU	(sizeof(sw_cnf)/sizeof(sw_cnf[0]))


//==============================================
//SW操作の定義
//==============================================
static const unsigned char sw_cnt[]=		//ｱﾌﾟﾘ制御情報
{

//SW0の操作-------------------------------------
#ifdef SW0_BIT
	SW0_CNT,
#endif

//SW1の操作-------------------------------------
#ifdef SW1_BIT
	SW1_CNT,
#endif

//SW2の操作-------------------------------------
#ifdef SW2_BIT
	SW2_CNT,
#endif

//SW3の操作-------------------------------------
#ifdef SW3_BIT
	SW3_CNT,
#endif

//SW4の操作-------------------------------------
#ifdef SW4_BIT
	SW4_CNT,
#endif

//SW5の操作-------------------------------------
#ifdef SW5_BIT
	SW5_CNT,
#endif

//SW6の操作-------------------------------------
#ifdef SW6_BIT
	SW6_CNT,
#endif

//SW7の操作-------------------------------------
#ifdef SW7_BIT
	SW7_CNT,
#endif
};

#include "func_SW.c"				//SW評価ﾓｼﾞｭｰﾙを呼込む
#endif


#ifdef CdS_EN				//CdSを実装するとき
//==============================================
//CdS構成の定義
//==============================================
#include "func_CdS.c"				//CdS評価ﾓｼﾞｭｰﾙを呼込む
#endif


#ifdef CVD_EN				//CVDを実装するとき
//==============================================
//CVD構成の定義
//==============================================
#define	CVD_MODE	0x80			//ﾓｰﾄﾞ設定ﾏｽｸ
static const struct CVD_CNF
{
	GPIO_TypeDef *gpio;			//GPIOｱﾄﾞﾚｽ
	unsigned short chsel;			//CHSELﾏｽｸ/ANIN番号
	unsigned char mode;			//b7:ﾓｰﾄﾞ(0=ﾘｱﾙﾓｰﾄﾞ､1=ﾀｲﾏｰﾓｰﾄﾞ)
						//b3-b0:ﾋﾞｯﾄ番号
}cvd_cnf[]=
{

//CVD0の構成-------------------------------------
#ifdef CVD0_BIT
#ifdef CVD0_TIMER
#define CVD0_MODE CVD_MODE
#define CVD_TIMER
#endif
#ifdef CVD0_REAL
#define CVD0_MODE 0x00
#define CVD_REAL
#endif
	{CVD0_GPIO,CVD0_CHSEL,CVD0_MODE|CVD0_BIT},
#endif

//CVD1の構成-------------------------------------
#ifdef CVD1_BIT
#ifdef CVD1_TIMER
#define CVD1_MODE CVD_MODE
#define CVD_TIMER
#endif
#ifdef CVD1_REAL
#define CVD1_MODE 0x00
#define CVD_REAL
#endif
	{CVD1_GPIO,CVD1_CHSEL,CVD1_MODE|CVD1_BIT},
#endif

//CVD2の構成-------------------------------------
#ifdef CVD2_BIT
#ifdef CVD2_TIMER
#define CVD2_MODE CVD_MODE
#define CVD_TIMER
#endif
#ifdef CVD2_REAL
#define CVD2_MODE 0x00
#define CVD_REAL
#endif
	{CVD2_GPIO,CVD2_CHSEL,CVD2_MODE|CVD2_BIT},
#endif

//CVD3の構成-------------------------------------
#ifdef CVD3_BIT
#ifdef CVD3_TIMER
#define CVD3_MODE CVD_MODE
#define CVD_TIMER
#endif
#ifdef CVD3_REAL
#define CVD3_MODE 0x00
#define CVD_REAL
#endif
	{CVD3_GPIO,CVD3_CHSEL,CVD3_MODE|CVD3_BIT},
#endif

//CVD4の構成-------------------------------------
#ifdef CVD4_BIT
#ifdef CVD4_TIMER
#define CVD4_MODE CVD_MODE
#define CVD_TIMER
#endif
#ifdef CVD4_REAL
#define CVD4_MODE 0x00
#define CVD_REAL
#endif
	{CVD4_GPIO,CVD4_CHSEL,CVD4_MODE|CVD4_BIT},
#endif

//CVD5の構成-------------------------------------
#ifdef CVD5_BIT
#ifdef CVD5_TIMER
#define CVD5_MODE CVD_MODE
#define CVD_TIMER
#endif
#ifdef CVD5_REAL
#define CVD5_MODE 0x00
#define CVD_REAL
#endif
	{CVD5_GPIO,CVD5_CHSEL,CVD5_MODE|CVD5_BIT},
#endif
};

#define	CVD_SU	(sizeof(cvd_cnf)/sizeof(cvd_cnf[0]))
						//CVDの実装数


//==============================================
//CVD操作の定義
//==============================================
static const unsigned char cvd_cnt[]=		//ｱﾌﾟﾘ制御情報
{

//CVD0の操作-------------------------------------
#ifdef CVD0_BIT
	CVD0_CNT,
#endif

//CVD1の操作-------------------------------------
#ifdef CVD1_BIT
	CVD1_CNT,
#endif

//CVD2の操作-------------------------------------
#ifdef CVD2_BIT
	CVD2_CNT,
#endif

//CVD3の操作-------------------------------------
#ifdef CVD3_BIT
	CVD3_CNT,
#endif

//CVD4の操作-------------------------------------
#ifdef CVD4_BIT
	CVD4_CNT,
#endif

//CVD5の操作-------------------------------------
#ifdef CVD5_BIT
	CVD5_CNT,
#endif
};

#include "func_CVD.c"				//CVD評価ﾓｼﾞｭｰﾙを呼込む
#endif


//==============================================
//固有の作業域
//==============================================
#if defined(KYOKU_RAND)||defined(ONSEIN_RAND)||defined(ONSEII_RAND)||defined(ONSEIC_RAND)||defined(ONSEIS_RAND)	//どれかでﾗﾝﾀﾞﾑが必要なとき
static unsigned char rand_x;			//ﾗﾝﾀﾞﾏｲｽﾞの種
#endif
#if PART_N>0
static unsigned char kyoku;
#ifdef KYOKU_RAND
static unsigned char kyoku_rand_msk;
#endif
#endif
#if VOICE_N>0
static unsigned char onseiN;
#ifdef ONSEIN_RAND
static unsigned char onseiN_rand_msk;
#endif
#endif
#if VOICE_I>0
static unsigned char onseiI;
#ifdef ONSEII_RAND
static unsigned char onseiI_rand_msk;
#endif
#endif
#if VOICE_C>0
static unsigned char onseiC;
#ifdef ONSEIC_RAND
static unsigned char onseiC_rand_msk;
#endif
#endif
#if VOICE_S>0
static unsigned char onseiS;
#ifdef ONSEIS_RAND
static unsigned char onseiS_rand_msk;
#endif
#endif


#if defined(KYOKU_RAND)||defined(ONSEIN_RAND)||defined(ONSEII_RAND)||defined(ONSEIC_RAND)||defined(ONSEIS_RAND)	//どれかでﾗﾝﾀﾞﾑが必要なとき
//==============================================
//ﾗﾝﾀﾞﾏｲｽﾞのﾋﾞｯﾄﾏｽｸﾊﾟﾀｰﾝを計算する
//==============================================
static unsigned char rand_msk(
	unsigned char no)			//実装数
{
//DBG_C('\n');
//DBG_C('p');
//DBG_B(no);
	if(no>127) return(0xff);
	if(no>63) return(0x7f);
	if(no>31) return(0x3f);
	if(no>15) return(0x1f);
	if(no>7) return(0x0f);
	if(no>3) return(0x07);
	return(0x03);
}


//==============================================
//ﾗﾝﾀﾞﾏｲｽﾞ
//==============================================
static unsigned char rand(void)
{

//DBG_C('r');
	rand_x=(rand_x<<2)+rand_x+1;
//DBG_B(rand_x);
	return(rand_x);
}
#endif


//==============================================
//ｱﾌﾟﾘの初期設定
//==============================================
static void apl_init(void)
{

//DBG_C('\n');
//DBG_C('$');

	//ﾗﾝﾀﾞﾏｲｽﾞの種
#if defined(KYOKU_RAND)||defined(ONSEIN_RAND)||defined(ONSEII_RAND)||defined(ONSEIC_RAND)||defined(ONSEIS_RAND)	//どれかでﾗﾝﾀﾞﾑが必要なとき
 //DBG_C('r');
#if defined(DEV_F030)||defined(DEV_L010)//F030､L010のとき
	ADC1->SMPR|=ADC_SMPR_SMP_Msk;		//ｻﾝﾌﾟﾘﾝｸﾞ時間は最大値
						//　(239.5ADCｸﾛｯｸｻｲｸﾙ=17us)
	ADC1->CHSELR=(uint32_t)RAND_CHSEL;	//変換chを設定する
	ADC1->CR|=ADC_CR_ADEN;			//ADC1有効
	while(!(ADC1->ISR&ADC_ISR_ADRDY)){};	//ADC1有効の完了を待つ
	ADC1->CR|=ADC_CR_ADSTART;		//変換開始
	while(!(ADC1->ISR&ADC_ISR_EOC));	//変換完了を待つ
	rand_x=ADC1->DR;			//変換結果の下位8ﾋﾞｯﾄを種にする
	ADC1->CR|=ADC_CR_ADDIS;			//ADC1を無効
	while(ADC1->CR&ADC_CR_ADEN){};		//ADC1の無効を待つ
#endif
#ifdef DEV_F103				//F103のとき
	ADC1->SMPR1=0x00000007;			//ｻﾝﾌﾟﾘﾝｸﾞ時間は最大値
						//　(239.5ADCｸﾛｯｸｻｲｸﾙ=22us)
	ADC1->SQR1=RAND_CHSEL;			//変換chを設定する
	ADC1->CR2|=ADC_CR2_ADON;		//ADC1有効
	wait_1us();				//起動時間待ち
	ADC1->CR2|=ADC_CR2_ADON;		//変換開始
	ADC1->CR2|=ADC_CR2_ADON;		//変換開始
	while(!(ADC1->SR&ADC_SR_EOC));		//変換完了を待つ
	ADC1->CR2&=~ADC_CR2_ADON;		//ADC1無効
	wait_1us();				//無効時間待ち
#endif
#if defined(DEV_F302)			//F302のとき
	ADC1->SMPR1|=0x00000038;		//ｻﾝﾌﾟﾘﾝｸﾞ時間は最大値
						//　(601.5ADCｸﾛｯｸｻｲｸﾙ=55us)
	ADC1->SQR1=(uint32_t)RAND_CHSEL;	//変換chを設定する
	ADC1->CR|=ADC_CR_ADEN;			//ADC1有効
	while(!(ADC1->ISR&ADC_ISR_ADRDY)){};	//ADC1有効の完了を待つ
	ADC1->CR|=ADC_CR_ADSTART;		//変換開始
	while(!(ADC1->ISR&ADC_ISR_EOC));	//変換完了を待つ
	rand_x=ADC1->DR;			//変換結果の下位8ﾋﾞｯﾄを種にする
	ADC1->CR|=ADC_CR_ADDIS;			//ADC1無効
	while(ADC1->CR&ADC_CR_ADEN){};		//ADC1無効を待つ
#endif
#ifdef DEV_G03x				//G03xのとき
	ADC1->SMPR|=ADC_SMPR_SMP1_Msk;		//ｻﾝﾌﾟﾘﾝｸﾞ時間は最大値
						//　(160.5ADCｸﾛｯｸｻｲｸﾙ=17us)
	ADC1->CHSELR=(uint32_t)RAND_CHSEL;	//変換chを設定する
	ADC1->CR|=ADC_CR_ADEN;			//ADC1有効
	while(!(ADC1->ISR&ADC_ISR_ADRDY)){};	//ADC1有効の完了を待つ
	ADC1->CR|=ADC_CR_ADSTART;		//変換開始
	while(!(ADC1->ISR&ADC_ISR_EOC));	//変換完了を待つ
	ADC1->CR|=ADC_CR_ADDIS;			//ADC1を無効
	while(ADC1->CR&ADC_CR_ADEN){};		//ADC1の無効を待つ
#endif
	rand_x=ADC1->DR;			//変換結果の下位8ﾋﾞｯﾄを種にする
#endif

	//ﾗﾝﾀﾞﾏｲｽﾞのﾋﾞｯﾄﾏｽｸﾊﾟﾀｰﾝ設定
#if (PART_N>0)&&defined(KYOKU_RAND)
	kyoku_rand_msk=rand_msk(SONG_IDX.no);
//DBG_B(kyoku_rand_msk);
#endif
#if (VOICE_N>0)&&defined(ONSEIN_RAND)
	onseiN_rand_msk=rand_msk(VOICEN_SU);
//DBG_B(onseiN_rand_msk);
#endif
#if (VOICE_I>0)&&defined(ONSEII_RAND)
	onseiI_rand_msk=rand_msk(VOICEI_SU);
//DBG_B(onseiI_rand_msk);
#endif
#if (VOICE_C>0)&&defined(ONSEIC_RAND)
	onseiC_rand_msk=rand_msk(VOICEC_SU);
//DBG_B(onseiC_rand_msk);
#endif
#if (VOICE_S>0)&&defined(ONSEIS_RAND)
	onseiS_rand_msk=rand_msk(VOICES_SU);
//DBG_B(onseiS_rand_msk);
#endif

	//LEDﾎﾟｰﾄ設定
#ifdef LED0_BIT				//LED0を実装するとき
	GPIO_OUT(LED0_GPIO,LED0_BIT);		//出力ﾓｰﾄﾞ
#endif
#ifdef LED1_BIT				//LED1を実装するとき
	GPIO_OUT(LED1_GPIO,LED1_BIT);		//出力ﾓｰﾄﾞ
#endif
#ifdef LED2_BIT				//LED2を実装するとき
	GPIO_OUT(LED2_GPIO,LED2_BIT);		//出力ﾓｰﾄﾞ
#endif
#ifdef LED3_BIT				//LED3を実装するとき
	GPIO_OUT(LED3_GPIO,LED3_BIT);		//出力ﾓｰﾄﾞ
#endif
#ifdef LED4_BIT				//LED4を実装するとき
	GPIO_OUT(LED4_GPIO,LED5_BIT);		//出力ﾓｰﾄﾞ
#endif
#ifdef LED5_BIT				//LED5を実装するとき
	GPIO_OUT(LED5_GPIO,LED5_BIT);		//出力ﾓｰﾄﾞ
#endif
#ifdef LED6_BIT				//LED6を実装するとき
	GPIO_OUT(LED6_GPIO,LED6_BIT);		//出力ﾓｰﾄﾞ
#endif

	//SWﾎﾟｰﾄ設定
//DBG_C('p');
#ifdef SW_EN				//SWを実装するとき
#ifdef SW0_BIT				//SW0を実装するとき
#ifdef SW0_INV				//負論理のとき
	GPIO_INPU(SW0_GPIO,SW0_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする
#else					//正論理のとき
	GPIO_INPD(SW0_GPIO,SW0_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙﾀﾞｳﾝする
#endif
#endif

#ifdef SW1_BIT				//SW1を実装するとき
#ifdef SW1_INV				//負論理のとき
	GPIO_INPU(SW1_GPIO,SW1_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする
#else					//正論理のとき
	GPIO_INPD(SW1_GPIO,SW1_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙﾀﾞｳﾝする
#endif
#endif

#ifdef SW2_BIT				//SW2を実装するとき
#ifdef SW2_INV				//負論理のとき
	GPIO_INPU(SW2_GPIO,SW2_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする
#else					//正論理のとき
	GPIO_INPD(SW2_GPIO,SW2_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙﾀﾞｳﾝする
#endif
#endif

#ifdef SW3_BIT				//SW3を実装するとき
#ifdef SW3_INV				//負論理のとき
	GPIO_INPU(SW3_GPIO,SW3_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする
#else					//正論理のとき
	GPIO_INPD(SW3_GPIO,SW3_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙﾀﾞｳﾝする
#endif
#endif

#ifdef SW4_BIT				//SW4を実装するとき
#ifdef SW4_INV				//負論理のとき
	GPIO_INPU(SW4_GPIO,SW4_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする
#else					//正論理のとき
	GPIO_INPD(SW4_GPIO,SW4_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙﾀﾞｳﾝする
#endif
#endif

#ifdef SW5_BIT				//SW5を実装するとき
#ifdef SW5_INV				//負論理のとき
	GPIO_INPU(SW5_GPIO,SW5_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする
#else					//正論理のとき
	GPIO_INPD(SW5_GPIO,SW5_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙﾀﾞｳﾝする
#endif
#endif

#ifdef SW6_BIT				//SW6を実装するとき
#ifdef SW6_INV				//負論理のとき
	GPIO_INPU(SW6_GPIO,SW6_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする
#else					//正論理のとき
	GPIO_INPD(SW6_GPIO,SW6_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙﾀﾞｳﾝする
#endif
#endif

#ifdef SW7_BIT				//SW7を実装するとき
#ifdef SW7_INV				//負論理のとき
	GPIO_INPU(SW7_GPIO,SW7_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする
#else					//正論理のとき
	GPIO_INPD(SW7_GPIO,SW7_BIT);		//入力ﾓｰﾄﾞﾌﾟﾙﾀﾞｳﾝする
#endif
#endif
#endif

	//CVDﾎﾟｰﾄ設定
#ifdef CVD_EN				//CVDを実装するとき
#ifdef CVD0_BIT				//CVD0を実装するとき
#ifndef CVD_EXPD			//CVDを外部ﾌﾟﾙﾀﾞｳﾝしないとき
	GPIO_INPD(CVD0_GPIO,CVD0_BIT);	//内部ﾌﾟﾙﾀﾞｳﾝする
#endif
#endif
#ifdef CVD1_BIT				//CVD1を実装するとき
#ifndef CVD_EXPD			//CVDを外部ﾌﾟﾙﾀﾞｳﾝしないとき
	GPIO_INPD(CVD1_GPIO,CVD1_BIT);	//内部ﾌﾟﾙﾀﾞｳﾝする
#endif
#endif
#ifdef CVD2_BIT				//CVD2を実装するとき
#ifndef CVD_EXPD			//CVDを外部ﾌﾟﾙﾀﾞｳﾝしないとき
	GPIO_INPD(CVD2_GPIO,CVD2_BIT);	//内部ﾌﾟﾙﾀﾞｳﾝする
#endif
#endif
#ifdef CVD3_BIT				//CVD0を実装するとき
#ifndef CVD_EXPD			//CVDを外部ﾌﾟﾙﾀﾞｳﾝしないとき
	GPIO_INPD(CVD3_GPIO,CVD3_BIT);	//内部ﾌﾟﾙﾀﾞｳﾝする
#endif
#endif
#endif

#ifdef SLEEP_EN				//Sleepを実装するとき
	//Wakeup設定
#if defined(CdS_EN)||defined(CVD_EN)||defined(SW_PLNG)
					//CdSかCVDを実装するとき､
					//　SWをﾎﾟｰﾘﾝｸﾞするときは
					//　EXTI17(ｱﾗｰﾑA)でWakeupする
//
//DBG_C('a');
#if defined(DEV_F030)||defined(DEV_F302)||defined(DEV_L010)
					//F030､F302､L010のとき(ここから)
	//RTC設定
	RCC->APB1ENR|=RCC_APB1ENR_PWREN;	//電源ｲﾝﾀﾌｪｰｽにｸﾛｯｸ供給
	RCC->CSR|=RCC_CSR_LSION;		//LSI有効
	while(!(RCC->CSR&RCC_CSR_LSIRDY));	//LSIﾚﾃﾞｨを待つ
	PWR->CR|=PWR_CR_DBP;			//RTCｱｸｾｽ許可
#if defined(DEV_F030)||defined(DEV_F302)//F030､F302では
	RCC->BDCR|=RCC_BDCR_RTCSEL_LSI;		//RTCｸﾛｯｸｿｰｽはLSI
	RCC->BDCR|=RCC_BDCR_RTCEN;		//RTCにｸﾛｯｸ供給
#endif
#ifdef DEV_L010				//L010では
	RCC->CSR|=RCC_CSR_RTCEN|RCC_CSR_RTCSEL_1;
						//RTC有効､ｸﾛｯｸｿｰｽはLSI
#endif

	//ｶﾚﾝﾀﾞｰ設定
//DBG_C('b');
	RTC->WPR=0xca;				//書込み保護を
	RTC->WPR=0x53;				//　解除する
	RTC->ISR|=RTC_ISR_INIT;			//初期化ﾓｰﾄﾞ開始
	while(!(RTC->ISR&RTC_ISR_INITF)){};	//初期化ﾓｰﾄﾞ開始を待つ
//DBG_C('c');
	RTC->PRER=0x00240000+AWU_PR-1;		//ﾌﾟﾘｽｹ(37/37kHz*(AWU_PR-1)[ms])
	RTC->TR=0x00000000;			//時刻
	RTC->DR=0x00000000;			//日付
	RTC->ISR&=~RTC_ISR_INIT;		//初期化ﾓｰﾄﾞ終了

	//ｱﾗｰﾑA設定
//DBG_C('d');
	RTC->ALRMAR=RTC_ALRMAR_MSK4|RTC_ALRMAR_MSK3|
		RTC_ALRMAR_MSK2|RTC_ALRMAR_MSK1;//ｶｳﾝﾀ更新毎にｱﾗｰﾑA

//DBG_C('e');
	//EXTI17設定(RTCｱﾗｰﾑ)
#if defined(DEV_F302)			//F302では
//	EXTI->EMR|=EXTI_EMR_MR17;		//EXTI17をｲﾍﾞﾝﾄ有効
#endif
	EXTI->RTSR|=EXTI_RTSR_TR17;		//EXTI17立上りﾄﾘｶﾞ許可
#if defined(DEV_F030)||defined(DEV_L010)//F030､L010では
	NVIC->ISER[0]=(1<<RTC_IRQn);		//RTC(throughEXTI)割込み許可
#endif
#endif					//F030､F302､L010のとき(ここまで)

#if defined(DEV_F103)			//F103のとき(ここから)
	//RTC設定
	RCC->CSR|=RCC_CSR_LSION;		//LSI有効
	while(!(RCC->CSR&RCC_CSR_LSIRDY)){};	//LSIﾚﾃﾞｨを待つ
//DBG_C('f');
	RCC->APB1ENR|=RCC_APB1ENR_PWREN|RCC_APB1ENR_BKPEN;
						//電源ｲﾝﾀﾌｪｰｽにｸﾛｯｸ供給､BKP許可
	PWR->CR|=PWR_CR_DBP;			//RTCｱｸｾｽ許可
	RCC->BDCR|=RCC_BDCR_RTCSEL_LSI;		//RTCｸﾛｯｸｿｰｽはLSI
	RCC->BDCR|=RCC_BDCR_RTCEN;		//RTCにｸﾛｯｸ供給
//DBG_C('g');

	//RTCﾚｼﾞｽﾀ設定
	while(!(RTC->CRL&RTC_CRL_RTOFF)){};	//RTCﾚｼﾞｽﾀの更新完了を待つ
	RTC->CRL|=RTC_CRL_CNF;			//設定ﾓｰﾄﾞ開始
	RTC->PRLH=0;				//周期を1ms
	RTC->PRLL=39;				//　(PRL+1)/LSI=40/40kHz=1ms
	RTC->ALRH=0;				//AWU周期(ｱﾗｰﾑ)を設定
	RTC->ALRL=AWU_PR-1;			//(AWU_PR-1)[ms]
	RTC->CRH|=RTC_CRH_ALRIE;		//ｱﾗｰﾑ割込み有効
	RTC->CRL&=~RTC_CRL_CNF;			//設定ﾓｰﾄﾞ終了
	while(!(RTC->CRL&RTC_CRL_RTOFF)){};	//RTCﾚｼﾞｽﾀの更新完了を待つ
//DBG_C('h');

	//EXTI17ｲﾍﾞﾝﾄ設定(RTCｱﾗｰﾑ)
	EXTI->EMR|=EXTI_EMR_MR17;		//EXTI17をｲﾍﾞﾝﾄ有効
	EXTI->RTSR|=EXTI_RTSR_TR17;		//EXTI17立上りﾄﾘｶﾞ有効
//DBG_C('i');
#endif					//F103のとき(ここまで)

#if defined(DEV_G03x)			//G03xのとき(ここから)
//DBG_C('j');
	RCC->CSR|=RCC_CSR_LSION;		//LSI有効
	while(!(RCC->CSR&RCC_CSR_LSIRDY)){};	//LSIﾚﾃﾞｨを待つ
	RCC->APBENR1|=RCC_APBENR1_PWREN;	//電源ｲﾝﾀﾌｪｰｽにｸﾛｯｸ供給
	RCC->APBENR1|=RCC_APBENR1_RTCAPBEN;	//RTCAPBにｸﾛｯｸ供給
	PWR->CR1|=PWR_CR1_DBP;			//RTCｱｸｾｽ許可
	RCC->BDCR|=RCC_BDCR_RTCSEL_1;		//RTCｸﾛｯｸｿｰｽはLSI
	RCC->BDCR|=RCC_BDCR_RTCEN;		//RTCにｸﾛｯｸ供給
//DBG_C('k');

	//WUT設定
	RTC->WPR=0xca;				//書込み保護を
	RTC->WPR=0x53;				//　解除する
//DBG_C('l');
	RTC->CR&=~RTC_CR_WUTE;			//WUT機能を無効
	while(!(RTC->ICSR&RTC_ICSR_WUTWF)){};	//無効完了を待つ
//DBG_C('m');
	RTC->WUTR=AWU_PR*2-1;			//(AWU_PR-1)[ms]
	RTC->CR&=~RTC_CR_WUCKSEL_Msk;		//WUTｸﾛｯｸはLSI/16(=2kHz)
	RTC->CR|=RTC_CR_WUTIE;			//WUT割込みを有効
//DBG_C('n');
//DBG_SYNC();

	//EXTI19割込み設定(WUT)
	NVIC->ISER[0]=(1<<RTC_TAMP_IRQn);	//RTC(throughEXTI)割込み許可
//DBG_C('o');
#endif					//G03xのとき(ここまで)

#else					//CdSもCVDも実装しないとき､
					//　SWをﾎﾟｰﾘﾝｸﾞしないときは
					//　EXTI0～15(SW)割込みでWakeupする
//DBG_C('p');
#ifdef SW_EN				//SWを実装するとき

	//EXTI設定
#ifdef DEV_L010			//L010のとき
	RCC->APB2ENR|=RCC_APB2ENR_SYSCFGEN;	//SYSCFGにｸﾛｯｸ供給
#endif
#if SW_EXTICR1!=0x00000000
	EXTICR_REG->EXTICR[0]=SW_EXTICR1;
#endif
#if SW_EXTICR2!=0x00000000
	EXTICR_REG->EXTICR[1]=SW_EXTICR2;
#endif
#if SW_EXTICR3!=0x00000000
	EXTICR_REG->EXTICR[2]=SW_EXTICR3;
#endif
#if SW_EXTICR4!=0x00000000
	EXTICR_REG->EXTICR[3]=SW_EXTICR4;
#endif
#ifdef DEV_L010			//L010のとき
	RCC->APB2ENR&=~RCC_APB2ENR_SYSCFGEN;	//SYSCFGのｸﾛｯｸ停止
#endif
#if defined(DEV_F030)||defined(DEV_F103)||defined(DEV_F302)||defined(DEV_L010)
					//F030､F103､F302､L010のとき
	EXTI->FTSR|=SW_EXTIMSK;			//立下りﾄﾘｶﾞ許可
	EXTI->RTSR|=SW_EXTIMSK;			//立上りﾄﾘｶﾞ許可
#endif
#if defined(DEV_G03x)			//G03xのとき
	EXTI->FTSR1|=SW_EXTIMSK;		//立下りﾄﾘｶﾞ許可
	EXTI->RTSR1|=SW_EXTIMSK;		//立上りﾄﾘｶﾞ許可
//DBG_C('q');
#endif
#if defined(DEV_F030)||defined(DEV_L010)||defined(DEV_G03x)
					//F030､L010､G03xのとき
	NVIC->ISER[0]|=
		(1<<EXTI0_1_IRQn)|		//EXTI0_1割込み許可
		(1<<EXTI2_3_IRQn)|		//EXTI2_3割込み許可
		(1<<EXTI4_15_IRQn);		//EXTI4_15割込み許可
#endif
#if defined(DEV_F103)||defined(DEV_F302)//F103､F302のとき
	EXTI->EMR|=				//ﾗｲﾝ0～15をｲﾍﾞﾝﾄ許可
		EXTI_EMR_MR0|EXTI_EMR_MR1|
		EXTI_EMR_MR2|EXTI_EMR_MR3|
		EXTI_EMR_MR4|EXTI_EMR_MR5|
		EXTI_EMR_MR6|EXTI_EMR_MR7|
		EXTI_EMR_MR8|EXTI_EMR_MR9|
		EXTI_EMR_MR10|EXTI_EMR_MR11|
		EXTI_EMR_MR12|EXTI_EMR_MR13|
		EXTI_EMR_MR14|EXTI_EMR_MR15;
//DBG_C('r');
#endif
#endif
#endif
#endif
}


#ifdef	SLEEP_EN			//Sleep機能を実装するとき
#if defined(CdS_EN)||defined(CVD_EN)||defined(SW_PLNG)
					//CdSかCVDを実装するとき､
					//　SWをﾎﾟｰﾘﾝｸﾞするとき
#if defined(DEV_F030)||defined(DEV_L010)//F030､L010のとき(ここから)
//==============================================
//RTC(throughEXTI)割込みﾊﾝﾄﾞﾗ
//==============================================
void RTC_IRQHandler(void)
{

//DBS_H;
	EXTI->PR=0x00020000;			//EXTI17割込みﾌﾗｸﾞをｸﾘｱする
	RTC->ISR&=~RTC_ISR_ALRAF;		//ｱﾗｰﾑAﾌﾗｸﾞをｸﾘｱする
//DBS_L;
}
#endif					//F030､L010のとき(ここまで)


#ifdef DEV_G03x				//G03xのとき(ここから)
//==============================================
//RTC(throughEXTI)割込みﾊﾝﾄﾞﾗ
//==============================================
void RTC_TAMP_IRQHandler(void)
{

//DBS_H;
	RTC->SCR=RTC_SCR_CWUTF;			//WUTﾌﾗｸﾞをｸﾘｱする
//DBS_L;
}
#endif					//G03xのとき(ここまで)

#else					//CdSもCVDも実装しないとき､
					//　SWをﾎﾟｰﾘﾝｸﾞしないとき
#if defined(DEV_F030)||defined(DEV_L010)||defined(DEV_G03x)
					//F030､L010､G03xのとき
//==============================================
//EXTI割込みﾊﾝﾄﾞﾗ
//==============================================
void EXTI0_1_IRQHandler(void)
{

#if defined(DEV_F030)||defined(DEV_L010)//F030､L010のとき
	EXTI->PR=0x00000003;			//ﾍﾟﾝﾃﾞｨﾝｸﾞをｸﾘｱする
#endif
#ifdef DEV_G03x				//G03xのとき
	EXTI->RPR1=0x00000003;			//立上りﾍﾟﾝﾃﾞｨﾝｸﾞをｸﾘｱする
	EXTI->FPR1=0x00000003;			//立下りﾍﾟﾝﾃﾞｨﾝｸﾞをｸﾘｱする
#endif
}

void EXTI2_3_IRQHandler(void)
{

#if defined(DEV_F030)||defined(DEV_L010)//F030､L010のとき
	EXTI->PR=0x0000000c;			//ﾍﾟﾝﾃﾞｨﾝｸﾞをｸﾘｱする
#endif
#ifdef DEV_G03x				//G03xのとき
	EXTI->RPR1=0x0000000c;			//立上りﾍﾟﾝﾃﾞｨﾝｸﾞをｸﾘｱする
	EXTI->FPR1=0x0000000c;			//立下りﾍﾟﾝﾃﾞｨﾝｸﾞをｸﾘｱする
#endif
}

void EXTI4_15_IRQHandler(void)
{

#if defined(DEV_F030)||defined(DEV_L010)//F030､L010のとき
	EXTI->PR=0x0000fff0;			//ﾍﾟﾝﾃﾞｨﾝｸﾞをｸﾘｱする
#endif
#ifdef DEV_G03x				//G03xのとき
	EXTI->RPR1=0x0000fff0;			//立上りﾍﾟﾝﾃﾞｨﾝｸﾞをｸﾘｱする
	EXTI->FPR1=0x0000fff0;			//立上りﾍﾟﾝﾃﾞｨﾝｸﾞをｸﾘｱする
#endif
}
#endif
#endif


//==============================================
//Sleep処理
//==============================================
static void apl_sleep(void)
{
#if defined(CdS_EN)||defined(CVD_EN)||defined(SW_PLNG)
					//CdSかCVDを実装するとき､
					//　SWをﾎﾟｰﾘﾝｸﾞするとき
	unsigned char n;
#endif

DBG_C('\n');
DBG_C('<');
DBG_C('@');

	//LED消灯
#ifdef LED0_BIT				//LED0を実装するとき
	GPIO_L(LED0_GPIO,LED0_BIT);		//消灯する
#endif
#ifdef LED1_BIT				//LED1を実装するとき
	GPIO_L(LED1_GPIO,LED1_BIT);		//消灯する
#endif
#ifdef LED2_BIT				//LED2を実装するとき
	GPIO_L(LED2_GPIO,LED2_BIT);		//消灯する
#endif
#ifdef LED3_BIT				//LED3を実装するとき
	GPIO_L(LED3_GPIO,LED3_BIT);		//消灯する
#endif
#ifdef LED4_BIT				//LED4を実装するとき
	GPIO_L(LED4_GPIO_LED4_BIT);		//消灯する
#endif
#ifdef LED5_BIT				//LED5を実装するとき
	GPIO_L(LED5,GPIO,LED5_BIT);		//消灯する
#endif
#ifdef LED6_BIT				//LED6を実装するとき
	GPIO_L(LED6_GPIO,LED6_BIT);		//消灯する
#endif

#if VOICE_S>0				//音声S再生を実装するとき
	//W25をﾊﾟﾜｰﾀﾞｳﾝ
DBG_C('a');
	W25_PWR_DOWN();				//W25をﾊﾟﾜｰﾀﾞｳﾝする
#endif

	//SPIをﾘｾｯﾄ
#if VOICE_C+VOICE_S>0			//SPIを使うとき
DBG_C('b');
	spi_reset();				//SPIﾎﾟｰﾄをﾘｾｯﾄする
#endif

	//PWMを停止
DBG_C('c');
	dev_pwm_stop();				//PWMを停止する

	//Sleep処理
#if defined(CdS_EN)||defined(CVD_EN)||defined(SW_PLNG)
					//CdSかCVDを実装するとき､
					//　SWをﾎﾟｰﾘﾝｸﾞするときは
					//　RTCwithEXTIでWakeupする

	for(;;)
	{
DBG_C('d');
		//SLEEPDEEPﾓｰﾄﾞへ移行

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if defined(DEV_F030)||defined(DEV_F302)||defined(DEV_L010)
					//F030､F302､L010のとき(ここから)
DBG_C('e');
		RTC->WPR=0xca;			//書込み保護を
		RTC->WPR=0x53;			//　解除する
		RTC->CR=0x00000000;		//ｱﾗｰﾑA機能無効
		while(!(RTC->ISR&RTC_ISR_ALRAWF));
						//無効を待つ
		RTC->CR=RTC_CR_ALRAIE|RTC_CR_ALRAE|RTC_CR_OSEL_0;
						//ｱﾗｰﾑA機能有効､割込み有効､出力有効
		RTC->WPR=0xff;			//書込み保護を設定する
		EXTI->PR=0xffffffff;		//EXTIﾍﾟﾝﾃﾞｨﾝｸﾞをｸﾘｱする

//++++++++++++++++++++++++++++++++++++++++
#if defined(DEV_F030)			//F030では
		EXTI->IMR|=EXTI_IMR_MR17;	//EXTI17(AWU)を割込み許可
DBG_SYNC();
		__DSB();			//ﾃﾞｰﾀ同期ﾊﾞﾘｱ
		__WFI();			//SLEEPDEEPﾓｰﾄﾞへ移行
		EXTI->IMR=0x00000000;		//EXTI割込み禁止
#endif

//++++++++++++++++++++++++++++++++++++++++
#if defined(DEV_F302)			//F302では
		RTC->CR|=RTC_CR_WUTE;		//AWUを有効にする
		EXTI->EMR|=EXTI_EMR_MR17;	//EXTI17をｲﾍﾞﾝﾄ有効
DBG_SYNC();
		__DSB();			//ﾃﾞｰﾀ同期ﾊﾞﾘｱ
		__WFE();			//SLEEPDEEPﾓｰﾄﾞへ移行
		EXTI->EMR&=~EXTI_EMR_MR17;	//EXTI17をｲﾍﾞﾝﾄ無効
		RTC->CR&=~RTC_CR_WUTE;		//AWUを無効にする
#endif

//++++++++++++++++++++++++++++++++++++++++
#if defined(DEV_L010)			//L010では
		EXTI->IMR|=EXTI_IMR_IM17;	//EXTI17(AWU)を割込み許可
DBG_SYNC();
		__DSB();			//ﾃﾞｰﾀ同期ﾊﾞﾘｱ
		__WFI();			//SLEEPDEEPﾓｰﾄﾞへ移行
		EXTI->IMR=0x00000000;		//EXTI割込み禁止
#endif

		SystemInit();			//ｼｽﾃﾑｸﾛｯｸを再設定する
DBG_C('f');
#endif					//F030､F302､L010のとき(ここまで)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if defined(DEV_F103)			//F103のとき(ここから)
DBG_C('g');
		while(!(RTC->CRL&RTC_CRL_RTOFF)){};
						//RTCﾚｼﾞｽﾀの更新完了を待つ
DBG_C('h');
		RTC->CRL|=RTC_CRL_CNF;		//設定ﾓｰﾄﾞ開始
		RTC->CNTL=0;			//ｶｳﾝﾀをｸﾘｱ
		RTC->CNTH=0;
		RTC->CRL&=~RTC_CRL_CNF;		//設定ﾓｰﾄﾞ終了
		while(!(RTC->CRL&RTC_CRL_RTOFF)){};
						//RTCﾚｼﾞｽﾀの更新完了を待つ
DBG_C('i');
DBG_SYNC();
		__DSB();			//ﾃﾞｰﾀ同期ﾊﾞﾘｱ
		__WFE();			//ﾃﾞｨｰﾌﾟｽﾘｰﾌﾟへ移行
		SystemInit();			//ｼｽﾃﾑｸﾛｯｸを再設定する

DBG_C('j');
#endif					//F103のとき(ここまで)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef DEV_G03x				//G03xのとき(ここから)
DBG_C('l');
		RTC->CR|=RTC_CR_WUTE;		//WUT機能を有効
DBG_SYNC();
		__DSB();			//ﾃﾞｰﾀ同期ﾊﾞﾘｱ
		__WFI();			//STOP1ﾓｰﾄﾞへ移行
		RTC->CR&=~RTC_CR_WUTE;		//WUT機能を無効
		SystemInit();			//ｼｽﾃﾑｸﾛｯｸを再設定する
DBG_C('m');
#endif					//G03xのとき(ここまで)

#ifdef SW_EN				//SWを実装するとき
		//SWを評価する
DBG_C('n');
		for(n=0;n<SW_SU;n++)
		{
			if(sw_read(n,1)) break;	//操作を検知したら抜ける
		}
		if(n<SW_SU) break;
#endif

#ifdef CdS_EN				//CdSを実装するとき
		//CdSを評価する
DBG_C('o');
		if(cds_read(1)) break;		//操作を検知したら抜ける
#endif

#ifdef CVD_EN				//CVDを実装するとき
		//CVDを評価する
DBG_C('p');
		for(n=0;n<CVD_SU;n++)
		{
			if(cvd_read(n,1)) break;//操作を検知したら抜ける
		}
		if(n<CVD_SU) break;
#endif
	}

#else					//CdSもCVDも実装しないとき､
					//　SWをﾎﾟｰﾘﾝｸﾞしないときは
					//　EXTI0～15(SW)でWakeupする
	//STOPﾓｰﾄﾞへ移行
#if defined(DEV_F030)||defined(DEV_L010)
					//F030､L010のとき(ここから)
DBG_C('r');
	ADC1->CR|=ADC_CR_ADDIS;			//ADC1を停止
	EXTI->PR=0xffffffff;			//EXTIﾍﾟﾝﾃﾞｨﾝｸﾞをｸﾘｱする
	EXTI->IMR=SW_EXTIMSK;			//EXTI割込み許可
DBG_SYNC();
	__DSB();				//ﾃﾞｰﾀ同期ﾊﾞﾘｱ
	__WFI();				//STOPﾓｰﾄﾞへ移行
	EXTI->IMR=0x00000000;			//EXTI割込み禁止
	SystemInit();				//ｼｽﾃﾑｸﾛｯｸを再設定する
DBG_C(';');
	ADC1->CR|=ADC_CR_ADEN;			//ADC1有効
#endif					//F030､L010のとき(ここまで)

#if defined(DEV_F103)			//F103のとき(ここから)
DBG_C('s');
	ADC1->CR2&=~ADC_CR2_ADON;		//ADC1を停止
DBG_SYNC();
	__DSB();				//ﾃﾞｰﾀ同期ﾊﾞﾘｱ
	__WFE();				//STOPﾓｰﾄﾞへ移行
	SystemInit();				//ｼｽﾃﾑｸﾛｯｸを再設定する
DBG_C(';');
	ADC1->CR2|=ADC_CR2_ADON;		//ADC1有効
#endif					//F103のとき(ここまで)

#if defined(DEV_F302)			//F302のとき(ここから)
DBG_C('t');
	ADC1->CR|=ADC_CR_ADDIS;			//ADC1を停止
DBG_SYNC();
	__DSB();				//ﾃﾞｰﾀ同期ﾊﾞﾘｱ
	__WFE();				//STOPﾓｰﾄﾞへ移行
	SystemInit();				//ｼｽﾃﾑｸﾛｯｸを再設定する
DBG_C(';');
	ADC1->CR|=ADC_CR_ADEN;			//ADC1有効
#endif					//F103のとき(ここまで)

#ifdef DEV_G03x				//G03xのとき(ここから)
DBG_C('u');
	ADC1->CR|=ADC_CR_ADDIS;			//ADC1を停止
	while(ADC1->CR&ADC_CR_ADEN){};		//ADC1の停止を待つ
	ADC1->CR&=~ADC_CR_ADVREGEN;		//ADCﾚｷﾞｭﾚｰﾀ無効
	EXTI->IMR1=SW_EXTIMSK;			//EXTI割込み許可
DBG_SYNC();
	__DSB();				//ﾃﾞｰﾀ同期ﾊﾞﾘｱ
	__WFI();			//STOP1ﾓｰﾄﾞへ移行
	EXTI->IMR1=0x00000000;			//EXTI割込み禁止
	SystemInit();				//ｼｽﾃﾑｸﾛｯｸを再設定する
DBG_C(';');
	ADC1->ISR|=ADC_ISR_ADRDY;		//ADRDYをｸﾘｱしておく
	ADC1->CR|=ADC_CR_ADVREGEN;		//ADCﾚｷﾞｭﾚｰﾀ有効
	ADC1->CR|=ADC_CR_ADEN;			//ADC1有効
//	while(!(ADC1->ISR&ADC_ISR_ADRDY)){};	//ADC1有効の完了を待つ
DBG_C('+');
#endif					//G03xのとき(ここまで)

#endif

	//PWMを再稼働
DBG_C('v');
	dev_pwm_start();			//PWMを開始する

	//SPIをｾｯﾄ
#if VOICE_C+VOICE_S>0			//SPIを使うとき
DBG_C('w');
	spi_set();				//SPIﾎﾟｰﾄをｾｯﾄする
#endif

	//SDを初期化
#if VOICE_C>0				//音声C再生を実装するとき
DBG_C('x');
	mmsd_init();				//SDCを初期化する
#endif

	//W25をﾊﾟﾜｰｱｯﾌﾟ
#if VOICE_S>0				//音声S再生を実装するとき
DBG_C('y');
	W25_PWR_UP();				//W25をﾊﾟﾜｰｱｯﾌﾟする
#endif
DBG_C('>');
}
#endif


//==============================================
//固有の処理(ｺｰﾙﾊﾞｯｸ関数)
//==============================================
static void CB_KOYUU(void)
{
#ifdef SLEEP_EN				//Sleep機能を実装するとき
	static unsigned short sleep_tmr=0;	//Sleepﾀｲﾏｰ
#endif
	unsigned char res,cnt,n;
#if PART_N>0				//ｵﾙｺﾞｰﾙ演奏を実装するとき
	struct SONG_IDX *p;
#endif

//DBG_C('\n');
//DBG_C('K');
	//稼働ﾁｪｯｸ
#ifdef SLEEP_EN				//Sleep機能を実装するとき
//DBG_C('e');
	res=0;					//仮に非稼働にしておく
#if PART_N>0				//ｵﾙｺﾞｰﾙ演奏を実装するとき
	res|=song.no;				//稼働しているときは稼働中の設定
#endif
#if VOICE_N>0				//音声N再生を実装するとき
	res|=voiceN[0].no;			//稼働しているときは稼働中の設定
#endif
#if VOICE_I>0				//音声I再生を実装するとき
	res|=voiceI.no;				//稼働しているときは稼働中の設定
#endif
#if VOICE_C>0				//音声C再生を実装するとき
	res|=voiceC.no;				//稼働しているときは稼働中の設定
#endif
#if VOICE_S>0				//音声S再生を実装するとき
	res|=voiceS[0].no;			//稼働しているときは稼働中の設定
#endif
//DBG_B(res);
	if(res)					//稼働しているときは
	{
		sleep_tmr=0;			//　Sleep時限をｸﾘｱする
	}
	else					//稼働していないときは
	{
//DBG_C('X');
//DBG_S(sleep_tmr);
		if(++sleep_tmr>SLEEP_TMR/KOYUU_PR)
						//　Sleep時限を超えたら
		{
			apl_sleep();		//　Sleepする
			sleep_tmr=0;		//　Sleep時限をｸﾘｱする
		}
	}
#endif

	//操作ﾁｪｯｸ
	res=0;					//仮に操作無しにしておく
#ifdef SW_EN				//SWを実装するとき
//DBG_C('s');
	if(!res)				//操作が無かったら
	{
#if VOICE_S>0				//音声Sを実装するとき
					//音声CはSPIを専有するのでﾘｾｯﾄ/ｾｯﾄできない
		spi_reset();			//SPIﾎﾟｰﾄをﾘｾｯﾄする
		wait_us(5);			//SWの電圧安定を待つ
#endif
//DBS_H;
		for(n=0;n<SW_SU;n++)		//SWの実装数まで繰返す
		{
			if((res=sw_read(n,0)))	//SW操作があったとき
			{
				cnt=sw_cnt[n];	//操作対象を得る
				break;		//抜ける
			}
		}
//DBS_L;
#if VOICE_S>0				//音声Sを実装するとき
					//音声CはSPIを専有するのでﾘｾｯﾄ/ｾｯﾄできない
		spi_set();			//SPIﾎﾟｰﾄをｾｯﾄする
#endif
	}
//DBG_B(res);
#endif
#ifdef CdS_EN				//CdSを実装するとき
//DBG_C('c');
	if(!res)				//操作が無かったら
	{
		if((res=cds_read(0)))		//CdS操作があったとき
		{
			cnt=CdS_CNT;		//操作対象を得る
		}
	}
//DBG_B(res);
#endif
#ifdef CVD_EN
//DBG_C('v');				//CVDを実装するとき
	if(!res)				//操作が無かったら
	{
		for(n=0;n<CVD_SU;n++)		//CVDの実装数まで繰返す
		{
			if((res=cvd_read(n,0)))	//CVD操作があったとき
			{
				cnt=cvd_cnt[n];	//操作対象を得る
				break;		//抜ける
			}
		}
	}
//DBG_B(res);
#endif

	//操作の実行
//DBG_C('e');
//DBG_B(res);
	switch(res)				//操作内容で処理する
	{
	case 0:				//操作がないとき
#ifdef KYOKU_AUTO			//ｵﾙｺﾞｰﾙ演奏を自動開始するとき
#if PART_N>0				//ｵﾙｺﾞｰﾙ演奏を実装するとき
//DBG_C('o');
//DBG_B(song.no);
		if(!song.no)			//演奏してなかったら
		{
#ifdef KYOKU_RAND			//曲番号をﾗﾝﾀﾞﾏｲｽﾞするとき
//DBG_C('\n');
			kyoku+=(rand()&kyoku_rand_msk)+1;
						//ﾗﾝﾀﾞﾑに増加する
			while(kyoku>SONG_IDX.no) kyoku-=SONG_IDX.no;
						//実装数内に収める
#else					//曲番号を順に進めるとき
			if(++kyoku>SONG_IDX.no) kyoku=1;
						//曲番号を進める
#endif
//DBG_B(kyoku);
			SONG_SEL(kyoku);
		}
#endif
#endif
#ifdef ONSEIN_AUTO			//音声N再生を自動開始するとき
#if VOICE_N>0				//音声N再生を実装するとき
//DBG_C('n');
//DBG_B(voiceN[0].no);
		if(!voiceN[0].no)		//再生してなかったら
		{
#ifdef ONSEIN_RAND			//音声N番号をﾗﾝﾀﾞﾏｲｽﾞするとき
			onseiN+=(rand()&onseiN_rand_msk)+1;
						//ﾗﾝﾀﾞﾑに増加する
			while(onseiN>VOICEN_SU) onseiN-=VOICEN_SU;
						//実装数内に収める
#else					//音声N番号を順に進めるとき
			if(++onseiN>VOICEN_SU) onseiN=1;
						//音声番号を進める
#endif
//DBG_B(onseiN);
			VOICEN_SEL(0,onseiN);	//再生開始する
		}
#endif
#endif
#ifdef ONSEII_AUTO			//音声I再生を自動開始するとき
#if VOICE_I>0				//音声I再生を実装するとき
//DBG_C('i');
//DBG_B(voiceI.no);
		if(!voiceI.no)			//再生してなかったら
		{
#ifdef ONSEII_RAND			//音声I番号をﾗﾝﾀﾞﾏｲｽﾞするとき
			onseiI+=(rand()&onseiI_rand_msk)+1;
						//ﾗﾝﾀﾞﾑに増加する
			while(onseiI>VOICEI_SU) onseiI-=VOICEI_SU;
						//実装数内に収める
#else					//音声I番号を順に進めるとき
			if(++onseiI>VOICEI_SU) onseiI=1;
						//音声番号を進める
#endif
//DBG_B(onseiI);
			VOICEI_SEL(onseiI);	//再生開始する
		}
#endif
#endif
#ifdef ONSEIC_AUTO			//音声C再生を自動開始するとき
#if VOICE_C>0				//音声C再生を実装するとき
//DBG_C('c');
//DBG_B(voiceC.no);
		if(!voiceC.no)			//再生してなかったら
		{
#ifdef ONSEIC_RAND			//音声C番号をﾗﾝﾀﾞﾏｲｽﾞするとき
			onseiC+=(rand()&onseiC_rand_msk)+1;
						//ﾗﾝﾀﾞﾑに増加する
			while(onseiC>VOICEC_SU) onseiC-=VOICEC_SU;
						//実装数内に収める
#else					//音声C番号を順に進めるとき
			if(++onseiC>VOICEC_SU) onseiC=1;
						//音声番号を進める
#endif
//DBG_B(onseiC);
			VOICEC_SEL(onseiC);	//再生開始する
		}
#endif
#endif
#ifdef ONSEIS_AUTO			//音声S再生を自動開始するとき
#if VOICE_S>0				//音声S再生を実装するとき
//DBG_C('s');
//DBG_B(voiceS.no);
		if(!voiceS[0].no)		//再生してなかったら
		{
#ifdef ONSEIS_RAND			//音声S番号をﾗﾝﾀﾞﾏｲｽﾞするとき
			onseiS+=(rand()&onseiS_rand_msk)+1;
						//ﾗﾝﾀﾞﾑに増加する
			while(onseiS>VOICES_SU) onseiS-=VOICES_SU;
						//実装数内に収める
#else					//音声S番号を順に進めるとき
			if(++onseiS>VOICES_SU) onseiS=1;
						//音声番号を進める
#endif
//DBG_B(onseiS);
			VOICES_SEL(0,onseiS);	//再生開始する
		}
#endif
#endif
		break;
	case 1:				//短ｵﾝ検知のとき
#if PART_N>0				//ｵﾙｺﾞｰﾙ演奏を実装するとき
		if(cnt&ORGEL_CNT)		//操作がｵﾙｺﾞｰﾙのとき
		{
#ifdef KYOKU_RAND			//曲番号をﾗﾝﾀﾞﾏｲｽﾞするとき
//DBG_C('\n');
			kyoku+=(rand()&kyoku_rand_msk)+1;
						//ﾗﾝﾀﾞﾑに増加する
//DBG_B(kyoku);
			while(kyoku>SONG_IDX.no) kyoku-=SONG_IDX.no;//実装数内に収める
//DBG_B(kyoku);
#else					//曲番号を順に進めるとき
			if(++kyoku>SONG_IDX.no) kyoku=1;
						//曲番号を進める
#endif
//DBG_B(kyoku);
			SONG_SEL(kyoku);	//演奏開始する
		}
#endif
#if VOICE_N>0				//音声0再生を実装するとき
		if(cnt&VOICEN_CNT)		//操作が音声0のとき
		{
#ifdef ONSEIN_RAND			//音声N番号をﾗﾝﾀﾞﾏｲｽﾞするとき
			onseiN+=(rand()&onseiN_rand_msk)+1;
						//ﾗﾝﾀﾞﾑに増加する
			while(onseiN>VOICEN_SU) onseiN-=VOICEN_SU;
						//実装数内に収める
#else					//音声N番号を順に進めるとき
			if(++onseiN>VOICEN_SU) onseiN=1;
						//音声番号を進める
#endif
//DBG_B(onseiN);
			VOICEN_SEL(0,onseiN);	//再生開始する
		}
#endif
#if VOICE_I>0				//音声I再生を実装するとき
		if(cnt&VOICEI_CNT)		//操作が音声Iのとき
		{
#ifdef ONSEII_RAND			//音声I番号をﾗﾝﾀﾞﾏｲｽﾞするとき
			onseiI+=(rand()&onseiI_rand_msk)+1;
						//ﾗﾝﾀﾞﾑに増加する
			while(onseiI>VOICEI_SU) onseiI-=VOICEI_SU;
						//実装数内に収める
#else					//音声I番号を順に進めるとき
			if(++onseiI>VOICEI_SU) onseiI=1;
						//音声番号を進める
#endif
//DBG_B(onseiI);
			VOICEI_SEL(onseiI);	//再生開始する
		}
#endif
#if VOICE_C>0				//音声C再生を実装するとき
		if(cnt&VOICEC_CNT)		//操作が音声Cのとき
		{
#ifdef ONSEIC_RAND			//音声C番号をﾗﾝﾀﾞﾏｲｽﾞするとき
			onseiC+=(rand()&onseiC_rand_msk)+1;
						//ﾗﾝﾀﾞﾑに増加する
			while(onseiC>VOICEC_SU) onseiC-=VOICEC_SU;
						//実装数内に収める
#else					//音声C番号を順に進めるとき
			if(++onseiC>VOICEC_SU) onseiC=1;
						//音声番号を進める
#endif
//DBG_B(onseiC);
			VOICEC_SEL(onseiC);	//再生開始する
		}
#endif
#if VOICE_S>0				//音声S再生を実装するとき
		if(cnt&VOICES_CNT)		//操作が音声Sのとき
		{
#ifdef ONSEIS_RAND			//音声S番号をﾗﾝﾀﾞﾏｲｽﾞするとき
//DBG_C('\n');
			onseiS+=(rand()&onseiS_rand_msk)+1;
						//ﾗﾝﾀﾞﾑに増加する
//DBG_B(onseiS);
			while(onseiS>VOICES_SU) onseiS-=VOICES_SU;
						//実装数内に収める
#else					//音声S番号を順に進めるとき
			if(++onseiS>VOICES_SU) onseiS=1;
						//音声番号を進める
//DBG_B(onseiS);
#endif
//DBG_B(onseiS);
			VOICES_SEL(0,onseiS);	//再生開始する
		}
#endif
		break;
	case 2:				//長ｵﾝ一致のとき
#if PART_N>0				//ｵﾙｺﾞｰﾙ演奏を実装するとき
		if(cnt&ORGEL_CNT)		//操作がｵﾙｺﾞｰﾙのとき
		{
			SONG_SEL(0);		//演奏中断する
		}
#endif
#if VOICE_N>0				//音声0再生を実装するとき
		if(cnt& VOICEN_CNT)		//操作が音声0のとき
		{
			VOICEN_SEL(0,0);	//再生中断する
		}
#endif
#if VOICE_I>0				//音声I再生を実装するとき
		if(cnt&VOICEI_CNT)		//操作が音声Iのとき
		{
			VOICEI_SEL(0);		//再生中断する
		}
#endif
#if VOICE_C>0				//音声C再生を実装するとき
		if(cnt&VOICEC_CNT)		//操作が音声Cのとき
		{
			VOICEC_SEL(0);		//再生中断する
		}
#endif
#if VOICE_S>0				//音声S再生を実装するとき
		if(cnt&VOICES_CNT)		//操作が音声Sのとき
		{
			VOICES_SEL(0,0);	//再生中断する
		}
#endif
		break;
	}

	//LED点滅
#if PART_N>0				//ｵﾙｺﾞｰﾙ演奏を実装するとき
	if(song.no)				//演奏中のとき
	{
#ifdef LED0_BIT				//LED0を実装するとき
		if(part[0].onpu_cd&0x0001) GPIO_H(LED0_GPIO,LED0_BIT);
		else GPIO_L(LED0_GPIO,LED0_BIT);
#endif
#ifdef LED1_BIT				//LED1を実装するとき
		if(part[0].onpu_cd&0x0002) GPIO_H(LED1_GPIO,LED1_BIT);
		else GPIO_L(LED1_GPIO,LED1_BIT);
#endif
#ifdef LED2_BIT				//LED2を実装するとき
		if(part[0].onpu_cd&0x0004) GPIO_H(LED2_GPIO,LED2_BIT);
		else GPIO_L(LED2_GPIO,LED2_BIT);
#endif
#ifdef LED3_BIT				//LED3を実装するとき
		if(part[0].onpu_cd&0x0008) GPIO_H(LED3_GPIO,LED3_BIT);
		else GPIO_L(LED3_GPIO,LED3_BIT);
#endif
#ifdef LED4_BIT				//LED4を実装するとき
		if(part[0].onpu_cd&0x0010) GPIO_H(LED4_GPIO,LED4_BIT);
		else GPIO_L(LED4_GPIO,LED4_BIT);
#endif
#ifdef LED5_BIT				//LED5を実装するとき
#if PART_N==1				//1ﾊﾟｰﾄのみ実装するとき
		if(part[0].onpu_cd&0x0020) GPIO_H(LED5_GPIO,LED5_BIT);
		else GPIO_L(LED5_GPIO,LED5_BIT);
#else					//2ﾊﾟｰﾄ以上実装するとき
		if(part[1].onpu_cd&0x0001) GPIO_H(LED5_GPIO,LED5_BIT);
		else GPIO_L(LED5_GPIO,LED5_BIT);
#endif
#endif
#ifdef LED6_BIT				//LED6を実装するとき
#if PART_N==1				//1ﾊﾟｰﾄのみ実装するとき
		if(part[0].onpu_cd&0x0040) GPIO_H(LED6_GPIO,LED6_BIT);
		else GPIO_L(LED6_GPIO,LED6_BIT);
#else					//2ﾊﾟｰﾄ以上実装するとき
		if(part[1].onpu_cd&0x0002) GPIO_H(LED6_GPIO,LED6_BIT);
		else GPIO_L(LED6_GPIO,LED6_BIT);
#endif
#endif
	}
#endif
//DBG_C('~');
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
static void CB_VOICES_END(
	unsigned char ch)			//音声Sのch番号
{
}
#endif
