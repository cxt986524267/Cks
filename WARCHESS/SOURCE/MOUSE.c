/**********************************************************
	2020 刘云笛、陈旭桐 WARCHESS战棋
	本文件来自两位学长（孙兆锦&郭浩南）的工作，不计入总代码量
	
	经lyd魔改：（其实是将两份鼠标的优点结合）
		1.重新绘制了鼠标（使用了证券投资魏靖旻和邓述民函数，转成SVGA模式，增加了不同鼠标形态）
		2.修改了存鼠标背景的mouse_sq数组大小，以及存储范围（根据重新绘制的大小调整）
		3.增加了解决鼠标留痕问题函数clrmous(MouseX, MouseY);
*********************************************************/


/*******************************         MOUSE.C         ********************************/ 
/*本函数库记载有关鼠标实现及相关操作          
    不计入总代码量              */ 

#include"common.h"


union REGS regs; 

int MouseX = 0, MouseY = 0, press = 0, MouseS = 0;
int Mouse_flag = 1; 
int mouse_sq[16][20] = {0}; 


/*画鼠标函数*/ 
void Cursor(int x,int y)
{
	
	switch(MouseS)
	{
		case 1:                                  //手势鼠标
		{
				Line64k(x-1,y+9,x-1,y+8,65503);
				Line64k(x,y+7,x,y+11,65503);
				Line64k(x+1,y+6,x+1,y+13,65503);
				Line64k(x+2,y+8,x+2,y+14,65503);
				Line64k(x+3,y-1,x+3,y+15,65503);
				Circleup64k(x+4,y-1,1,65503);
				Line64k(x+4,y-2,x+4,y+15,65503);
				Line64k(x+5,y-1,x+5,y+16,65503);
				Circleup64k(x+6,y+3,1,65503);
				Line64k(x+6,y+2,x+6,y+16,65503);
				Line64k(x+7,y+3,x+7,y+17,65503);
				Circleup64k(x+8,y+5,1,65503);
				Line64k(x+8,y+4,x+8,y+17,65503);
				Line64k(x+9,y+5,x+9,y+16,65503);
				Circleup64k(x+10,y+7,1,65503);
				Line64k(x+10,y+6,x+10,y+16,65503);
				Line64k(x+11,y+7,x+11,y+13,65503);

				Line64k(x-1,y+9,x-1,y+8,44373);
				Line64k(x-1,y+8,x+1,y+6,44373);
				Line64k(x+1,y+6,x+3,y+10,44373);
				Line64k(x+3,y+10,x+3,y-1,44373);
				Circleup64k(x+4,y-1,1,44373);
				Line64k(x+5,y-1,x+5,y+5,44373);
				Circleup64k(x+6,y+3,1,44373);
				Line64k(x+7,y+3,x+7,y+7,44373);
				Circleup64k(x+8,y+5,1,44373);
				Line64k(x+9,y+5,x+9,y+9,44373);
				Circleup64k(x+10,y+7,1,44373);
				Line64k(x+11,y+7,x+11,y+13,44373);
				Circlerd64k(x+7,y+13,4,44373);
				Line64k(x+7,y+17,x+3,y+15,44373);
				Line64k(x+3,y+15,x+1,y+13,44373);
				Line64k(x+1,y+13,x-1,y+9,44373);
		}
			break;
		case 2:                        //光标
		{
			Line64k(x+1,y-1,x+9,y-1,44373);
			Line64k(x+1,y+15,x+9,y+15,44373);
			Line64k(x+5,y-1,x+5,y+15,44373);
		}
			break;
		case 3:                        //十字
		{
			Line64k(x-1,y+7,x+11,y+7,65503);
			Line64k(x+5,y-1,x+5,y+15,65503);
		}
			break;
		default:              //默认鼠标
		{
			Line64k(x,y,x,y+13,65503);
			Line64k(x+1,y+1,x+1,y+12,65503);
			Line64k(x+2,y+2,x+2,y+11,65503);
			Line64k(x+3,y+3,x+3,y+10,65503);
			Line64k(x+4,y+4,x+4,y+12,65503);
			Line64k(x+5,y+5,x+5,y+9,65503);
			Line64k(x+5,y+11,x+5,y+14,65503);
			Line64k(x+6,y+6,x+6,y+9,65503);
			Line64k(x+6,y+13,x+6,y+15,65503);
			Line64k(x+7,y+7,x+7,y+9,65503);
			Line64k(x+8,y+8,x+8,y+9,65503);
			Line64k(x+9,y+9,x+9,y+9,65503);
			
			Line64k(x-1,y-1,x-1,y+14,44373);
			Line64k(x-1,y+14,x+3,y+11,44373);
			Line64k(x+3,y+11,x+3,y+12,44373);
			Line64k(x+3,y+12,x+4,y+13,44373);
			Line64k(x+4,y+13,x+4,y+14,44373);
			Line64k(x+4,y+14,x+7,y+17,44373);
			Line64k(x+7,y+17,x+7,y+13,44373);
			Line64k(x+7,y+13,x+6,y+12,44373);
			Line64k(x+6,y+12,x+6,y+11,44373);
			Line64k(x+6,y+11,x+5,y+10,44373);
			Line64k(x+5,y+10,x+11,y+10,44373);
			Line64k(x+11,y+10,x-1,y-2,44373);
		}
		break;
	}
}

