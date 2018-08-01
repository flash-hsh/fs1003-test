#include "pwm.h"
//#include "led.h"
//#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//PWM  驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

vu16 pwm_val_fs[4]={0};//存放1-4号FS的pwm值

static vu8 savePinState_FS1;  
static vu8 nowPinState_FS1;
static vu8 RisingFallingFlag_FS1;//0：没有变化 1：下降沿 2：上升沿
static vu16 Cnt_FS1;
//vu16 LowVal_FS1;//低电平时间

static vu8 savePinState_FS2;
static vu8 nowPinState_FS2;
static vu8 RisingFallingFlag_FS2;//0：没有变化 1：下降沿 2：上升沿
static vu16 Cnt_FS2;
//vu16 LowVal_FS2;//低电平时间

static vu8 savePinState_FS3;
static vu8 nowPinState_FS3;
static vu8 RisingFallingFlag_FS3;//0：没有变化 1：下降沿 2：上升沿
static vu16 Cnt_FS3;
//vu16 LowVal_FS3;//低电平时间

static vu8 savePinState_FS4;
static vu8 nowPinState_FS4;
static vu8 RisingFallingFlag_FS4;//0：没有变化 1：下降沿 2：上升沿
static vu16 Cnt_FS4;
//vu16 LowVal_FS4;//低电平时间


#if 1
void PwmInputGpio_Init(void)//IO初始化
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOA,GPIOE时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //KEY0 KEY1 KEY2对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOE2,3,4


}




void TIM4_Cap_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    PwmInputGpio_Init();

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
	
    TIM_TimeBaseStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化TIM4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM4,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);


    savePinState_FS1=0;
    savePinState_FS2=0;
    savePinState_FS3=0;
    savePinState_FS4=0;
    
    nowPinState_FS1=0;
    nowPinState_FS2=0;
    nowPinState_FS3=0;
    nowPinState_FS4=0;
    
    RisingFallingFlag_FS1=0;
    RisingFallingFlag_FS2=0;
    RisingFallingFlag_FS3=0;
    RisingFallingFlag_FS4=0;
    
    Cnt_FS1=0;
    Cnt_FS2=0;
    Cnt_FS3=0;
    Cnt_FS4=0;

//    LowVal_FS1=0;
//    LowVal_FS2=0;
//    LowVal_FS3=0;
//    LowVal_FS4=0;

	//TIM_Cmd(TIM4, ENABLE);  //使能TIMx	，DISABLE
    TIM_Cmd(TIM4, DISABLE);
}
//定时器3中断服务程序
void TIM4_IRQHandler(void)   //TIM4中断
{
	
        nowPinState_FS1=PwmInputPin_FS1;
        nowPinState_FS2=PwmInputPin_FS2;
        nowPinState_FS3=PwmInputPin_FS3;
        nowPinState_FS4=PwmInputPin_FS4;

    
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
		{
		
#if 1       
        if((savePinState_FS1==1) && (nowPinState_FS1==0))
        {        
            RisingFallingFlag_FS1=1;
            Cnt_FS1=0;
        
        }
        else if((savePinState_FS1==0) && (nowPinState_FS1==1))
        {
            RisingFallingFlag_FS1=2;      
        }
//        else
//        {
//        
//            RisingFallingFlag_FS1=0;      
//        
//        
//        }
        
        //savePinState_FS1=nowPinState_FS1;
        
        if(PwmInputPin_FS1==0)
        {
            Cnt_FS1++;
        
        }
        else if(RisingFallingFlag_FS1==2)
        {
        
            //LowVal_FS1=Cnt_FS1;
            pwm_val_fs[0]=Cnt_FS1;
        
        }
        
#endif       
        
        
#if 1       
        if((savePinState_FS2==1) && (nowPinState_FS2==0))
        {        
            RisingFallingFlag_FS2=1;
            Cnt_FS2=0;
        
        }
        else if((savePinState_FS2==0) && (nowPinState_FS2==1))
        {
            RisingFallingFlag_FS2=2;      
        }
        
        //savePinState_FS2=nowPinState_FS2;
        
        if(PwmInputPin_FS2==0)
        {
            Cnt_FS2++;
        
        }
        else if(RisingFallingFlag_FS2==2)
        {
        
            //LowVal_FS2=Cnt_FS2;
			pwm_val_fs[1]=Cnt_FS2;
			
        
        }
        
        
#endif           
  

#if 1       
        if((savePinState_FS3==1) && (nowPinState_FS3==0))
        {        
            RisingFallingFlag_FS3=1;
            Cnt_FS3=0;
        
        }
        else if((savePinState_FS3==0) && (nowPinState_FS3==1))
        {
            RisingFallingFlag_FS3=2;      
        }
//        else
//        {
//        
//            RisingFallingFlag_FS3=0;      
//        
//        
//        }
        
        
       // savePinState_FS3=nowPinState_FS3;
        
        if(PwmInputPin_FS3==0)
        {
            Cnt_FS3++;
        
        }
        else if(RisingFallingFlag_FS3==2)
        {
        
            //LowVal_FS3=Cnt_FS3;
			pwm_val_fs[2]=Cnt_FS3;
			
        
        }
        
        
#endif           
 
#if 1       
        if((savePinState_FS4==1) && (nowPinState_FS4==0))
        {        
            RisingFallingFlag_FS4=1;
            Cnt_FS4=0;
        
        }
        else if((savePinState_FS4==0) && (nowPinState_FS4==1))
        {
            RisingFallingFlag_FS4=2;      
        }
//        else
//        {
//        
//            RisingFallingFlag_FS4=0;      
//        
//        
//        }
        
        //savePinState_FS4=nowPinState_FS4;
        
        if(PwmInputPin_FS4==0)
        {
            Cnt_FS4++;
        
        }
        else if(RisingFallingFlag_FS4==2)
        {
        
            //LowVal_FS4=Cnt_FS4;
			pwm_val_fs[3]=Cnt_FS4;
            
        
        }
        
        
#endif           
  
        

        }
      
      
      
      
      
      savePinState_FS1=nowPinState_FS1;
      savePinState_FS2=nowPinState_FS2;
      savePinState_FS3=nowPinState_FS3;
      savePinState_FS4=nowPinState_FS4;
      
      TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志 		

}


#endif
