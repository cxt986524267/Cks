/*****************本文件来自两位学长的工作，不计入总代码量******************************/

/*********************************************************************************
Copyright(c)  2018 孙兆锦&郭浩南  夺岛演习仿真模拟系统
file_name: outtext.c
data: 2019.9.1
version: 3.1
Description: 汉字库
data :	2017/8/13
			version 1.0 版本调试完成
		2017/8/17
				优化算法结构
				
		2017/8/30
				添加输出ASC码功能
		2017/10/26
			将汉字输出和ASC码输出融为一体
******************************************************************/

#include "common.h"

/*********************************************************
调用函数
	void Putpixel64k(int x, int y, unsigned char color);
	
**************************************************/
void Outtext(int x,int y, char *s, int size, int part, int color)
{
	FILE *hzk_p;
	unsigned char quma,weima;	
	unsigned long offset;		
	int i,j,pos;
	int shift;
	unsigned char mask[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	
	switch(size)
	{
		case 16:
			{			
				char mat[32];													//建立32字节的字符数组储存信息
				hzk_p=fopen("HZK\\HZK16.dat","rb");					//打开汉字库文件
				
				if(hzk_p==NULL)
				{
					printf("\n Can't open file HZK ! Press any key to return.");
					getch();
					exit(1);
				}
				while(*s!='\0')													//当字符串没有结束时
				{
				
					if(*s>0&&*s<127)//如果字符串中出现非中文
					{
						weima=*s-32;//读取位码
						offset=(94*2+weima-1)*32l;
						shift=1;
					}
					else
					{
						quma=s[0]-128-32;
						weima=s[1]-128-32;											//计算区位码
						offset=(94 * (quma-1) + (weima-1)) * 32L;
						shift=2;
					}
					fseek(hzk_p,offset,0);
					fread(mat,32,1,hzk_p);
					for(i=0;i<16;i++)
					{
						pos=2*i;
						for(j=0;j<16;j++)
						{
							if(( mask[j%8]&mat[pos+j/8] ) != NULL)
								Putpixel64k(x + j ,y + i ,color);		
						}
					
					}
					s+=shift;
					x+=part;
						
				}
				fclose(hzk_p);
				break;
			}
			
			
			
			
			
		case 24:
			{		
					char mat[72];
					hzk_p=fopen("HZK\\HZK24.dat","rb");
					if(hzk_p==NULL)
					{
						printf("\n Can't open file HZK ! Press any key to return.");
						getch();
						exit(1);
					}
					while(*s!='\0')
					{
						if(*s>0&&*s<127)//如果字符串中出现非中文
						{
							weima=*s-32;//读取位码
							offset=(94*2+weima-1)*72l;
							shift=1;
						}
						else
						{
							quma=s[0]-128-32;
							weima=s[1]-128-32;
							offset=(94 * (quma-1) + (weima-1)) * 72L;
							shift=2;
						}
						fseek(hzk_p,offset,0);
						fread(mat,72,1,hzk_p);
						for(i=0;i<24;i++)
						{
							pos=3*i;
							for(j=0;j<24;j++)
							{
								if(( mask[j%8]&mat[pos+j/8] ) != NULL)
									Putpixel64k(x + j ,y + i  ,color);		
							}
						
						}
						s+=shift;
						x+=part;
							
					}
					fclose(hzk_p);
					break;
			
			}
				
		case 32:
			{		
					char mat[128];
					hzk_p=fopen("HZK\\HZK32K","rb");
					if(hzk_p==NULL)
					{
						printf("\n Can't open file HZK ! Press any key to return.");
						getch();
						exit(1);
					}
					while(*s!='\0')
					{
						if(*s>0&&*s<127)//如果字符串中出现非中文
						{
							weima=*s-32;//读取位码
							offset=(94*2+weima-1)*128l;
							shift=1;
						}
						else
						{
							quma=s[0]-128-32;
							weima=s[1]-128-32;
							offset=(94 * (quma-1) + (weima-1)) * 128L;
							shift=2;
						}
						fseek(hzk_p,offset,0);
						fread(mat,128,1,hzk_p);
						for(i=0;i<32;i++)
						{
							pos=4*i;
							for(j=0;j<32;j++)
							{
								if(( mask[j%8]&mat[pos+j/8] ) != NULL)
									Putpixel64k(x + j ,y + i  ,color);			
						
							}
						}
						s+=shift;
						x+=part;
							
					}
					fclose(hzk_p);
					break;
			}
			
		
			
		case 48:
			{		
				char mat[288];
				hzk_p=fopen("HZK\\HZK48K","rb");
				if(hzk_p==NULL)
				{
					printf("\n Can't open file HZK ! Press any key to return.");
					getch();
					exit(1);
				}
				while(*s!='\0')
				{
					if(*s>0&&*s<127)//如果字符串中出现非中文
					{
						weima=*s-32;//读取位码
						offset=(94*2+weima-1)*288l;
						shift=1;
					}
					else
					{
						quma=s[0]-128-32;
						weima=s[1]-128-32;
						offset=(94 * (quma-1) + (weima-1)) * 288L;
						shift=2;
					}
					fseek(hzk_p,offset,0);
					fread(mat,288,1,hzk_p);
					for(i=0;i<48;i++)
					{
						pos=6*i;
						for(j=0;j<48;j++)
						{
							if(( mask[j%8]&mat[pos+j/8] ) != NULL)
								Putpixel64k(x + j ,y + i  ,color);		
						}
					}
					s+=shift;
					x+=part;
						
				}
				fclose(hzk_p);
				break;
			}
	}
}

void out_ASC(int x, int y, char *s, int color)
{
	FILE *hzk_p;
	unsigned long offset;		
	int i,j;
	char mat[16];													//建立32字节的字符数组储存信息
	hzk_p=fopen("HZK\\ASC16","rb");					//打开汉字库文件
	if(hzk_p==NULL)
	{
		printf("\n Can't open file HZK ! Press any key to return.");
		getch();
		exit(1);
	}
	while(*s!='\0')
	{
		offset=(*s)*16+1;			 //计算该字符在字库中偏移量
		fseek(hzk_p,offset,SEEK_SET);
		fread(mat,16,1,hzk_p); 			//取出字符16字节的点阵字模存入buffer中
		for(i=0;i<16;i++)
		{
			for(j=0;j<8;j++)
			{
				if(((mat[i]>>(7-j))&0x1)!=NULL)
				{
					Putpixel64k(x+j,y+i,color);
				}
			}
		}

		s++; //一个字符内码占用一个字节
		x+=8;//字符间间隔
	}	
	fclose(hzk_p);
	
		
}


void out_ASCplus(int x, int y, char *s, int color)
{
	FILE *hzk_p;
	unsigned long offset;		
	int i,j,m,n;
	char mat[16];													//建立32字节的字符数组储存信息
	hzk_p=fopen("HZK\\ASC16","rb");					//打开汉字库文件
	if(hzk_p==NULL)
	{
		printf("\n Can't open file HZK ! Press any key to return.");
		getch();
		exit(1);
	}
	while(*s!='\0')
	{
		offset=(*s)*16+1;			 //计算该字符在字库中偏移量
		fseek(hzk_p,offset,SEEK_SET);
		fread(mat,16,1,hzk_p); 			//取出字符16字节的点阵字模存入buffer中
		for(i=0;i<16;i++)
		{
			for(j=0;j<8;j++)
			{
				if(((mat[i]>>(7-j))&0x1)!=NULL)
				{
					for(m=0;m<2;m++)
					{
						for(n=0;n<2;n++)
						{
								Putpixel64k(x + j * 2 + m,y + i * 2 + n,color);
						}
					}
				}
			}
		}

		s++; //一个字符内码占用一个字节
		x+=20;//字符间间隔
	}	
	fclose(hzk_p);
	
}


int Outtext24(int x1,int y1,int x2,int y2, char *s, int part, int color)
{
	FILE *hzk_p;
	unsigned char quma,weima;	
	unsigned long offset;		
	int i,j,pos;
	int shift;
	int x = x1+54,y = y1;
	int k = 0,state = 0;
	unsigned char mask[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	
	char mat[72];
	hzk_p=fopen("HZK\\HZK24.dat","rb");
	if(hzk_p==NULL)
	{
		printf("\n Can't open file HZK ! Press any key to return.");
		getch();
		exit(1);
	}
	while(*s!='\0')
	{
		if(*s>0&&*s<127)//如果字符串中出现非中文
		{
			weima=*s-32;//读取位码
			offset=(94*2+weima-1)*72l;
			shift=1;
		}
		else
		{
			quma=s[0]-128-32;
			weima=s[1]-128-32;
			offset=(94 * (quma-1) + (weima-1)) * 72L;
			shift=2;
		}
		fseek(hzk_p,offset,0);
		fread(mat,72,1,hzk_p);
		for(i=0;i<24;i++)
		{
			pos=3*i;
			for(j=0;j<24;j++)
			{
				if(( mask[j%8]&mat[pos+j/8] ) != NULL)
					Putpixel64k(x + j ,y + i  ,color);		
			}
		}
		s+=shift;
		k+=shift; 
		x+=part;
		if(x>x2-24)
		{
			x = x1;
			y = y+27;
			if(y >y2-24)
			{
				state = k;
				break;
			}
		}
	}
	fclose(hzk_p);	
	return state;
}


unsigned long puthz(int x0,int y0,char *path,unsigned long len,int page)	//从len开始显示txt文件里整段文字信息，并返回下一页首个字的offset
{
	FILE *user;
	unsigned long lg=0;				//用于计算文件总长度，结束for循环
	char s[3]={0};					//每次读取2个字节，第3位'\0'表示结束
	int x,y;					//显示字体的坐标
	user=fopen(path,"r");				//判断能否打开txt文件
	if(user==NULL)  exit(1);
	fseek(user,0,2);					//fseek到文件尾，读取总长度
	lg=ftell(user);
	fseek(user,len,0);					//回到len处开始输出
	if(page==1) x=x0+16*2;
	if(page>1)  x=x0+16*2;
	for(y=y0;len<lg;len=ftell(user))
	{
		if((x+16)>=992)				//判断行输出是否超出界面
		{
			y+=24;                //上下间隔24
			x=x0;
		}
		if((y+24)>=768)				//判断行数是否达到最大
		{
			lg=ftell(user);			//读取当前offset，返回
			fclose(user);			//fclose避免文件损毁
			return lg;
		}
		fread(s,1,2,user);				//读取2个字节
		if(s[0]=='\n')				//当s[0]为'\n'即表示换行
		{
			fseek(user,-1,1);			//因为已读取，即从当前位置返回一个字节
			y+=24;
			x=x0+16*2;
			continue;
		}
	
		if((s[0]>=0)&&(s[0]<='z'))		//读取的为英文输入
		{
		  ENtext16(x,y,s[0],0);
		  fseek(user,-1,1);			//仅仅输出了一位，则从当前位置返回一位
		  x+=8;
		}
		else					//读取的为中文，输出一个汉字
		{
			CHtext16(x,y,s,0);
			x+=16;
		}
	}
	fclose(user);					//fclose文件，避免文件损坏
	return 0;					//文件已读完，返回0作为无下一页标志
}
//////////////////////////////////////////////////////////////////////////////


void CHtext16(   int x,int y,char *s,int color)		//输出一个汉字的函数
{
	
	FILE *hzk;
	int i,j,k;
	unsigned char qh,wh;					//区码位码定义					//weima
 	unsigned long offset;					//偏移量定义
 	char buffer[32];					//32为字模定义


	if ((hzk=fopen("HZK\\HZK16.dat","rb"))==NULL)		//判断汉字库能否打开
	{
		
        fclose(hzk);
		printf("\n Can't open file HZK ! Press any key to return.");
        getch();
		exit(1);
	}
	
	qh=s[0]-0xa0;						//计算区码位码，即-0xa0
	wh=s[1]-0xa0;	
	offset=(94*(qh-1)+(wh-1))*32L; 				//计算位移量，
	fseek(hzk,offset,0);
	fread(buffer,32,1,hzk);           			//读出字模信息
	
	for(i=0;i<16;i++)
	{
		for(j=0;j<2;j++)
		{
			for(k=0;k<8;k++)
			{
				if((buffer[i*2+j]>>(7- k))&0x1)  //为1的位显示
				{
					Putpixel64k(x+k+j*8,y+i,color);	//打点函数
				}
			}
		}
	}
	fclose(hzk);
}
///////////////////////////////////////////////////////////////////////////
void ENtext16( int x,int y,char s,int color)		//输出一个英文函数
{

	FILE *asc;
	int i,j;
	unsigned long offset;
	 char buffer[16];

	if ((asc=fopen("HZK\\ASC16","rb"))==NULL)
	{
		printf("\n Can't open file HZK ! Press any key to return.");
		getch();
		exit(1);
	}

	
	offset=s*16L;                         	//计算位移量
	fseek(asc,offset,0);
	fread(buffer,16,1,asc);                        	//读出字模信息
	
	for(i=0;i<16;i++)
	{
		for(j=0;j<8;j++)
		{
			if((buffer[i]>>(7-j))&0x1)    	//为1的位显示
			{
				Putpixel64k(x+j,y+i,color);
			}
		}
	}
	fclose(asc);
}
///////////////////////////////////////
