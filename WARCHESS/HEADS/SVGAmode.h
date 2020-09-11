/**********************************************************
Copyright (C) 2018  孙兆锦&郭浩南  夺岛演习仿真模拟系统 
File name:		SVGAmode.c
Author:			孙兆锦&郭浩南
Version：		3.2
Date：			2017/9/3
Description：	SVGA显示模式实现文件，包括该模式下基本的初始化函数、操作函数、画图函数、读图函数等的实现。
				宏处理在SVGAmode.h中。
				
				因为宏定义的缘故，严禁在其他文件中定义名称为R、G、B、r、g、b的变量！
				为了与graphics.h中的函数区别，所有自写函数名首字母大写！
				所有自写函数均以屏幕参考系为准！
				
				Putbmp16函数采用其他同学代码，函数Line、Circle、Circlefill为资料代码，不计入工作量！
				
				对本文件的修改请及时按如下格式记录在下面（每个修改单独列出）！

Function List：
	1.void SetSVGA256(void);
		SVGA显示模式设置函数，设为0x105
		
	2.void SetSVGA64k(void);
		SVGA显示模式设置函数，设为0x117
	
	3.unsigned int GetSVGA(void);
		获取SVGA显示模式号bx
		
	4.void Selectpage(register char page);
		带判断功能的换页函数，解决读写显存时跨段寻址问题
		
		
	6.void Putpixel64k(int x, int y, unsigned int color);
		画点函数，其他画图函数的基础，仅适用于0x117模式！
		
		
	8.int Putbmp64k(int x, int y, const char * path);
		24位非压缩bmp图定位显示函数
	
	9.void Xorpixel(int x, int y, unsigned char color);
		按位异或画点函数
		
	10.void Horizline(int x, int y, int width, unsigned char color);
		画水平线函数
		
	12.void Line64k(int x1, int y1, int x2, int y2, unsigned int color)；
		64k模式画一般直线函数
		
	14.void Bar64k(int x1, int y1, int x2, int y2, unsigned int color);
		64k模式画矩形块函数
		
	15.void rectangle64k(int x1, int y1, int x2, int y2, unsigned int color);
		64k模式画矩形框函数
		
	16.void Circle64k(int xc, int yc, int radius, unsigned int color);
		64k模式画圆圈函数
		
	18.void Circlefill64k(int xc, int yc, int radius, unsigned int color);
		64k模式画实心圆函数
	
	19.unsigned int Getpixel64k(int x, int y);
		取点颜色函数，仅适用于0x117模式！
	
	20.void Circlehalf64k(int xc, int yc, int radius, unsigned int color);
		画上半圆的函数
		
	21.void Circleright64k(int xc, int yc, int radius, unsigned int color);
		画右半圆圈
		
	22.void Circleleft64k(int xc, int yc, int radius, unsigned int color);
		画左半圆圈
		
	23.int Linex(int x1, int y1, int x2, int y2, int color);
		画水平直线

	24.int Liney(int x1, int y1, int x2, int y2, int color);
		画垂直直线
		
	25.void Circleup64k(int xc, int yc, int radius, unsigned int color);
		画上半圆弧
		
	26.void Circlerd64k(int xc, int yc, int radius, unsigned int color);
		画右下半圆弧
		
	27.void Bar64k_radial_re(int x1, int y1, int x2, int y2, unsigned int color, int fill_time);
		从外到内放射状填充bar，最后一个参数为总延迟时间，可以为0（单位ms）
		
	28.void Bar64k_radial(int x1, int y1, int x2, int y2, unsigned int color, int fill_time);
		从内到外放射状填充bar，最后一个参数为总延迟时间，可以为0（单位ms）
**********************************************************/

#ifndef	SVGA_H
#define	SVGA_H

#include "common.h"

/*画图函数调色板颜色结构*/
typedef struct
{
	unsigned char R;	/*红色分量，RED缩写*/
	unsigned char G;	/*绿色分量，GREEN缩写*/
	unsigned char B;	/*蓝色分量，BLUE缩写*/
} COLORS;

