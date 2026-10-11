#ifdef	SHDBS
//==============================================
//ﾃﾞﾊﾞｸﾞ信号の初期設定
//==============================================
static void dbs_init(void)
{

	GPIO_L(DBS_GPIO,DBS_BIT);		//ﾃﾞﾊﾞｸﾞ信号をL
	GPIO_OUT(DBS_GPIO,DBS_BIT);		//ﾃﾞﾊﾞｸﾞ信号を出力
}


//==============================================
//ﾃﾞﾊﾞｸﾞ信号出力のﾏｸﾛ定義
//==============================================
#define	DBS_H		GPIO_H(DBS_GPIO,DBS_BIT);
#define	DBS_L		GPIO_L(DBS_GPIO,DBS_BIT);
#else
#define DBS_H
#define DBS_L
#endif


//==============================================
//1usの待ち
//==============================================
static void wait_1us(void)
{

	SysTick->CTRL=0x00000004;		//ｸﾛｯｸｿｰｽはAHB､例外ｱｻｰﾄしない､ｶｳﾝﾀ無効
	SysTick->LOAD=SYS_CLK-SYS_CLK/2;	//比較値を設定
	SysTick->VAL=0;				//ｶｳﾝﾀを初期化
	SysTick->CTRL=0x00000005;		//ｸﾛｯｸｿｰｽはAHB､例外ｱｻｰﾄしない､ｶｳﾝﾀ有効
	while(!(SysTick->CTRL&0x00010000));	//ﾌﾗｸﾞｾｯﾄを待つ
}


//==============================================
//us単位の待ち
//==============================================
static void wait_us(
	unsigned short us)			//待ち時間[us]
{

	SysTick->CTRL=0x00000004;		//ｸﾛｯｸｿｰｽはAHB､例外ｱｻｰﾄしない､ｶｳﾝﾀ無効
	SysTick->LOAD=SYS_CLK*us-SYS_CLK/2;	//比較値を設定
	SysTick->VAL=0;				//ｶｳﾝﾀを初期化
	SysTick->CTRL=0x00000005;		//ｸﾛｯｸｿｰｽはAHB､例外ｱｻｰﾄしない､ｶｳﾝﾀ有効
	while(!(SysTick->CTRL&0x00010000));	//ﾌﾗｸﾞｾｯﾄを待つ
}


//==============================================
//ms単位の待ち
//==============================================
static void wait_ms(
	unsigned short ms)			//待ち時間[ms]
{

	while(ms--)
	{
		wait_us(1000);
	}
}


#if defined(SHDBG)||defined(SHDBG_IN)	//ﾃﾞﾊﾞｸﾞ情報出力かﾃﾞﾊﾞｸﾞ情報入力するとき
#ifndef SHBPS				//ﾎﾞｰﾚｰﾄが宣言されていないとき
#define	SHBPS 38400				//38400bpsとする
#endif
#ifdef SHDBG_SOFT			//UARTをｿﾌﾄ実装するとき
#ifdef SHDBG				//ﾃﾞﾊﾞｸﾞ情報を出力するとき
//==============================================
//ﾃﾞﾊﾞｸﾞ情報出力の初期設定
//==============================================
static void dbg_init(void)
{

	GPIO_H(DBG_GPIO,DBG_BIT);		//ﾃﾞﾊﾞｸﾞ情報をH
	GPIO_OUT(DBG_GPIO,DBG_BIT);		//ﾃﾞﾊﾞｸﾞ情報を出力
}


//==============================================
//送信完了の同期
//==============================================
#define	outpt_sync()


