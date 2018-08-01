#include "recheck_task.h"


#include "sys.h"
#include "gpio.h"
#include "delay.h"
#include "key.h"
#include "timer.h"


#include "ILI9486.h"
#include "gui_lcd.h"
#include "usart.h"

#include "malloc.h"  


#include "fsm.h"
#include "protocol_fs.h"
#include "display.h"
#include "message.h"
#include "serial.h"
#include "string.h"


//������
TaskHandle_t RecheckTask_Handler;
vu16 TimerCnt = 0;
int event_id = ERROR_EVENT; //�¼�ת�ơ���>״̬ת��


void key_process(void);
u8 Scan_Function(void);
void scanID_readID_cmp(void);


//������
void   recheck_task(void *pvParameters)
{

	//u8 i,ChannelNum;
	id_DisDrawUI();
	
	uartMode=2;
	uartModePin=2;

	
	while(1)		
	{


		//GUI_DispStringAt("Hello World!",0,0);
		//printf("������ά��ɨ���¼���ȥִ��Scan_QR_Codes_Function\r\n");
		
#if 1		
		if(TimerFlag == 1)//10ms
		{ 	
			TimerFlag = 0;
			TimerCnt++;
			key_process();
			//vTaskDelay(10);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	

		}
		if(uartMode == 0 )
		{
			uartMode=0;
			uartModePin=0;

			FilterIdCheck();//��¼���ٴμ��
			delay_ms(20);
			FilterIdCheck();//��¼���ٴμ��
			delay_ms(20);
			FilterIdCheck();//��¼���ٴμ��
			delay_ms(20);
			vTaskDelay(200);
			scanID_readID_cmp();
			uartMode=2;
			uartModePin=2;
			Filter_DataInit();

		
		}

		if(uartMode == 1 )
		{
			Scan_Function();
			vTaskDelay(200);
			uartMode=0;
			uartModePin=0;

		
		}
		
		
#endif		
	}





}



void key_process(void)
{			
	TASK_KeyScan();	
	
    
	  if(isKeyClicked & KEY_SCANNING)//ɨ�밴ť������
		{
			isKeyClicked &=  (~KEY_SCANNING);	   	   	    
			//event = KEY_SCAN_TRIGGER;//������ά��ɨ���¼���ȥִ��Scan_QR_Codes_Function
			printf("������ά��ɨ���¼���ȥִ��Scan_QR_Codes_Function");
			uartMode=1;
			uartModePin=1;

		}	

}



u8 Scan_Function(void)
{
	// do something
	//printf("Scan_QR_Codes_Function!\r\n");
		u8 ret;
		int cnt = 10;
#if 1

		SendCMDtoScannDevice(); //����ɨ��ָ��
		do
		{
			delay_ms(500);
			ret =	ReceiveID();
			cnt--;
			
		}
		while (ret&&cnt);

	if (ret == 1) //ID����ʧ��
	    return 1;
	
	return 0;
#endif

}

void scanID_readID_cmp(void)
{
	u8 ChannelNum;

	
	for(ChannelNum = 0;ChannelNum < 4;ChannelNum++)
	{	
		
		if(array_cmp(FS[ChannelNum].IDWriteBuff,FS[ChannelNum].RecieveIdBuff,6) == 0)
		  FS[ChannelNum].JudgeFlag = true;						
		else
		  FS[ChannelNum].JudgeFlag = false;
						
	}

			
	
	id_Display();



	



}













