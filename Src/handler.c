#include "handler.h"
#define Key_High HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_7) == 1 
#define Key_Low HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_7) == 0
#define ModeKey_High HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6) == 1 
#define ModeKey_Low HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6) == 0

// 读取按键
void button_handler(void)
{

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
    // int key_num = Read_Keys();
    // if (key_num)
    // {
    //   if (key_num == 13)
    //   {
    //     // add code here
    //     HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
    //   }
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
    //}
}
