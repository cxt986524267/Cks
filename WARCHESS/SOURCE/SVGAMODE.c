/**********************************************************
该文件由15级姜君学长编写，17级戴志伟学长提供，不计入总代码量 
File name:		SVGAmode.c
Author:			姜君&戴志伟
Version：		3.2
Description：	SVGA显示模式实现文件，包括该模式下基本的初始化函数、操作函数、画图函数、读图函数等的实现。
				宏处理在SVGAmode.h中。
				
				因为宏定义的缘故，严禁在其他文件中定义名称为R、G、B、r、g、b的变量！
				为了与graphics.h中的函数区别，所有自写函数名首字母大写！
				所有自写函数均以屏幕参考系为准！
				
				Putbmp16函数采用其他同学代码，函数Line、Circle、Circlefill为资料代码，不计入工作量！
				
				对本文件的修改请及时按如下格式记录在下面（每个修改单独列出）！


**********************************************************/

#include "SVGAmode.h"

void SetSVGA256(void)
{
	/*****************************************************
	在dos.h中REGS的定义如下：
	struct WORDREGS
	{
		unsigned int ax, bx, cx, dx, si, di, cflag, flags;
	};
	
	struct BYTEREGS
	{
		unsigned char al, ah, bl, bh, cl, ch, dl, dh;
	};
	
	union REGS
	{
		struct WORDREGS x;
		struct BYTEREGS h;
	};
	这样al对应ax的低八位，ah对应ax的高八位，以此类推。
	调用时需要查表决定各入口参数取值,获取返回值表示的信息。
	*****************************************************/
	union REGS graph_regs;
	
	/*设置VESA VBE模式的功能号*/
	graph_regs.x.ax = 0x4f02;
	graph_regs.x.bx = 0x105;
	int86(0x10, &graph_regs, &graph_regs);
	
	/*ax != 0x004f意味着初始化失败，输出错误信息见上,下同*/
	if (graph_regs.x.ax != 0x004f)
	{
		printf("Error in setting SVGA mode!\nError code:0x%x\n", graph_regs.x.ax);
		delay(5000);
		exit(1);
	}
}


void SetSVGA64k(void)
{
	/*REGS联合体见上*/
	union REGS graph_regs;
	
	/*设置VESA VBE模式的功能号*/
	graph_regs.x.ax = 0x4f02;
	graph_regs.x.bx = 0x117;
	int86(0x10, &graph_regs, &graph_regs);
	
	/*ax != 0x004f意味着初始化失败，输出错误信息见上,下同*/
	if (graph_regs.x.ax != 0x004f)
	{
		printf("Error in setting SVGA mode!\nError code:0x%x\n", graph_regs.x.ax);
		delay(5000);
		exit(1);
	}
}


unsigned int GetSVGA(void)
{
	/*REGS联合体见上*/
	union REGS graph_regs;
	
	/*获取当前VESA VBE模式的功能号*/
	graph_regs.x.ax = 0x4f03;
	int86(0x10, &graph_regs, &graph_regs);
	
	/*显示错误信息*/
	if (graph_regs.x.ax != 0x004f)
	{
		printf("Error in getting SVGA mode!\nError code:0x%x\n", graph_regs.x.ax);
		delay(5000);
		exit(1);
	}
	
	return graph_regs.x.bx;
}


void Selectpage(register char page)
{
	union REGS r;
	static unsigned char old_page=0;//上一次的页面，由于每次调用该函数都要使用，故设为静态变量
	static unsigned char flag=0;
	r.x.ax =0x4f05;
	r.x.bx=0;//换页功能号设定
	if (flag == 0)
	{
		old_page = page;
		r.x.dx = page;
		int86(0x10, &r, &r);
		flag++;
	}
	if(page!=old_page)
	{
		old_page = page;//保存这一次的页面
		r.x.dx = page;
		int86(0x10, &r, &r);
	}
}


