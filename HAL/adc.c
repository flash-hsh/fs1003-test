#include "adc.h"
#include "stm32f4xx_adc.h"

#define ADC_SampleTime ADC_SampleTime_480Cycles	//252us		   
void  Adc_Init(void)
{ 	
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

	ADC_DeInit();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC3, ENABLE); //ʹ��ADC1ʱ��

	//�ȳ�ʼ��ADC1ͨ��5 IO��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//PA5 ͨ��5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA5 ͨ��5
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��  
						
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
	
	
	//�ȳ�ʼ��ADC1ͨ��5 IO��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;//PA5 ͨ��5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  
						
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//��λ����	 
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_2; //DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
	


	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = ChannelNumAdc1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��


	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime );		//ADC1,ADCͨ��,����˳��,����ʱ������	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,2,ADC_SampleTime );	//ADC1,ADCͨ��,����˳��,����ʱ������
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,3,ADC_SampleTime );		//ADC1,ADCͨ��,����˳��,����ʱ������	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,4,ADC_SampleTime );	//ADC1,ADCͨ��,����˳��,����ʱ������
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,5,ADC_SampleTime );	//ADC1,ADCͨ��,����˳��,����ʱ������	

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);  
	ADC_DMACmd(ADC1,ENABLE);// ʹ��ADC��DMA����
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADCx


	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = ChannelNumAdc3;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC3, &ADC_InitStructure);//ADC��ʼ��




	ADC_RegularChannelConfig(ADC3,ADC_Channel_8,1,ADC_SampleTime );	//ADC1,ADCͨ��,����˳��,����ʱ������	
	ADC_RegularChannelConfig(ADC3,ADC_Channel_9,2,ADC_SampleTime );	//ADC1,ADCͨ��,����˳��,����ʱ������
	ADC_RegularChannelConfig(ADC3,ADC_Channel_14,3,ADC_SampleTime );	//ADC1,ADCͨ��,����˳��,����ʱ������	
	ADC_RegularChannelConfig(ADC3,ADC_Channel_15,4,ADC_SampleTime );	//ADC1,ADCͨ��,����˳��,����ʱ������
	ADC_RegularChannelConfig(ADC3,ADC_Channel_4,5,ADC_SampleTime );	//ADC1,ADCͨ��,����˳��,����ʱ������	
	ADC_RegularChannelConfig(ADC3,ADC_Channel_5,6,ADC_SampleTime );	//ADC1,ADCͨ��,����˳��,����ʱ������	
	ADC_RegularChannelConfig(ADC3,ADC_Channel_6,7,ADC_SampleTime );	//ADC1,ADCͨ��,����˳��,����ʱ������
	ADC_RegularChannelConfig(ADC3,ADC_Channel_7,8,ADC_SampleTime );	//ADC1,ADCͨ��,����˳��,����ʱ������	

	
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);  
	ADC_DMACmd(ADC3,ENABLE);// ʹ��ADC��DMA����
	
	ADC_Cmd(ADC3, ENABLE);	//ʹ��ָ����ADCx
	
}		

////���ADCֵ
////ch:ͨ��ֵ
//u16 Get_Adc(ADC_TypeDef* ADCx,u8 ch)   
//{
//	ADC_RegularChannelConfig(ADCx,ch,1,ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,����ʱ��Ϊ13.5����			    
//  
//	ADC_SoftwareStartConv(ADCx);		//ʹ��ָ����ADC1�����ת����������	
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

//	return ADC_GetConversionValue(ADCx);	//�������һ��ADC1�������ת�����
//}


//���ADCֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
//u16 Get_Adc(u8 ch)   
//{
//	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
//  
//	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

//	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
//}
////��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
////ch:ͨ�����
////times:��ȡ����
////����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
//u16 Get_Adc_Average(u8 ch,u8 times)
//{
//	u32 temp_val=0;
//	u8 t;
//	for(t=0;t<times;t++)
//	{
//		temp_val+=Get_Adc(ch);
//		delay_ms(5);
//	}
//	return temp_val/times;
//} 