//==============================================
//1文字を送信(ｿﾌﾄUART)
//==============================================
static void outpt_c(				//戻り値無し
	unsigned char c)			//送信する文字
{
	unsigned char n=8;

	GPIO_L(DBG_GPIO,DBG_BIT);		//ｽﾀｰﾄﾋﾞｯﾄ
	wait_us(1000000/SHBPS);			//1ﾋﾞｯﾄ巾
	while(n--)				//8ﾋﾞｯﾄ分繰返す
	{
		if(c&1) GPIO_H(DBG_GPIO,DBG_BIT);
						//ﾃﾞｰﾀﾋﾞｯﾄ
		else GPIO_L(DBG_GPIO,DBG_BIT);
		wait_us(1000000/SHBPS);		//1ﾋﾞｯﾄ巾
		c>>=1;				//次のﾃﾞｰﾀﾋﾞｯﾄへ進む
	}
	GPIO_H(DBG_GPIO,DBG_BIT);		//ｽﾄｯﾌﾟﾋﾞｯﾄ
	wait_us(3000000/SHBPS);			//3ﾋﾞｯﾄ巾
}
#endif


#else					//内蔵UARTﾓｼﾞｭｰﾙを使うとき
//==============================================
//UARTの初期設定
//==============================================
static void dbg_init(void)
{

#ifdef DEV_F030				//F030のとき
	RCC->APB2ENR|=RCC_APB2ENR_USART1EN;	//USART1にｸﾛｯｸ供給
	//RCC_CFGR3のUSART1SWは00(USART1ｸﾛｯｸはPCLK)初期値
	USART1->BRR=(SYS_CLK*10000000/SHBPS+5)/10;
						//ﾎﾞｰﾚｰﾄ設定
	USART1->CR2=USART_CR2_STOP_1;		//ｽﾄｯﾌﾟ2ﾋﾞｯﾄ
	USART1->CR1=USART_CR1_TE|USART_CR1_RE|USART_CR1_UE;
						//ﾃﾞｰﾀ8ﾋﾞｯﾄ､ﾊﾟﾘﾃｨ無し､送信有効､
						//　受信有効､UART有効
#ifdef SHDBG				//ﾃﾞﾊﾞｸﾞ情報を出力するとき
	GPIO_AF(DBG_GPIO,DBG_BIT,DBG_AF);	//ﾃﾞﾊﾞｸﾞ情報出力を交代機能に設定
#endif
#ifdef SHDBG_IN				//ﾃﾞﾊﾞｸﾞ情報を入力するとき
	GPIO_AF(DBG_IN_GPIO,DBG_IN_BIT,DBG_IN_AF);
						//ﾃﾞﾊﾞｸﾞ情報入力を交代機能に設定
#endif
#endif

#ifdef DEV_F302				//F302のとき
	RCC->APB1ENR|=RCC_APB1ENR_USART2EN;	//USART2にｸﾛｯｸ供給
	//RCC_CFGR3のUSART2SWは00(USART2ｸﾛｯｸはPCLK2)初期値
	USART2->BRR=(SYS_CLK*5000000/SHBPS+5)/10;
						//ﾎﾞｰﾚｰﾄ設定
	USART2->CR2=USART_CR2_STOP_1;		//ｽﾄｯﾌﾟ2ﾋﾞｯﾄ
	USART2->CR1=USART_CR1_TE|USART_CR1_RE|USART_CR1_UE;
						//ﾃﾞｰﾀ8ﾋﾞｯﾄ､ﾊﾟﾘﾃｨ無し､送信有効､
						//　受信有効､UART有効
#ifdef SHDBG				//ﾃﾞﾊﾞｸﾞ情報を出力するとき
	GPIO_AF(DBG_GPIO,DBG_BIT,DBG_AF);	//ﾃﾞﾊﾞｸﾞ情報出力を交代機能に設定
#endif
#ifdef SHDBG_IN				//ﾃﾞﾊﾞｸﾞ情報を入力するとき
	GPIO_AF(DBG_IN_GPIO,DBG_IN_BIT,DBG_IN_AF);
						//ﾃﾞﾊﾞｸﾞ情報入力を交代機能に設定
#endif
#endif

#ifdef DEV_F103				//F103のとき
	RCC->APB2ENR|=RCC_APB2ENR_USART1EN;	//USART1にｸﾛｯｸ供給
#ifdef DBG_REMAP			//ﾃﾞﾊﾞｸﾞ情報USARTをﾘﾏｯﾌﾟするとき
	AFIO->MAPR|=DBG_REMAP;			//USARTをﾘﾏｯﾌﾟする
#endif
	//RCC_CFGR3のUSART1SWは00(USART1ｸﾛｯｸはPCLK)初期値
	USART1->BRR=(SYS_CLK*10000000/SHBPS+5)/10;
						//ﾎﾞｰﾚｰﾄ設定
	USART1->CR2=USART_CR2_STOP_1;		//ｽﾄｯﾌﾟ2ﾋﾞｯﾄ
	USART1->CR1=USART_CR1_TE|USART_CR1_RE|USART_CR1_UE;
						//ﾃﾞｰﾀ8ﾋﾞｯﾄ､ﾊﾟﾘﾃｨ無し､送信有効､
						//　受信有効､UART有効
#ifdef SHDBG				//ﾃﾞﾊﾞｸﾞ情報を出力するとき
	GPIO_AF(DBG_GPIO,DBG_BIT);		//ﾃﾞﾊﾞｸﾞ情報出力を交代機能に設定
#endif
#ifdef SHDBG_IN				//ﾃﾞﾊﾞｸﾞ情報を入力するとき
	GPIO_AF(DBG_IN_GPIO,DBG_IN_BIT);	//ﾃﾞﾊﾞｸﾞ情報入力を交代機能に設定
#endif
#endif

#ifdef DEV_L010				//L010のとき
	RCC->APB1ENR|=RCC_APB1ENR_USART2EN;	//USART2にｸﾛｯｸ供給
	//RCC_CFGR3のUSART2SWは00(USART2ｸﾛｯｸはPCLK)初期値
	USART2->BRR=(SYS_CLK*10000000/SHBPS+5)/10;
						//ﾎﾞｰﾚｰﾄ設定
	USART2->CR2=USART_CR2_STOP_1;		//ｽﾄｯﾌﾟ2ﾋﾞｯﾄ
	USART2->CR1=USART_CR1_TE|USART_CR1_RE|USART_CR1_UE;
						//ﾃﾞｰﾀ8ﾋﾞｯﾄ､ﾊﾟﾘﾃｨ無し､送信有効､
						//　受信有効､UART有効
#ifdef SHDBG				//ﾃﾞﾊﾞｸﾞ情報を出力するとき
	GPIO_AF(DBG_GPIO,DBG_BIT,DBG_AF);	//ﾃﾞﾊﾞｸﾞ情報出力を交代機能に設定
#endif
#ifdef SHDBG_IN				//ﾃﾞﾊﾞｸﾞ情報を入力するとき
	GPIO_AF(DBG_IN_GPIO,DBG_IN_BIT,DBG_IN_AF);
						//ﾃﾞﾊﾞｸﾞ情報入力を交代機能に設定
#endif
#endif

#ifdef DEV_G03x				//G03xのとき
	RCC->APBENR1|=RCC_APBENR1_USART2EN;	//USART2にｸﾛｯｸ供給
	//RCC_CCIPRのUSART1SELは00(USART1ｸﾛｯｸはPCLK)初期値
	USART2->BRR=(SYS_CLK*5000000/SHBPS+5)/10;
						//ﾎﾞｰﾚｰﾄ設定
	USART2->CR2=USART_CR2_STOP_1;		//ｽﾄｯﾌﾟ2ﾋﾞｯﾄ
	USART2->CR1=USART_CR1_TE|USART_CR1_RE|USART_CR1_UE;
						//ﾃﾞｰﾀ8ﾋﾞｯﾄ､ﾊﾟﾘﾃｨ無し､送信有効､
						//　受信有効､UART有効
#ifdef SHDBG				//ﾃﾞﾊﾞｸﾞ情報を出力するとき
	GPIO_AF(DBG_GPIO,DBG_BIT,DBG_AF);	//ﾃﾞﾊﾞｸﾞ情報出力を交代機能に設定
#endif
#ifdef SHDBG_IN				//ﾃﾞﾊﾞｸﾞ情報を入力するとき
	GPIO_AF(DBG_IN_GPIO,DBG_IN_BIT,DBG_IN_AF);
						//ﾃﾞﾊﾞｸﾞ情報入力を交代機能に設定
#endif
#endif
}


