#ifndef UARTBPS				//ﾎﾞｰﾚｰﾄが宣言されていないとき
#define	UARTBPS 38400				//38400bpsとする
#endif
#ifdef UART_SOFT			//UARTをｿﾌﾄ実装するとき
//==============================================
//UART出力の初期設定
//==============================================
static void uart_init(void)
{

	GPIO_H(UART_GPIO,UART_BIT);		//ｼﾘｱﾙ情報をH
	GPIO_OUT(UART_GPIO,UART_BIT);		//ｼﾘｱﾙ情報を出力
}


//==============================================
//送信完了の同期
//==============================================
#define	uart_out_sync()


//==============================================
//1文字を送信(ｿﾌﾄUART)
//==============================================
static void uart_out(				//戻り値無し
	unsigned char c)			//送信する文字
{
	unsigned char n=8;

	GPIO_L(UART_GPIO,UART_BIT);		//ｽﾀｰﾄﾋﾞｯﾄ
	wait_us(1000000/UARTBPS);		//1ﾋﾞｯﾄ巾
	while(n--)				//8ﾋﾞｯﾄ分繰返す
	{
		if(c&1) GPIO_H(UART_GPIO,UART_BIT);
						//ﾃﾞｰﾀﾋﾞｯﾄ
		else GPIO_L(UART_GPIO,UART_BIT);
		wait_us(1000000/UARTBPS);	//1ﾋﾞｯﾄ巾
		c>>=1;				//次のﾃﾞｰﾀﾋﾞｯﾄへ進む
	}
	GPIO_H(UART_GPIO,UART_BIT);		//ｽﾄｯﾌﾟﾋﾞｯﾄ
	wait_us(2000000/UARTBPS);		//2ﾋﾞｯﾄ巾
}


