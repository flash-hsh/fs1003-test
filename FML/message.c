//ҳü��Ϣ���� ʱ�䣬���Σ������
#include "message.h"
#include "rtc.h" 
#include "gui_lcd.h"
#include "ILI9486.h"
#include <stdio.h>
#include "key.h"
#include "fatfs.h"

IDTypDef ID;
MessagePlaceTypDef MessagePlace;
u16 MessageOverTimeCnt = 0;
u8 MessageUpDownCnt = 0;
u16 Flash_Buffer[14];			//Ҫд�뵽STM32 FLASH������ 0:Batch,1:ProcedureNum
u8 MessageBuf[100];				//ʱ�����Ϣ
bool MessageGlintFlag = true;	//��˸�����־	true����
bool MessageSetFlag = false;	//��Ϣ���ñ�־	true������״̬
//eg:2017��11��16��  17:26:00      ��256����  �����:0xFF  ��47��

void getTime(void)
{

	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
		
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	calendar.hour = RTC_TimeStruct.RTC_Hours;
	calendar.min = RTC_TimeStruct.RTC_Minutes;
	calendar.sec = RTC_TimeStruct.RTC_Seconds;
		
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
	calendar.year= RTC_DateStruct.RTC_Year+1999;	
	calendar.month=RTC_DateStruct.RTC_Month;
	calendar.date=RTC_DateStruct.RTC_Date;
	calendar.weeks=RTC_DateStruct.RTC_WeekDay;
	


}



void MessageToBuffDis(void)//��Ϣת�浽���鲢��ʾ
{		
	getTime();	
	if(MessageSetFlag == true) return;
	
	sprintf((char*)MessageBuf,"%4d��%02d��%02d��  %02d:%02d:%02d      ��%03d����  �����:%#04X  ��%02d�� ",\
			calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec,\
			ID.Batch,ID.ProcedureNum,calendar.weeks);	

	Show_Str(0, 0,POINT_COLOR,BACK_COLOR,MessageBuf,16,0);
}