#ifdef SHDBG_IN				//ﾃﾞﾊﾞｸﾞ情報を入力するとき
//==============================================
//1文字を受信
//==============================================
static short input_c(void)			//受信完了時は受信ﾃﾞｰﾀを返す
						//未受信時は-1を返す
{

#ifdef DEV_F030				//F030のとき
	if(USART1->ISR&USART_ISR_RXNE) return(USART1->RDR);
						//受信完了していたら受信ﾃﾞｰﾀを返す
	return(-1);				//未受信を返す
#endif

#ifdef DEV_F302				//F302のとき
	if(USART2->SR&USART_SR_RXNE) return(USART2->RDR);
						//受信完了していたら受信ﾃﾞｰﾀを返す
	return(-1);				//未受信を返す
#endif

#ifdef DEV_L010				//L010のとき
	if(USART2->ISR&USART_ISR_RXNE) return(USART2->RDR);
						//受信完了していたら受信ﾃﾞｰﾀを返す
	return(-1);				//未受信を返す
#endif

#ifdef DEV_G03x				//G03xのとき
	if(USART2->ISR&USART_ISR_RXFNE) return(USART1->RDR);
						//受信完了していたら受信ﾃﾞｰﾀを返す
	return(-1);				//未受信を返す
#endif
}
#endif


