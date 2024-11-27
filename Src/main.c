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
#include <stdio.h>
#include <stdlib.h>
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

// some variable for adc
uint16_t ADCBuf[198] = {0};
uint16_t DATABuf[188] = {0};
uint16_t ProcessedBuf[564] = {0};
uint8_t ADCState = 0;             // 0 Default 1 HalfComplete 2 Complete
uint8_t ADCProcessedBufState = 0; // 0  1  2

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t max_val = 0;
uint16_t min_val = UINT16_MAX;

void find_min_max(void)
{
  // uint16_t max_pos = 0;
  // uint16_t min_pos = 0;
  max_val = 0;
  min_val = UINT16_MAX;
  // 直接遍历整个ProcessedBuf
  for (uint16_t i = 0; i < 564; i++)
  {
    if (ProcessedBuf[i] > max_val)
    {
      max_val = ProcessedBuf[i];
      // max_pos = i;
    }
    if (ProcessedBuf[i] < min_val)
    {
      min_val = ProcessedBuf[i];
      // min_pos = i;
    }
  }
}

void detect_r_peaks(uint16_t current_buf_idx);
uint16_t DrawBuff[180][11];
void RenderChart(void);
void draw_x_axis(int time);
void draw_y_axis(float Vmax);

void draw_data(void)
{
#define FIRST_LINE_Y 3
#define FIRST_LINE_X 45
  draw_x_axis(1);
  draw_y_axis(3.3);
  LCD_ShowString(FIRST_LINE_X, FIRST_LINE_Y, "Vmax:", BLUE, WHITE, 16, 0); // 40+40
  LCD_ShowString(FIRST_LINE_X + 80, FIRST_LINE_Y, "Vmin:", BLUE, WHITE, 16, 0);
  // LCD_ShowString(FIRST_LINE_X+160+8, FIRST_LINE_Y+5, "AI:", BLUE, WHITE, 16, 0);
  LCD_ShowString(160 + 30 + 15, FIRST_LINE_Y, "OK", BLUE, WHITE, 32, 0);
  LCD_ShowFloatNum1(FIRST_LINE_X + 40, FIRST_LINE_Y, max_val / 4095.0 * 3.3, 3, RED, WHITE, 16);
  LCD_ShowFloatNum1(FIRST_LINE_X + 120, FIRST_LINE_Y, min_val / 4095.0 * 3.3, 3, RED, WHITE, 16);
  // LCD_ShowFloatNum1(FIRST_LINE_X+200, FIRST_LINE_Y, 23.55, 3, RED, WHITE, 16);

#define SECOND_LINE_Y 20
#define SECOND_LINE_X 45
  LCD_ShowString(SECOND_LINE_X, SECOND_LINE_Y, "Freq:", BLUE, WHITE, 16, 0);
  LCD_ShowIntNum(SECOND_LINE_X + 40, SECOND_LINE_Y, 21.2 * 60, 3, RED, WHITE, 16);
  // LCD_ShowString(SECOND_LINE_X+60+28, SECOND_LINE_Y, "OK", BLUE, BLACK, 16, 0);
  // LCD_ShowString(FIRST_LINE_X+160, SECOND_LINE_Y, "Poss:", BLUE, WHITE, 16, 0);
  // LCD_ShowFloatNum1(SECOND_LINE_X+60+28+28, SECOND_LINE_Y, 20.0, 3, RED, WHITE, 16);
  // LCD_ShowIntNum(SECOND_LINE_X+210, SECOND_LINE_Y, 21.2 * 60, 3, BLUE, WHITE, 16);
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
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)(ADCBuf + 10), 188);

  HAL_TIM_Base_Start_IT(&htim6);
  LCD_Init();
  LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
  HAL_UART_Transmit(&huart1, "hello", 5, 5);
  HAL_TIM_Base_Start_IT(&htim2);
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

  draw_data();
  //

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  while (1)
  {
    if (ADCState == 2)
    {
      //uint8_t header[4] = {0x16, 0x80, 0x16, 0x80};
      //HAL_UART_Transmit(&huart1, (uint8_t *)header, 4, 1);
      //HAL_UART_Transmit_DMA(&huart1, (uint8_t *)ProcessedBuf, 564 * 2);
      ADCState = 0;
      detect_r_peaks(ADCProcessedBufState);
      find_min_max();
      LCD_ShowFloatNum1(FIRST_LINE_X + 40, FIRST_LINE_Y, max_val / 4095.0 * 3.3, 3, RED, WHITE, 16);
      LCD_ShowFloatNum1(FIRST_LINE_X + 120, FIRST_LINE_Y, min_val / 4095.0 * 3.3, 3, RED, WHITE, 16);
      // RenderChart();
      // HAL_Delay(1000);
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

/* USER CODE BEGIN PV */

void DrawLineArray(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint16_t *array, uint16_t width)
{
  uint16_t t;
  int xerr = 0, yerr = 0, delta_x, delta_y, distance;
  int incx, incy, uRow, uCol;
  delta_x = x2 - x1; // 计算坐标增量
  delta_y = y2 - y1;
  uRow = x1; // 画线起点坐标
  uCol = y1;
  if (delta_x > 0)
    incx = 1; // 设置单步方向
  else if (delta_x == 0)
    incx = 0; // 垂直�??
  else
  {
    incx = -1;
    delta_x = -delta_x;
  }
  if (delta_y > 0)
    incy = 1;
  else if (delta_y == 0)
    incy = 0; // 水平�??
  else
  {
    incy = -1;
    delta_y = -delta_y;
  }
  if (delta_x > delta_y)
    distance = delta_x; // 选取基本增量坐标�??
  else
    distance = delta_y;
  for (t = 0; t < distance + 1; t++)
  {
    array[uCol * width + uRow] = color; // 画点
    xerr += delta_x;
    yerr += delta_y;
    if (xerr > distance)
    {
      xerr -= distance;
      uRow += incx;
    }
    if (yerr > distance)
    {
      yerr -= distance;
      uCol += incy;
    }
  }
}

#define CHART_X_START 45
#define CHART_Y_START 45
int pos = 0;
int posC = 0;
int freshNum = 0;
void RenderChart()
{
  memset(DrawBuff, 0, 180 * 11 * 2);
  for (int j = 0; j < 180; j++)
  {
    DrawBuff[j][10] = WHITE;
  }
  for (int i = 0; i < 5; i++)
  {
    DrawLineArray(i * 2, 180 - (int)(((ProcessedBuf[(i + pos) % 564]) * 180.0f / 4095.0f)), i * 2 + 2, 180 - (int)(((ProcessedBuf[(i + pos + 1) % 564]) * 180.0f / 4096.0f)), BLUE, (uint16_t *)DrawBuff, 11);
  }
  if (posC + 10 >= 180)
  {
    posC = 0;
  }
  LCD_FillArray(CHART_X_START + posC, CHART_Y_START, CHART_X_START + posC + 11 - 1, CHART_Y_START + 180 - 1, (uint8_t *)DrawBuff);
  pos += 5;
  posC += 10;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim->Instance == TIM2)
  {
    // 心电采集读取
    freshNum = (freshNum + 1) % 5;
    if (freshNum == 0)
    {
      RenderChart();
    }
  }
}



float current_frequency = 0.0f; // 当前频率(Hz)

/* USER CODE BEGIN PV */

#define MIN_SAMPLING_FREQ 50  // �??小采样频�?? Hz
#define MAX_SAMPLING_FREQ 1000 // �??大采样频�?? Hz
#define SAMPLES_PER_CYCLE 125   // 每个心跳周期期望的采样点�??

// 添加采样频率调整相关变量
uint32_t current_sampling_freq = 125; // 当前采样频率
uint32_t target_sampling_freq = 125;  // 目标采样频率

// 调整TIM2和ADC采样频率
void adjust_sampling_frequency(float signal_frequency)
{
  if (signal_frequency <= 0)
    return;

  // 计算目标采样频率：确保每个周期有足够的采样点
  target_sampling_freq = (uint32_t)(signal_frequency * SAMPLES_PER_CYCLE);

  // 限制采样频率范围
  if (target_sampling_freq < MIN_SAMPLING_FREQ)
  {
    target_sampling_freq = MIN_SAMPLING_FREQ;
  }
  else if (target_sampling_freq > MAX_SAMPLING_FREQ)
  {
    target_sampling_freq = MAX_SAMPLING_FREQ;
  }
  // 发�?�采样频率更新信�??
    char msg[32];
    sprintf(msg, "TG:%d Hz\r\n", (int)target_sampling_freq);
	HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 10);
  // 如果频率变化超过10%才更�??
  if (abs((int32_t)target_sampling_freq - (int32_t)current_sampling_freq) > (current_sampling_freq / 10))
  {
    // 计算TIM2的周期�??
    // APB1 Timer Clock = 180MHz/4 = 45MHz
    uint32_t timer_period = (45000000 / target_sampling_freq) - 1;

    // 停止定时器和ADC
//    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
//   HAL_ADC_Stop_DMA(&hadc1);

//    // 更新TIM2配置
   TIM2->ARR = timer_period;
    TIM2->CCR2 = timer_period / 2; // 50% 占空�??

//    // 重启定时器和ADC
//   HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
//   HAL_ADC_Start_DMA(&hadc1, (uint32_t *)(ADCBuf + 10), 188);

   current_sampling_freq = target_sampling_freq;

    // 发�?�采样频率更新信�??
    char msg[32];
    sprintf(msg, "SF:%d Hz\r\n", (int)current_sampling_freq);
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 10);
  }
}


