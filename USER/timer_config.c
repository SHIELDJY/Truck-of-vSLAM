 #include "timer_config.h"

u32 speed_R;
u32 speed_L;

void umotor(int32_t speed_L, int32_t speed_R)
{
	if (speed_L>0)							//左轮正转
	{	
		TIM_SetCompare1(TIM3,(u32)speed_L*0.2);
		TIM_SetCompare2(TIM3,0);
	}
	else if(speed_L<=0)
	{
		speed_L = -speed_L;
		TIM_SetCompare1(TIM3,0);	//左轮反转
		TIM_SetCompare2(TIM3,(u32)speed_L*0.2);
	}

	if (speed_R>0)							//右轮正转
	{	
		TIM_SetCompare3(TIM3,(u32)speed_R*0.2);
		TIM_SetCompare4(TIM3,0);
	}
	else if(speed_R<=0)
	{
		speed_R = -speed_R;
		TIM_SetCompare3(TIM3,0);	//右轮反转
		TIM_SetCompare4(TIM3,(u32)speed_R*0.2);
	}
}

/*定时器3中断服务程序（电机）*/
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIM3的中断待处理位:TIM 中断源 
		}
}
   				
/*定时器4中断服务程序（码盘）*/	 
void TIM4_IRQHandler(void)
{ 
	u32 interval=0; 	//编码器产生一个脉冲内所计个数，计一个数为0.02ms
	u32 displace=0;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//发生更新事件
		{	    				
			interval=50000;	
			TIM_SetCounter(TIM4,0); 
		}
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{
			interval=TIM_GetCapture1(TIM4);
			TIM_SetCounter(TIM4,0); 
			displace++;
			if(displace==1000000000)
			displace=0;
		}			     	    					   
 
   TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
}

/*
*函数：TIM_PWM_Init(arr,psc)
*功能：TIM3 通道1与通道2初始化 PWM输出初始化
*参数：arr-自动重装值 psc-时钟预分频数
*其他：映射：TIM3 Channel1--GPIOA6
						TIM3 Channel2--GPIOA7
						TIM3 Channel3--GPIOB0
						TIM3 Channel4--GPIOB1
*/
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO A外设和AFIO复用功能模块时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO B外设和AFIO复用功能模块时钟
	
		//设置该引脚为复用输出功能,输出TIM3 CH1的PWM脉冲波形	GPIOA.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOA.7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	 //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; 			//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 		//设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	

	//初始化TIM3 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	//初始化TIM3 Channel3 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	//初始化TIM3 Channel4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3	

}

//定时器4通道1输入捕获配置

/*
*函数：TIM4_Cap_Init(u16 arr,u16 psc)
*功能：定时器4通道1输入捕获配置
*参数：arr-自动重装值 psc-时钟预分频数
*其他：映射：TIM4 Channel1->GPIOB6
						TIM4 Channel2->GPIOB7
*/

void TIM4_Cap_Init(u16 arr,u16 psc)
{	

	TIM_ICInitTypeDef  TIM4_ICInitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		//使能TIM4时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  	//使能GPIOB时钟
	
	/*初始化GPIOB_Pin6端口（TIM4_Channerl_1）*/
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  		//PB6 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	//PB6 输入  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);									//PB6 上拉
	
	/*初始化GPIOB_Pin7端口（TIM4_Channerl_2）*/
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;  		//PB7 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	//PB7 输入  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);									//PB7 上拉
	
	/*初始化定时器4 TIM4*/ 
	TIM_TimeBaseStructure.TIM_Period = arr; 											//设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 										//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 			//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 							//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	/*初始化TIM4_Channerl_1输入捕获参数*/
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1; 									//CC1S=01 	选择输入端 IC1映射到TI1上
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising ;			//上升沿捕获
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	//映射到TI1上
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;					 	//配置输入分频,不分频 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;													//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
	/*初始化TIM4_Channerl_2输入捕获参数*/
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2; 									//CC1S=01 	选择输入端 IC2映射到TI1上
	TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising ;			//上升沿捕获
	TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	//映射到TI1上
	TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;					 	//配置输入分频,不分频 
	TIM4_ICInitStructure.TIM_ICFilter = 0x00;													//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM4, &TIM4_ICInitStructure);
	
	/*中断分组初始化*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  						//TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  	//先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 					//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  														//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM4,ENABLE ); 	//使能定时器4

}

