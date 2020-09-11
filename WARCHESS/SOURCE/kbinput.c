/********************************************************************
Copyright(c)  2020 刘云笛、陈旭桐 	WARCHESS
file_name: kbinput.c
author: 刘云笛
version: 2
Description: 键盘输入
date:2020/9/9

更新日志
9.9		解决缓冲区问题（实际不在本文件中）
		将绘图函数改为自定义绘图函数（减少了一定代码量）
		实现输入满后仍能退格和回车
		两个函数合并为一个！！！

函数目录
1.kbinput:	键盘输入并显示原文
2.kbinput:	键盘输入并显示星号
******************************************************************/
#include "common.h"
#define KB_SIZE 32				//textsize:32*32
#define KB_FONT TRIPLEX_FONT	//textstyle: TRIPLE_FONT
/*
x1y2, x2y2输入框坐标
length:输入框长度
mode 1输出原文；0输出*
s 接受输入结果
*/
void kbinput(int x1, int y1, int x2, int y2, char *s, int mode)
{
	int i = 0;//已输入字数
	int length = (x2 - x1) / KB_SIZE;//最大字数
	char ch[2] = {0, 0};
	
	Mouse_savebk2();
	
	frame(x1, y1, x2, y2, 65535);	//覆盖原先文字
	s[0] = 0;				//清空原先内容
	Line64k(x1+5, y2-5-KB_SIZE, x1+5, y2-5, 44373);
	
	while(1)
	{
		Newxy();
		
		if(mouse_press(x1, y1, x2, y2) == 3)//在界面外点击，退出输入
		{
			Line64k(x1+5+i*KB_SIZE, y2-5-KB_SIZE, x1+5+i*KB_SIZE, y2-5, 65535);
			return ;
		}
		
		if(bioskey(1))
		{			
			*ch = bioskey(0);
			if(isalnum(*ch) && i < length)//输入
			{
				s[i] = *ch;
				s[i+1] = 0;
				*ch = mode * ch[0] + !mode * '*';		//两个函数合并为一个的核心语句
				Line64k(x1+5+i*KB_SIZE, y2-5-KB_SIZE, x1+5+i*KB_SIZE, y2-5, 65535);
				Outtext(x1+5+i*KB_SIZE, y2-5-KB_SIZE, ch, KB_SIZE, 40, 44373);
				i++;
				Line64k(x1+5+i*KB_SIZE, y2-5-KB_SIZE, x1+5+i*KB_SIZE, y2-5, 44373);
			}
			
			else if(*ch == '\r')//回车结束输入
			{
				Line64k(x1+5+i*KB_SIZE, y2-5-KB_SIZE, x1+5+i*KB_SIZE, y2-5, 65535);
				return ;
			}
			
			else if(*ch == '\b' && i)//退格
			{
				Line64k(x1+5+i*KB_SIZE, y2-5-KB_SIZE, x1+5+i*KB_SIZE, y2-5, 65535);
				i--;
				Bar64k(x1+5+i*KB_SIZE, y2-5-KB_SIZE, x1+5+i*KB_SIZE+KB_SIZE, y2-5-KB_SIZE+KB_SIZE, 65535);
				Line64k(x1+5+i*KB_SIZE, y2-5-KB_SIZE, x1+5+i*KB_SIZE, y2-5, 44373);
				s[i] = 0;
			}
		}
	}
}