uint8_t get_prev_buf_idx(uint8_t current_idx)
{
  return (current_idx + 2) % 3;
}

uint8_t get_next_buf_idx(uint8_t current_idx)
{
  return (current_idx + 1) % 3;
}
uint16_t get_current_value(uint16_t *buffer1, uint16_t *buffer2, uint16_t *buffer3, uint16_t i) {
    if (i < 188) {
        return buffer1[i];
    } else if (i < 376) {
        return buffer2[i - 188];
    } else {
        return buffer3[i - 376];
    }
}

uint16_t get_prev_value(uint16_t *buffer1, uint16_t *buffer2, uint16_t *buffer3, uint16_t i) {
    if (i < 188) {
        return (i > 0) ? buffer1[i - 1] : buffer1[0];
    } else if (i < 376) {
        return (i > 188) ? buffer2[i - 189] : buffer1[187];
    } else {
        return (i > 376) ? buffer3[i - 377] : buffer2[187];
    }
}

uint16_t get_next_value(uint16_t *buffer1, uint16_t *buffer2, uint16_t *buffer3, uint16_t i) {
    if (i < 188) {
        return (i < 187) ? buffer1[i + 1] : buffer2[0];
    } else if (i < 376) {
        return (i < 375) ? buffer2[i - 187] : buffer3[0];
    } else {
        return (i < 562) ? buffer3[i - 375] : buffer3[187];
    }
}
void detect_r_peaks(uint16_t current_buf_idx)
{
	current_buf_idx = (current_buf_idx+1)%3;

	uint16_t trigCount = 0;
	
	uint16_t *buffer1 = &ProcessedBuf[188 * get_prev_buf_idx(current_buf_idx)];
  uint16_t *buffer2 = &ProcessedBuf[188 * current_buf_idx];
  uint16_t *buffer3 = &ProcessedBuf[188 * get_next_buf_idx(current_buf_idx)];
  static uint16_t samples_since_last_r = 0;

  for (uint16_t i = 0+3; i < 564-3; i++)
  {
    samples_since_last_r++;

    // 获取当前点和相邻点的�?

//    uint16_t current_value = get_current_value(buffer1, buffer2, buffer3, i);
		uint16_t current_value = 1000;
    uint16_t prev_value = get_prev_value(buffer1, buffer2, buffer3, i);
    uint16_t next_value = get_next_value(buffer1, buffer2, buffer3, i);

//			char msg[20];
//      sprintf(msg, "%d\r\n", current_value);
//      HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 10);

    if (
        current_value > get_prev_value(buffer1, buffer2, buffer3, i - 1) &&
        current_value > get_prev_value(buffer1, buffer2, buffer3, i - 2) &&
        current_value > get_prev_value(buffer1, buffer2, buffer3, i - 3) &&
        current_value < get_next_value(buffer1, buffer2, buffer3, i + 1) &&
        current_value < get_next_value(buffer1, buffer2, buffer3, i + 2) &&
        current_value < get_next_value(buffer1, buffer2, buffer3, i + 3) &&
				samples_since_last_r > 5
		)
    {
			trigCount++;
			if(trigCount>1){
				char msg[20];
				sprintf(msg, "R:%f\r\n", current_sampling_freq*1.0f/samples_since_last_r);
				
				HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 10);
				//adjust_sampling_frequency(125*1.0f/samples_since_last_r);
			}
      samples_since_last_r = 0;
      
      int heart_rate = (int)(current_frequency * 60);
      
    }
  }
}

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

