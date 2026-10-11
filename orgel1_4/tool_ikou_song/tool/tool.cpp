//#define SHDBG		//ﾃﾞﾊﾞｸﾞﾛｸﾞ(log.log)を出力するときに宣言する

//【実行形態】
//Win32ｺﾝｿｰﾙｱﾌﾟﾘｹｰｼｮﾝ
//
//【機能】
//songﾌｧｲﾙをCH32Vorgel1_1からSTM32orgel1_3へ移行する
//
//【ﾊﾟﾗﾒｰﾀ】
//P1:曲名
//P2:元ﾌｧｲﾙのﾃﾞｨﾚｸﾄﾘ
//P3:先ﾌｧｲﾙのﾃﾞｨﾚｸﾄﾘ
//
//【改変履歴】
//2024.4.3	新規作成


#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <share.h>
#include <stdlib.h>


//======================================
//規定値
//======================================
#define MAX_LEN 1000


////////////////////////////////////////
//ここからﾃﾞﾊﾞｸﾞ用の記述
////////////////////////////////////////


//======================================
//ﾃﾞﾊﾞｸﾞﾛｸﾞを出力するﾏｸﾛ
//======================================
#define LOGFNAME "log.log"
#ifdef SHDBG
	#define Hm(x) {SHDBGm(__FILE__,__LINE__,(char *)(x));}
	#define Hd(x) {SHDBGd(__FILE__,__LINE__,#x,(int)(x));}
	#define Hld(x) {SHDBGld(__FILE__,__LINE__,#x,(long)(x));}
	#define Hu(x) {SHDBGu(__FILE__,__LINE__,#x,(unsigned int)(x));}
	#define Hlu(x) {SHDBGlu(__FILE__,__LINE__,#x,(unsigned long)(x));}
	#define Hx(x) {SHDBGx(__FILE__,__LINE__,#x,(unsigned int)(x));}
	#define Hlx(x) {SHDBGlx(__FILE__,__LINE__,#x,(unsigned long)(x));}
	#define Hs(x) {SHDBGs(__FILE__,__LINE__,#x,(char *)(x));}
	#define Hdump(x,l) {SHDBGdump(__FILE__,__LINE__,#x,(unsigned char *)(x),l);}
#else
	#define Hm(x)
	#define Hd(x)
	#define Hld(x)
	#define Hu(x)
	#define Hlu(x)
	#define Hx(x)
	#define Hlx(x)
	#define Hs(x)
	#define Hdump(x,l)
#endif

//======================================
//ﾛｸﾞﾌｧｲﾙへ出力する
//======================================
static void SHDBGlog(
	char *log)							//ﾛｸﾞ内容
{
	int fd;

//	if((fd=open(LOGFNAME,O_RDWR|O_APPEND|O_CREAT,07666))==-1)
	if((_sopen_s(&fd,LOGFNAME,O_RDWR|O_APPEND|O_CREAT,_SH_DENYNO,_S_IREAD|_S_IWRITE))==-1)
										//ﾛｸﾞﾌｧｲﾙをｵｰﾌﾟﾝする
	{
		return;
	}
	_write(fd,log,strlen(log));			//ﾛｸﾞを書出す
	_close(fd);
}

//======================================
//ﾛｸﾞ編集(ﾒｯｾｰｼﾞ表示のみ)
//======================================
static void SHDBGm(
	char *file,
	int line,
	char *msg)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%.64s\n",file,line,msg);
	SHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(%d表示)
//======================================
static void SHDBGd(
	char *file,
	int line,
	char *name,
	int value)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=%d\n",file,line,name,value);
	SHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(%u表示)
//======================================
static void SHDBGu(
	char *file,
	int line,
	char *name,
	unsigned int value)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=%u\n",file,line,name,value);
	SHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(%x表示)
