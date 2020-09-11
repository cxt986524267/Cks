#ifndef _DRAW_h_
#define _DRAW_h_

#include "common.h"
void shadow(int x1, int y1, int x2, int y2, int color1, int color2);  //带阴影的文本框
void shadow_l(int x1, int y1, int x2, int y2, int color); //带大阴影的文本框
void frame(int x1, int y1, int x2, int y2, int color); //带边框的文本框
void file_draw(int x1, int y1, int x2, int y2); //绘制档案图形
int Outtextxx(int x1, int y, int x2, char *s,int flag, int color); //根据两点坐标输出汉字
void outtextxyc(int x1, int y1, char *s, int color);

/*void file_draw(int x1, int y1, int x2, int y2); //绘制档案图形
void barc(int x1, int y1, int x2, int y2, int color); //集成setcolor与bar，下同
void linec(int x1, int y1, int x2, int y2, int color);
void outtextxyc(int x1, int y1, char *s, int color); */

#endif