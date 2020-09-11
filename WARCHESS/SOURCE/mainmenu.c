/********************************************************************
Copyright(c)  2020 刘云笛、陈旭桐 	WARCHESS
file_name: mainmenu.c
author: 陈旭桐
version: 1.0
Description: 主菜单
date:2020/9/9

更新日志

函数目录
1.mainmenu:		主菜单函数
2.mmenu_draw:	绘制主菜单函数
******************************************************************/
#include "common.h"

int mainmenu(char *user)
{
	mmenu_draw(user); //绘制主菜单函数
	clrmous(MouseX, MouseY);//更新鼠标状态，防止留痕
	
	while(1)
	{
		Newxy();
		
		if(mouse_press(0, 0, 50, 50) == 1)
		{
			return 1;
		}
		
		if(mouse_press(330, 140+85, 330+250, 140+85+55))
		{
			
		}
		
		if(mouse_press(330, 140+85, 330+250, 140+85+55))
		{
			
		}
	}
}

void mmenu_draw(char *user)
{
	clrmous(MouseX, MouseY);
	shadow(50, 32, 50+535, 32+90, 65535, 58816);
	Outtextxx(65, 52, 65+505, "欢迎回来指挥官", 48, 65499);
	shadow(330, 140, 330+250, 140+55, 65535, 58816);


	shadow(330, 140+85, 330+250, 140+85+55, 58816, 58816);


	shadow(330, 140+85+85, 330+250, 140+85+85+55, 58816, 58816);

	
	shadow(330, 140+85+85+85, 330+250, 140+85+85+85+55, 58816, 58816);


	file_draw(50, 140, 50+260, 140+85+85+85+55);


}