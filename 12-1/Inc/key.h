#ifndef __KEY_H
#define __KEY_H

#include "stm32g4xx_hal.h"
#include "stm32g4xx_it.h"
#include "main.h"

// 按键结果结构体 需要注意的是任意俩者不能够同时为1
struct keys{
	//按键单次按下  0-无效   1-按键按下
	int flag;
	//双击按键的标志 0-无效  1-双次按下
	int doubleflag;
	//长按按键的标志 0-无效   1-长按
	int longflag;
};

//按键状态判断的结构体
struct keyState{
	//记录按键状态机的状态
	unsigned char judgeSate;
	//记录按键的状态
	unsigned char keyState;
	//记录按键是否双击 双击就会计时
	unsigned char doubleClickTimerFlag;
	//记录按键按下的时间
	int keyTime;
	//记录按键双击的时间
	unsigned char doubleClickTime;
};

void key_init(void);

//定义一个按键变量 key[0]=B1 key[1]=B2 key[2]=B3 key[3]=B4 
extern struct keys key[];

/***************************************************************************************************
 * 函数功能：按键扫描函数 注意此函数放在定时器中断（10ms）中的使用效果最佳 否则双击与长按会出现问题
 * 函数参数：无
 * 函数返回值：无
****************************************************************************************************/
void key_scanf(void);

#endif

