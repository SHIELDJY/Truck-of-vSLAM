#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "csb_ceju_uart.h"
#include "timer_config.h"
#include "csb_ceju_uart.h" 

void TIM1_Int_Init(u16 arr,u16 psc);
 
#endif
