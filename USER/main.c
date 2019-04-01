#include "stm32f10x.h"
#include "main.h"

/*全局变量声明*/
u32 speed_M;
u32 direction;

int main(void)
 {	
	 /*局部变量声明*/
	 double degree;
	 
	 /*参数初始值*/
	 degree = 90;
	 direction = 128;
	 
	 /*延时初始化*/
	 delay_init();	 
	 
	 /*UART初始化*/
	 uart_csb_init();	//Tx->GPIOA9
										//Rx->GPIOA10
	 
	 /*电机PWM初始化*/
	 TIM3_PWM_Init(199,72-1);	//left  motor->GPIOA6 & GPIOA7
														//right motor->GPIOB0 & GPIOB1
	 
	 /*码盘PWM捕捉初始化*/
	 TIM4_Cap_Init(199,72-1);	//left  -> GPIOB6
														//right -> GPIOB7
	 
	 /*定时器中断初始化*/
	 TIM1_Int_Init(100-1,3840-1);//100Khz的计数频率，计数到10为0.1ms  

 	while(1)
	{
		degree = direction*0.708;
		speed_L = speed_M-20*cos(degree);
		speed_R = speed_M+20*cos(degree);
	}

}
 
