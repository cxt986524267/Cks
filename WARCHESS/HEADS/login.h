#ifndef _Loginin_H_
#define _Loginin_H_

int login(char *user);										//登录界面核心函数
void drawlogin(void);										//登录界面绘制
int login_check(char *username, char *password);			//能否登陆检查，包括是否填写判断和用户名密码比对

#endif

