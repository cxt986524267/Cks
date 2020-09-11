/*****************本文件来自两位学长的工作，不计入总代码量******************************/


/*********************************************************************************
Copyright(c)  2019 孙兆锦&郭浩南  夺岛演习仿真模拟系统 
file_name: outtext.h
data: 2019.9.11
author: 孙兆锦&郭浩南
version: 3.1
Description: 汉字库 

******************************************************************/




#ifndef  outtext_h
#define  outtext_h

void Outtext(int x,int y, char *s, int size, int part, int color);
 
void out_ASC(int x, int y, char *s, int color);

void out_ASCplus(int x, int y, char *s, int color);

int Outtext24(int x1,int y1,int x2,int y2, char *s, int part, int color);

unsigned long puthz(int x0,int y0,char *path,unsigned long len,int page);

void CHtext16(int x,int y,char *s,int color);

void ENtext16( int x,int y,char s,int color);
#endif
