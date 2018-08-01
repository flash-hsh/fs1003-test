#include "test_task.h"
#include "voltage_task.h"
#include "rtc_task.h"
#include "recheck_task.h"


#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "config.h"

#include "gpio.h"
#include "key.h"
#include "ILI9486.h"
#include "timer.h"
#include "pwm.h"
#include "adc.h"
#include "dma.h"
#include "rtc.h" 
#include "motor.h"
#include "fatfs.h"
#include "buzzer.h"
#include "malloc.h" 
#include "fsm.h"
#include "protocol_fs.h"



//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	delay_init(168);					//��ʼ����ʱ����
	
	uart_init(115200);     				//��ʼ������
	FS_uart_init(9600);
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ��
	TIM3_Int_Init(199,4199); // SYS ��ʱ��ʱ��84M����Ƶϵ��4200������84M/4200=20Khz�ļ���Ƶ�ʣ�����200��Ϊ10ms     
 	TIM4_Cap_Init(999,83);	//��1Mhz��Ƶ�ʼ���  Tout= ((arr+1)*(psc+1))/Tclk
	
	My_RTC_Init();		 		//��ʼ��RTC
	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//����WAKE UP�ж�,1�����ж�һ��

	GpioConfiguration();
	
	buzzerInit(499,83);  //2KHz
	TIM_SetCompare2(TIM12,250);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	
	LCD_Init();           //��ʼ��LCD FSMC�ӿ�
	KEY_Init();
	Adc_Init();	
	ADCInit_DMA();
	MOTOR_Init();
	//��ʼ��USB����	
  	//USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks);  

	//������¼�����ϵ����,���򴫸������ڸ�λ״̬
	ProgKey1 = 1;
	ProgKey2 = 1;
	ProgKey3 = 1;
	ProgKey4 = 1;	
	delay_ms(200);
	ProgKey1 = 0;
	ProgKey2 = 0;
	ProgKey3 = 0;
	ProgKey4 = 0;
	
	
	uartModePin=1; 
	uartMode=1;
	

		
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}


//��ʼ����������
void start_task(void *pvParameters)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);//����CRCʱ��
   
	taskENTER_CRITICAL();           //�����ٽ���

#if FUNCTION_CONFIGURATION==0	
    //������װ��¼��������
    xTaskCreate((TaskFunction_t )test_task,             
                (const char*    )"test_task",           
                (uint16_t       )TEST_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TEST_TASK_PRIO,        
                (TaskHandle_t*  )&TestTask_Handler);  

    //������ѹʵʱ��ʾ����
    xTaskCreate((TaskFunction_t )voltage_task,             
                (const char*    )"voltage_task",           
                (uint16_t       )VOLTAGE_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )VOLTAGE_TASK_PRIO,        
                (TaskHandle_t*  )&VoltageTask_Handler);  

    //����ʱ��ʵʱ��ʾ����
    xTaskCreate((TaskFunction_t )rtc_task,             
                (const char*    )"rtc_task",           
                (uint16_t       )RTC_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )RTC_TASK_PRIO,        
                (TaskHandle_t*  )&RTCTask_Handler);  
				
#elif FUNCTION_CONFIGURATION==1				
    //������������
    xTaskCreate((TaskFunction_t )recheck_task,             
                (const char*    )"recheck_task",           
                (uint16_t       )RECHECK_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )RECHECK_TASK_PRIO,        
                (TaskHandle_t*  )&RecheckTask_Handler);  
				
#endif			
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}








