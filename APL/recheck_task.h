#ifndef __RECHECK_TASK_H
#define	__RECHECK_TASK_H

#include "FreeRTOS.h"
#include "task.h"

//�������ȼ�
#define RECHECK_TASK_PRIO		5
//�����ջ��С	
#define RECHECK_STK_SIZE 		128 

//������
extern TaskHandle_t RecheckTask_Handler;
//������
void   recheck_task(void *pvParameters);



#endif

















