/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern uint16_t ADCBuf[188+140] ;
extern uint16_t DATABuf[188] ;
extern uint8_t ADCState ;             // 0 Default 1 HalfComplete 2 Complete
extern uint8_t ADCProcessedBufState ; // 0  1  2
extern uint16_t ProcessedBuf[564];
extern int Cursor;
extern int heart_rate;
extern uint16_t max_val ;
extern uint16_t min_val ;
extern float feq ;

extern int tick ;
extern int last_dma_tick ;
extern float point_num_each_tick ;
extern float pos ;

extern int aiClass;
extern float AI_possibility;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