/*初始化鼠标函数，四个形参确定鼠标可移动范围*/ 
int Initmouse(int xmi, int xma, int ymi, int yma)
{
    int retcode; 
    regs.x.ax = 0; 
    int86(51, &regs, &regs); 
    retcode = regs.x.ax; 
    if(retcode == 0)
    {
	    return 0; 
    }
    regs.x.ax = 7; 
    regs.x.cx = xmi; 
    regs.x.dx = xma; 
    int86(51, &regs, &regs); 
    regs.x.ax = 8; 
    regs.x.cx = ymi; 
    regs.x.dx = yma; 
    //regs.x.bx = 0; 
    int86(51, &regs, &regs); 
    Mouse_flag = 1; 
    return retcode; 
}
/*读取鼠标状态函数*/ 
int Readmouse(void)
{
    //int xx0 = MouseX, yy0 = MouseY, buto = 0; 
    int xnew, ynew; 
    //  do
    //  {
	regs.x.ax = 3; 
	int86(51, &regs, &regs); 
	xnew = regs.x.cx; 
	ynew = regs.x.dx; 
	press = regs.x.bx; 
	delay(10); 
    //  }
    //  while(xnew == xx0 && ynew == yy0 && press == buto); 
    MouseX = xnew; 
    MouseY = ynew; 
    return  0; 
}


/*存鼠标覆盖背景函数*/ 
void Mouse_savebk(int x, int y)
{
    register int i, j; 
    for(i = 0; i <= 15; i ++ )
    for(j = 0; j <= 19; j ++ )
    mouse_sq[i][j] = Getpixel64k(x + i - 1, y + j - 2); 
}

/*单独存鼠标覆盖背景函数*/ 
void Mouse_savebk2(void)
{
    Mouse_savebk(MouseX, MouseY); 
}

/*恢复覆盖背景函数*/ 
void Mouse_putbk(int x, int y)
{
    register int i, j; 
    for(i = 0; i <= 15; i ++ )
    for(j = 0; j <= 19; j ++ )
    Putpixel64k(x + i - 1, y + j - 2, mouse_sq[i][j]); 
}

/*判断鼠标是否在某区域按下*/ 
int Mouse_press(int x1, int y1, int x2, int y2)
{
    if(MouseX >= x1 && MouseX <= x2 && MouseY >= y1 && MouseY <= y2 && press)
    return 1; 
    else
    return 0; 
}
/*判断鼠标是否在某区域以外按下*/ 
int Mouse_pressother(int x1, int y1, int x2, int y2)
{
	if ((MouseX  <  x1 || MouseX >= x2 || MouseY < y1 || MouseY > y2) && press)
		return 1; 
	else
		return 0; 
}
/*判断鼠标是否位于某区域上方*/ 
int Mouse_above(int x1, int y1, int x2, int y2)
{
    if(MouseX >= x1 && MouseX <= x2 && MouseY >= y1 && MouseY <= y2)
	  return 1; 
    else
	  return 0; 
}

/*判断鼠标是否位于某区域外*/ 
int Mouse_aboveother(int x1, int y1, int x2, int y2)
{
	if(MouseX  <  x1 || MouseX >= x2 || MouseY  <  y1 || MouseY  >  y2)
		return 1; 
	else
		return 0; 
}

/*******************lyd魔改部分***************************/

/*判断鼠标是否在某区域按下*/ 
int mouse_press(int x1, int y1, int x2, int y2)
{
	//在框中点击，则返回1
	if(MouseX > x1 
	&&MouseX < x2
	&&MouseY > y1
	&&MouseY < y2
	&&press == 1)
	{
		return 1;
	}
	
	//在框中未点击，则返回2
	else if(MouseX > x1 
	&&MouseX < x2
	&&MouseY > y1
	&&MouseY < y2
	&&press == 0)
	{
		return 2;
	}
	
	//在框外点击，返回3
	else if((MouseX < x1 
	|| MouseX > x2
	|| MouseY < y1
	|| MouseY > y2)
	&&press == 1)
	{
		return 3;
	}
		
	//在框中点击右键，则返回4
	else if(MouseX > x1 
	&&MouseX < x2
	&&MouseY > y1
	&&MouseY < y2
	&&press == 2)
	{
		return 4;
	}
	
	//框外不点击
	else
	{
		return 0;
	}
}

void clrmous(int nx,int ny)//清除鼠标
{
	if(Mouse_flag==1)
	{
		//Cursor(nx,ny);
		Mouse_putbk(nx,ny);
		Mouse_flag=0;
	}
}

void drawmous(int nx,int ny)
{
	if(Mouse_flag==0)
	{
	    Cursor(nx,ny);
		Mouse_flag=1;
	}
}


/*更新鼠标位置函数*/ 

void Newxy(void)
{
    int xx0 = MouseX, yy0 = MouseY; 
	Readmouse(); 
	if (MouseX == xx0 && MouseY == yy0) return; 
    clrmous(xx0, yy0); 
	Mouse_savebk(MouseX, MouseY); 
	drawmous(MouseX,MouseY);
}

// void Newxy(void)
// {
    // int xx0 = MouseX, yy0 = MouseY, x, y; 
	// Readmouse(); 
	// if (MouseX == xx0 && MouseY == yy0) return; 
	// if(Mouse_flag)
    // Mouse_putbk(xx0, yy0); 
	// Mouse_savebk(MouseX, MouseY); 
	// Cursor(MouseX, MouseY); 
	// Mouse_flag = 1; 
// }