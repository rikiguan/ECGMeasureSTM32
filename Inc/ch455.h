#include "stm32f429xx.h"
#define APB1_TIMER_CLK (90000000)

#define SCL_CLR GPIOC->ODR&=~(0X1<<14);
#define SCL_SET GPIOC->ODR|=(0X1<<14);
#define SDA_CLR GPIOC->ODR&=~(0X1<<15);
#define SDA_SET GPIOC->ODR|=(0X1<<15);
void TIM6_Delay(uint16_t arr, uint16_t psc);
void Ch455_WR(uint8_t data1, uint8_t data2);
uint8_t Ch455_RD(void);
/*Users don't have to care about the codes above. */

/*These three functions can be easily used by users.*/
void Ch455_Init(void);
uint8_t Read_Keys(void);
void Set_Nixie_tube(uint8_t num, uint8_t val);

