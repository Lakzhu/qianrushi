#ifndef __KEY_H
#define __KEY_H

#include "stm32g4xx_hal.h"
#include "stm32g4xx_it.h"
#include "main.h"

// ��������ṹ�� ��Ҫע������������߲��ܹ�ͬʱΪ1
struct keys{
	//�������ΰ���  0-��Ч   1-��������
	int flag;
	//˫�������ı�־ 0-��Ч  1-˫�ΰ���
	int doubleflag;
	//���������ı�־ 0-��Ч   1-����
	int longflag;
};

//����״̬�жϵĽṹ��
struct keyState{
	//��¼����״̬����״̬
	unsigned char judgeSate;
	//��¼������״̬
	unsigned char keyState;
	//��¼�����Ƿ�˫�� ˫���ͻ��ʱ
	unsigned char doubleClickTimerFlag;
	//��¼�������µ�ʱ��
	int keyTime;
	//��¼����˫����ʱ��
	unsigned char doubleClickTime;
};

void key_init(void);

//����һ���������� key[0]=B1 key[1]=B2 key[2]=B3 key[3]=B4 
extern struct keys key[];

/***************************************************************************************************
 * �������ܣ�����ɨ�躯�� ע��˺������ڶ�ʱ���жϣ�10ms���е�ʹ��Ч����� ����˫���볤�����������
 * ������������
 * ��������ֵ����
****************************************************************************************************/
void key_scanf(void);

#endif

