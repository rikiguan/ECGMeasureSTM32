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

//some variable for adc
uint16_t ADCBuf[198] = {0};
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
uint16_t DrawBuff[180][11];
void RenderChart();
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
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)(ADCBuf+10), 188);
	

	HAL_TIM_Base_Start_IT(&htim6);
	LCD_Init();		
	LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); 
  HAL_UART_Transmit(&huart1,"hello",5,5);
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


		LCD_ShowFloatNum1(60, 40, 2.3, 3, RED, WHITE, 16);
		LCD_ShowFloatNum1(120, 20, 2.3, 3, RED, WHITE, 16);
		LCD_ShowFloatNum1(120, 40, 2.3, 3, RED, WHITE, 16);

//		for (i = 0; i < 100; i++)
//		{
//			LCD_DrawLine(20 + 2 * i, 220 - 200 * adc_data_copy[start_draw + start_time * i] / start_Vmax, 20 + 2 * (i + 1), 220 - 200 * adc_data_copy[start_draw + start_time * (i + 1)] / start_Vmax, WHITE);
//		}

//		LCD_ShowFloatNum1(60, 20, Freq, 3, RED, WHITE, 16);
//		LCD_ShowString(170, 35, "bpm:", BLUE, WHITE, 16, 0);
//		LCD_ShowIntNum(210, 35, Freq * 60, 3, BLUE, WHITE, 16);

//	
		

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	while(1){
		if(ADCState  == 2){
			uint8_t header[4]={0x16,0x80,0x16,0x80};
			HAL_UART_Transmit(&huart1, (uint8_t *)header, 4,1);
			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)ProcessedBuf, 564*2);
	    ADCState=0;    
      //RenderChart();
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

/* USER CODE BEGIN PV */

void DrawLineArray(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color,uint16_t *array,uint16_t width)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直�? 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平�? 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标�? 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		array[uCol*width+uRow]=color;//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
int pos=0;
int posC=0;
int freshNum=0;
void RenderChart(){
		memset(DrawBuff,0,180*11*2);
    for(int j=0;j<180;j++){
      DrawBuff[j][10]=WHITE;
    }
		for (int i = 0; i < 5; i++)
		{
      DrawLineArray(i*2,(int)(((ProcessedBuf[(i+pos)%564])*180.0f/4096.0f)),i*2+2,(int)(((ProcessedBuf[(i+pos+1)%564])*180.0f/4096.0f)),BLUE,(uint16_t *)DrawBuff,11);
		}
		if(posC+10>=180){
			posC=0;
		}
		LCD_FillArray(40+posC,50,40+posC+10,229, (uint8_t*)DrawBuff);
		pos+=5;
		posC+=10;

}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim->Instance == TIM2){
      //心电采集读取
      freshNum = (freshNum+1)%5;
      if(freshNum==0){
        RenderChart();
      }
  }
  
}











#define THRESHOLD 3000  // R波检测阈�?
#define MIN_RR_SAMPLES 50  // �?小RR间隔
#define SAMPLING_RATE 500  // 采样�?(Hz)，根据您的实际采样率调整

typedef struct {
    uint16_t intervals[10];  // 存储�?�?10个RR间隔
    uint8_t head;           // 循环队列头指�?
    uint16_t total;         // 间隔总和
    uint8_t count;          // 有效间隔数量
} RR_Buffer;

RR_Buffer rr_buffer = {0};
float current_frequency = 0.0f;  // 当前频率(Hz)


/* USER CODE BEGIN PV */

#define MIN_SAMPLING_FREQ 250   // �?小采样频�? Hz
#define MAX_SAMPLING_FREQ 1000  // �?大采样频�? Hz
#define SAMPLES_PER_CYCLE 50    // 每个心跳周期期望的采样点�?

// 添加采样频率调整相关变量
uint32_t current_sampling_freq = 500;  // 当前采样频率
uint32_t target_sampling_freq = 500;   // 目标采样频率

