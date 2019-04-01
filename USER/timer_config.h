#ifndef __TIMER_CONFIG_H
#define __TIMER_CONFIG_H
#include "sys.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

extern u32 interval;
 
void umotor(int32_t speed_L, int32_t speed_R);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_Cap_Init(u16 arr,u16 psc);


#endif 
