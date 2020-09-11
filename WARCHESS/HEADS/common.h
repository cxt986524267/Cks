/****************************************************
Copyright(c)  2020 刘云笛、陈旭桐 	WARCHESS战棋
file_name: common.h
author: 刘云笛、陈旭桐
version: 1
Description: 头文件及定义结构、枚举
****************************************************/ 
#ifndef	_COMMON_H_
#define	_COMMON_H_

#define SCR_WIDTH 1024
#define SCR_HEIGHT 768


#include <stdio.h>     /**文件操作函数头文件**/
#include <conio.h>     /**getch()函数头文件**/
#include <stdlib.h>	   /**rand()、exit()等函数头文件**/
#include <malloc.h>    /**动态分配内存头文件**/
#include <graphics.h>  /**自带图像处理头文件**/
#include <string.h>    /**字符串处理函数头文件**/
#include <bios.h> 	   /**bioskey()函数头文件**/
#include <dos.h>       /**int86()等关于中断的头文件**/
#include <time.h>	   /**time()函数的头文件**/
#include <math.h>      /**数学函数的头文件**/
#include <ctype.h>     /**字符函数的头文件**/
#include <process.h>

#include "outtext.h"	//汉字库
#include "SVGAmode.h"	//SVGA
#include "mouse.h"		//鼠标

#include "draw.h"		//自定义画图函数
#include "kbinput.h"	//键盘输入显示
#include "homepage.h"	//主页
#include "login.h"		//登录界面
#include "register.h"	//注册界面
#include "mainmenu.h"	//主菜单界面
#include "save.h"		//用户存档相关函数

enum MENU
{
	EXIT, HOMEPAGE, LOGIN, REGIS, MAINMENU, ABOUT
};//主界面功能选项

#endif