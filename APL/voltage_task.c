#include "voltage_task.h"


//������
TaskHandle_t VoltageTask_Handler;
//������
void   voltage_task(void *pvParameters)
{

	u8 Voltage_task=0;
	
	while(1)
	{
		Voltage_task++;	//����2ִ�д�����1 ע��task1_num2�ӵ�255��ʱ������㣡��
		//printf("����Voltage_task�Ѿ�ִ�У�%d��\r\n",Voltage_task);
        vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}





}





