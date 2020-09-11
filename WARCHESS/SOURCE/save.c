//该文件尚未测试


/********************************************************************
Copyright(c)  2020 刘云笛、陈旭桐 	WARCHESS战棋
file_name: save.c
author: 刘云笛
version: 0
Description:对战信息文件处理
date:

更新日志																	后续可能需要加密文件防挂，目前暂不考虑
																			
																			***注意*** 此函数添加存档只能按顺序添加，不能让用户点选任意一个添加

函数目录
1.创建用户存档文件
2.
******************************************************************/
#define MAP_SIZE 20
#define MAX_SAVE_NUM 6
#include "common.h"

/**************************************************************************************************
save文件设计：											位数表：自开头偏移量
	文件名：username
	第一行：文件中存档个数，1位int								共3
	
	以下重复多个存档块的设计
	------------------------------------
	第1行：存档序号 示例：save01								0
	第2行 日期  示例：20200910 1509								8
	第3行（1个int）：回合数										24
	第4行（2个int）：蓝方、红方 资源							31
	
	以下n行*n列地图。											44
	地图标志：
	-100地图障碍，0000空
	蓝方 1000大本营，1100工兵，1200步兵，1300炮兵，1400坦克，1500反物质狙击兵（名称暂定），1600医疗站 （后两位为生命值）				**********注意左边需要重新写**********
	红方为蓝方+1000
	------------------------------------				地图共(6*SIZE+2)*SIZE
	Author：刘云笛
*************************************************************************************************/

/*******************************************
Function：savefile_creat
Author：刘云笛
Description: 快速创建地图文件
*******************************************/
int savefile_creat(char *user)//如果返回1则存档满，快速创建失败							*******地形待加入其中*******
{
	char filename[25] = "SAVES//";
	FILE *fp;
	int n;
	
	strcat(filename, user);
	strcat(filename, ".txt");
	if( (fp = fopen(filename, "r+")) == NULL)
	{
	       //	Outtext(50, 50, "文件读取错误", 32, 40, 63488);					*******************看我！！！！！！！！！****************
	       //	delay(6000);
		return 0;
	}
	fscanf(fp, "%d", &n);//读取存档个数
	if(n >= MAX_SAVE_NUM)
	{
		return 1;
	}
	fseek(fp, 0, 0);
	fprintf(fp, "%d", n + 1);//增加存档个数

	fseek(fp, 0, 2);//移动到文件尾，开始存档创建
	savefile_init(fp, n + 1);//初始信息填写
	fclose(fp);
	return 0;
}

/*******************************************
Function：seek_savinfo
Author：刘云笛
Description: 在对战信息文件中定位
	将fp定位到第n个存档第x行第y列
	若x=0，将fp定位到第n个存档首
*******************************************/

void seek_savinfo(FILE *fp, int n, int x, int y)//fp 指向用户对战信息文件的指针
{
	// if(n <= 0 || n >= MAX_SAVE_NUM || x < 0 || y <= 0)
	// {
		// 报错
	// }
	int data = 0;
	fseek(fp, 3, 0);//跳过开始数字
	fseek(fp, (44+(6*MAP_SIZE+2)*MAP_SIZE)*(n-1), 1);//跳过n-1个存档
	if(x == 0)
	{
		return;
	}

	fseek(fp, 44, 1);//跳过“存档头”
	fseek(fp, (6*MAP_SIZE+2)*(x-1) + 6*(y-1), 1);//跳到正确位置

	fscanf(fp, "%d", &data);
}

/*******************************************
Function：savefile_init
Author：刘云笛
Description: 对某一个存档进行覆盖式初始化
*******************************************/

void savefile_init(FILE *fp, int n)//n为当前存档号
{
	int i = 0, j = 0;
	time_t rawtime;
    struct tm *info;

	fprintf(fp, "save%02d\n", n);

	//当前时间输入
	time( &rawtime );
    info = localtime( &rawtime );
	fprintf(fp, "%4d%02d%02d %02d:%02d\n", info->tm_year + 1900, info->tm_mon + 1, info->tm_mday, info->tm_hour, info->tm_min);

	//初始化内容创建
	fprintf(fp, "00000\n");						/***********需要设置回合上限************/
	fprintf(fp, "00000 00000\n");				/***********需要设置资源上限************/
	for(i = 0; i < MAP_SIZE; i ++)//地图初始化
	{
		for(j = 0; j < MAP_SIZE; j ++)
		{
			fprintf(fp, "%-6s", "00000");
		}
		fprintf(fp, "\n");
	}
	
}
/*********************测试函数，记得删除********************/















