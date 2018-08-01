//�������
#include "motor.h"
#include "delay.h"
#include "usart.h"


volatile Position_e cur_position = CRITICAL_POINT;	//��ǰ���λ�ã���ʼ��Ϊ�ٽ�㣬�ϵ��Զ�У׼
volatile Pulse_t	 pulse;	//������


void TIM1_PWM_Init(u16 arr,u16 psc)
{
	
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM1); //GPIOB14����Ϊ��ʱ��1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;           //GPIOB14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PB14
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ����ʱ��1
	
	//��ʼ��TIM14 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (arr+1)/2; //ռ�ձ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//���ͬ�࣬TIM_OCNPolarity_Highʱ�������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);        


	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ�� 
	TIM_ITConfig(TIM1,TIM_IT_CC2,ENABLE);									//ʹ��ƥ���ж�
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM1,DISABLE);     												//��ʼ��ʧ��TIM1           

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure); 


	

}

void EXTIX_Init(void)//�����λ���ڣ��ⲿ�ж�ʵ�� 
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ʹ��PORTFʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //KEY0 KEY1 KEY2��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);//PE2 ���ӵ��ж���2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);//PE3 ���ӵ��ж���3

	
  /* ����EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	
	/* ����EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 ;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����


	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//�ⲿ�ж�2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����






}



void MOTOR_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	//MotorDir_Pin 	MotorEn_Pin
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF12,F13��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
 		
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //PF14û���õ�����������ģʽ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	
	//TIM1_PWM_Init(999,27); 								//168M/28*1000=6000Hz	0.5sת��1Ȧ
	TIM1_PWM_Init(999,19); 								//168M/20*1000=8400Hz	0.5sת��1Ȧ	
	EXTIX_Init();

	//��������ʼ��
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

//�ⲿ�жϷ������
void EXTI0_IRQHandler(void)
{
	//Ӳ�����100nf���ݣ��ɲ���ʱ

    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
		EXTI_ClearITPendingBit(EXTI_Line0); 		
		if(MotorDetLimiUp_Pin==0)					//����λ
		{
            MotorOff();
			pulse.pulse_count = 0;
			cur_position = CRITICAL_POINT;	//Խ���			
		}
    }
}


void EXTI1_IRQHandler(void)
{
	//Ӳ�����100nf���ݣ��ɲ���ʱ
    if (EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
 		
		
		if(MotorDetLimiDown_Pin==0)					//����λ
		{			
            MotorOff();
			pulse.pulse_count = 0;
			cur_position = ZERO_POINT;	//���

		}
		EXTI_ClearITPendingBit(EXTI_Line1); //���LINE0�ϵ��жϱ�־λ 
		
	}


}

void motor_drive(Position_e next_position,int pulseNum)
{

	Rotation_Diretion_e direction ;
	u8 cnt = 100;
	
	if(next_position == ZERO_POINT)
	{

		direction = REV;	//�������У׼ʱ���õ����������λ���أ�cur_position��ΪZERO_POINT

	}

	else if(next_position - cur_position == 1)
	{

		direction = FWD;
		cur_position = next_position;	//λ��ת�Ƶ�ǰλ��

	}

	else if(next_position - cur_position < 0 )
	{

		direction = REV;
		cur_position = next_position;	//λ��ת�Ƶ�ǰλ��

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
	
	while(pulse.pulse_count && cnt) //�ȴ�TIM1�жϽ�������pulse_count����Ϊ0�������λ
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













