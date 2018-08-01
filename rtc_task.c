#include "rtc_task.h"

//任务句柄
TaskHandle_t RTCTask_Handler;
//任务函数
void   rtc_task(void *pvParameters)
{

	u8 RTC_task=0;
	
	while(1)
	{
		RTC_task++;	//任务2执行次数加1 注意task1_num2加到255的时候会清零！！
		//printf("任务RTC_task已经执行：%d次\r\n",RTC_task);
        vTaskDelay(1000);                           //延时1s，也就是1000个时钟节拍	
	}






}
