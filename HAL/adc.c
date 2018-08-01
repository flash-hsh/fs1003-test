#include "adc.h"
#include "stm32f4xx_adc.h"

#define ADC_SampleTime ADC_SampleTime_480Cycles	//252us		   
void  Adc_Init(void)
{ 	
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

	ADC_DeInit();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC3, ENABLE); //使能ADC1时钟

	//先初始化ADC1通道5 IO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//PA5 通道5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA5 通道5
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化  
						
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
	
	
	//先初始化ADC1通道5 IO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;//PA5 通道5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化  
						
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//复位结束	 
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_2; //DMA失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
	


	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//非扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = ChannelNumAdc1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化


	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime );		//ADC1,ADC通道,采样顺序,采样时间周期	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,2,ADC_SampleTime );	//ADC1,ADC通道,采样顺序,采样时间周期
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,3,ADC_SampleTime );		//ADC1,ADC通道,采样顺序,采样时间周期	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,4,ADC_SampleTime );	//ADC1,ADC通道,采样顺序,采样时间周期
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,5,ADC_SampleTime );	//ADC1,ADC通道,采样顺序,采样时间周期	

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);  
	ADC_DMACmd(ADC1,ENABLE);// 使能ADC的DMA请求
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADCx


	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = ChannelNumAdc3;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC3, &ADC_InitStructure);//ADC初始化




	ADC_RegularChannelConfig(ADC3,ADC_Channel_8,1,ADC_SampleTime );	//ADC1,ADC通道,采样顺序,采样时间周期	
	ADC_RegularChannelConfig(ADC3,ADC_Channel_9,2,ADC_SampleTime );	//ADC1,ADC通道,采样顺序,采样时间周期
	ADC_RegularChannelConfig(ADC3,ADC_Channel_14,3,ADC_SampleTime );	//ADC1,ADC通道,采样顺序,采样时间周期	
	ADC_RegularChannelConfig(ADC3,ADC_Channel_15,4,ADC_SampleTime );	//ADC1,ADC通道,采样顺序,采样时间周期
	ADC_RegularChannelConfig(ADC3,ADC_Channel_4,5,ADC_SampleTime );	//ADC1,ADC通道,采样顺序,采样时间周期	
	ADC_RegularChannelConfig(ADC3,ADC_Channel_5,6,ADC_SampleTime );	//ADC1,ADC通道,采样顺序,采样时间周期	
	ADC_RegularChannelConfig(ADC3,ADC_Channel_6,7,ADC_SampleTime );	//ADC1,ADC通道,采样顺序,采样时间周期
	ADC_RegularChannelConfig(ADC3,ADC_Channel_7,8,ADC_SampleTime );	//ADC1,ADC通道,采样顺序,采样时间周期	

	
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);  
	ADC_DMACmd(ADC3,ENABLE);// 使能ADC的DMA请求
	
	ADC_Cmd(ADC3, ENABLE);	//使能指定的ADCx
	
}		

////获得ADC值
////ch:通道值
//u16 Get_Adc(ADC_TypeDef* ADCx,u8 ch)   
//{
//	ADC_RegularChannelConfig(ADCx,ch,1,ADC_SampleTime_480Cycles );	//ADC1,ADC通道,采样时间为13.5周期			    
//  
//	ADC_SoftwareStartConv(ADCx);		//使能指定的ADC1的软件转换启动功能	
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

//	return ADC_GetConversionValue(ADCx);	//返回最近一次ADC1规则组的转换结果
//}


//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
//u16 Get_Adc(u8 ch)   
//{
//	  	//设置指定ADC的规则组通道，一个序列，采样时间
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
//  
//	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

//	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
//}
////获取通道ch的转换值，取times次,然后平均 
////ch:通道编号
////times:获取次数
////返回值:通道ch的times次转换结果平均值
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


