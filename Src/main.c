/* USER CODE BEGIN Header */

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
#include "qecg_data.h"
#include "qecg.h"
#include "dataProcess.h"
#include "handler.h"
#include "utils.h"
#include "callback.h"
#include "graphic.h"
#include "ai.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int aiClass = -1;
float AI_possibility = 0;
int Cursor = 0;
int heart_rate;
uint16_t ProcessedBuf[564] = {0};
uint16_t ADCBuf[188 + 140] = {0};
uint16_t DATABuf[188*4] = {0};
uint8_t ADCState = 0;             // 0 Default 1 HalfComplete 2 Complete
uint8_t ADCProcessedBufState = 0; // 0  1  2
uint16_t max_val = 0;
uint16_t min_val = UINT16_MAX;
float feq = 0;

int tick = 0;
int last_dma_tick = 0;
float point_num_each_tick = 2;
float pos = 0;

int jumpBuffer = 0;

#define DELAY 1000

void Inithuawei(void)
{
  // 华为云串口操作
  printf("ATI\r\n");
  HAL_Delay(DELAY);
  HAL_Delay(DELAY);

  // IP detection

  printf("AT+MIPCALL?\r\n");
  HAL_Delay(DELAY);

  printf("AT+MIPCALL=1\r\n");
  // printf("获取IP成功\r\n");
  HAL_Delay(DELAY);
  // huawei cloud connection
  printf("AT+HMDIS\r\n");
  HAL_Delay(DELAY);
  HAL_Delay(DELAY);
  printf("AT+HMCON=0,60,\"47f88e2625.st1.iotda-device.cn-north-4.myhuaweicloud.com\",\"1883\",\"67526b6cef99673c8ad2f66e_ecgtest\",\"338634101\",0\r\n");
  HAL_Delay(DELAY);
  HAL_Delay(DELAY);
  HAL_Delay(DELAY);
  printf("连接成功\r\n");
  HAL_Delay(DELAY);
}

void voice(int heartrate)
{
  if (heartrate >= 120)
  {

    printf("AT+GTTS=1,\"\xd0\xc4\xc2\xca\xb9\xfd\xb8\xdf\",1\r\n");
    printf("AT+GTTS=0\r\n");
  }
  else if (heartrate <= 60)
  {

    printf("AT+GTTS=1,\"\xd0\xc4\xc2\xca\xb9\xfd\xb5\xcd\",1\r\n");

    printf("AT+GTTS=0\r\n");
  }
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
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  // HAL_UART_Transmit(&huart1, "hello", 5, 5);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  // HAL_ADC_Start_DMA(&hadc1, (uint32_t *)(ADCBuf + 140), 188);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)(DATABuf), 188*4);
  // Inithuawei();

  LCD_Init();
  LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  if (ai_init() != 0)
  {
    return -1;
  }

  draw_ui();

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  int heartratelength;
  while (1)
  {
    if (ADCState == 2)
    {
      // uint8_t header1[8] = {0x16, 0x90, 0x16, 0x90,0x00,0x10,0x00,0x00};
      // HAL_UART_Transmit(&huart1, (uint8_t *)header1, 8, 1);
      uint8_t header[4] = {0x16, 0x80, 0x16, 0x80};
      HAL_UART_Transmit(&huart1, (uint8_t *)header, 4, 1);
      HAL_UART_Transmit_DMA(&huart1, (uint8_t *)ADCBuf, 564 * 2);
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
      
      if (heart_rate < 100.0f)
        heartratelength = 78;
      else
        heartratelength = 79;
        // printf("AT+HMPUB=1,\"$oc/devices/67526b6cef99673c8ad2f66e_ecgtest/sys/properties/report\",%d,\"{\\\"services\\\":[{\\\"service_id\\\":\\\"HeartRateMonitor\\\",\\\"properties\\\":{\\\"HeartRate\\\":%d}}]}\"\r\n", heartratelength,heart_rate );
        // voice(heart_rate);  
    }

    button_handler();
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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

/* USER CODE BEGIN PV */

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

#ifdef USE_FULL_ASSERT
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
