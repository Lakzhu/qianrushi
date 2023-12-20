/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "user.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
	key_up = 0U,
	key_down,
	key_wait
}key_states;

extern UART_HandleTypeDef huart1;
extern unsigned char num;
uint8_t lcd_str0[20],lcd_str1[20],lcd_str2[20],lcd_str3[20];
uint8_t Rxbuff[7];
uint8_t password[4] = {'1','2','3','\0'};
int password_text[4] = {0,0,0,0};
uint8_t jiemian = 0, qing = 0, password_HZ = 0;
unsigned char lcd_flag = 0, pwm_time=0, error_count = 0,led2_time = 0,led2_flag=0;
extern struct keys key[4];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
//	key_init();
	LCD_Init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	//黑背景 白字 关闭所有LED
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	led_turndown();
	LCD_Clear(Black);
	//定时器启动
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_PWM_Start_IT(&htim2,TIM_CHANNEL_1);
	//打开串口的中断接收功能
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&Rxbuff,sizeof(Rxbuff));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {	
		//pwm 切换输出
		Pwm_chage();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
		key_scanf();
		if(key[0].flag == 1)//按键输入密码
		{
			password_HZ = 1;
			password_text[0] = (password_text[0] + 1)%10;
		}
		if(key[1].flag == 1)
		{
			password_HZ = 1;
			password_text[1] = (password_text[1] + 1)%10;
		}
		if(key[2].flag == 1)
		{
			password_HZ = 1;
			password_text[2] = (password_text[2] + 1)%10;
		}
		if(key[3].flag == 1)//按键4确认
		{
			if((uint8_t)(password_text[0] + 48) == password[0] && 
					(uint8_t)(password_text[1] + 48) == password[1] &&
						(uint8_t)(password_text[2] + 48) == password[2]){
				lcd_flag = 1;
				LCD_Clear(Black);
			}
			else{//错误计数
				error_count = error_count + 1;
			}
		}
		for(qing = 0; qing < 4; qing++)
		{
			key[qing].flag = 0;
			key[qing].doubleflag = 0;
			key[qing].longflag = 0;
		}
	}
	if(htim->Instance == TIM4)
	{
		if(lcd_flag == 0){//led2 0.1秒闪烁
			if(error_count >= 3 && led2_time <= 50){
					led2_time++;
					switch(led2_flag){
						case 0:led2_turnOn();led2_flag = 1;break;
						case 1:led2_turnDown();led2_flag = 0;break;
					}
			}
			else if(led2_time >= 50){
				led2_time = 0;
				error_count = 2;
				led_turndown();
			}
			if(password_HZ == 0){//启动界面显示
				sprintf((char*)lcd_str0,"       PSD           ");
				sprintf((char*)lcd_str1,"    B1:%c            ",'@');
				sprintf((char*)lcd_str2,"    B2:%c            ",'@');
				sprintf((char*)lcd_str3,"    B3:%c            ",'@');
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				LCD_DisplayStringLine(Line2,lcd_str0);
				LCD_DisplayStringLine(Line4,lcd_str1);
				LCD_DisplayStringLine(Line5,lcd_str2);
				LCD_DisplayStringLine(Line6,lcd_str3);
			}
			else{//密码锁显示
				sprintf((char*)lcd_str0,"       PSD           ");
				sprintf((char*)lcd_str1,"    B1:%c            ",(uint8_t)(password_text[0] + 48));
				sprintf((char*)lcd_str2,"    B2:%c            ",(uint8_t)(password_text[1] + 48));
				sprintf((char*)lcd_str3,"    B3:%c            ",(uint8_t)(password_text[2] + 48));
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
				LCD_DisplayStringLine(Line2,lcd_str0);
				LCD_DisplayStringLine(Line4,lcd_str1);
				LCD_DisplayStringLine(Line5,lcd_str2);
				LCD_DisplayStringLine(Line6,lcd_str3);
			}
		}
		else{
				if(pwm_time <= 50){//PWM 2KHZ计时
					pwm_time++;
				}
				if(pwm_time <= 50){//2KHZ
					sprintf((char*)lcd_str0,"       STA           ");
					sprintf((char*)lcd_str1,"    F:2000HZ         ");
					sprintf((char*)lcd_str2,"    D:10%%           ");
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
					LCD_DisplayStringLine(Line2,lcd_str0);
					LCD_DisplayStringLine(Line4,lcd_str1);
					LCD_DisplayStringLine(Line5,lcd_str2);
				}
				else{//正常界面显示
					sprintf((char*)lcd_str0,"       STA           ");
					sprintf((char*)lcd_str1,"    F:1000HZ         ");
					sprintf((char*)lcd_str2,"    D:50%%           ");
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
					LCD_DisplayStringLine(Line2,lcd_str0);
					LCD_DisplayStringLine(Line4,lcd_str1);
					LCD_DisplayStringLine(Line5,lcd_str2);
				}
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){//串口传输密码，密码替换
		// 重新使能中断
		HAL_UART_Receive_IT(huart,(uint8_t *)&Rxbuff,sizeof(Rxbuff)); 
		if(Rxbuff[3] == '-' && Rxbuff[0] == password[0] && Rxbuff[1] == password[1] && Rxbuff[2] == password[2]
		&& Rxbuff[4] >= '0' && Rxbuff[4] <= '9' && Rxbuff[5] >= '0' && Rxbuff[5] <= '9'
			&& Rxbuff[6] >= '0' && Rxbuff[6] <= '9'){
				password[0] = Rxbuff[4];
				password[1] = Rxbuff[5];
				password[2] = Rxbuff[6];
			}
	}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