void MessageSetDis(void)//����ģʽ��Ϣ��ʾ
{
	
	getTime();	
	
	if(MessageSetFlag == false) return;
	//��˸
	if(MessageGlintFlag)
	{
		MessageGlintFlag = false;
		sprintf((char*)MessageBuf,"%04d��%02d��%02d��  %02d:%02d:%02d      ��%03d����  �����:%#04X  ��%2d�� ",\
				calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec,\
				ID.Batch,ID.ProcedureNum,calendar.weeks);	
	}
	else
	{
		MessageGlintFlag = true;
		switch(MessagePlace)
		{
			case MessagePlace_year:
				sprintf((char*)MessageBuf,"    ��%02d��%02d��  %02d:%02d:%02d      ��%03d����  �����:%#04X  ��%2d�� ",\
						calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec,\
						ID.Batch,ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_month:
				sprintf((char*)MessageBuf,"%04d��  ��%02d��  %02d:%02d:%02d      ��%03d����  �����:%#04X  ��%2d�� ",\
						calendar.year,calendar.date,calendar.hour,calendar.min,calendar.sec,\
						ID.Batch,ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_date:
				sprintf((char*)MessageBuf,"%04d��%02d��  ��  %02d:%02d:%02d      ��%03d����  �����:%#04X  ��%2d�� ",\
						calendar.year,calendar.month,calendar.hour,calendar.min,calendar.sec,\
						ID.Batch,ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_hour:
				sprintf((char*)MessageBuf,"%04d��%02d��%02d��    :%02d:%02d      ��%03d����  �����:%#04X  ��%2d�� ",\
						calendar.year,calendar.month,calendar.date,calendar.min,calendar.sec,\
						ID.Batch,ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_min:
				sprintf((char*)MessageBuf,"%04d��%02d��%02d��  %02d:  :%02d      ��%03d����  �����:%#04X  ��%2d�� ",\
						calendar.year,calendar.month,calendar.date,calendar.hour,calendar.sec,\
						ID.Batch,ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_sec:
				sprintf((char*)MessageBuf,"%04d��%02d��%02d��  %02d:%02d:        ��%03d����  �����:%#04X  ��%2d�� ",\
						calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,\
						ID.Batch,ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_batch:
				sprintf((char*)MessageBuf,"%04d��%02d��%02d��  %02d:%02d:%02d      ��   ����  �����:%#04X  ��%2d�� ",\
						calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec,\
						ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_procedure:
				sprintf((char*)MessageBuf,"%04d��%02d��%02d��  %02d:%02d:%02d      ��%03d����  �����:      ��%2d�� ",\
						calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec,\
						ID.Batch,calendar.weeks);	
			break;
			default:break;
		}
	}
	
	MessageOverTimeCnt++;
	if(MessageOverTimeCnt>=MessageOverTime)//��ʱ�˳�����ģʽ
	{
		MessageOverTimeCnt = 0;
		MessageSetFlag = false;
		//SdFatfsInit();//����������ļ�

		//RTC_Set(calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec);
		//RTC_ITConfig(RTC_IT_SEC, ENABLE);	//ʹ��RTC���ж�		
		
		//Flash_Buffer[0] = ID.Batch;
		//Flash_Buffer[1] = ID.ProcedureNum;
		//STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Buffer,2);
	}	
	Show_Str(0, 0,POINT_COLOR,BACK_COLOR,MessageBuf,16,0);	
}

void MessageUpDown(u8 updown)//0:up	!0:down
{
	if(updown==0)
	{
		switch(MessagePlace)
		{
			case MessagePlace_year:
				if(calendar.year<2099)
				{
					calendar.year++;			
				}
				else
				{
					calendar.year = 2000;
				}
			break;
			case MessagePlace_month:
				if(calendar.month<12)
				{
					calendar.month++;			
				}
				else
				{
					calendar.month = 1;
				}
			break;
			case MessagePlace_date:
				if(calendar.date<31)
				{
					calendar.date++;			
				}
				else
				{
					calendar.date = 1;
				}
			break;
			case MessagePlace_hour:
				if(calendar.hour<23)
				{
					calendar.hour++;			
				}
				else
				{
					calendar.hour = 0;
				}
			break;
			case MessagePlace_min:
				if(calendar.min<59)
				{
					calendar.min++;			
				}
				else
				{
					calendar.min = 0;
				}
			break;
			case MessagePlace_sec:
				if(calendar.sec<59)
				{
					calendar.sec++;			
				}
				else
				{
					calendar.sec = 0;
				}
			break;
			case MessagePlace_batch:
				if(ID.Batch<255)
				{
					ID.Batch++;			
				}
				else
				{
					ID.Batch = 1;
				}
			break;
			case MessagePlace_procedure:
				if(ID.ProcedureNum<0x1F)
				{
					ID.ProcedureNum++;			
				}
				else
				{
					ID.ProcedureNum = 0;
				}
			break;
			default:break;
		}
	}
	else
	{
		switch(MessagePlace)
		{
			case MessagePlace_year:
				if(calendar.year>2000)
				{
					calendar.year--;			
				}
				else
				{
					calendar.year = 2099;
				}
			break;
			case MessagePlace_month:
				if(calendar.month>1)
				{
					calendar.month--;			
				}
				else
				{
					calendar.month = 12;
				}
			break;
			case MessagePlace_date:
				if(calendar.date>1)
				{
					calendar.date--;			
				}
				else
				{
					calendar.date = 31;
				}
			break;
			case MessagePlace_hour:
				if(calendar.hour>0)
				{
					calendar.hour--;			
				}
				else
				{
					calendar.hour = 23;
				}
			break;
			case MessagePlace_min:
				if(calendar.min>0)
				{
					calendar.min--;			
				}
				else
				{
					calendar.min = 59;
				}
			break;
			case MessagePlace_sec:
				if(calendar.sec>0)
				{
					calendar.sec--;			
				}
				else
				{
					calendar.sec = 59;
				}
			break;
			case MessagePlace_batch:
				if(ID.Batch>1)
				{
					ID.Batch--;			
				}
				else
				{
					ID.Batch = 255;
				}
			break;
			case MessagePlace_procedure:
				if(ID.ProcedureNum>1)
				{
					ID.ProcedureNum--;			
				}
				else
				{
					ID.ProcedureNum = 0x1F;
				}
			break;
			default:break;
		}	
	}
}

void MessageUpDownQuick(void)	//�������ټӼ�
{
	if(MessageSetFlag != true) return;
	
	if(KeyPress & KEY_UP)
	{
		MessageUpDownCnt++;
		if(MessageUpDownCnt>=10)//����ʱ��
		{
			MessageUpDownCnt = 10;
		    MessageOverTimeCnt = 0;
			MessageGlintFlag = true;
			MessageUpDown(0);
		}
	}
	if(KeyPress & KEY_DOWN)
	{
		MessageUpDownCnt++;
		if(MessageUpDownCnt>=10)
		{
			MessageUpDownCnt = 10;
            MessageOverTimeCnt = 0;
			MessageGlintFlag = true;
			MessageUpDown(1);
		}
	}
}

