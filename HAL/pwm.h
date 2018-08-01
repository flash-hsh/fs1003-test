
#ifndef __PWM_H
#define __PWM_H
#include "sys.h"



//extern vu16 LowVal_FS1;//FS1 PWMֵ�ɼ�

//extern vu16 LowVal_FS2;//FS1 PWMֵ�ɼ�

//extern vu16 LowVal_FS3;//FS1 PWMֵ�ɼ�

//extern vu16 LowVal_FS4;//FS1 PWMֵ�ɼ�

extern vu16 pwm_val_fs[4];//���1-4��FS��pwmֵ


#define PwmInputPin_FS1   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)//��ȡ����0
#define PwmInputPin_FS2   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)//��ȡ����1
#define PwmInputPin_FS3   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)//��ȡ����2 
#define PwmInputPin_FS4   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)//��ȡ����3(WK_UP) 

 
void PwmInputGpio_Init(void);//IO��ʼ��
void TIM4_Cap_Init(u16 arr, u16 psc);
#endif
