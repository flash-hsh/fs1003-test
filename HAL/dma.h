#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"

extern volatile u16 BuffAdc1Receave[];
extern volatile u16 BuffAdc3Receave[];
extern volatile u16 BuffVolt[];			

//void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr);
void ADCInit_DMA(void);
void ReadVolt(void);//∆Ù∂ØµÁ—π≤‚ ‘
#endif




