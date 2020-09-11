/********************************************************************
Copyright(c)  2020 刘云笛、陈旭桐 	WARCHESS
file_name: draw.c
author: 陈旭桐
version: 1.2
Description: 自定义绘图函数
date:2020/9/8

更新日志
9.8		增加了阴影、边框自定义绘图函数
9.9		增加了绘制文档图形

函数目录
1.shadow/shadow_l: 			绘制带（大）阴影的文本框
2.frame: 					绘制带边框的文本框
3.filedraw:					绘制文档图形
4.Bar64kc/linec/outtextxyc：	集成前景色设置函数与普通图形函数
******************************************************************/
#include"common.h"

//x1, y1, x2, y2为对角线两点坐标
void shadow(int x1, int y1, int x2, int y2, int color1, int color2) 
{
	Bar64k(x1 + 5, y1 + 5, x2 + 5, y2 + 5, 33808);
	Bar64k(x1, y1, x2, y2, color1);
	Bar64k(x1+2, y1+2, x2, y2, color2);
	
}

void shadow_l(int x1, int y1, int x2, int y2, int color)
{
	Bar64k(x1 + 10, y1 + 10, x2 + 10, y2 + 10, 33808);
	Bar64k(x1, y1, x2, y2, color);
	rectangle64k(x1, y1, x2, y2, 33808);
}

void frame(int x1, int y1, int x2, int y2, int color)
{
	Bar64k(x1, y1, x2, y2, color);
	rectangle64k(x1 + 1, y1 + 1, x2 - 1, y2 - 1, 33808);
}

void file_draw(int x1, int y1, int x2, int y2) //x2 - x1 不能是奇数
{
	shadow_l(x1, y1, x2, y2, 65504);
	frame(x1+15, y1+15, x2-15, y2-15, 65535);

	/* 利用x2-x1为矩形长度，计算距离两边为60和80的椭圆横轴
	   纵轴为横轴一半
	   然后利用直线封口 
	   setcolor(LIGHT33808);
	ellipse((x2-x1)/2+x1, y1, 0, 180, (x2-x1)/2-60, ((x2-x1)/2-60)/2); 
	ellipse((x2-x1)/2+x1, y1, 0, 180, (x2-x1)/2-80, ((x2-x1)/2-80)/2);
	line(x2-60, y1, x2-80, y1);
	line(x1+60, y1, x1+80, y1);
	setfillstyle(1, LIGHT33808);
	floodfill((x2-x1)/2+x1,y1-(((x2-x1)/2-80)/2)-5,LIGHT33808);*/
}

void outtextxyc(int x1, int y1, char *s, int color)
{
	setcolor(color);
	outtextxy(x1, y1, s);
}

//通过两点横坐标输出汉字省去计算part的时间，同时返回part值
int Outtextxx(int x1, int y, int x2, char *s,int flag, int color)
{
	int num = strlen(s) / 2;				//汉字个数
	int part = (x2-x1-flag) / (num - 1);	//由汉字数组长度 length = x2 - x1 = flag + (num - 1) X part 算出。
	Outtext(x1, y, s, flag, part, color);
	return part;
}