#ifndef __RTC_TASK_H
#define	__RTC_TASK_H

#include "FreeRTOS.h"
#include "task.h"

//�������ȼ�
#define RTC_TASK_PRIO		2
//�����ջ��С	
#define RTC_STK_SIZE 		128  

//������
extern TaskHandle_t RTCTask_Handler;
//������
void   rtc_task(void *pvParameters);



#endif