#ifdef SHDBG				//ﾃﾞﾊﾞｸﾞ情報を出力するとき
//==============================================
//送信完了の同期
//==============================================
static void outpt_sync(void)
{

#ifdef DEV_F030				//F030のとき
	while(!(USART1->ISR&USART_ISR_TC));	//送信完了を待つ
#endif

#ifdef DEV_F302				//F302のとき
	while(!(USART2->ISR&USART_ISR_TC));	//送信完了を待つ
#endif

#ifdef DEV_F103				//F103のとき
	while(!(USART1->SR&USART_SR_TC));	//送信完了を待つ
#endif

#ifdef DEV_L010				//L010のとき
	while(!(USART2->ISR&USART_ISR_TC));	//送信完了を待つ
#endif

#ifdef DEV_G03x				//G03xのとき
	while(!(USART2->ISR&USART_ISR_TC));	//送信完了を待つ
#endif
}


//==============================================
//1文字を送信
//==============================================
static void outpt_c(				//戻り値無し
	unsigned char c)			//送信する文字
{

#ifdef DEV_F030				//F030のとき
	while(!(USART1->ISR&USART_ISR_TXE));	//送信ﾃﾞｰﾀﾚｼﾞｽﾀの空きを待つ
	USART1->TDR=c;				//1文字を送信する
#endif

#ifdef DEV_F302				//F302のとき
	while(!(USART2->ISR&USART_ISR_TXE));	//送信ﾃﾞｰﾀﾚｼﾞｽﾀの空きを待つ
	USART2->TDR=c;				//1文字を送信する
#endif

#ifdef DEV_F103				//F103のとき
	while(!(USART1->SR&USART_SR_TXE));	//送信ﾃﾞｰﾀﾚｼﾞｽﾀの空きを待つ
	USART1->DR=c;				//1文字を送信する
#endif

#ifdef DEV_L010				//L010のとき
	while(!(USART2->ISR&USART_ISR_TXE));	//送信ﾃﾞｰﾀﾚｼﾞｽﾀの空きを待つ
	USART2->TDR=c;				//1文字を送信する
#endif

#ifdef DEV_G03x				//G03xのとき
	while(!(USART2->ISR&USART_ISR_TXE_TXFNF)){};
						//送信ﾃﾞｰﾀﾊﾞﾌｧの空きを待つ
	USART1->TDR=c;				//1文字を送信する
#endif
}
#endif
#endif