#else					//内蔵UARTﾓｼﾞｭｰﾙを使うとき
//==============================================
//UARTの初期設定
//==============================================
static void uart_init(void)
{

#ifdef DEV_F030				//F030のとき
#ifdef DEV_UART2			//UART2を使うとき
	RCC->APB1ENR|=RCC_APB1ENR_USART2EN;	//USART2にｸﾛｯｸ供給
#else
	RCC->APB2ENR|=RCC_APB2ENR_USART1EN;	//USART1にｸﾛｯｸ供給
#endif
	//RCC_CFGR3のUSART1SWは00(USART1ｸﾛｯｸはPCLK)初期値
	UARTx->BRR=(SYS_CLK*10000000/UARTBPS+5)/10;
						//ﾎﾞｰﾚｰﾄ設定
	UARTx->CR2=USART_CR2_STOP_1;		//ｽﾄｯﾌﾟ2ﾋﾞｯﾄ
	UARTx->CR1=USART_CR1_TE|USART_CR1_RE|USART_CR1_UE;
						//ﾃﾞｰﾀ8ﾋﾞｯﾄ､ﾊﾟﾘﾃｨ無し､送信有効､
						//　受信有効､UART有効
	GPIO_AF(UART_GPIO,UART_BIT,UART_AF);	//ｼﾘｱﾙ情報出力を交代機能に設定
	GPIO_AF(UART_IN_GPIO,UART_IN_BIT,UART_IN_AF);
						//ｼﾘｱﾙ情報入力を交代機能に設定
#endif

#ifdef DEV_F302				//F302のとき
#ifdef DEV_UART2			//UART2を使うとき
	RCC->APB1ENR|=RCC_APB1ENR_USART2EN;	//USART2にｸﾛｯｸ供給
#else
	RCC->APB2ENR|=RCC_APB2ENR_USART1EN;	//USART1にｸﾛｯｸ供給
#endif
	//RCC_CFGR3のUSART2SWは00(USART2ｸﾛｯｸはPCLK2)初期値
	UARTx->BRR=(SYS_CLK*5000000/UARTBPS+5)/10;
						//ﾎﾞｰﾚｰﾄ設定
	UARTx->CR2=USART_CR2_STOP_1;		//ｽﾄｯﾌﾟ2ﾋﾞｯﾄ
	UARTx->CR1=USART_CR1_TE|USART_CR1_RE|USART_CR1_UE;
						//ﾃﾞｰﾀ8ﾋﾞｯﾄ､ﾊﾟﾘﾃｨ無し､送信有効､
						//　受信有効､UART有効
	GPIO_AF(UART_GPIO,UART_BIT,UART_AF);	//ｼﾘｱﾙ情報出力を交代機能に設定
	GPIO_AF(UART_IN_GPIO,UART_IN_BIT,UART_IN_AF);
						//ｼﾘｱﾙ情報入力を交代機能に設定
#endif

#ifdef DEV_F103				//F103のとき
#ifdef DEV_UART2			//UART2を使うとき
	RCC->APB1ENR|=RCC_APB1ENR_USART2EN;	//USART2にｸﾛｯｸ供給
#else
	RCC->APB2ENR|=RCC_APB2ENR_USART1EN;	//USART1にｸﾛｯｸ供給
#endif
#ifdef UART_REMAP			//ｼﾘｱﾙ情報USARTをﾘﾏｯﾌﾟするとき
	AFIO->MAPR|=UART_REMAP;			//USARTをﾘﾏｯﾌﾟする
#endif
	//RCC_CFGR3のUSART1SWは00(USART1ｸﾛｯｸはPCLK)初期値
	UARTx->BRR=(SYS_CLK*10000000/UARTBPS+5)/10;
						//ﾎﾞｰﾚｰﾄ設定
	UARTx->CR2=USART_CR2_STOP_1;		//ｽﾄｯﾌﾟ2ﾋﾞｯﾄ
	UARTx->CR1=USART_CR1_TE|USART_CR1_RE|USART_CR1_UE;
						//ﾃﾞｰﾀ8ﾋﾞｯﾄ､ﾊﾟﾘﾃｨ無し､送信有効､
						//　受信有効､UART有効
	GPIO_AF(UART_GPIO,UART_BIT);		//ｼﾘｱﾙ情報出力を交代機能に設定
	GPIO_AF(UART_IN_GPIO,UART_IN_BIT);	//ｼﾘｱﾙ情報入力を交代機能に設定
#endif

#ifdef DEV_L010				//L010のとき
#ifdef DEV_UART2			//UART2を使うとき
	RCC->APB1ENR|=RCC_APB1ENR_USART2EN;	//USART2にｸﾛｯｸ供給
#else
	RCC->APB2ENR|=RCC_APB2ENR_USART1EN;	//USART1にｸﾛｯｸ供給
#endif
	//RCC_CFGR3のUSART2SWは00(USART2ｸﾛｯｸはPCLK)初期値
	UARTx->BRR=(SYS_CLK*10000000/UARTBPS+5)/10;
						//ﾎﾞｰﾚｰﾄ設定
	UARTx->CR2=USART_CR2_STOP_1;		//ｽﾄｯﾌﾟ2ﾋﾞｯﾄ
	UARTx->CR1=USART_CR1_TE|USART_CR1_RE|USART_CR1_UE;
						//ﾃﾞｰﾀ8ﾋﾞｯﾄ､ﾊﾟﾘﾃｨ無し､送信有効､
						//　受信有効､UART有効
	GPIO_AF(UART_GPIO,UART_BIT,UART_AF);	//ｼﾘｱﾙ情報出力を交代機能に設定
	GPIO_AF(UART_IN_GPIO,UART_IN_BIT,UART_IN_AF);
						//ｼﾘｱﾙ情報入力を交代機能に設定
#endif

#ifdef DEV_G03x				//G03xのとき
#ifdef DEV_UART2			//UART2を使うとき
	RCC->APB1ENR|=RCC_APB1ENR_USART2EN;	//USART2にｸﾛｯｸ供給
#else
	RCC->APB2ENR|=RCC_APB2ENR_USART1EN;	//USART1にｸﾛｯｸ供給
#endif
	//RCC_CCIPRのUSART1SELは00(USART1ｸﾛｯｸはPCLK)初期値
	UARTx->BRR=(SYS_CLK*5000000/UARTBPS+5)/10;
						//ﾎﾞｰﾚｰﾄ設定
	UARTx->CR2=USART_CR2_STOP_1;		//ｽﾄｯﾌﾟ2ﾋﾞｯﾄ
	UARTx->CR1=USART_CR1_TE|USART_CR1_RE|USART_CR1_UE;
						//ﾃﾞｰﾀ8ﾋﾞｯﾄ､ﾊﾟﾘﾃｨ無し､送信有効､
						//　受信有効､UART有効
	GPIO_AF(UART_GPIO,UART_BIT,UART_AF);	//ｼﾘｱﾙ情報出力を交代機能に設定
	GPIO_AF(UART_IN_GPIO,UART_IN_BIT,UART_IN_AF);
						//ｼﾘｱﾙ情報入力を交代機能に設定
#endif
}