/**********************************************************
Function：		Putpixel64k

Description：	画点函数，其他画图函数的基础，仅适用于0x117模式！

Calls：			Selectpage

Called By：		Putbmp64k
				MousePutBk
				MouseDraw
				
Input：			int x					像素横坐标，从左到右增加，0为最小值（屏幕参考系）
				int y					像素纵坐标，从上到下增加，0为最小值（屏幕参考系）
				unsigned int color		颜色数，共有64k种

Output：		在屏幕上画指定颜色的点

Return：		None
Others：		None
**********************************************************/
void Putpixel64k(int x, int y, unsigned int color)
{
	/*显存指针常量，指向显存首地址，指针本身不允许修改*/
	unsigned int far *const video_buffer = (unsigned int far *)0xa0000000L;
	
	/*要切换的页面号*/
	unsigned char new_page;
	
	/*对应显存地址偏移量*/
	unsigned long int page;
	
	/*判断点是否在屏幕范围内，不在就退出*/
	if(x < 0 || x > (SCR_WIDTH - 1) || y < 0 || y > (SCR_HEIGHT - 1))
	{
		return;
	}
	
	/*计算显存地址偏移量和对应的页面号，做换页操作*/
	page = ((unsigned long int)y << 10) + x;
	new_page = page >> 15;	/*32k个点一换页，除以32k的替代算法*/
	Selectpage(new_page);
	
	/*向显存写入颜色，对应点画出*/
	video_buffer[page] = color;	
}

/**********************************************************
Function：		Putbmp64k

Description：	24位非压缩bmp图定位显示函数。
				只支持24位非压缩bmp图，宽度像素最大允许为1024！
				其余bmp类型均不支持！
				仅在0x117模式下使用！
				为了简化，没有设置文件类型检测功能检测功能，请勿读入不合要求的文件！

Calls：			Putpixel64k

				fseek
				fread
				fclose
				outportb
				malloc
				free

Called By：		AutoSimulate
				HandOperate
				Menu
				
Input：			int x		图片左上角的横坐标（屏幕参考系）
				int y		图片左上角的纵坐标（屏幕参考系）
				const char * path	bmp图片路径

Output：		屏幕上显示图片

Return：		0	显示成功
				-1	显示失败
				
Others：		None
**********************************************************/
int Putbmp64k(int x, int y, const char * path)
{
	/*指向图片文件的文件指针*/
	FILE * fpbmp;
	
	/*行像素缓存指针*/
	COLORS24 * buffer;
	
	/*图片的宽度、高度、一行像素所占字节数（含补齐空字节）*/
	long int width, height, linebytes;
	
	/*循环变量*/
	int i, j;
	
	/*图片位深*/
	unsigned int bit;
	
	/*压缩类型数*/
	unsigned long int compression;
	
	/*打开文件*/
	if ((fpbmp = fopen(path, "rb")) == NULL)
	{
		return 0;
	}
	
	/*读取位深*/
	fseek(fpbmp, 28L, 0);
	fread(&bit, 2, 1, fpbmp);
	
	/*非24位图则退出*/
	if (bit != 24U)
	{
		return 0;
	}
	
	/*读取压缩类型*/
	fseek(fpbmp, 30L, 0);
	fread(&compression, 4, 1, fpbmp);
	
	/*采用压缩算法则退出*/
	if (compression != 0UL)
	{
		return 0;
	}
	
	/*读取宽度、高度*/
	fseek(fpbmp, 18L, 0);
	fread(&width, 4, 1, fpbmp);
	fread(&height, 4, 1, fpbmp);
	
	/*宽度超限则退出*/
	if (width > SCR_WIDTH)
	{
		return 0;
	}

	/*计算一行像素占字节数，包括补齐的空字节*/
	linebytes = (3 * width) % 4;
	
	if(!linebytes)
	{
		linebytes = 3 * width;
	}
	else
	{
		linebytes = 3 * width + 4 - linebytes;
	}

	/*开辟行像素数据动态储存空间*/
	if ((buffer = (COLORS24 *)malloc(linebytes)) == 0)
	{
		return 0;
	}
	
	/*行扫描形式读取图片数据并显示*/
	fseek(fpbmp, 54L, 0);
	for (i = height - 1; i > -1; i--)
	{
		fread(buffer, linebytes, 1, fpbmp);	/*读取一行像素数据*/
		
		/*一行像素的数据处理和画出*/
		for (j = 0; j < width; j++)
		{
			/*0x117模式下，原图红绿蓝各8位分别近似为5位、6位、5位*/
			buffer[j].R >>= 3;
			buffer[j].G >>= 2;
			buffer[j].B >>= 3;
			Putpixel64k(j + x, i + y,
			((((unsigned int)buffer[j].R) << 11)
			| (((unsigned int)buffer[j].G) << 5)
			| ((unsigned int)buffer[j].B)));	/*计算最终颜色，红绿蓝从高位到低位排列*/
		}
	}
	
	free(buffer);	
	fclose(fpbmp);
	
	return 1;	
}

