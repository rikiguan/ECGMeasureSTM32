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
 *target1:改操作界�??????(OK)
 *target2:摆脱滤波操作（也还好�??????
 *target3:加入暂停与坐标轴功能（矩阵键盘）
 *target4:
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
uint16_t ProcessedBuf[564] = {0}; // 3个存放数�??????
uint8_t ADCState = 0;             // 0 Default 1 HalfComplete 2 Complete
uint8_t ADCProcessedBufState = 0; // 0  1  2

// 两个功能�??????
#define Key_High HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_7) == 1 //
#define Key_Low HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_7) == 0
#define ModeKey_High HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6) == 1 //
#define ModeKey_Low HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6) == 0
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t max_val = 0;
uint16_t min_val = UINT16_MAX;
float feq = 0;
int heart_rate;
uint16_t DrawBuff[180][11];
int jumpBuffer = 0;
int Cursor = 0;
#define MIN_SAMPLING_FREQ 5        // // �??????小采样频�?????? Hz
#define MAX_SAMPLING_FREQ 10000000 // �??????大采样频�?????? Hz
#define SAMPLES_PER_CYCLE 50       //  每个心跳周期期望的采样点�??????

// 函数声明
void detect_r_peaks(uint16_t current_buf_idx);
void RenderChart1(void);
void draw_data(void);
void find_min_max(void);
void button_handler(void);

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
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
//HAL_UART_Transmit(&huart1, "hello", 5, 5);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)(ADCBuf + 10), 198);

  //HAL_TIM_Base_Start_IT(&htim6);
  LCD_Init();
  LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
  HAL_TIM_Base_Start_IT(&htim6);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  draw_data();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  while (1)
  {
    if (ADCState == 2)
    {
			uint8_t header[4] = {0x16, 0x80, 0x16, 0x80};
			// HAL_UART_Transmit(&huart1, (uint8_t *)header, 4, 1);
      // HAL_UART_Transmit_DMA(&huart1, (uint8_t *)ProcessedBuf, 564 * 2);
      ADCState = 0;
      if (jumpBuffer == 0)
      {
        detect_r_peaks(ADCProcessedBufState);
      }
      else
      {
        jumpBuffer--;
      }
      find_min_max();
      LCD_ShowFloatNum1(120, 220, max_val / 4095.0 * 3.3, 3, RED, WHITE, 16);
      LCD_ShowFloatNum1(200, 220, min_val / 4095.0 * 3.3, 3, RED, WHITE, 16);
    }


    // 光标
    if (Key_Low)
    {
      HAL_Delay(10);
      if (Key_Low)
      {
        HAL_Delay(10);
        if (Key_Low)
          Cursor = (Cursor + 5) % 180;
        do
        {
          while (Key_Low)
            ;
          HAL_Delay(10);
        } while (Key_Low);
      }
    };

    // 按键控制读取多个周期进行自�?�应
    if (ModeKey_Low)
    {
      HAL_Delay(10);
      if (ModeKey_Low)
        do
        {
          while (ModeKey_Low)
            ;
          HAL_Delay(10);
        } while (ModeKey_Low);
    }

    // button_handler();
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
    incx = 0; // 垂直恒定�??????
  else
  {
    incx = -1;
    delta_x = -delta_x;
  }
  if (delta_y > 0)
    incy = 1;
  else if (delta_y == 0)
    incy = 0; // 水平恒定�??????
  else
  {
    incy = -1;
    delta_y = -delta_y;
  }
  if (delta_x > delta_y)
    distance = delta_x; // 选取基本增量坐标
  else
    distance = delta_y;
  for (t = 0; t < distance + 1; t++)
  {
    array[uCol * width + uRow] = color; //
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

// 将波形转化为二维图表进行刷屏
#define CHART_X_START 10
#define CHART_Y_START 25
#define Y_TOTAL 190
#define X_TOTAL 188

float pos = 0;
int posC = 0;
int freshNum = 0;

uint16_t DrawBuff1[Y_TOTAL][10];

void RenderChart1()
{
  int posS = pos;
  for (posC = 0; posC < X_TOTAL; posC += 10)
  {
    memset(DrawBuff1, BLACK, Y_TOTAL * 10 * 2);

    for (int i = 0; i < 5; i++)
    {
      DrawLineArray(i * 2, Y_TOTAL - (int)(((ProcessedBuf[(i + posS) % 564]) * 140.0f / 4095.0f)) - 1, i * 2 + 2, Y_TOTAL - (int)(((ProcessedBuf[(i + posS + 1) % 564]) * 140.0f / 4095.0f)) - 1, YELLOW, (uint16_t *)DrawBuff1, 10);
    }
    for (int y = Y_TOTAL - 1; y >= 0; y -= 20)
    {
      for (int x = 0; x < 10; x++)
      {
        if ((CHART_X_START + posC + x) % 10 < 5)
        {
          DrawBuff1[y][x] = RED;
        }
      }
    }
    for (int x = 0; x < 10; x++)
    {
      DrawBuff1[Cursor][x] = GREEN;
    }

    LCD_FillArray(CHART_X_START + posC, CHART_Y_START, CHART_X_START + posC + 10 - 1, CHART_Y_START + Y_TOTAL - 1, (uint8_t *)DrawBuff1);
    // LCD_DrawRectangle(CHART_X_START + posC,0,CHART_X_START + posC + 10 - 1,215,BLACK);
    posS += 5;
  }
  LCD_ShowIntNum(210, 0, heart_rate, 3, RED, WHITE, 16);
}


int tick = 0;
int last_dma_tick = 0;
float point_num_each_tick = 2;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  //系统刷屏幕
  if (htim->Instance == TIM6)
  {
    // 心电采集读取
    freshNum = (freshNum + 1) % 5;
    pos += point_num_each_tick;
    if (freshNum == 0)
    {
      RenderChart1();
    }
    tick++;
    // char str[10];
    // sprintf(str, "tick:%d\n\r", tick);
    // HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), 1000);
  }

}

/* USER CODE BEGIN PV */

// 获取循环idx的前后下�??????
uint8_t get_prev_buf_idx(uint8_t current_idx)
{
  return (current_idx + 2) % 3;
}

uint8_t get_next_buf_idx(uint8_t current_idx)
{
  return (current_idx + 1) % 3;
}

// 实现循环数组读取adc数据
uint16_t get_current_value(uint16_t *buffer1, uint16_t *buffer2, uint16_t *buffer3, uint16_t i)
{
  if (i < 188)
  {
    return buffer1[i];
  }
  else if (i < 376)
  {
    return buffer2[i - 188];
  }
  else
  {
    return buffer3[i - 376];
  }
}

uint16_t get_prev_value(uint16_t *buffer1, uint16_t *buffer2, uint16_t *buffer3, uint16_t i)
{
  if (i < 188)
  {
    return (i > 0) ? buffer1[i - 1] : buffer1[0];
  }
  else if (i < 376)
  {
    return (i > 188) ? buffer2[i - 189] : buffer1[187];
  }
  else
  {
    return (i > 376) ? buffer3[i - 377] : buffer2[187];
  }
}

uint16_t get_next_value(uint16_t *buffer1, uint16_t *buffer2, uint16_t *buffer3, uint16_t i)
{
  if (i < 188)
  {
    return (i < 187) ? buffer1[i + 1] : buffer2[0];
  }
  else if (i < 376)
  {
    return (i < 375) ? buffer2[i - 187] : buffer3[0];
  }
  else
  {
    return (i < 562) ? buffer3[i - 375] : buffer3[187];
  }
}

// �??????测R�??????
#define WINDOW_SIZE_MAIN 3
volatile float captureValues11[WINDOW_SIZE_MAIN];
uint8_t captureIndex11 = 0;

void detect_r_peaks(uint16_t current_buf_idx)
{
  current_buf_idx = (current_buf_idx + 1) % 3;

  uint16_t trigCount = 0; // �??????测到的心电周期数

  uint16_t *buffer1 = &ProcessedBuf[188 * get_prev_buf_idx(current_buf_idx)]; // 获取前一组开头地�??????
  uint16_t *buffer2 = &ProcessedBuf[188 * current_buf_idx];                   // 获取当前组开头地�??????
  uint16_t *buffer3 = &ProcessedBuf[188 * get_next_buf_idx(current_buf_idx)]; // 获取下一组开头地�??????
  static uint16_t samples_since_last_r = 0;

  for (uint16_t i = 0 + 3; i < 564 - 3; i++)
  {
    samples_since_last_r++; // 用于记录自上次检测到R峰�?�以来的样本数�??

    // 获取当前点和相邻点的坐标
    uint16_t current_value = 2000; // 随便设的中间�??????
    uint16_t prev_value = get_prev_value(buffer1, buffer2, buffer3, i);
    uint16_t next_value = get_next_value(buffer1, buffer2, buffer3, i);

    if (
        current_value > get_prev_value(buffer1, buffer2, buffer3, i - 1) &&
        current_value < get_next_value(buffer1, buffer2, buffer3, i + 1) &&
        samples_since_last_r > 5 // 至少5个点，找到一个R波上升沿
    )
    {
      trigCount++; // �??????测到的心电周期数++
      if (trigCount > 1)
      {
        avgFilter_FLOAT(125 * 1.0f / samples_since_last_r, (float *)captureValues11, &captureIndex11, (float *)&(feq), WINDOW_SIZE_MAIN);
        samples_since_last_r = 0;
        LCD_ShowFloatNum1(40, 220, feq, 3, RED, WHITE, 16);
        heart_rate = (int)(feq * 60);
      }
    }
  }
}

void draw_data(void)
{
  LCD_ShowString(80, 220, "Vmax:", BLUE, WHITE, 16, 0);
  LCD_ShowString(160, 220, "Vmin:", BLUE, WHITE, 16, 0);
  LCD_ShowString(0, 220, "Freq:", BLUE, WHITE, 16, 0);
  LCD_ShowString(170, 0, "bpm:", BLUE, WHITE, 16, 0);
  LCD_DrawLine(9, 24, 9, 215, BLACK);
  LCD_DrawLine(0, 215, 240, 215, BLACK);
  LCD_DrawLine(0, 24, 240, 24, BLACK);
  for (int i = 0; i <= 3; i++) // �??????4个标记：0V�??????1V�??????2V�??????3V
  {
    LCD_DrawLine(7, 215 - 55 * i, 10, 215 - 55 * i, BLACK);
    LCD_ShowIntNum(0, 198 - 55 * i, i, 1, BLACK, WHITE, 16);
  }
  LCD_ShowString(0, 0, "V", BLUE, WHITE, 16, 0);
}

// adc读�?�返�??????
//  ADC Half	�??????半返�??????
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (ADCState == 0)
  {
    memcpy(DATABuf, ADCBuf + 10, 2 * 94);
    memcpy(ADCBuf, ADCBuf + 188, 2 * 10);
    ADCState = 1;
    Opt_ADC_Value(ADCBuf + 10 - 10, ProcessedBuf + 188 * ADCProcessedBufState, 3, 2, 94, 5, 5);
  }
	
        // char msg[32];
        // sprintf(msg, "AA:%d Hz\r\n", (int)pos);
        // HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 10);
        
        //initFlag = 1;
    HAL_UART_Transmit(&huart1, "DMA Half\n", 9, 10);
}
uint8_t initFlag =0;
// ADC Full 测满返回
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (ADCState == 1)
  {

    memcpy(DATABuf + 94, ADCBuf + 94 + 10, 2 * 94);
    ADCState = 2;
    Opt_ADC_Value(ADCBuf + 10 - 10 + 94, ProcessedBuf + 188 * ADCProcessedBufState + 94, 3, 2, 94, 5, 5);
    ADCProcessedBufState = (ADCProcessedBufState + 1) % 3;

    HAL_UART_Transmit(&huart1, "DMA Done\n", 9, 10);
    char msg[64];
    int addition=0;
    if(initFlag == 0 &ADCProcessedBufState == 2){
      pos=0;
      initFlag=1;
    }else if (ADCProcessedBufState == 2)
    {
      pos=pos -188*3;
      addition=(int)pos;
    }else if (ADCProcessedBufState == 1)
    {
      addition=(int)pos -188*2;
    }else if (ADCProcessedBufState == 0)
    {
      addition=(int)pos -188;
    }
    //打印addition
    sprintf(msg, "addition:%d\n",addition);
    HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 10);
  if(addition >10){
    addition =10;
  }
  if(addition <-10){
    addition =-10;
  }
    point_num_each_tick=(188.0f-addition)/(tick -last_dma_tick);
    //打印point_num_each_tick和pos和ADCProcessedBufState

    sprintf(msg, "%dpoint_num_each_tick:%f pos:%f ADCProcessedBufState:%d\n",(tick -last_dma_tick), point_num_each_tick,pos,ADCProcessedBufState);
    HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 10);
    last_dma_tick = tick;
    

    
  }
	
        // char msg[32];
        // sprintf(msg, "BB:%d Hz\r\n", (int)pos);
        // HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 10);
        //pos = 0;
        //initFlag = 1;
      
}

void button_handler(void)
{
  int key_num = Read_Keys();
  if (key_num)
  {
    if (key_num == 13)
    {
      // add code here
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
    }
    //		else if(key_num==14)
    //		{
    //
    //		}
    //		else if(key_num==15)
    //		{
    //
    //		}
    //		if (key_num == 9)
    //		{
    //
    //		}
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