//==============================================
//送信完了の同期
//==============================================
static void uart_out_sync(void)
{

#ifdef DEV_F030				//F030のとき
	while(!(UARTx->ISR&USART_ISR_TC));	//送信完了を待つ
#endif

#ifdef DEV_F302				//F302のとき
	while(!(UARTx->ISR&USART_ISR_TC));	//送信完了を待つ
#endif

#ifdef DEV_F103				//F103のとき
	while(!(UARTx->SR&USART_SR_TC));	//送信完了を待つ
#endif

#ifdef DEV_L010				//L010のとき
	while(!(UARTx->ISR&USART_ISR_TC));	//送信完了を待つ
#endif

#ifdef DEV_G03x				//G03xのとき
	while(!(UARTx->ISR&USART_ISR_TC));	//送信完了を待つ
#endif
}


//==============================================
//1文字を送信
//==============================================
static void uart_out(				//戻り値無し
	unsigned char c)			//送信する文字
{

#ifdef DEV_F030				//F030のとき
	while(!(UARTx->ISR&USART_ISR_TXE));	//送信ﾃﾞｰﾀﾚｼﾞｽﾀの空きを待つ
	UARTx->TDR=c;				//1文字を送信する
#endif

#ifdef DEV_F302				//F302のとき
	while(!(UARTx->ISR&USART_ISR_TXE));	//送信ﾃﾞｰﾀﾚｼﾞｽﾀの空きを待つ
	UARTx->TDR=c;				//1文字を送信する
#endif

#ifdef DEV_F103				//F103のとき
	while(!(UARTx->SR&USART_SR_TXE));	//送信ﾃﾞｰﾀﾚｼﾞｽﾀの空きを待つ
	UARTx->DR=c;				//1文字を送信する
#endif

#ifdef DEV_L010				//L010のとき
	while(!(UARTx->ISR&USART_ISR_TXE));	//送信ﾃﾞｰﾀﾚｼﾞｽﾀの空きを待つ
	UARTx->TDR=c;				//1文字を送信する
#endif

#ifdef DEV_G03x				//G03xのとき
	while(!(UARTx->ISR&USART_ISR_TXE_TXFNF)){};
						//送信ﾃﾞｰﾀﾊﾞﾌｧの空きを待つ
	UARTx->TDR=c;				//1文字を送信する
#endif
}


//==============================================
//1文字を受信
//==============================================
static short uart_in(void)			//受信完了時は受信ﾃﾞｰﾀを返す
						//未受信時は-1を返す
{

#ifdef DEV_F030				//F030のとき
	if(UARTx->ISR&USART_ISR_RXNE) return(UARTx->RDR);
						//受信完了していたら受信ﾃﾞｰﾀを返す
	return(-1);				//未受信を返す
#endif

#ifdef DEV_F302				//F302のとき
	if(UARTx->SR&USART_SR_RXNE) return(UARTx->RDR);
						//受信完了していたら受信ﾃﾞｰﾀを返す
	return(-1);				//未受信を返す
#endif

#ifdef DEV_L010				//L010のとき
	if(UARTx->ISR&USART_ISR_RXNE) return(UARTx->RDR);
						//受信完了していたら受信ﾃﾞｰﾀを返す
	return(-1);				//未受信を返す
#endif

#ifdef DEV_G03x				//G03xのとき
	if(UARTx->ISR&USART_ISR_RXFNE) return(UARTx->RDR);
						//受信完了していたら受信ﾃﾞｰﾀを返す
	return(-1);				//未受信を返す
#endif
}
#endif
