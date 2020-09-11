/********************************************************************
Copyright(c)  2020 刘云笛、陈旭桐 	WARCHESS
file_name: register.c
author: 陈旭桐、刘云笛
version: 1.2
Description: 注册界面
date:2020/9/8

更新日志
9.9		优化了注册界面
		增加了密码加密算法
		增加了将用户信息写入文件的函数
		增加了检测是否正确输入的函数
		解决了进入kbinput函数之前缓冲区有内容的bug
		
函数目录
1.register_:			注册函数
2.drawregi:				绘制注册界面函数
3.user_creat:			创建用户函数
4.password_classified:	密码文件加密函数
5.regis_check：			检测是否正确输入函数
******************************************************************/
#include "common.h"

int register_()
{
	char user[10] = "", password[20] = "", password_r[20] = "";
	int flag = 0;	//防止重复高亮
	
	drawregi();
	delay(100);
	
	while(1)
	{
		Newxy();
		if(bioskey(1))
		{
			getch();
		}//防止缓冲区中有内容影响输入
	
		if(mouse_press(0, 0, 50, 50) == 1)//返回主页面
		{
			return HOMEPAGE;
		}
		
		if(flag == 0 && mouse_press(256-15, 390-15, 384+15, 422+15) == 2)	//高亮注册键
		{
			flag = 1;
			frame(256-15, 390-15, 384+15, 422+15, 65535);
			Outtext(256, 380,"注册", 48, 80, 30513);
		}
		if(flag == 1 && mouse_press(256-15, 390-15, 384+15, 422+15) == 0)	//取消高亮
		{
			flag = 0;
			shadow_l(256-15, 390-15, 384+15, 422+15, 65535);
			Outtext(256, 380,"注册", 48, 80, 30513);
		}
		if(mouse_press(256-15, 390-15, 384+15, 422+15) == 1)	//点击注册键
		{
			if(regis_check(user, password, password_r))
			{
				user_creat(user, password);
				
				shadow_l(320-200, 240-100, 320+200, 240+100, MAGENTA);
				Outtext(120+30, 140+40,"注册成功！", 48, 80, 65535);
				Outtext(120+92, 340-60,"三秒后进入登录界面", 16, 25, 65535);
				delay(1000);
				Bar64k(120+92, 340-60, 120+92+25*8+16, 340-60+16, MAGENTA);
				Outtext(120+92, 340-60,"二秒后进入登录界面", 16, 25, 65535);
				delay(1000);
				Bar64k(120+92, 340-60, 120+92+25*8+16, 340-60+16, MAGENTA);
				Outtext(120+92, 340-60,"一秒后进入登录界面", 16, 25, 65535);
				delay(1000);
				return LOGIN;
			}
				Bar64k(120+92, 340-60, 120+92+25*8+16, 340-60+16, MAGENTA);
				Outtext(120+92, 340-60,"正在返回注册界面", 16, 25, 65535);
				delay(1000);
				return REGIS;
		}
		
		if(mouse_press(180, 165, 180+370, 165+50) == 1)					//输入用户名
		{
			kbinput(180, 165, 180+370, 165+50, user, 1);
		}
		if(mouse_press(180, 165+70, 180+370, 165+50+70) == 1)					//输入密码
		{
			kbinput(180, 165+70, 180+370, 165+50+70, password, 0);
		}
		if(mouse_press(180, 165+140, 180+370, 165+50+140) == 1)					//再次输入密码
		{
			kbinput(180, 165+140, 180+370, 165+50+140, password_r, 0);
		}
	}
}

void drawregi()
{
	Bar64k(0, 0, 1024, 768, 34847);
	shadow_l(70, 30, 70+500, 30+90, 65535);
	shadow_l(256-15, 390-15, 384+15, 422+15, 65535);
	Bar64k(0,0,50,50, 64384);
	frame(180, 165, 180+370, 165+50, 65535);             //用户名
	Outtext(12, 175,"用户名", 32, 59, 30513);
	Outtext(200, 185, "八位以内，大写字母和数字", 16, 22, 27469);
	
	frame(180, 165+70, 180+370, 165+50+70, 65535);  //密码
	Outtext(12, 175+70,"密码", 32, 118, 30513);
	Outtext(200, 185+70, "密码区分大小写哦", 16, 30, 27469);
	
	frame(180, 165+140, 180+370, 165+50+140, 65535);  //再次输入
	Outtext(12, 175+140, "再次输入", 32, 40, 30513);
	Outtext(200, 185+140, "两次输入保持一致", 16, 30, 27469);

	Outtext(85,50,"欢迎注册本系统", 48, 70, 31);
	Outtext(256, 380,"注册", 48, 80, 30513);
}

