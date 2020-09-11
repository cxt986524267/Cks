/********************************************************************
Copyright(c)  2020 刘云笛、陈旭桐 	WARCHESS战棋
file_name: loginmenu.c
author: 陈旭桐、刘云笛
version: 2.0
Description: 登录界面
date:2020/9/8

更新日志
2020/9/9
优化了登录界面
增加了判断是否正确输入的函数
增加了用户名与文件匹配的功能

函数目录
1.login：		登录函数
2.drawlogin：	绘制登录界面函数
3.login_check：	检测输入信息函数
******************************************************************/
#include "common.h" 

int login(char *username)  
{
	char password[20] = "\0";	//初始化密码数组
	int flag = 0;				//防止重复高亮
	
	drawlogin();
	Mouse_savebk2();//更新鼠标状态，防止留痕
	
	while(1)
	{
		Newxy();
		
		if(bioskey(1))
		{
			getch();
		}//防止缓冲区中有内容影响输入
		
		if(mouse_press(0, 0, 80, 80) == 1)
		{
			return 1;
		}
		
		if(flag == 0 && mouse_press(385, 600, 640, 700) == 2)	//高亮登录键
		{
			flag = 1;
			frame(385, 600, 640, 700, 65535);
			Outtext(410, 608,"登录", 48, 80, 63488);
			Mouse_savebk2();//更新鼠标状态，防止留痕
		}
		
		if(flag == 1 && mouse_press(385, 600, 640, 700) == 0)	//取消高亮	
		{
			flag = 0;
			shadow_l(385, 600, 640, 700, 65535);
			Outtext(410, 608,"登录", 48, 80, 63488);
		}	
		
		if(mouse_press(180, 180,180+370, 180+50) == 1)						//输入用户名
		{
			kbinput(180, 180,180+370, 180+50, username, 1);
		}
		
		if(mouse_press(180, 180+100, 180+370, 180+50+100) == 1)				//输入密码
		{
			kbinput(180, 180+100, 180+370, 180+50+100, password, 0);
		}
		
		if(mouse_press(256-15, 390-15, 384+15, 422+15) == 1)				//点击登录键
		{
			if(login_check(username, password) == 1)						//判断是否与文件匹配
			{
				shadow_l(320-200, 240-100, 320+200, 240+100, 65535);
				delay(500);
				Outtext(150, 240-32,"正", 32, 102, 2463);					//虚假的动画效果
				delay(500);
				Outtext(150, 240-32,"正在", 32, 102, 2463);
				delay(500);
				Outtext(150, 240-32,"正在登", 32, 102, 2463);
				delay(500);
				Outtext(150, 240-32,"正在登录", 32, 102, 2463);
				delay(500);
				shadow_l(320-200, 240-100, 320+200, 240+100, 5);
				Outtextxx(150, 240-32, 150+340, "登录成功！", 32, 2463);
				delay(1000);
				return LOGIN;
			}
			Outtext(120+92, 340-60,"正在返回登录界面", 16, 25, 65535);
			delay(1000);
			return LOGIN;
		}
	}
}

void drawlogin()
{
	Bar64k(0, 0, 1024, 768, 25290);
	Bar64k(0, 0, 50, 50, 65535);
	shadow_l(70, 50, 70+500, 50+90, 65535); 
	shadow_l(256-15, 390-15, 384+15, 422+15, 65535);
	
	frame(180, 180,180+370, 180+50, 65535);             //用户名
	Outtext(180+20,180+15,"大写字母和数字", 16, 30, LIGHTGRAY);
	frame(180, 180+100, 180+370, 180+50+100, 65535);  	//密码
	Outtext(180+20,280+15,"注意区分大小写", 16, 30, LIGHTGRAY);
	Outtext(85,70,"欢迎使用本系统", 48, 70, GREEN);
	Outtext(70, 190,"用户名", 32, 30, 63488);
	Outtext(70, 290,"密码", 32, 62, 63488);
	Outtext(256, 380,"登录", 48, 80, 63488);
}

int login_check(char *username, char *password)
{
	FILE *fp;
	unsigned long long pw, pwt;
	char filename[30] = "users\\";
	
	strcat(filename, username);
	strcat(filename, ".txt");
	
	if(*username == '\0')//未输入用户名
	{
		shadow_l(320-200, 240-100, 320+200, 240+100, 5);
		Outtextxx(150, 180, 150+340, "请输入用户名", 32, 65535);
		fclose(fp);
		return 0;
	}
	if(*password == '\0')//未输入密码
	{
		shadow_l(320-200, 240-100, 320+200, 240+100, 5);
		Outtextxx(150, 180, 150+340, "请输入密码", 32, 65535);
		fclose(fp);
		return 0;
	}
	
	//用户名判断
	if((fp = fopen(filename, "r")) == NULL)
	{
		shadow_l(320-200, 240-100, 320+200, 240+100, 5);
		Outtextxx(150, 180, 150+340, "未找到该用户", 32, 65535);
		fclose(fp);
		return 0;
	}
	
	//密码判断
	fscanf(fp, "%lld%lld", &pw, &pwt);
	if(pw == password_classified(password, 67) && pwt == password_classified(password, 71) )
	{
		fclose(fp);
		return 1;
	}
	else
	{
		*password = 0;
		shadow_l(320-200, 240-100, 320+200, 240+100, 5);
		Outtextxx(150, 180, 150+340, "密码错误", 32, 65535);
		fclose(fp);
		return 0;
	}
}