/*24位bmp图颜色结构*/
typedef struct
{
	unsigned char B;	/*蓝色分量，BLUE缩写*/
	unsigned char G;	/*绿色分量，GREEN缩写*/
	unsigned char R;	/*红色分量，RED缩写*/
} COLORS24;

/*bmp位图颜色表颜色结构*/
typedef struct tagRGBQUAD
{
    unsigned char b;	/*蓝色分量，RED缩写*/
	unsigned char g;	/*绿色分量，GREEN缩写*/
	unsigned char r;	/*红色分量，BLUE缩写*/
	unsigned char reserved;		/*保留字*/
} RGBQUAD;

/*bmp位图信息头数据用于说明位图的尺寸等信息*/
typedef struct tagBITMAPINFOHEADER
{
	unsigned long int biSize;	/*本结构所占用字节数（15-18字节）*/
	long int biWidth;			/*位图的宽度，以像素为单位（19-22字节）*/
	long int biHeight;			/*位图的高度，以像素为单位（23-26字节）*/
	unsigned int biPlanes;		/*目标设备的级别，必须为1(27-28字节）*/
	unsigned int biBitCount;	/*每个像素所需的位数，必须是1（双色），4(16色），8(256色）16(高彩色)或24（真彩色）之一（29-30字节）*/
	unsigned long int biCompression;	/*位图压缩类型，必须是0（不压缩），1(BI_RLE8压缩类型）或2(BI_RLE4压缩类型）之一（31-34字节）*/
	unsigned long int biSizeImage;		/*位图的大小(其中包含了为了补齐行数是4的倍数而添加的空字节)，以字节为单位（35-38字节）*/
	long int biXPelsPerMeter;			/*位图水平分辨率，每米像素数（39-42字节）*/
	long int biYPelsPerMeter;			/*位图垂直分辨率，每米像素数（43-46字节)*/
	unsigned long int biClrUsed;		/*位图实际使用的颜色表中的颜色数（47-50字节）*/
	unsigned long int biClrImportant;	/*位图显示过程中重要的颜色数（51-54字节）*/
} BITMAPINFOHEADER;


extern void SetSVGA256(void);


extern void SetSVGA64k(void);

extern unsigned int GetSVGA(void);

extern void Selectpage(register char page);

extern void Putpixel64k(int x, int y, unsigned int color);

extern int Putbmp64k(int x, int y, const char * path);

extern void Xorpixel(int x, int y, unsigned char color);

extern void Horizline(int x, int y, int width, unsigned char color);

extern void Line64k(int x1, int y1, int x2, int y2, unsigned int color);

extern void Bar64k(int x1, int y1, int x2, int y2, unsigned int color);

extern void rectangle64k(int x1, int y1, int x2, int y2, unsigned int color);

extern void Circle64k(int xc, int yc, int radius, unsigned int color);

extern void Circlefill64k(int xc, int yc, int radius, unsigned int color);

extern unsigned int Getpixel64k(int x, int y);

extern int Putbmp16(const char * path);			/*16位全屏贴图函数*/

extern void Circlehalf64k(int xc, int yc, int radius, unsigned int color);

extern void Circleright64k(int xc, int yc, int radius, unsigned int color);

extern void Circleleft64k(int xc, int yc, int radius, unsigned int color);

extern void LightBarWiden(int x1, int y1, int x2 ,int y2 ,int color);

int Linex(int x1, int y1, int x2, int y2, int color);

int Liney(int x1, int y1, int x2, int y2, int color);

void Circleup64k(int xc, int yc, int radius, unsigned int color);

void Circlerd64k(int xc, int yc, int radius, unsigned int color);

void Bar64k_radial_re(int x1, int y1, int x2, int y2, unsigned int color, int fill_time);

void Bar64k_radial(int x1, int y1, int x2, int y2, unsigned int color, int fill_time);
#endif
