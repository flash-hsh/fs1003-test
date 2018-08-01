#ifndef __VOLTAGE_TASK_H
#define	__VOLTAGE_TASK_H

#include "FreeRTOS.h"
#include "task.h"

//任务优先级
#define VOLTAGE_TASK_PRIO		3
//任务堆栈大小	
#define VOLTAGE_STK_SIZE 		128  

//任务句柄
extern TaskHandle_t VoltageTask_Handler;
//任务函数
void   voltage_task(void *pvParameters);



#endif
