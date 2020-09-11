#ifndef _MOUSE_H_
#define _MOUSE_H_

void Cursor(int x,int y);
int Initmouse(int xmi,int xma,int ymi,int yma);
int Readmouse(void);
void Newxy(void);
void Mouse_savebk(int x,int y);
void Mouse_savebk2(void);
void Mouse_putbk(int x,int y);
int Mouse_press(int x1,int y1,int x2,int y2);
int Mouse_pressother(int x1, int y1, int x2, int y2);
int Mouse_above(int x1,int y1,int x2,int y2);
int Mouse_aboveother(int x1, int y1, int x2, int y2);

int mouse_press(int x1, int y1, int x2, int y2);//如果在框中点击，则返回1；在框中未点击，则返回2；在框外点击，则返回3；不在框中则返回0
void clrmous(int nx,int ny);//清除鼠标
void drawmous(int nx,int ny);//绘制鼠标，结合flag

extern int MouseX, MouseY, press, MouseS;
extern int Mouse_flag; 
#endif
