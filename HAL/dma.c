#include "dma.h"
#include "adc.h"
#include "protocol_fs.h"

DMA_InitTypeDef DMA_InitStructure;
u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 	  
u16 DMA2_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 	  
extern bool DisplayUpdataEnable;//ʹ�����ݸ���

volatile u16 BuffAdc1Receave[ChannelNumAdc1] = {1,2,3,4,5};
volatile u16 BuffAdc3Receave[ChannelNumAdc3] = {1,2,3,4,5,6,7,8};

//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����  
//void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
//{ 
void ADCInit_DMA(void)
{	
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    
    //DMA����
    DMA_InitStructure.DMA_Channel = DMA_Channel_0; //ѡ��ͨ����
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR); //��Χ�豸��ַ,ADC_DR_DATA�������ݼĴ���
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(u16 *)BuffAdc1Receave; //DMA�洢����ַ,�Լ����õĻ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //���䷽��,����->�洢��
    DMA_InitStructure.DMA_BufferSize = ChannelNumAdc1; //DMA�����С,���ݴ�����16*8
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����Ƿ�Ϊ����ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�洢���Ƿ�Ϊ����ģʽ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�������ݳ��Ȱ����(16λ)
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //�洢�����ݳ��Ȱ���(16λ)
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMAģʽ,��ͨģʽ��ѭ��ģʽ,����˫����ģʽ,��Ҫ��������
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA���ȼ�,�����ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //��ʹ��FIFO
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; //FIFO��ֵ
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //�洢��ͻ��,���δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //����ͻ��,���δ���
    DMA_Init(DMA2_Stream0, &DMA_InitStructure); //��ʼ��DMA2_Stream0,��ӦΪADC3

     //����DMA�ж�
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TC); //����жϱ�־   
    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE); //��������ж�                                       
    DMA_Cmd(DMA2_Stream0, ENABLE); //ʹ��DMA	

 
 
 



    //DMA����
    DMA_InitStructure.DMA_Channel = DMA_Channel_2; //ѡ��ͨ����
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC3->DR); //��Χ�豸��ַ,ADC_DR_DATA�������ݼĴ���
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(u16 *)BuffAdc3Receave; //DMA�洢����ַ,�Լ����õĻ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //���䷽��,����->�洢��
    DMA_InitStructure.DMA_BufferSize =  ChannelNumAdc3; //DMA�����С,���ݴ�����32*8
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����Ƿ�Ϊ����ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�洢���Ƿ�Ϊ����ģʽ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�������ݳ��Ȱ����(16λ)
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //�洢�����ݳ��Ȱ���(16λ)
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMAģʽ,��ͨģʽ��ѭ��ģʽ,����˫����ģʽ,��Ҫ��������
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA���ȼ�,�����ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //��ʹ��FIFO
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; //FIFO��ֵ
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //�洢��ͻ��,���δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //����ͻ��,���δ���
    DMA_Init(DMA2_Stream1, &DMA_InitStructure); //��ʼ��DMA2_Stream0,��ӦΪADC3




     //����DMA�ж�
    DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TC); //����жϱ�־   
    DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE); //��������ж�                                       
    DMA_Cmd(DMA2_Stream1, ENABLE); //ʹ��DMA	






					
    //DMA�ж�����
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;  //DMA2_Stream0�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;  //��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;        //�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);    //����ָ���Ĳ�����ʼ��NVIC�Ĵ���    

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;	//�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  		//�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  							//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���


} 

void Adc1Change(void)
{
	
	FS[CHANNEL_FS1].V33Volt = ((BuffAdc1Receave[0]*VREF)>>12)*151/100;
	FS[CHANNEL_FS2].V33Volt = ((BuffAdc1Receave[1]*VREF)>>12)*151/100;
	FS[CHANNEL_FS3].V33Volt = ((BuffAdc1Receave[2]*VREF)>>12)*151/100;
	FS[CHANNEL_FS4].V33Volt = ((BuffAdc1Receave[3]*VREF)>>12)*151/100;
	
	//��Դ 5V
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

void ReadVolt(void)//������ѹ����
{
	
	
	ADC_SoftwareStartConv(ADC3);		
	DMA_Cmd(DMA2_Stream1, ENABLE);         			
	ADC_SoftwareStartConv(ADC1);		
	DMA_Cmd(DMA2_Stream0, ENABLE);         		
}


void DMA2_Stream0_IRQHandler(void)
{
	
    if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))  //�ж�DMA��������ж�  
    {
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		Adc1Change();
		DisplayUpdataEnable = true;
	
    }
	
	
}
void DMA2_Stream1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1))  //�ж�DMA��������ж�  
    {
		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);
		Adc3Change();
		DisplayUpdataEnable = true;
	
    }
}














