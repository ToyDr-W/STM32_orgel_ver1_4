//==============================================
//PWM周期割込み処理
//==============================================
void TIM2_IRQHandler(void)
{
	short pcm;				//PCM値

//DBS_H;					//割込み処理開始を表示する
	TIM2->SR=0;				//割込みﾌﾗｸﾞをｸﾘｱする

	//PCM値を取得
	if(buf_read==buf_write) return;		//ﾃﾞｰﾀが無かったら飛ばす
	pcm=buf[buf_read];			//PCM値を取出す
	if(buf_read==(OUT_BUF_N-1)) buf_read=0;	//ﾊﾞｯﾌｧの後尾になったら先頭に戻す
	else buf_read++;			//ﾊﾞｯﾌｧを進める

	//ﾃﾞｭｰﾃｨｻｲｸﾙを設定
#if BTL==0				//ｼﾝｸﾞﾙ出力のとき
	TIM2->CCR4=((long)pcm+32768)*PWM_STEP/65536;
						//正相を設定する
#endif

#if BTL==1				//ｺﾝﾌﾟﾘ出力のとき
	TIM2->CCR4=TIM2->CCR2=((long)pcm+32768)*PWM_STEP/65536;
						//正相(逆相)を設定する
#endif

#if BTL>1					//ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ出力のとき
	if(pcm>0)				//正値のとき
	{
		TIM2->CCR2=0;			//逆相を止める
		TIM2->CCR4=(long)pcm*PWM_STEP/32768;
						//正相を設定する
	}
	else if(pcm<0)				//負値のとき
	{
		TIM2->CCR4=0;			//正相を止める
		TIM2->CCR2=(long)(-pcm)*PWM_STEP/32768;
						//逆相を設定する
	}
	else					//停止のとき
	{
		TIM2->CCR4=0;			//正相を止める
		TIM2->CCR2=0;			//逆相を止める
	}
#endif
//DBS_L;					//割込み処理終了を表示する
}


//==============================================
//PWM周期を待つ
//==============================================
static void wait_pwm(void)
{

	TIM2->SR=0;				//ｲﾍﾞﾝﾄﾌﾗｸﾞをｸﾘｱする
	while(!(TIM2->SR&TIM_SR_UIF));		//更新ｲﾍﾞﾝﾄを待つ
}


#ifdef	SLEEP_EN			//Sleep機能を実装するとき
//==============================================
//PWMを停止
//==============================================
static void dev_pwm_stop(void)
{

//DBG_C('\n');
//DBG_C('s');
	//DC更新を待つ
//DBG_C('1');
	wait_ms(10);				//10ms間はPWM割込み処理を回す

	//PWMを停止
//DBG_C('2');
	TIM2->DIER=0;				//TIM2割込み禁止

	//ﾃﾞｭｰﾃｨｻｲｸﾙを設定
#if BTL==0				//ｼﾝｸﾞﾙ出力のとき
//DBG_C('3');
	while(TIM2->CCR4)			//DCが0になるまで繰返す
	{
		TIM2->CCR4--;			//ﾗﾝﾌﾟ下降
		wait_pwm();			//PWM周期を待つ
	}
#endif

#if BTL==1				//ｺﾝﾌﾟﾘ出力のとき
//DBG_C('4');
	TIM2->CCR4=TIM2->CCR2=PWM_STEP/2;	//DCを50%にする
	wait_pwm();				//PWM周期を待つ
	TIM2->CCER=0;				//PWM出力を止める
#endif

#if BTL>1				//ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ出力のとき
//DBG_C('5');
	TIM2->CCR4=TIM2->CCR2=0;		//DCを0%にする
	wait_pwm();				//PWM周期を待つ
#endif

#if BTL==3				//ﾌﾞﾚｰｷ出力のとき
	TIM2->CCER&=~(TIM_CCER_CC4P|TIM_CCER_CC2P);
						//正相と逆相をｱｸﾃｨﾌﾞHにする
#endif
//DBG_C('6');
}
#endif


