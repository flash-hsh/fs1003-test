#include "voltage_task.h"


//任务句柄
TaskHandle_t VoltageTask_Handler;
//任务函数
void   voltage_task(void *pvParameters)
{

	u8 Voltage_task=0;
	
	while(1)
	{
		Voltage_task++;	//任务2执行次数加1 注意task1_num2加到255的时候会清零！！
		//printf("任务Voltage_task已经执行：%d次\r\n",Voltage_task);
        vTaskDelay(1000);                           //延时1s，也就是1000个时钟节拍	
	}





}





