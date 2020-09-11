/****************************************************
Copyright(c)  2020 刘云笛、陈旭桐 	WARCHESS战棋
file_name: main.c
author: 刘云笛、陈旭桐
version: 2.0 
Description: 主函数
****************************************************/ 
#include"common.h"

int main(void)
{
	int page = HOMEPAGE;
	char user[10] = "";				//初始化用户名数组
	
	SetSVGA64k(); 					//分辨率1024*768
	//Initmouse(1, 1023, 1, 767); 
	Initmouse(0, 1024, 0, 768); 
	
	while(1)
	{
		switch(page)
		{
			case EXIT:				//exit
				exit(0);
				break;
				
			case HOMEPAGE:			//首页
				page = homepage();
				break;
				
			case LOGIN:				//登录界面
				page = login(user);
				break;
			
			case REGIS:				//注册界面
				page = register_();
				break;
				
			case MAINMENU:			//主菜单
				page = mainmenu(user);
				break;
		}
	}
}