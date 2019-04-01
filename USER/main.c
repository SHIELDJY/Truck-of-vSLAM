#include "stm32f10x.h"
#include "main.h"

/*ȫ�ֱ�������*/
u32 speed_M;
u32 direction;

int main(void)
 {	
	 /*�ֲ���������*/
	 double degree;
	 
	 /*������ʼֵ*/
	 degree = 90;
	 direction = 128;
	 
	 /*��ʱ��ʼ��*/
	 delay_init();	 
	 
	 /*UART��ʼ��*/
	 uart_csb_init();	//Tx->GPIOA9
										//Rx->GPIOA10
	 
	 /*���PWM��ʼ��*/
	 TIM3_PWM_Init(199,72-1);	//left  motor->GPIOA6 & GPIOA7
														//right motor->GPIOB0 & GPIOB1
	 
	 /*����PWM��׽��ʼ��*/
	 TIM4_Cap_Init(199,72-1);	//left  -> GPIOB6
														//right -> GPIOB7
	 
	 /*��ʱ���жϳ�ʼ��*/
	 TIM1_Int_Init(100-1,3840-1);//100Khz�ļ���Ƶ�ʣ�������10Ϊ0.1ms  

 	while(1)
	{
		degree = direction*0.708;
		speed_L = speed_M-20*cos(degree);
		speed_R = speed_M+20*cos(degree);
	}

}
 
