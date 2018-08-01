#include "pwm.h"
//#include "led.h"
//#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//PWM  ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

vu16 pwm_val_fs[4]={0};//���1-4��FS��pwmֵ

static vu8 savePinState_FS1;  
static vu8 nowPinState_FS1;
static vu8 RisingFallingFlag_FS1;//0��û�б仯 1���½��� 2��������
static vu16 Cnt_FS1;
//vu16 LowVal_FS1;//�͵�ƽʱ��

static vu8 savePinState_FS2;
static vu8 nowPinState_FS2;
static vu8 RisingFallingFlag_FS2;//0��û�б仯 1���½��� 2��������
static vu16 Cnt_FS2;
//vu16 LowVal_FS2;//�͵�ƽʱ��

static vu8 savePinState_FS3;
static vu8 nowPinState_FS3;
static vu8 RisingFallingFlag_FS3;//0��û�б仯 1���½��� 2��������
static vu16 Cnt_FS3;
//vu16 LowVal_FS3;//�͵�ƽʱ��

static vu8 savePinState_FS4;
static vu8 nowPinState_FS4;
static vu8 RisingFallingFlag_FS4;//0��û�б仯 1���½��� 2��������
static vu16 Cnt_FS4;
//vu16 LowVal_FS4;//�͵�ƽʱ��


#if 1
void PwmInputGpio_Init(void)//IO��ʼ��
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOA,GPIOEʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //KEY0 KEY1 KEY2��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4


}




void TIM4_Cap_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    PwmInputGpio_Init();

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM4ʱ��
	
    TIM_TimeBaseStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ��TIM4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
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

	//TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx	��DISABLE
    TIM_Cmd(TIM4, DISABLE);
}
//��ʱ��3�жϷ������
void TIM4_IRQHandler(void)   //TIM4�ж�
{
	
        nowPinState_FS1=PwmInputPin_FS1;
        nowPinState_FS2=PwmInputPin_FS2;
        nowPinState_FS3=PwmInputPin_FS3;
        nowPinState_FS4=PwmInputPin_FS4;

    
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
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
      
      TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx�����жϱ�־ 		

}


#endif
