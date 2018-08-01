
#ifndef __PWM_H
#define __PWM_H
#include "sys.h"



//extern vu16 LowVal_FS1;//FS1 PWM值采集

//extern vu16 LowVal_FS2;//FS1 PWM值采集

//extern vu16 LowVal_FS3;//FS1 PWM值采集

//extern vu16 LowVal_FS4;//FS1 PWM值采集

extern vu16 pwm_val_fs[4];//存放1-4号FS的pwm值


#define PwmInputPin_FS1   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)//读取按键0
#define PwmInputPin_FS2   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)//读取按键1
#define PwmInputPin_FS3   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)//读取按键2 
#define PwmInputPin_FS4   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)//读取按键3(WK_UP) 

 
void PwmInputGpio_Init(void);//IO初始化
void TIM4_Cap_Init(u16 arr, u16 psc);
#endif
