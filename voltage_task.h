#ifndef __VOLTAGE_TASK_H
#define	__VOLTAGE_TASK_H

#include "FreeRTOS.h"
#include "task.h"

//�������ȼ�
#define VOLTAGE_TASK_PRIO		3
//�����ջ��С	
#define VOLTAGE_STK_SIZE 		128  

//������
extern TaskHandle_t VoltageTask_Handler;
//������
void   voltage_task(void *pvParameters);



#endif
