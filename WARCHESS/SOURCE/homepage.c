/********************************************************************
Copyright(c)  2020 刘云笛、陈旭桐 	WARCHESS
file_name: homepage.c
author: 陈旭桐
version: 1.2
Description: 主页界面
date:2020/9/8

更新日志
9.9		增加了主页界面的按键,优化了界面效果
		增加了调试入口


函数目录
1.homepage：	主页函数
2.draw_h_page:	主页界面
******************************************************************/
#include "common.h"

int homepage()
{
	int flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0;	//防止重复标亮, 此处需要优化成一个flag
	
	draw_h_page(); //绘制主页函数
	Mouse_savebk2();//更新鼠标状态，防止留痕

	while(1)
	{
		Newxy();
		
		if(bioskey(1))
		{
			getch();
		}//防止缓冲区中有内容影响输入
	
		if(mouse_press(590, 0, 640, 50) == 1) //退出
		{
			return EXIT;
		}
		
		if(flag1 == 0 && mouse_press(70, 230, 35+235, 230+70) == 2)	//标亮登录键
		{
			flag1 = 1;
			MouseS = 1;
			clrmous(MouseX, MouseY);
			Bar64k_radial(70, 230, 35+235, 230+70, 2016, 500);
			Outtext(100+10, 240,"登录", 48, 70, 0);
			Mouse_savebk2();//更新鼠标状态，防止留痕
		}
		
		if(flag1 == 1 && mouse_press(70, 230, 35+235, 230+70) == 0)	//取消标亮
		{
			flag1 = 0;
			MouseS = 0;
			clrmous(MouseX, MouseY);
			Bar64k_radial_re(70, 230, 35+235, 230+70, 65535, 500);
			Outtext(100+10, 240,"登录", 48, 70, 2016);
		}
		
		if(flag2 == 0 && mouse_press(570-200, 230, 570, 230+70) == 2)	//标亮注册键
		{
			flag2 = 1;
			MouseS = 1;
			clrmous(MouseX, MouseY);
			Bar64k(570-200, 230, 570, 230+70,44373);
			Outtext(400+10, 240,"注册", 48, 70, 2016);
		}
		
		if(flag2 == 1 && mouse_press(570-200, 230, 570, 230+70) == 0)	//取消标亮
		{
			flag2 = 0;
			MouseS = 0;
			clrmous(MouseX, MouseY);
			shadow_l(570-200, 230, 570, 230+70, 65535);
			Outtext(400+10, 240,"注册", 48, 70, 2016);
		}
		
		if(flag3 == 0 && mouse_press(70, 330, 35+235, 330+70) == 2)	//标亮关于键
		{
			flag3 = 1;
			MouseS = 1;
			clrmous(MouseX, MouseY);
			Bar64k(70, 330, 35+235, 330+70,44373);
			Outtext(100+10, 340,"关于", 48, 70, 2016);
			Mouse_savebk2();//更新鼠标状态，防止留痕
		}
		
		if(flag3 == 1 && mouse_press(70, 330, 35+235, 330+70) == 0)	//取消标亮
		{
			flag3 = 0;
			MouseS = 0;
			clrmous(MouseX, MouseY);
			shadow_l(70, 330, 35+235, 330+70, 65535);
			Outtext(100+10, 340,"关于", 48, 70, 2016);
		}
		
		if(flag4 == 0 && mouse_press(570-200, 330, 570, 330+70) == 2)	//标亮退出键
		{
			flag4 = 1;
			MouseS = 1;
			clrmous(MouseX, MouseY);
			Bar64k(570-200, 330, 570, 330+70,44373);
			Outtext(400+10, 340,"退出", 48, 70, 2016);
			Mouse_savebk2();//更新鼠标状态，防止留痕
		}
		
		if(flag4 == 1 && mouse_press(570-200, 330, 570, 330+70) == 0)	//取消标亮
		{
			flag4 = 0;
			MouseS = 0;
			clrmous(MouseX, MouseY);
			shadow_l(570-200, 330, 570, 330+70, 65535);
			Outtext(400+10, 340,"退出", 48, 70, 2016);
		}
		
		else if(mouse_press(70, 70, 70+500, 70+90) == 4)				//为了方便调试,右键点击标题直接跳过登录界面进入主菜单
		{
			return MAINMENU;
		}
		
		else if(mouse_press(70, 330, 35+235, 330+70) == 1)
		{
			return ABOUT;
		}
		
		else if(mouse_press(70, 230, 35+235, 230+70) == 1)				//点击登录键
		{
			return LOGIN;
		}
		
		else if(mouse_press(570-200, 230, 570, 230+70) == 1)			//点击注册键
		{
			return REGIS;
		}
		else if(mouse_press(570-200, 330, 570, 330+70) == 1)			//点击退出键
		{
			exit(0);
		}
	}
}

void draw_h_page()
{
	Bar64k(0, 0, 1024, 768, 29186);
	Bar64k(590, 0, 640, 50,65535);
	Outtext(599, 9,"退", 32, 0, RED);
	
	shadow_l(70, 70, 70+500, 70+90, 65535); 
	Outtext(85,90,"红蓝军对抗演练", 48, 70, 2016);
	
	shadow_l(70, 230, 35+235, 230+70, 65535);
	Outtext(100+10, 240,"登录", 48, 70, 2016);
	
	shadow_l(570-200, 230, 570, 230+70, 65535);
	Outtext(400+10, 240,"注册", 48, 70, 2016);
	
	shadow_l(70, 330, 35+235, 330+70, 65535);
	Outtext(100+10, 340,"关于", 48, 70, 2016);
	
	shadow_l(570-200, 330, 570, 330+70, 65535);
	Outtext(400+10, 340,"退出", 48, 70, 2016);
}