#ifdef SHDBG				//ﾃﾞﾊﾞｸﾞ情報を出力するとき
//==============================================
//文字列(文字列の最後は\0)を送信
//==============================================
static void outpt_a(
	unsigned char *a)			//送信する文字列
{

	while(*a) outpt_c(*a++);		//文字列を送信する
}


//==============================================
//charを16進2文字にして送信する
//==============================================
static void outpt_b(
	unsigned char b)			//送信するﾊﾞｲﾄﾃﾞｰﾀ
{
	unsigned char c;

	c=b>>4;					//上位ﾆﾌﾞﾙを取出す
	if(c>9) c+=55;				//16進文字に変換する
	else c+=48;
	outpt_c(c);				//1文字を送信する
	c=(unsigned char)(b&0x0f);		//下位ﾆﾌﾞﾙを取出す
	if(c>9) c+=55;				//16進文字に変換する
	else c+=48;
	outpt_c(c);				//1文字を送信する
}


//==============================================
//shortを16進4文字にして送信する
//==============================================
static void outpt_s(
	unsigned short s)			//送信するshortﾃﾞｰﾀ
{

	outpt_b(s>>8);				//上位ﾊﾞｲﾄを16進で送信する
	outpt_b(s);				//下位ﾊﾞｲﾄを16進で送信する
}


//==============================================
//longを16進8文字にして送信する
//==============================================
static void outpt_l(
	unsigned long l)			//送信するlongﾃﾞｰﾀ
{

	outpt_b(l>>24);				//最上位ﾊﾞｲﾄを16進で送信する
	outpt_b(l>>16);
	outpt_b(l>>8);
	outpt_b(l);				//最下位ﾊﾞｲﾄを16進で送信する
}


//==============================================
//複数ﾊﾞｲﾄを16進2文字にして送信する
//==============================================
static void outpt_d(
	unsigned char *d,			//送信するﾊﾞｲﾄﾃﾞｰﾀのｱﾄﾞﾚｽ
	unsigned char l)			//ﾊﾞｲﾄ数
{

	outpt_c('[');
	while(--l)
	{
		outpt_b(*d++);
		outpt_c('.');
	}
	outpt_b(*d++);
	outpt_c(']');
}


//==============================================
//ﾃﾞﾊﾞｸﾞ情報の16ﾋﾞｯﾄを10進5文字に変換して送信する
//==============================================
static void outpt_i(
	unsigned short i)			//送信する16ﾋﾞｯﾄﾃﾞｰﾀ
{
	unsigned char b[10];

	b[0]=i/10000;				//10000の桁を求める
	i-=b[0]*10000;
	b[1]=i/1000;				//1000の桁を求める
	i-=b[1]*1000;
	b[2]=i/100;				//100の桁を求める
	i-=b[2]*100;
	b[3]=i/10;				//10の桁を求める
	i-=b[3]*10;
	b[4]=i;					//1桁を求める
	outpt_c(b[0]+'0');			//10進文字に変換して送信する
	outpt_c(b[1]+'0');
	outpt_c(b[2]+'0');
	outpt_c(b[3]+'0');
	outpt_c(b[4]+'0');
}
#endif


//==============================================
//ﾃﾞﾊﾞｸﾞ情報入出力のﾏｸﾛ定義
//==============================================
#define DBG_IN() input_c()
#define DBG_C(c) outpt_c((unsigned char)c)
#define DBG_A(a) outpt_a((unsigned char *)a)
#define DBG_B(b) outpt_b((unsigned char)b)
#define DBG_S(s) outpt_s((unsigned short)s)
#define DBG_L(l) outpt_l((unsigned int)l)
#define DBG_D(d,l) outpt_d((unsigned char *)d,l)
#define DBG_I(s) outpt_i((unsigned short)s)
#define DBG_SYNC() outpt_sync()
#else
#define DBG_IN()
#define DBG_C(c)
#define DBG_A(a)
#define DBG_B(b)
#define DBG_S(s)
#define DBG_L(l)
#define DBG_D(d,l)
#define DBG_I(s)
#define DBG_SYNC()
#endif
