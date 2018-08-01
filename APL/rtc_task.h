#ifndef __RTC_TASK_H
#define	__RTC_TASK_H

#include "FreeRTOS.h"
#include "task.h"

//任务优先级
#define RTC_TASK_PRIO		2
//任务堆栈大小	
#define RTC_STK_SIZE 		128  

//任务句柄
extern TaskHandle_t RTCTask_Handler;
//任务函数
void   rtc_task(void *pvParameters);



#endif