// ADC Half
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
  // HAL_UART_Transmit(&huart1,"hello",5,5);
  if (ADCState == 0)
  {
    memcpy(DATABuf, ADCBuf + 10, 2 * 94);
    memcpy(ADCBuf, ADCBuf + 188, 2 * 10);
    ADCState = 1;
    Opt_ADC_Value(ADCBuf + 10 - 10, ProcessedBuf + 188 * ADCProcessedBufState, 5, 3, 94, 5, 5);
    // HAL_UART_Transmit(&huart1,"half",4,5);
  }
}
// ADC Full
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  // HAL_UART_Transmit(&huart1,"hell2",5,5);
  // HAL_UART_Transmit(&huart1,"bk",2,5);
  if (ADCState == 1)
  {

    memcpy(DATABuf + 94, ADCBuf + 94 + 10, 2 * 94);
    ADCState = 2;
    Opt_ADC_Value(ADCBuf + 10 - 10 + 94, ProcessedBuf + 188 * ADCProcessedBufState + 94, 5, 3, 94, 5, 5);
    // memcpy(ProcessedBuf+188*ADCProcessedBufState+ 94, ADCBuf + 94, 2 * 94);
    ADCProcessedBufState = (ADCProcessedBufState + 1) % 3;
    if (ADCProcessedBufState == 2)
    {
      pos = 0;
    }
    // HAL_UART_Transmit(&huart1,"ok",2,5);
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
