/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD/lcd_init.h"
#include "LCD/lcd.h"
#include "LCD/pic.h"
#include <string.h>
#include "filter.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//some variable for adc
uint16_t ADCBuf[188] = {0};
uint16_t DATABuf[188] = {0};
uint16_t ProcessedBuf[564] = {0};
uint8_t ADCState = 0; // 0 Default 1 HalfComplete 2 Complete
uint8_t ADCProcessedBufState = 0; // 0  1  2 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t DrawBuff[180][10];
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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI5_Init();
  MX_ADC1_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADCBuf, 188);
	

	HAL_TIM_Base_Start_IT(&htim6);
	LCD_Init();		
	LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); 
  HAL_UART_Transmit(&huart1,"hello",5,5);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	// int pos = 0;
	// while (1)
	// {

//		Vmax = 0;
//		Freq = 0;
//		Vmin = 3.3; 
//		Vave = 0;
//		int i = 0;


//		LCD_ShowFloatNum1(60, 40, Vmax, 3, RED, WHITE, 16);
//		LCD_ShowFloatNum1(120, 20, Vmin, 3, RED, WHITE, 16);
//		LCD_ShowFloatNum1(120, 40, Vave, 3, RED, WHITE, 16);

//		for (i = 0; i < 100; i++)
//		{
//			LCD_DrawLine(20 + 2 * i, 220 - 200 * adc_data_copy[start_draw + start_time * i] / start_Vmax, 20 + 2 * (i + 1), 220 - 200 * adc_data_copy[start_draw + start_time * (i + 1)] / start_Vmax, WHITE);
//		}

//		LCD_ShowFloatNum1(60, 20, Freq, 3, RED, WHITE, 16);
//		LCD_ShowString(170, 35, "bpm:", BLUE, WHITE, 16, 0);
//		LCD_ShowIntNum(210, 35, Freq * 60, 3, BLUE, WHITE, 16);

//	
		
		
	// 	for (int i = 0; i < 10; i++)
	// 	{
	// 		if(i+pos>=188){
	// 			pos = 0;
	// 			break;
	// 		}
	// 		//LCD_DrawLine(20 +  i, 220 - 200 * DATABuf[i] / start_Vmax, 20 +  (i + 1), 220 - 200 * DATABuf[ (i + 1)] / start_Vmax, RED);
			
	// 		for (int k = 0; k < 180; k++)
	// 		{
	// 			DrawBuff[k][i]=WHITE;
	// 		}
	// 		DrawBuff[50+(int)((float)(DATABuf[i+pos])*10/4096)][i] = BLUE;
	// 	}
		
	// 	LCD_FillArray(40+pos,50,40+pos+9,229, (uint8_t*)DrawBuff);
	// 	ADCState=0;
	// 	pos+=10;

	// }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	while(1){
		if(ADCState  == 2){
			uint8_t header[4]={0x16,0x80,0x16,0x80};
			HAL_UART_Transmit(&huart1, (uint8_t *)header, 4,1);
			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)ProcessedBuf, 564*2);
	    ADCState=0;    
			//HAL_Delay(1000);
		}

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */



void draw_x_axis(int time)
{
	LCD_DrawLine(17, 221, 221, 219, BLACK); 
	for (int i = 1; i <= 4; i++)	
	{
		LCD_DrawLine(17 + 40 * i, 221, 17 + 40 * i, 211, BLACK);
		LCD_ShowIntNum(10 + 40 * i, 221, i * 66 * time, 3, BLUE, WHITE, 16);
		}
	LCD_ShowString(220, 220, "ms", BLUE, WHITE, 16, 0);
}
void draw_y_axis(float Vmax)
{
	LCD_DrawLine(17, 221, 17, 17, BLACK);
	for (int i = 1; i <= 4; i++) 
	{
		LCD_DrawLine(17, 221 - 40 * i, 27, 221 - 40 * i, BLACK);
		LCD_ShowFloatNum1(6, 218 - 40 * i, i * Vmax / 5, 3, BLUE, WHITE, 16);
	}
	LCD_ShowString(20, 20, "V", BLUE, WHITE, 16, 0);
}




//ADC Half
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
  //HAL_UART_Transmit(&huart1,"hello",5,5);
	if (ADCState == 0)
	{
		memcpy(DATABuf, ADCBuf, 2 * 94);
		ADCState = 1;
    Opt_ADC_Value(ADCBuf,ProcessedBuf+188*ADCProcessedBufState,5,3,94);
		//HAL_UART_Transmit(&huart1,"half",4,5);
	}
}
//ADC Full
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	//HAL_UART_Transmit(&huart1,"hell2",5,5);
	if (ADCState == 1)
	{
		
		memcpy(DATABuf + 94, ADCBuf + 94, 2 * 94);
		ADCState = 2;
   // Opt_ADC_Value(ADCBuf + 94,ProcessedBuf+188*ADCProcessedBufState+ 94,10,8,94);
    memcpy(ProcessedBuf+188*ADCProcessedBufState+ 94, ADCBuf + 94, 2 * 94);
		ADCProcessedBufState = (ADCProcessedBufState+1) %3;
		//HAL_UART_Transmit(&huart1,"ok",2,5);
    
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
	   tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
