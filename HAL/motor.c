//步进电机
#include "motor.h"
#include "delay.h"
#include "usart.h"


volatile Position_e cur_position = CRITICAL_POINT;	//当前电机位置，初始化为临界点，上电自动校准
volatile Pulse_t	 pulse;	//脉冲数


void TIM1_PWM_Init(u16 arr,u16 psc)
{
	
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM1); //GPIOB14复用为定时器1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;           //GPIOB14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PB14
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (arr+1)/2; //占空比
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//输出同相，TIM_OCNPolarity_High时输出反相
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);        


	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
	TIM_ITConfig(TIM1,TIM_IT_CC2,ENABLE);									//使能匹配中断
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM1,DISABLE);     												//初始化失能TIM1           

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure); 


	

}

void EXTIX_Init(void)//马达限位检测口，外部中断实现 
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能PORTF时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //KEY0 KEY1 KEY2对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE2,3,4

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);//PE2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);//PE3 连接到中断线3

	
  /* 配置EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	
	/* 配置EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 ;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置


	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//外部中断2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置






}



void MOTOR_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	//MotorDir_Pin 	MotorEn_Pin
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

  //GPIOF12,F13初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
 		
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //PF14没有用到，设置输入模式
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	
	//TIM1_PWM_Init(999,27); 								//168M/28*1000=6000Hz	0.5s转动1圈
	TIM1_PWM_Init(999,19); 								//168M/20*1000=8400Hz	0.5s转动1圈	
	EXTIX_Init();

	//脉冲数初始化
	pulse.pulse_count               = 0;
	pulse.zero2_cleanFilter         = BanquanPulse*STANDCARD1;
	pulse.cleanFilter2_dirtyFilter  = BanquanPulse*STANDCARD2;
	pulse.dirtyFilter2_halfFilter   = BanquanPulse*STANDCARD3;
	pulse.halfFilter2_cleanFilter   = pulse.cleanFilter2_dirtyFilter + pulse.dirtyFilter2_halfFilter;
	pulse.back2_zeroPoint			= pulse.halfFilter2_cleanFilter + pulse.zero2_cleanFilter;
		
}




void TIM1_CC_IRQHandler(void)
{
			
	if(TIM_GetITStatus(TIM1,TIM_IT_CC2)!=RESET)
	{				
		pulse.pulse_count--;
                   
		if(pulse.pulse_count <= 0)
		{
			MotorOff();
			
		}		
		TIM_ClearITPendingBit(TIM1,TIM_IT_CC2);


	}
}

//外部中断服务程序
void EXTI0_IRQHandler(void)
{
	//硬件需加100nf电容，可不延时

    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
		EXTI_ClearITPendingBit(EXTI_Line0); 		
		if(MotorDetLimiUp_Pin==0)					//上限位
		{
            MotorOff();
			pulse.pulse_count = 0;
			cur_position = CRITICAL_POINT;	//越界点			
		}
    }
}


void EXTI1_IRQHandler(void)
{
	//硬件需加100nf电容，可不延时
    if (EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
 		
		
		if(MotorDetLimiDown_Pin==0)					//下限位
		{			
            MotorOff();
			pulse.pulse_count = 0;
			cur_position = ZERO_POINT;	//零点

		}
		EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE0上的中断标志位 
		
	}


}

void motor_drive(Position_e next_position,int pulseNum)
{

	Rotation_Diretion_e direction ;
	u8 cnt = 100;
	
	if(next_position == ZERO_POINT)
	{

		direction = REV;	//进行零点校准时，让电机碰到下限位开关，cur_position变为ZERO_POINT

	}

	else if(next_position - cur_position == 1)
	{

		direction = FWD;
		cur_position = next_position;	//位置转移当前位置

	}

	else if(next_position - cur_position < 0 )
	{

		direction = REV;
		cur_position = next_position;	//位置转移当前位置

	}

	else
		return;
	
	pulse.pulse_count = pulseNum;
	
	switch (direction)
	{		
		case FWD:
				MotorFwd();
				MotorOn();
		
		break;

		case REV:	
				MotorRev();
				MotorOn();
		
		break;

		default:break;
		
	}
	
	while(pulse.pulse_count && cnt) //等待TIM1中断将脉冲数pulse_count减到为0，电机到位
		{
			delay_ms(100);
			cnt--;	
		}
}

void motor_calibration(void)
{


	if(cur_position == CRITICAL_POINT)
		motor_drive(ZERO_POINT,pulse.back2_zeroPoint);
	else if(cur_position == ZERO_POINT)		
		motor_drive(CLEAN_FILTER_POSITION,pulse.zero2_cleanFilter);
	else
		return;
}













