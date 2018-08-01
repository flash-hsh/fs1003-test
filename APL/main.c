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



//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);					//初始化延时函数
	
	uart_init(115200);     				//初始化串口
	FS_uart_init(9600);
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMCCM);		//初始化CCM内存池
	TIM3_Int_Init(199,4199); // SYS 定时器时钟84M，分频系数4200，所以84M/4200=20Khz的计数频率，计数200次为10ms     
 	TIM4_Cap_Init(999,83);	//以1Mhz的频率计数  Tout= ((arr+1)*(psc+1))/Tclk
	
	My_RTC_Init();		 		//初始化RTC
	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//配置WAKE UP中断,1秒钟中断一次

	GpioConfiguration();
	
	buzzerInit(499,83);  //2KHz
	TIM_SetCompare2(TIM12,250);	//修改比较值，修改占空比
	
	LCD_Init();           //初始化LCD FSMC接口
	KEY_Init();
	Adc_Init();	
	ADCInit_DMA();
	MOTOR_Init();
	//初始化USB主机	
  	//USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks);  

	//消除烧录器初上电干扰,否则传感器处于复位状态
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
	

		
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}


//开始任务任务函数
void start_task(void *pvParameters)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);//开启CRC时钟
   
	taskENTER_CRITICAL();           //进入临界区

#if FUNCTION_CONFIGURATION==0	
    //创建工装烧录测试任务
    xTaskCreate((TaskFunction_t )test_task,             
                (const char*    )"test_task",           
                (uint16_t       )TEST_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TEST_TASK_PRIO,        
                (TaskHandle_t*  )&TestTask_Handler);  

    //创建电压实时显示任务
    xTaskCreate((TaskFunction_t )voltage_task,             
                (const char*    )"voltage_task",           
                (uint16_t       )VOLTAGE_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )VOLTAGE_TASK_PRIO,        
                (TaskHandle_t*  )&VoltageTask_Handler);  

    //创建时钟实时显示任务
    xTaskCreate((TaskFunction_t )rtc_task,             
                (const char*    )"rtc_task",           
                (uint16_t       )RTC_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )RTC_TASK_PRIO,        
                (TaskHandle_t*  )&RTCTask_Handler);  
				
#elif FUNCTION_CONFIGURATION==1				
    //创建复检任务
    xTaskCreate((TaskFunction_t )recheck_task,             
                (const char*    )"recheck_task",           
                (uint16_t       )RECHECK_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )RECHECK_TASK_PRIO,        
                (TaskHandle_t*  )&RecheckTask_Handler);  
				
#endif			
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}








