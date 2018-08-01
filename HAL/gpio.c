#include "gpio.h"

void GpioConfiguration(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOA,GPIOE时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_15;//PB15 推挽输出,控制背光
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化


	GPIO_ResetBits(GPIOF,GPIO_Pin_15); //气压阀 关
	
	ProgKey1 = 0;
	ProgKey2 = 0;
	ProgKey3 = 0;
	ProgKey4 = 0;
	
	uartModePin=1; 
}
