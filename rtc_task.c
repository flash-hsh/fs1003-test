#include "rtc_task.h"

//������
TaskHandle_t RTCTask_Handler;
//������
void   rtc_task(void *pvParameters)
{

	u8 RTC_task=0;
	
	while(1)
	{
		RTC_task++;	//����2ִ�д�����1 ע��task1_num2�ӵ�255��ʱ������㣡��
		//printf("����RTC_task�Ѿ�ִ�У�%d��\r\n",RTC_task);
        vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}






}