/**********************************************************
Function：		Xorpixel

Description：	按位异或画点函数

Calls：			Selectpage

Called By：		XorCarBmp
				
Input：			int x					像素横坐标，从左到右增加，0为最小值（屏幕参考系）
				int y					像素纵坐标，从上到下增加，0为最小值（屏幕参考系）
				unsigned char color		颜色数，共有256种

Output：		在屏幕上画异或点

Return：		None
Others：		None
**********************************************************/
void Xorpixel(int x, int y, unsigned char color)
{
	/*显存指针常量，指向显存首地址，指针本身不允许修改*/
	unsigned char far * const video_buffer = (unsigned char far *)0xa0000000L;
	
	/*要切换的页面号*/
	unsigned char new_page;
	
	/*对应显存地址偏移量*/
	unsigned long int page;
	
	/*判断点是否在屏幕范围内，不在就退出*/
	if(x < 0 || x > (SCR_WIDTH - 1) || y < 0 || y > (SCR_HEIGHT - 1))
	{
		return;
	}
	
	/*计算显存地址偏移量和对应的页面号，做换页操作*/
	page = ((unsigned long int)y << 10) + x;
	new_page = page >> 16;
	Selectpage(new_page);
	
	/*按位异或方式向显存写入颜色，对应点画出*/
	video_buffer[page] ^= color;	
}

/**********************************************************
Function：		Horizline

Description：	画水平线函数
				可以接收超出屏幕范围的数据，只画出在屏幕内部分
				因为没有防止整型变量溢出的判断，画超出屏幕的线时应防止输入特大数据

Calls：			Selectpage

Called By：		Line
				Bar
				Circlefill

Input：			int x					起始点横坐标，从左到右增加，0为最小值（屏幕参考系）
				int y					起始点纵坐标，从上到下增加，0为最小值（屏幕参考系）
				int width				水平长度，为正向右延伸，为负向左延伸
				unsigned char color		颜色数，共有256种

Output：		屏幕上画出水平线

Return：		None
Others：		None
**********************************************************/
void Horizline(int x, int y, int width, unsigned char color)
{
	/*显存指针常量，指向显存首地址，指针本身不允许修改*/
	unsigned int far * const video_buffer = (unsigned int far *)0xa0000000L;
	
	/*要切换的页面号*/
	unsigned char new_page;
	
	/*对应显存地址偏移量*/
	unsigned long int page;
	
	/*i是x的临时变量，后作循环变量*/
	int i;
	
	/*判断延伸方向，让起始点靠左*/
	if (width < 0)
	{
		x = x + width;
		width = -width;
	}
	
	i = x;
	
	/*省略超出屏幕左边部分*/
	if (x < 0)
	{
		x = 0;
		width += i;
	}
	
	/*整条线在屏幕外时退出*/
	if (x >= SCR_WIDTH)
	{
		return;
	}
	
	/*整条线在屏幕外时退出*/
	if (y < 0 || y >= SCR_HEIGHT)
	{
		return;
	}
	
	/*省略超出屏幕右边部分*/
	if (x + width > SCR_WIDTH)
	{
		width = SCR_WIDTH - x;
	}
	
	/*计算显存地址偏移量和对应的页面号，做换页操作*/
	page = ((unsigned long int)y << 10) + x;
	new_page = page >> 16;
	Selectpage(new_page);
	
	/*向显存写入颜色，水平线画出*/
	for (i = 0; i < width; i++)
	{
		*(video_buffer + page + i) = color;
	}
}



