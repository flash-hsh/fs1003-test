#ifndef __RECHECK_TASK_H
#define	__RECHECK_TASK_H

#include "FreeRTOS.h"
#include "task.h"

//任务优先级
#define RECHECK_TASK_PRIO		5
//任务堆栈大小	
#define RECHECK_STK_SIZE 		128 

//任务句柄
extern TaskHandle_t RecheckTask_Handler;
//任务函数
void   recheck_task(void *pvParameters);



#endif

















