#ifndef __DIANJI_TIM3_PWM_H
#define __DIANJI_TIM3_PWM_H
#include "sys.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

 
void umotor(int32_t speed_L, int32_t speed_R);
void TIM3_PWM_Init(u16 arr,u16 psc);


#endif 