/**********************************************************
Function：		Line64k

Description：	画一般直线函数
				可以接收超出屏幕范围的数据，只画出在屏幕内部分
				因为没有防止整型变量溢出的判断，画超出屏幕的线时应防止输入特大数据

Calls：			Putpixel256
				
				abs

Called By：		LightSW
				LightNE
				LightNW
				LightSE

Input：			int x1					起始点横坐标，从左到右增加，0为最小值（屏幕参考系）
				int y1					起始点纵坐标，从上到下增加，0为最小值（屏幕参考系）
				int x2					终止点横坐标（屏幕参考系）
				int y2					终止点纵坐标（屏幕参考系）
				unsigned char color		颜色数，共有65536种

Output：		屏幕上画出直线

Return：		None
Others：		None
**********************************************************/
void Line64k(int x1, int y1, int x2, int y2, unsigned int color)
{
	int	dx, dy;			/*直线x、y坐标差值*/
	int dx2, dy2;		/*加快运算速度的中间值*/
	int xinc, yinc;		/*判断想、y增加方向的符号值*/
	int d, dxy;			/*决策变量*/
	int i;
	/*计算坐标差值和中间变量*/
	dx = abs(x2 - x1);
	dx2 = dx << 1;
	dy = abs(y2 - y1);
	dy2 = dy << 1;
	
	/*判断直线x坐标增加方向*/
	if (x2 > x1)
	{
		xinc = 1;
	}
	
	/*如果是竖直线*/
	else if (x2 == x1)
	{
		/*y坐标排序*/
		if (y1 > y2)
		{
			dx = y1;
			y1 = y2;
			y2 = dx;
		}
		
		/*画竖直线*/
		for (dx = y1; dx <= y2; dx++)
		{
			Putpixel64k(x1, dx, color);
		}
		
		return;
	}
	
	else
	{
		xinc = -1;
	}
	
	/*判断直线y坐标增加方向*/
	if (y2 > y1)
	{
		yinc = 1;
	}
	
	/*如果是水平线*/
	else if (y2 == y1)
	{
		for(i=x1;i<=x2;i++)
		{
			Putpixel64k(i,y1,color);
		}
		return;
	}
	
	else
	{
		yinc = -1;
	}
	
	/*******************************
	以下运用Bresenham算法生成直线。
	该算法是得到公认的成熟的快速算法。
	具体细节略去。
	*******************************/
	Putpixel64k(x1, y1, color);
	
	if (dx >= dy)
	{
		d = dy2 - dx;
		dxy = dy2 - dx2;
		
		while (dx--)
		{
			if (d <= 0)
			{
				d += dy2;
			}
			
			else
			{
				d += dxy;
				y1 += yinc;
			}
			
			x1 += xinc;
			Putpixel64k(x1, y1, color);
		}
	}
	
	else
	{
		d = dx2 - dy;
		dxy = dx2 - dy2;
		
		while (dy--)
		{
			if (d <= 0)
			{
				d += dx2;
			}
			
			else
			{
				d += dxy;
				x1 += xinc;
			}
			
			y1 += yinc;
			Putpixel64k(x1, y1, color);
		}
	}
}



/**********************************************************
Function：		Bar64k
Description：	画矩形块函数
				可以接收超出屏幕范围的数据，只画出在屏幕内部分
				因为没有防止整型变量溢出的判断，画超出屏幕的部分时应防止输入特大数据

Calls：			Horizline

Called By：		LightSW
				LightNE
				LightNW
				LightSE

Input：			int x1					对角点1的横坐标，从左到右增加，0为最小值（屏幕参考系）
				int y1					对角点1的纵坐标，从上到下增加，0为最小值（屏幕参考系）
				int x2					对角点2的横坐标，从左到右增加，0为最小值（屏幕参考系）
				int y2					对角点2的纵坐标，从上到下增加，0为最小值（屏幕参考系）
				unsigned char color		颜色数，共有65535种

Output：		屏幕上画出矩形块

Return：		None
Others：		None
**********************************************************/
void Bar64k(int x1, int y1, int x2, int y2, unsigned int color)
{
	/*temp为临时变量和循环变量*/
	/*width为矩形长*/
	int temp, width;
	int i,j;
	/*x坐标排序*/
	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
	}
	
	/*y坐标排序*/
	if (y1 > y2)
	{
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	
	/*逐行扫描画出矩形*/
	for(i=x1;i<=x2;i++)
	{
		Line64k(i,y1,i,y2,color);
			/*for(j=y1;j<=y2;j++)
			{
					Putpixel64k(i,j,color);
			}*/
	}
}



