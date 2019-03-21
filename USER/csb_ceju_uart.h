#ifndef __CSB_CEJU_UART_H
#define __CSB_CEJU_UART_H
#include "stdio.h"	
#include "sys.h" 
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "sys.h"

#include "main.h"
#include "delay.h"

#define USART_REC_LEN  			10  	//�����������ֽ��� 10
	  	

    		
void uart_csb_init(void);
void SendData(u32 data);
#endif


