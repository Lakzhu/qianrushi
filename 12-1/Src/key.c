#include "key.h"

struct keys key[4] = {0,0,0,0};

/*********************************
 * �������ܣ������ĳ�ʼ������
 * ������������
 * ��������ֵ����
*********************************/
void key_init(void)
{
    GPIO_InitTypeDef GPIO_initStructure;

/*********����B4(PA0)��ʼ��*******************/
#if KEYPA
    __HAL_RCC_GPIOA_CLK_ENABLE();
    //��������ģʽ
    GPIO_initStructure.Mode = GPIO_MODE_IT_FALLING;
    //���óɵ�Ƶ
    GPIO_initStructure.Speed = GPIO_SPEED_FREQ_LOW;
    //GPIO��������
    GPIO_initStructure.Pin = GPIO_PIN_0;
    //���ŵ�ģʽ����
    GPIO_initStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA,&GPIO_initStructure);
#endif

/*********����B4��ʼ��*******************/
#if KEYPB
    __HAL_RCC_GPIOB_CLK_ENABLE();
        //��������ģʽ
    GPIO_initStructure.Mode = GPIO_MODE_IT_FALLING;
    //���óɵ�Ƶ
    GPIO_initStructure.Speed = GPIO_SPEED_FREQ_LOW;
    //GPIO��������
    GPIO_initStructure.Pin = KEYGPIOB;
    //���ŵ�ģʽ����
    GPIO_initStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB,&GPIO_initStructure);
#endif
}


/****************************************************************************************************
 * �������ܣ�����ɨ�躯�� ע��˺������ڶ�ʱ���жϣ�10ms���е�ʹ��Ч����� ����˫���볤�����������
 * ������������
 * ��������ֵ���� 
*****************************************************************************************************/
void key_scanf(void)
{
	static struct keyState _key[4];
	
	//��ȡ����������״̬
	_key[0].keyState = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	_key[1].keyState = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	_key[2].keyState = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	_key[3].keyState = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
	
	//������������״̬
	for(int i=0;i<4;i++){
		switch (_key[i].judgeSate){
			//������һ�ΰ���
			case 0:
				if(_key[i].keyState == 0){
					//��ת������״̬
					_key[i].judgeSate=1;
					//��հ���ʱ��
					_key[i].keyTime=0;
				}
				break;
			//�����ڶ��ΰ��� �������10ms��������������	
			case 1:
				//�����ٴΰ��� ��ת����״̬
				if(_key[i].keyState == 0)
					_key[i].judgeSate=2;
				//��һ�ΰ��������Ƕ������� ������Ч״̬ Ӧ���˻��ʼ��״̬
				else 
					_key[i].judgeSate=0;
				break;
			//ȷ���������º�Ĵ������
			case 2:
				//�ȴ��ɿ�����,�ҷǳ�����
				if((_key[i].keyState==1) && _key[i].keyTime<30){	
					//����˫�������ĵ�һ�Σ������ʱ
					if(_key[i].doubleClickTimerFlag == 0) {
						_key[i].doubleClickTimerFlag = 1;
						_key[i].doubleClickTime = 0;
					}
					//�ڼ�ʱ��Χ���ְ���һ��
					else{
						key[i].doubleflag=1;//˫�����
						_key[i].doubleClickTimerFlag = 0;
					}
					_key[i].judgeSate = 0;
				}
				//�ɿ����ǳ�����
				else if(_key[i].keyState==1 && _key[i].keyTime>=30)
				{
					_key[i].judgeSate = 0;
					key[i].longflag = 1;
				
				}
				//���� ��Ϊ������
				else 
					_key[i].keyTime++;
				break;
		}
		//�������ΰ���
		if(_key[i].doubleClickTimerFlag == 1 && _key[i].doubleClickTime >= 25) {
			key[i].flag = 1;
			_key[i].doubleClickTimerFlag = 0;
		}
		//����˫�� ˫����ʱ
		else if(_key[i].doubleClickTimerFlag == 1){
			_key[i].doubleClickTime++;
		}
	}
}

