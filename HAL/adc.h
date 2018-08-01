#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
#include "delay.h"
#define ChannelNumAdc1 5//启用通道数
#define ChannelNumAdc3 8

void Adc_Init(void);
//u16 Get_Adc(ADC_TypeDef * ADCx,u8 ch);  
void GetVolt(void);

u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  

#endif 