//======================================
static void SHDBGx(
	char *file,
	int line,
	char *name,
	unsigned int value)
{
	char s[128];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=0x%x\n",file,line,name,value);
	SHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(%s表示)
//======================================
static void SHDBGs(
	char *file,
	int line,
	char *name,
	char *value)
{
	char s[192];

	sprintf_s(s,sizeof(s),"%.32s %d:%s=%.128s\n",file,line,name,value);
	SHDBGlog(s);
}

//======================================
//ﾛｸﾞ編集関数(16進ﾀﾞﾝﾌﾟ表示)
//======================================
static void SHDBGdump(
	char *file,
	int line,
	char *name,
	unsigned char *data,
	int len)
{
	char s[128],*p;
	int n,m,l;

	sprintf_s(s,sizeof(s),"%.32s %d:%s(%x)%d\n",file,line,name,data,len);
	SHDBGlog(s);
	for(n=0;n<len;n+=16)
	{
		sprintf_s(s,sizeof(s),"[%04x]",n);
		p=s+6;
		if((m=len-n)>16) m=16;
		for(l=0;l<m;l++)
		{
			if(l==8) *p++='-';
			else if(l) *p++=' ';
			sprintf_s(p,3,"%02x",*data++);
			p+=2;
		}
		*p++='\n';
		*p='\0';
		SHDBGlog(s);
	}
}


////////////////////////////////////////
//ここからｱﾌﾟﾘｹｰｼｮﾝの記述
////////////////////////////////////////

//======================================
//ﾒｲﾝ処理
//======================================
int main(int argc,char *argv[])
{
	char i_fname[128];	//入力ﾌｧｲﾙ名
	char o_fname[128];	//出力ﾌｧｲﾙ名
	FILE *i_fp,*o_fp;	//ﾌｧｲﾙﾎﾟｲﾝﾀ
	char i_buf[MAX_LEN+10];
	char o_buf[MAX_LEN+20];
	int rec_n=0;
	int sw_put;
	char *p,*q;
	char n=0;

	//ﾊﾟﾗﾒｰﾀ取り込み
	/*DBG*/Hd(argc);
	if(argc<=1)
	{
		printf("【ﾊﾟﾗﾒｰﾀ】曲名 元ﾌｧｲﾙのﾃﾞｨﾚｸﾄﾘ 先ﾌｧｲﾙのﾃﾞｨﾚｸﾄﾘ\n");
		return(9);
	}

	//入力ﾌｧｲﾙのﾊﾟｽ名
	strcpy_s(i_fname,sizeof(i_fname),argv[2]);
	strcat_s(i_fname,sizeof(i_fname),"song_");
	strcat_s(i_fname,sizeof(i_fname),argv[1]);
	strcat_s(i_fname,sizeof(i_fname),".asm");
	/*DBG*/Hs(i_fname);

	//出力ﾌｧｲﾙのﾊﾟｽ名
	strcpy_s(o_fname,sizeof(o_fname),argv[3]);
	strcat_s(o_fname,sizeof(o_fname),"song_");
	strcat_s(o_fname,sizeof(o_fname),argv[1]);
	strcat_s(o_fname,sizeof(o_fname),".asm");
	/*DBG*/Hs(o_fname);

	//入力ﾌｧｲﾙのｵｰﾌﾟﾝ
	if(fopen_s(&i_fp,i_fname,"r"))
	{
		printf("入力ﾌｧｲﾙのfopen()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
		return(1);
	}

	//出力ﾌｧｲﾙのｵｰﾌﾟﾝ
	if(fopen_s(&o_fp,o_fname,"w"))
	{
		printf("出力ﾌｧｲﾙのfopen()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
		return(1);
	}

	for(;;)
	{

		//入力ﾌｧｲﾙから読み込む
		if(fgets(i_buf,MAX_LEN,i_fp)==NULL)
		{
			if(ferror(i_fp))
			{
				printf("入力ﾌｧｲﾙのfgets()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
				return(9);
			}
			if(feof(i_fp)) break;
		}
		rec_n++;

		//行末の改行を確認
		n=strnlen_s(i_buf,sizeof(i_buf));
		if(i_buf[n-1]!='\n')
		{
			i_buf[n]='\n';
			i_buf[n+1]=0;
		}

		//onpu以外の#includeをｺﾒﾝﾄｱｳﾄする
		if(p=strstr(i_buf,"#include"))
		{
			if(strstr(i_buf,"onpu_")||strstr(i_buf,"song_"))
			{
				q=p;
				while(*q)
				{
					if(*q=='#') *p++='\t';
					else if(*q=='"');
					else *p++=*q;
					q++;
				}
				*p=0;
			}
			else
			{
				sprintf_s(o_buf,sizeof(o_buf),
					";%s",i_buf);
				strcpy_s(i_buf,o_buf);
			}
		}

		//ｺﾒﾝﾄを;に替える
		p=q=i_buf;
		while(*p)
		{
			if((*p=='/')&&(*(p+1)=='/'))
			{
				*q++=';';
				p+=2;
			}
			else
			{
				*q++=*p++;
			}
		}
		*q=0;

		sw_put=1;

		if(i_buf[0]==';')
		{
			sprintf_s(o_buf,sizeof(o_buf),"%s",i_buf);
		}
		else if(strstr(i_buf,"#include"))
		{
			p=i_buf;
			while(*p++!='"');
			q=p;
			while(*p!='.') p++;
			*p=0;
			sprintf_s(o_buf,sizeof(o_buf),
				"\tif\t:def:%s\n"
				"\telse\n"
				"\tinclude\t%s.asm\n"
				"\tendif\n",
				q,q);
		}
		else if(i_buf[0]=='#') continue;
		else if(strstr(i_buf,"bgn_mac"))
		{
			sprintf_s(o_buf,sizeof(o_buf),"\tbgn_mac\n");
		}
		else if(strstr(i_buf,"end_mac"))
		{
			sprintf_s(o_buf,sizeof(o_buf),"\tend_mac\n");
		}
		else if((i_buf[0]=='/')&&(i_buf[1]=='/'))
		{
			o_buf[0]=';';
			sprintf_s(o_buf+1,sizeof(o_buf)-1,"%s",i_buf+2);
		}
		else
		{
			p=i_buf;
			q=o_buf;
			while(*p)
			{
				if(*p=='(')
				{
					*q++=' ';
					p++;
				}
				else if(*p==')')
				{
					p++;
				}
				else if(*p==':')
				{
					p++;
				}
				else
				{
					*q++=*p++;
				}
			}
			*q=0;
		}

		//出力ﾌｧｲﾙへ書き出す
		if(sw_put)
		{
			if(fputs(o_buf,o_fp)<0)
			{
				printf("出力ﾌｧｲﾙのfputs()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
				return(9);
			}
		}
	}

	//最後のendを書く
	if(fputs("\tend\n",o_fp)<0)
	{
		printf("出力ﾌｧｲﾙのfputs()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
		return(9);
	}

	//出力ﾌｧｲﾙのｸﾛｰｽﾞ
	if(fclose(o_fp))
	{
		printf("出力(ｱｾﾝﾌﾞﾗｿｰｽ)ﾌｧｲﾙのfclose()ｴﾗｰ fname:%s errno:%d\n",o_fname,errno);
		return(9);
	}

	//入力ﾌｧｲﾙのｸﾛｰｽﾞ
	if(fclose(i_fp))
	{
		printf("入力(10進txt形式)ﾌｧｲﾙのfclose()ｴﾗｰ fname:%s errno:%d\n",i_fname,errno);
		return(9);
	}

	printf("%d行処理しました\n",rec_n);
	return(0);
}
