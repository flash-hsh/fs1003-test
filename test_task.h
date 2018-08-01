#ifndef __TEST_TASK_H
#define	__TEST_TASK_H

#include "FreeRTOS.h"
#include "task.h"

//任务优先级
#define TEST_TASK_PRIO		4
//任务堆栈大小	
#define TEST_STK_SIZE 		256  

//任务句柄
extern TaskHandle_t TestTask_Handler;
//任务函数
void test_task(void *pvParameters);



#endif