void user_creat(char *username, char *password)
{
	FILE *fp;
	char filename[30] = "users\\";
	
	strcat(filename, username);
	strcat(filename, ".txt");
	
	if((fp = fopen(filename, "w+")) == NULL)
	{
		outtextxyc(10,10,"Can NOT creat user file", 30513);
		delay(6000);
		exit(1);
	}
	fprintf(fp, "%s\n%llu\n%llu",username, password_classified(password, 67), password_classified(password, 71));//这里不会给你注释的
	fclose(fp);
}

//陈旭桐初版，刘云笛修改，加密密码
unsigned long long password_classified(char *p, int radix)//字符数组转换为radix进制数 
{
	unsigned long long part1 = 0, part2 = 0;
	int length = strlen(p), i, tmp;
	//此处应有注释
	for(i = 0; i < length; i++)
	{   
        part1 *= radix;
		part2 *= radix;//自然溢出不管 
		if(isupper(p[i]) == 1)//大写 
		{
			tmp = p[i] - 'A' + 11;
			part1 += tmp;
		}
		else if(islower(p[i]) == 1)//小写 
		{
			tmp = p[i] - 'a' + 37;
			part1 += tmp;
		}
		else if(p[i] == '0')
		{
			tmp = 10;
			part1 += tmp;
		}
		else//数字1-9 
		{
			tmp = p[i] - '0';
			part1 += tmp;
		}
		
		//倒序
		if(isupper(p[length-1-i]) == 1)
		{
			tmp = p[length-1-i] - 'A' + 11;
			part2 += tmp;
		}
		else if(islower(p[length-1-i]) == 1)
		{
			tmp = p[length-1-i] - 'a' + 37;
			part2 += tmp;
		}
		else if(p[length-1-i] == '0')
		{
			tmp = 10;
			part2 += tmp;
		}
		else
		{
			tmp = p[length-1-i] - '0';
			part2 += tmp;
		}	
	}
	return part1 *(part2 * radix);
	/*
	在你看懂这个的时候，我们的加密系统就被你识破了QAQ
	可恶
	然而你也算不出密码（大数的因式分解，经典np类问题）
	哈哈哈哈哈哈哈哈哈哈哈XD
	*/
}

int regis_check(char *un, char *pw, char *pwr)
{
	FILE *fp;
	char filename[30] = "users\\";
	strcat(filename, un);
	strcat(filename, ".txt");
	
	if(*un == '\0')//未输入用户名
	{
		shadow_l(320-200, 240-100, 320+200, 240+100, 5);
		Outtextxx(150, 180, 150+340, "请输入用户名", 32, 65535);
		return 0;
	}
	else if(*pw == '\0')//未输入密码
	{
		shadow_l(320-200, 240-100, 320+200, 240+100, 5);
		Outtextxx(150, 180, 150+340, "请输入密码", 32, 65535);
		return 0;
	}
	else if(*pwr == '\0')//未再次输入密码
	{
		shadow_l(320-200, 240-100, 320+200, 240+100, 5);
		Outtextxx(150, 180, 150+340, "请再次输入密码", 32, 65535);
		return 0;
	}
	else if(strcmp(pw, pwr))//两次输入密码不一致
	{
		shadow_l(320-200, 240-100, 320+200, 240+100, 5);
		Outtextxx(150, 180, 150+340, "两次输入密码不一致", 32, 65535);
		return 0;
	}
	
	else if((fp=fopen(filename, "r")) != NULL)//用户名已存在
	{
		fclose(fp);
		shadow_l(320-200, 240-100, 320+200, 240+100, 5);
		Outtextxx(150, 180, 150+340, "用户名已存在", 32, 65535);
		return 0;
	}
	
	else //成功return LOGIN  返回登录界面
	{
		fclose(fp);
		return 1;
	}
}












