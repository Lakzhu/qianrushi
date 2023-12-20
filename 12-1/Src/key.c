#include "key.h"

struct keys key[4] = {0,0,0,0};

/*********************************
 * 函数功能：按键的初始化函数
 * 函数参数：无
 * 函数返回值：无
*********************************/
void key_init(void)
{
    GPIO_InitTypeDef GPIO_initStructure;

/*********按键B4(PA0)初始化*******************/
#if KEYPA
    __HAL_RCC_GPIOA_CLK_ENABLE();
    //浮空输入模式
    GPIO_initStructure.Mode = GPIO_MODE_IT_FALLING;
    //设置成低频
    GPIO_initStructure.Speed = GPIO_SPEED_FREQ_LOW;
    //GPIO引脚设置
    GPIO_initStructure.Pin = GPIO_PIN_0;
    //引脚的模式设置
    GPIO_initStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA,&GPIO_initStructure);
#endif

/*********按键B4初始化*******************/
#if KEYPB
    __HAL_RCC_GPIOB_CLK_ENABLE();
        //浮空输入模式
    GPIO_initStructure.Mode = GPIO_MODE_IT_FALLING;
    //设置成低频
    GPIO_initStructure.Speed = GPIO_SPEED_FREQ_LOW;
    //GPIO引脚设置
    GPIO_initStructure.Pin = KEYGPIOB;
    //引脚的模式设置
    GPIO_initStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB,&GPIO_initStructure);
#endif
}


/****************************************************************************************************
 * 函数功能：按键扫描函数 注意此函数放在定时器中断（10ms）中的使用效果最佳 否则双击与长按会出现问题
 * 函数参数：无
 * 函数返回值：无 
*****************************************************************************************************/
void key_scanf(void)
{
	static struct keyState _key[4];
	
	//获取按键的最新状态
	_key[0].keyState = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	_key[1].keyState = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	_key[2].keyState = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	_key[3].keyState = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
	
	//处理按键的最新状态
	for(int i=0;i<4;i++){
		switch (_key[i].judgeSate){
			//按键第一次按下
			case 0:
				if(_key[i].keyState == 0){
					//跳转按键的状态
					_key[i].judgeSate=1;
					//清空按键时间
					_key[i].keyTime=0;
				}
				break;
			//按键第二次按下 两次相隔10ms可以起到消抖作用	
			case 1:
				//按键再次按下 跳转按键状态
				if(_key[i].keyState == 0)
					_key[i].judgeSate=2;
				//上一次按键按下是抖动按下 属于无效状态 应该退回最开始的状态
				else 
					_key[i].judgeSate=0;
				break;
			//确定按键按下后的处理过程
			case 2:
				//等待松开过程,且非长按键
				if((_key[i].keyState==1) && _key[i].keyTime<30){	
					//可能双击按键的第一次，进入计时
					if(_key[i].doubleClickTimerFlag == 0) {
						_key[i].doubleClickTimerFlag = 1;
						_key[i].doubleClickTime = 0;
					}
					//在计时范围内又按了一次
					else{
						key[i].doubleflag=1;//双击情况
						_key[i].doubleClickTimerFlag = 0;
					}
					_key[i].judgeSate = 0;
				}
				//松开且是长按键
				else if(_key[i].keyState==1 && _key[i].keyTime>=30)
				{
					_key[i].judgeSate = 0;
					key[i].longflag = 1;
				
				}
				//按下 且为长按键
				else 
					_key[i].keyTime++;
				break;
		}
		//按键单次按下
		if(_key[i].doubleClickTimerFlag == 1 && _key[i].doubleClickTime >= 25) {
			key[i].flag = 1;
			_key[i].doubleClickTimerFlag = 0;
		}
		//按键双击 双击计时
		else if(_key[i].doubleClickTimerFlag == 1){
			_key[i].doubleClickTime++;
		}
	}
}

