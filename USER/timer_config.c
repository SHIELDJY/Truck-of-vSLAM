 #include "timer_config.h"

u32 speed_R;
u32 speed_L;

void umotor(int32_t speed_L, int32_t speed_R)
{
	if (speed_L>0)							//������ת
	{	
		TIM_SetCompare1(TIM3,(u32)speed_L*0.2);
		TIM_SetCompare2(TIM3,0);
	}
	else if(speed_L<=0)
	{
		speed_L = -speed_L;
		TIM_SetCompare1(TIM3,0);	//���ַ�ת
		TIM_SetCompare2(TIM3,(u32)speed_L*0.2);
	}

	if (speed_R>0)							//������ת
	{	
		TIM_SetCompare3(TIM3,(u32)speed_R*0.2);
		TIM_SetCompare4(TIM3,0);
	}
	else if(speed_R<=0)
	{
		speed_R = -speed_R;
		TIM_SetCompare3(TIM3,0);	//���ַ�ת
		TIM_SetCompare4(TIM3,(u32)speed_R*0.2);
	}
}

/*��ʱ��3�жϷ�����򣨵����*/
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIM3���жϴ�����λ:TIM �ж�Դ 
		}
}
   				
/*��ʱ��4�жϷ���������̣�*/	 
void TIM4_IRQHandler(void)
{ 
	u32 interval=0; 	//����������һ�����������Ƹ�������һ����Ϊ0.02ms
	u32 displace=0;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//���������¼�
		{	    				
			interval=50000;	
			TIM_SetCounter(TIM4,0); 
		}
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)//����1���������¼�
		{
			interval=TIM_GetCapture1(TIM4);
			TIM_SetCounter(TIM4,0); 
			displace++;
			if(displace==1000000000)
			displace=0;
		}			     	    					   
 
   TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
}

/*
*������TIM_PWM_Init(arr,psc)
*���ܣ�TIM3 ͨ��1��ͨ��2��ʼ�� PWM�����ʼ��
*������arr-�Զ���װֵ psc-ʱ��Ԥ��Ƶ��
*������ӳ�䣺TIM3 Channel1--GPIOA6
						TIM3 Channel2--GPIOA7
						TIM3 Channel3--GPIOB0
						TIM3 Channel4--GPIOB1
*/
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO A�����AFIO���ù���ģ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO B�����AFIO���ù���ģ��ʱ��
	
		//���ø�����Ϊ�����������,���TIM3 CH1��PWM���岨��	GPIOA.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOA.7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
	 //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; 			//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 		//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	

	//��ʼ��TIM3 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

	//��ʼ��TIM3 Channel3 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1

	//��ʼ��TIM3 Channel4 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3	

}

//��ʱ��4ͨ��1���벶������

/*
*������TIM4_Cap_Init(u16 arr,u16 psc)
*���ܣ���ʱ��4ͨ��1���벶������
*������arr-�Զ���װֵ psc-ʱ��Ԥ��Ƶ��
*������ӳ�䣺TIM4 Channel1->GPIOB6
						TIM4 Channel2->GPIOB7
*/

void TIM4_Cap_Init(u16 arr,u16 psc)
{	

	TIM_ICInitTypeDef  TIM4_ICInitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		//ʹ��TIM4ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  	//ʹ��GPIOBʱ��
	
	/*��ʼ��GPIOB_Pin6�˿ڣ�TIM4_Channerl_1��*/
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  		//PB6 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	//PB6 ����  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);									//PB6 ����
	
	/*��ʼ��GPIOB_Pin7�˿ڣ�TIM4_Channerl_2��*/
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;  		//PB7 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	//PB7 ����  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);									//PB7 ����
	
	/*��ʼ����ʱ��4 TIM4*/ 
	TIM_TimeBaseStructure.TIM_Period = arr; 											//�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 										//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 			//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 							//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	/*��ʼ��TIM4_Channerl_1���벶�����*/
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1; 									//CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising ;			//�����ز���
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	//ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;					 	//���������Ƶ,����Ƶ 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;													//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
	/*��ʼ��TIM4_Channerl_2���벶�����*/
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2; 									//CC1S=01 	ѡ������� IC2ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising ;			//�����ز���
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	//ӳ�䵽TI1��
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;					 	//���������Ƶ,����Ƶ 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;													//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
	
	/*�жϷ����ʼ��*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  						//TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  	//��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 					//�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  														//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��4

}