/**********************************************************
Function：		rectangle64k
Description：	画矩形框函数
				可以接收超出屏幕范围的数据，只画出在屏幕内部分
				因为没有防止整型变量溢出的判断，画超出屏幕的部分时应防止输入特大数据

Calls：			rectangle64k

Called By：		LightSW
				LightNE
				LightNW
				LightSE

Input：			int x1					对角点1的横坐标，从左到右增加，0为最小值（屏幕参考系）
				int y1					对角点1的纵坐标，从上到下增加，0为最小值（屏幕参考系）
				int x2					对角点2的横坐标，从左到右增加，0为最小值（屏幕参考系）
				int y2					对角点2的纵坐标，从上到下增加，0为最小值（屏幕参考系）
				unsigned char color		颜色数，共有65535种

Output：		屏幕上画出矩形框

Return：		None
Others：		None
**********************************************************/
void rectangle64k(int x1, int y1, int x2, int y2, unsigned int color)
{
	/*temp为临时变量和循环变量*/
	int temp;
	/*x坐标排序*/
	if (x1 > x2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
	}
	
	/*y坐标排序*/
	if (y1 > y2)
	{
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	
	/*画出矩形框*/
	Line64k(x1, y1, x2, y1,color);			//画上侧水平直线
	Line64k(x1, y1, x1, y2,color);			//画左侧竖直直线
	Line64k(x1, y2, x2, y2,color);			//画下侧水平直线
	Line64k(x2, y2, x2, y1,color);			//画右侧竖直直线
}


/**********************************************************
Function：		Circle64k
Description：	画圆圈函数
				可以接收超出屏幕范围的数据，只画出在屏幕内部分
				因为没有防止整型变量溢出的判断，画超出屏幕的部分时应防止输入特大数据

Calls：			Putpixel256

Called By：		LightSW
				LightNE
				LightNW
				LightSE

Input：			int xc					x_center的缩写，圆心横坐标（屏幕参考系）
				int yc					y_center的缩写，圆心纵坐标（屏幕参考系）
				int radius				半径，必须为正
				unsigned char color		颜色数，共有65535种

Output：		屏幕上画出圆圈

Return：		None
Others：		None
**********************************************************/
void Circle64k(int xc, int yc, int radius, unsigned int color)
{
	/*画圆圈的定位变量和决策变量*/
	int x, y, d;
	
	/*半径必须为正，否则退出*/
	if (radius <= 0)
	{
		return;
	}
	
	/************************************
	以下运用Bresenham算法生成圆圈。
	该算法是得到公认的成熟的快速算法。
	具体细节略去。
	************************************/
	y = radius;
	d = 3 - radius << 1;
	
	for (x = 0; x <= y; x++)
	{
		Putpixel64k(xc + x, yc + y, color);
		Putpixel64k(xc + x, yc - y, color);
		Putpixel64k(xc - x, yc - y, color);
		Putpixel64k(xc - x, yc + y, color);
		Putpixel64k(xc + y, yc + x, color);
		Putpixel64k(xc + y, yc - x, color);
		Putpixel64k(xc - y, yc - x, color);
		Putpixel64k(xc - y, yc + x, color);
		
		if (d < 0)
		{
			d += x * 4 + 6;
		}
		
		else
		{
			d += (x - y) * 4 + 10;
			y--;
		}
	}
}

/**********************************************************
Function：		Circlefill64k
Description：	画实心圆函数
				可以接收超出屏幕范围的数据，只画出在屏幕内部分
				因为没有防止整型变量溢出的判断，画超出屏幕的部分时应防止输入特大数据

Calls：			Horizline

Called By：		LightColorR
				LightColorY
				LightColorG
				LightColor1
				LightColor2
				LightColor3
				LightColor4
				LightColorD1
				LightColorD2
				LightColorD3
				TrafficLight

Input：			int xc					x_center的缩写，圆心横坐标（屏幕参考系）
				int yc					y_center的缩写，圆心纵坐标（屏幕参考系）
				int radius				半径，必须为正
				unsigned char color		颜色数，共有65536种

Output：		屏幕上画出圆圈

Return：		None
Others：		None
**********************************************************/
void Circlefill64k(int xc, int yc, int radius, unsigned int color)
{
	/*画圆圈的定位变量和决策变量*/
	int x = 0,
		y = radius,
		dx = 3,
		dy = 2 - radius - radius,
		d = 1 - radius;
	
	/*半径必须为正，否则退出*/
	if (radius <= 0)
	{
		return;
	}
	
	/************************************
	以下运用Bresenham算法生成实心圆。
	该算法是得到公认的成熟的快速算法。
	具体细节略去。
	************************************/
	while (x <= y)
	{
	
		Line64k(xc - x, yc - y, xc + x, yc - y, color);
		Line64k(xc - y, yc - x, xc + y, yc - x, color);
		Line64k(xc - y, yc + x, xc + y, yc + x, color);
		Line64k(xc - x, yc + y, xc + x, yc + y, color);
        
        if (d < 0)
        {
            d += dx;
            dx += 2;
        }
        
        else
        {
            d += (dx + dy);
            dx += 2;
            dy += 2;
            y--;
		}

        x++;
	}
}
/**********************************************************
Function：		Getpixel64k

Description：	取点颜色函数，仅适用于0x117模式！

Calls：			Selectpage
				
Called By：		MouseStoreBk

Input：			int x	像素横坐标，从左到右增加，0为最小值（屏幕参考系）
				int y	像素纵坐标，从上到下增加，0为最小值（屏幕参考系）

Output：		None

Return：		unsigned int	对应坐标点在显存里面的颜色
				
Others：		None
**********************************************************/


unsigned int Getpixel64k(int x, int y)
{
	/*显存指针常量，指向显存首地址，指针本身不允许修改*/
	unsigned int far * const video_buffer = (unsigned int far *)0xa0000000L;
	
	/*要切换的页面号*/
	unsigned char new_page;
	
	/*对应显存地址偏移量*/
	unsigned long int page;
	
	/*判断点是否在屏幕范围内，不在就退出*/
	if(x < 0 || x > (SCR_WIDTH - 1) || y < 0 || y > (SCR_HEIGHT - 1))
	{
		return 0;
	}
	
	/*计算显存地址偏移量和对应的页面号，做换页操作*/
	page = ((unsigned long int)y << 10) + x;
	new_page = page >> 15;	/*32k个点一换页，除以32k的替代算法*/
	Selectpage(new_page);
	
	/*返回颜色*/
	return video_buffer[page];	
}
int Putbmp16( const char * path)
{
	unsigned long shift;
	long int bfoffbits;
	unsigned int far *const head=(unsigned int far *)0xa0000000L;
	unsigned int* buffer;
	register int i,j,k;
	unsigned int width,height;
	FILE * fpbmp;
	fpbmp=fopen(path, "rb");
	if(fpbmp==NULL)
	{
		return 0;
	}
	if ((buffer = (unsigned int*)malloc(1024*2)) == 0)
	{
		return 0;
	}
	fseek(fpbmp, 18L, 0);
	fread(&width, 4, 1, fpbmp);
	fread(&height, 4, 1, fpbmp);
	fseek(fpbmp,0x0a,SEEK_SET);
	fread(&bfoffbits,sizeof(long),1,fpbmp);
	fseek(fpbmp,bfoffbits,SEEK_SET);
	for (i =24; i>0; i--)
	{
		Selectpage(i-1);
		for (j = 32; j>0; j--)
		{
			fread(buffer,1024,2, fpbmp);	
			_fmemcpy(head+((i-1)*32+j-1)*1024,buffer,1024*2);
		}
	}
	free(buffer);
	fclose(fpbmp);
	return 1;	
}


/**********************************************************
Function：		Circlehalf64k
Description：	画实心上半圆函数
				可以接收超出屏幕范围的数据，只画出在屏幕内部分
				因为没有防止整型变量溢出的判断，画超出屏幕的部分时应防止输入特大数据

Calls：			Horizline

Called By：		LightColorR
				LightColorY
				LightColorG
				LightColor1
				LightColor2
				LightColor3
				LightColor4
				LightColorD1
				LightColorD2
				LightColorD3
				TrafficLight

Input：			int xc					x_center的缩写，圆心横坐标（屏幕参考系）
				int yc					y_center的缩写，圆心纵坐标（屏幕参考系）
				int radius				半径，必须为正
				unsigned char color		颜色数，共有65536种

Output：		屏幕上画出上半圆圈

Return：		None
Others：		None
**********************************************************/
void Circlehalf64k(int xc, int yc, int radius, unsigned int color)
{
	/*画圆圈的定位变量和决策变量*/
	int x = 0,
		y = radius,
		dx = 3,
		dy = 2 - radius - radius,
		d = 1 - radius;
	
	/*半径必须为正，否则退出*/
	if (radius <= 0)
	{
		return;
	}
	
	/************************************
	以下运用Bresenham算法生成实心圆。
	该算法是得到公认的成熟的快速算法。
	具体细节略去。
	************************************/
	while (x <= y)
	{
	
		Line64k(xc - x, yc - y, xc + x, yc - y, color);
		Line64k(xc - y, yc - x, xc + y, yc - x, color);
        
        if (d < 0)
        {
            d += dx;
            dx += 2;
        }
        
        else
        {
            d += (dx + dy);
            dx += 2;
            dy += 2;
            y--;
		}

        x++;
	}
}

/**********************************************************
Function：		Circleleft64k
Description：	画左半圆圈函数
				可以接收超出屏幕范围的数据，只画出在屏幕内部分
				因为没有防止整型变量溢出的判断，画超出屏幕的部分时应防止输入特大数据

Calls：			Putpixel256

Called By：		LightSW
				LightNE
				LightNW
				LightSE

Input：			int xc					x_center的缩写，圆心横坐标（屏幕参考系）
				int yc					y_center的缩写，圆心纵坐标（屏幕参考系）
				int radius				半径，必须为正
				unsigned char color		颜色数，共有65535种

Output：		屏幕上画出左半圆圈

Return：		None
Others：		None
**********************************************************/
void Circleleft64k(int xc, int yc, int radius, unsigned int color)
{
	/*画圆圈的定位变量和决策变量*/
	int x, y, d;
	
	/*半径必须为正，否则退出*/
	if (radius <= 0)
	{
		return;
	}
	
	/************************************
	以下运用Bresenham算法生成圆圈。
	该算法是得到公认的成熟的快速算法。
	具体细节略去。
	************************************/
	y = radius;
	d = 3 - radius << 1;
	
	for (x = 0; x <= y; x++)
	{
		Putpixel64k(xc - x, yc - y, color);
		Putpixel64k(xc - x, yc + y, color);
		Putpixel64k(xc - y, yc - x, color);
		Putpixel64k(xc - y, yc + x, color);
		
		if (d < 0)
		{
			d += x * 4 + 6;
		}
		
		else
		{
			d += (x - y) * 4 + 10;
			y--;
		}
	}
}


/**********************************************************
Function：		Circleright64k
Description：	画右半圆圈函数
				可以接收超出屏幕范围的数据，只画出在屏幕内部分
				因为没有防止整型变量溢出的判断，画超出屏幕的部分时应防止输入特大数据

Calls：			Putpixel256

Called By：		LightSW
				LightNE
				LightNW
				LightSE

Input：			int xc					x_center的缩写，圆心横坐标（屏幕参考系）
				int yc					y_center的缩写，圆心纵坐标（屏幕参考系）
				int radius				半径，必须为正
				unsigned char color		颜色数，共有65535种

Output：		屏幕上画出右半圆圈

Return：		None
Others：		None
**********************************************************/
void Circleright64k(int xc, int yc, int radius, unsigned int color)
{
	/*画圆圈的定位变量和决策变量*/
	int x, y, d;
	
	/*半径必须为正，否则退出*/
	if (radius <= 0)
	{
		return;
	}
	
	/************************************
	以下运用Bresenham算法生成圆圈。
	该算法是得到公认的成熟的快速算法。
	具体细节略去。
	************************************/
	y = radius;
	d = 3 - radius << 1;
	
	for (x = 0; x <= y; x++)
	{
		Putpixel64k(xc + x, yc + y, color);
		Putpixel64k(xc + x, yc - y, color);
		Putpixel64k(xc + y, yc + x, color);
		Putpixel64k(xc + y, yc - x, color);
		
		if (d < 0)
		{
			d += x * 4 + 6;
		}
		
		else
		{
			d += (x - y) * 4 + 10;
			y--;
		}
	}
}


/***加宽版画亮色边框函数***/
void LightBarWiden(int x1, int y1, int x2 ,int y2 ,int color)
{
	Line64k(x1+1, y1+1, x2-1, y1+1,color);			//画上侧水平直线
	Line64k(x1+1, y1+1, x1+1, y2-1,color);			//画左侧竖直直线
	Line64k(x1+1, y2-1, x2-1, y2-1,color);			//画下侧水平直线
	Line64k(x2-1, y2-1, x2-1, y1+1,color);			//画右侧竖直直线
	
	Line64k(x1+2, y1+2, x2-2, y1+2,color);			//画上侧水平直线
	Line64k(x1+2, y1+2, x1+2, y2-2,color);			//画左侧竖直直线
	Line64k(x1+2, y2-2, x2-2, y2-2,color);			//画下侧水平直线
	Line64k(x2-2, y2-2, x2-2, y1+2,color);			//画右侧竖直直线
}


/*******以下函数来自孙兆锦、郭浩南 	夺岛军演模拟系统*******/

/*******Svga模式下的画横线函数*******
x1 y1：  直线一个端点的横纵坐标
x2 y2：直线的另一个端点的横坐标

color ：颜色值
************************************/ 
int Linex(int x1, int y1, int x2, int y2, int color)      
{
  int i; 
  if(x1 >= 1024||x2 >= 1024||y1 >= 768||y2 >= 768)
  return(0); 

  if(x1 <= 0||x2 <= 0||y1 <= 0||y2 <= 0) 
  return(0); 

  if(y1 != y2||x1 > x2)     //y1要保证和y2相等
  return(0); 
  for(i = x1; i <= x2; i ++ )
  Putpixel64k(i, y1, color); 
  return 0; 
}
/*******Svga模式下的画竖线函数******
x1 y1：  直线一个端点的横纵坐标
x2 y2：直线的另一个端点的横坐标

color ：颜色值

************************************/ 
int Liney(int x1, int y1, int x2, int y2, int color)
{
  int i; 
  if(x1 >= 1024||x2 >= 1024||y1 >= 768||y2 >= 768)
  return(0); 

  if(x1 <= 0||x2 <= 0||y1 <= 0||y2 <= 0)
  return(0); 

  if(x1 != x2||y1 > y2)             //x1要保证和x2相等
  return(0); 
  for(i = y1; i <= y2; i ++ )
  Putpixel64k(x1, i, color); 
  return 0; 
}

/*******以下函数来自刘云笛对前代码的修改，不计入总代码量*******/
//用于绘制鼠标

void Circleup64k(int xc, int yc, int radius, unsigned int color)
{
	/*画圆圈的定位变量和决策变量*/
	int x, y, d;
	
	/*半径必须为正，否则退出*/
	if (radius <= 0)
	{
		return;
	}
	
	/************************************
	以下运用Bresenham算法生成圆圈。
	该算法是得到公认的成熟的快速算法。
	具体细节略去。
	************************************/
	y = radius;
	d = 3 - radius << 1;
	
	for (x = 0; x <= y; x++)
	{
		Putpixel64k(xc + x, yc + y, color);
		Putpixel64k(xc - x, yc + y, color);
		Putpixel64k(xc + y, yc + x, color);
		Putpixel64k(xc - y, yc + x, color);
		
		if (d < 0)
		{
			d += x * 4 + 6;
		}
		
		else
		{
			d += (x - y) * 4 + 10;
			y--;
		}
	}
}

void Circlerd64k(int xc, int yc, int radius, unsigned int color)
{
	/*画圆圈的定位变量和决策变量*/
	int x, y, d;
	
	/*半径必须为正，否则退出*/
	if (radius <= 0)
	{
		return;
	}
	
	/************************************
	以下运用Bresenham算法生成圆圈。
	该算法是得到公认的成熟的快速算法。
	具体细节略去。
	************************************/
	y = radius;
	d = 3 - radius << 1;
	
	for (x = 0; x <= y; x++)
	{
		Putpixel64k(xc + x, yc - y, color);
		Putpixel64k(xc + y, yc - x, color);
		
		if (d < 0)
		{
			d += x * 4 + 6;
		}
		
		else
		{
			d += (x - y) * 4 + 10;
			y--;
		}
	}
}

/**********************************************************
Function：		Bar64k_radial_re
Description：	反向放射性画出矩形框，使用方法同Bar64k，最后一个参数为总延迟时间（单位mm）

Calls：			rectangle64k

Author：		刘云笛
**********************************************************/
void Bar64k_radial_re(int x1, int y1, int x2, int y2, unsigned int color,int fill_time)
{
	int n, t, i;
	
	if(x2-x1 > y2-y1)
	{
		n = ceil( ((float)y2-y1)/2 );//向上取整
	}
	else
	{
		n = ceil( ((float)y2-y1)/2 );//向上取整
	}
	t = fill_time / (n+1);
	
	for(i = 0;i <= n; i++)
	{
		rectangle64k(x1 + i, y1 + i, x2 - i, y2 - i, color);
		delay(t);//可能还需要调整
	}
}

/**********************************************************
Function：		Bar64k_radial
Description：	正向放射性画出矩形框，使用方法同Bar64k，最后一个参数为总延迟时间（单位mm）

Calls：			rectangle64k

Author：		刘云笛
**********************************************************/
void Bar64k_radial(int x1, int y1, int x2, int y2, unsigned int color, int fill_time)
{
	int n, t;
	
	if(x2-x1 > y2-y1)
	{
		n = ceil( ((float)y2-y1)/2 );//向上取整
	}
	else
	{
		n = ceil( ((float)y2-y1)/2 );//向上取整
	}
	t = fill_time / (n+1);
	
	for(;n >= 0; n --)
	{
		rectangle64k(x1 + n, y1 + n, x2 - n, y2 - n, color);
		delay(t);//可能还需要调整
	}
	
}