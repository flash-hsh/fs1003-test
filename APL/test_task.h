#ifndef __TEST_TASK_H
#define	__TEST_TASK_H

#include "FreeRTOS.h"
#include "task.h"

//�������ȼ�
#define TEST_TASK_PRIO		4
//�����ջ��С	
#define TEST_STK_SIZE 		256  

//������
extern TaskHandle_t TestTask_Handler;
//������
void test_task(void *pvParameters);



#endif
