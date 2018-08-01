#include "dma.h"
#include "adc.h"
#include "protocol_fs.h"

DMA_InitTypeDef DMA_InitStructure;
u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 	  
u16 DMA2_MEM_LEN;//保存DMA每次数据传送的长度 	  
extern bool DisplayUpdataEnable;//使能数据更新

volatile u16 BuffAdc1Receave[ChannelNumAdc1] = {1,2,3,4,5};
volatile u16 BuffAdc3Receave[ChannelNumAdc3] = {1,2,3,4,5,6,7,8};

//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:外设地址
//mar:存储器地址
//ndtr:数据传输量  
//void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
//{ 
void ADCInit_DMA(void)
{	
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    
    //DMA设置
    DMA_InitStructure.DMA_Channel = DMA_Channel_0; //选择通道号
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR); //外围设备地址,ADC_DR_DATA规则数据寄存器
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(u16 *)BuffAdc1Receave; //DMA存储器地址,自己设置的缓存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //传输方向,外设->存储器
    DMA_InitStructure.DMA_BufferSize = ChannelNumAdc1; //DMA缓存大小,数据传输量16*8
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设是否为增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //存储器是否为增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据长度半个字(16位)
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //存储器数据长度半字(16位)
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA模式,普通模式、循环模式,还有双缓冲模式,需要特殊设置
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA优先级,高优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //不使用FIFO
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; //FIFO阈值
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //存储器突发,单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发,单次传输
    DMA_Init(DMA2_Stream0, &DMA_InitStructure); //初始化DMA2_Stream0,对应为ADC3

     //设置DMA中断
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TC); //清除中断标志   
    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE); //传输完成中断                                       
    DMA_Cmd(DMA2_Stream0, ENABLE); //使能DMA	

 
 
 



    //DMA设置
    DMA_InitStructure.DMA_Channel = DMA_Channel_2; //选择通道号
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC3->DR); //外围设备地址,ADC_DR_DATA规则数据寄存器
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(u16 *)BuffAdc3Receave; //DMA存储器地址,自己设置的缓存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //传输方向,外设->存储器
    DMA_InitStructure.DMA_BufferSize =  ChannelNumAdc3; //DMA缓存大小,数据传输量32*8
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设是否为增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //存储器是否为增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据长度半个字(16位)
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //存储器数据长度半字(16位)
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA模式,普通模式、循环模式,还有双缓冲模式,需要特殊设置
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA优先级,高优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //不使用FIFO
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; //FIFO阈值
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //存储器突发,单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发,单次传输
    DMA_Init(DMA2_Stream1, &DMA_InitStructure); //初始化DMA2_Stream0,对应为ADC3




     //设置DMA中断
    DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TC); //清除中断标志   
    DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE); //传输完成中断                                       
    DMA_Cmd(DMA2_Stream1, ENABLE); //使能DMA	






					
    //DMA中断设置
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;  //DMA2_Stream0中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;  //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;        //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化NVIC寄存器    

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;	//中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  		//从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  							//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器


} 

void Adc1Change(void)
{
	
	FS[CHANNEL_FS1].V33Volt = ((BuffAdc1Receave[0]*VREF)>>12)*151/100;
	FS[CHANNEL_FS2].V33Volt = ((BuffAdc1Receave[1]*VREF)>>12)*151/100;
	FS[CHANNEL_FS3].V33Volt = ((BuffAdc1Receave[2]*VREF)>>12)*151/100;
	FS[CHANNEL_FS4].V33Volt = ((BuffAdc1Receave[3]*VREF)>>12)*151/100;
	
	//电源 5V
	FS[CHANNEL_FS1].V50Volt = ((BuffAdc1Receave[4]*VREF)>>12)*175/100;
	FS[CHANNEL_FS2].V50Volt = FS[CHANNEL_FS1].V50Volt;
	FS[CHANNEL_FS3].V50Volt = FS[CHANNEL_FS1].V50Volt;
	FS[CHANNEL_FS4].V50Volt = FS[CHANNEL_FS1].V50Volt;
	
	
}

void Adc3Change(void)
{
	
	FS[CHANNEL_FS1].Tp2Volt = (BuffAdc3Receave[0]*VREF)>>12;
	FS[CHANNEL_FS2].Tp2Volt = (BuffAdc3Receave[1]*VREF)>>12;
	FS[CHANNEL_FS3].Tp2Volt = (BuffAdc3Receave[2]*VREF)>>12;
	FS[CHANNEL_FS4].Tp2Volt = (BuffAdc3Receave[3]*VREF)>>12;
	
	FS[CHANNEL_FS1].Tp1Volt = (BuffAdc3Receave[4]*VREF)>>12;
	FS[CHANNEL_FS2].Tp1Volt = (BuffAdc3Receave[5]*VREF)>>12;
	FS[CHANNEL_FS3].Tp1Volt = (BuffAdc3Receave[6]*VREF)>>12;
	FS[CHANNEL_FS4].Tp1Volt = (BuffAdc3Receave[7]*VREF)>>12;
	
	
	
	
}

void ReadVolt(void)//启动电压测试
{
	
	
	ADC_SoftwareStartConv(ADC3);		
	DMA_Cmd(DMA2_Stream1, ENABLE);         			
	ADC_SoftwareStartConv(ADC1);		
	DMA_Cmd(DMA2_Stream0, ENABLE);         		
}


void DMA2_Stream0_IRQHandler(void)
{
	
    if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))  //判断DMA传输完成中断  
    {
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		Adc1Change();
		DisplayUpdataEnable = true;
	
    }
	
	
}
void DMA2_Stream1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1))  //判断DMA传输完成中断  
    {
		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
		Adc3Change();
		DisplayUpdataEnable = true;
	
    }
}