// 调整TIM2和ADC采样频率
void adjust_sampling_frequency(float signal_frequency) {
    if(signal_frequency <= 0) return;
    
    // 计算目标采样频率：确保每个周期有足够的采样点
    target_sampling_freq = (uint32_t)(signal_frequency * SAMPLES_PER_CYCLE);
    
    // 限制采样频率范围
    if(target_sampling_freq < MIN_SAMPLING_FREQ) {
        target_sampling_freq = MIN_SAMPLING_FREQ;
    } else if(target_sampling_freq > MAX_SAMPLING_FREQ) {
        target_sampling_freq = MAX_SAMPLING_FREQ;
    }
    
    // 如果频率变化超过10%才更�?
    if(abs((int32_t)target_sampling_freq - (int32_t)current_sampling_freq) > (current_sampling_freq / 10)) {
        // 计算TIM2的周期�??
        // APB1 Timer Clock = 180MHz/4 = 45MHz
        uint32_t timer_period = (45000000 / target_sampling_freq) - 1;
        
        // 停止定时器和ADC
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
        HAL_ADC_Stop_DMA(&hadc1);
        
        // 更新TIM2配置
        TIM2->ARR = timer_period;
        TIM2->CCR2 = timer_period / 2;  // 50% 占空�?
        
        // 重启定时器和ADC
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADCBuf, 188);
        
        current_sampling_freq = target_sampling_freq;
        
        // 发�?�采样频率更新信�?
        char msg[32];
        sprintf(msg, "SF:%d Hz\r\n", (int)current_sampling_freq);
        HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 10);
    }
}






// 更新RR间隔缓冲区并计算频率
void update_frequency(uint16_t new_interval) {
    if(rr_buffer.count >= 10) {
        // 移除�?老的间隔
        rr_buffer.total -= rr_buffer.intervals[rr_buffer.head];
    } else {
        rr_buffer.count++;
    }
    
    // 添加新间�?
    rr_buffer.intervals[rr_buffer.head] = new_interval;
    rr_buffer.total += new_interval;
    rr_buffer.head = (rr_buffer.head + 1) % 10;
    
    // 计算频率 (Hz)
    if(rr_buffer.count > 0) {
        float avg_interval = (float)rr_buffer.total / rr_buffer.count;
        current_frequency = SAMPLING_RATE / avg_interval;
        // adjust_sampling_frequency(current_frequency);
    }
}
uint8_t get_prev_buf_idx(uint8_t current_idx) {
    return (current_idx + 2) % 3;  // +2 等价�? -1
}

uint8_t get_next_buf_idx(uint8_t current_idx) {
    return (current_idx + 1) % 3;
}
// 修改后的R波检测函数，考虑ProcessedBuf的三个缓冲区
void detect_r_peaks(uint16_t current_buf_idx) {
    uint16_t* buffer1 = &ProcessedBuf[188 * get_prev_buf_idx(current_buf_idx)];
    uint16_t* buffer2 = &ProcessedBuf[188 * current_buf_idx];
    uint16_t* buffer3 = &ProcessedBuf[188 * get_next_buf_idx(current_buf_idx)];
    uint16_t* current_buffer;
    static uint16_t samples_since_last_r = 0;
    
    for(uint16_t i = 1; i < 563; i++) {  // 避免边界�?�?
        samples_since_last_r++;
        if(i<188){
            current_buffer = buffer1;
        }else if(i<376){
            current_buffer = buffer2;
        }else{
            current_buffer = buffer3;
        }
        // R波检测条�?
        if(current_buffer[i] > THRESHOLD && 
           current_buffer[i] > current_buffer[i-1] && 
           current_buffer[i] > current_buffer[i+1] && 
           samples_since_last_r > MIN_RR_SAMPLES) {
            
            // 更新频率计算
            
            update_frequency(samples_since_last_r);
            
            
            samples_since_last_r = 0;
            
            // 发�?�R波标记和当前心率
            char msg[20];
            int heart_rate = (int)(current_frequency * 60);  // 转换为BPM
            sprintf(msg, "R:%d\r\n", heart_rate);
            HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 10);
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




//ADC Half
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
  //HAL_UART_Transmit(&huart1,"hello",5,5);
	if (ADCState == 0)
	{
		memcpy(DATABuf, ADCBuf+10, 2 * 94);
    memcpy(ADCBuf, ADCBuf+188, 2 * 10);
		ADCState = 1;
    Opt_ADC_Value(ADCBuf+10-10,ProcessedBuf+188*ADCProcessedBufState,5,3,94,5,5);
		//HAL_UART_Transmit(&huart1,"half",4,5);
	}
}
//ADC Full
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	//HAL_UART_Transmit(&huart1,"hell2",5,5);
  //HAL_UART_Transmit(&huart1,"bk",2,5);
	if (ADCState == 1)
	{
		
		memcpy(DATABuf + 94, ADCBuf + 94+10, 2 * 94);
		ADCState = 2;
    Opt_ADC_Value(ADCBuf +10-10+ 94,ProcessedBuf+188*ADCProcessedBufState+ 94,5,3,94,5,5);
    //memcpy(ProcessedBuf+188*ADCProcessedBufState+ 94, ADCBuf + 94, 2 * 94);
		ADCProcessedBufState = (ADCProcessedBufState+1) %3;
    if(ADCProcessedBufState==2){
      pos=0;}
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