//==============================================
//PWMを再開
//==============================================
static void dev_pwm_start(void)
{

//DBG_C('\n');
//DBG_C('r');
	//ﾃﾞｭｰﾃｨｻｲｸﾙを設定
#if BTL==0				//ｼﾝｸﾞﾙ出力のとき
//DBG_C('1');
	while(TIM2->CCR4++<PWM_STEP/2)		//ﾗﾝﾌﾟ上昇
	{
		wait_pwm();			//PWM周期を待つ
	}
#endif

#if BTL==1				//ｺﾝﾌﾟﾘ出力のとき
//DBG_C('2');
	TIM2->CCER=TIM_CCER_CC4E|TIM_CCER_CC2E|TIM_CCER_CC2P;
						//CH4をｱｸﾃｨﾌﾞL､CH2をｸﾃｨﾌﾞHでﾋﾟﾝ出力
#endif

#if BTL==3				//ﾌﾞﾚｰｷ出力のとき
//DBG_C('3');
	TIM2->CCER|=TIM_CCER_CC4P|TIM_CCER_CC2P;//CH4とCH2をｱｸﾃｨﾌﾞLにする
#endif

	//PWMを再開
//DBG_C('4');
	TIM2->DIER=TIM_DIER_UIE;		//TIM2更新割込み許可
//DBG_C('5');
}


//==============================================
//PWM初期設定
//==============================================
static void pwm_init(void)
{

//DBG_C('p');

	//PWM設定(TIM2)
	RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;	//TIM2にｸﾛｯｸ供給

#if BTL==2				//ﾌﾞﾘｯｼﾞ出力のとき
	TIM2->ARR=PWM_STEP-1+PWM_DB_BRG;	//PWM周期(ﾃﾞｯﾄﾞﾊﾞﾝﾄﾞを考慮)
#else
	TIM2->ARR=PWM_STEP-1;			//PWM周期
#endif
	TIM2->CNT=0;				//ｶｳﾝﾀをｸﾘｱ
	TIM2->CR1|=TIM_CR1_CEN;			//ｶｳﾝﾀ有効
//DBG_C('t');

	//PWM設定(TIM2_CH4)
	TIM2->CCMR2|=(TIM_CCMR2_OC4M_2|TIM_CCMR2_OC4M_1|TIM_CCMR2_OC4PE);
						//CH4をPWMﾓｰﾄﾞ1､ﾌﾟﾘﾛｰﾄﾞ有効
	TIM2->CCER|=TIM_CCER_CC4E;		//CH4をﾋﾟﾝ出力
	GPIO_AF(PWM_F_GPIO,PWM_F_BIT,PWM_F_AF);	//PWM正相を交代機能に設定

#if BTL>0				//ｺﾝﾌﾟﾘ､ﾌﾞﾘｯｼﾞ､ﾌﾞﾚｰｷ出力のとき
					// CH2を使う
	//PWM設定(TIM2_CH2)
//DBG_C('c');
	TIM2->CCMR1|=(TIM_CCMR1_OC2M_2|TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2PE);
						//CH2をPWMﾓｰﾄﾞ1､ﾌﾟﾘﾛｰﾄﾞ有効
	GPIO_AF(PWM_R_GPIO,PWM_R_BIT,PWM_R_AF);	//PWM逆相を交代機能に設定
#endif

#if BTL==1				//ｺﾝﾌﾟﾘ出力のとき
	TIM2->CCER|=TIM_CCER_CC2P;		//CH2をｱｸﾃｨﾌﾞL
	TIM2->CCR4=PWM_STEP/2;			//CH4のDCを50%
	TIM2->CCR2=PWM_STEP/2;			//CH2のDCを50%
#endif

#if BTL>1				//ﾌﾞﾘｯｼﾞ､ﾌﾞﾚｰｷ出力のとき
	TIM2->CCER|=TIM_CCER_CC2E;		//CH2ををﾋﾟﾝ出力
#endif

//DBG_C('q');

	//PWM周期割込み設定
	NVIC->ISER[TIM2_IRQn/16]=(1<<(TIM2_IRQn&15));
						//TIM2割込み許可
//DBG_C('i');
	dev_pwm_start();			//PWMを開始する
}
