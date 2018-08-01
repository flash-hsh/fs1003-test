//页眉信息处理 时间，批次，工序号
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
u16 Flash_Buffer[14];			//要写入到STM32 FLASH的数组 0:Batch,1:ProcedureNum
u8 MessageBuf[100];				//时间等信息
bool MessageGlintFlag = true;	//闪烁亮灭标志	true：亮
bool MessageSetFlag = false;	//信息设置标志	true：设置状态
//eg:2017年11月16日  17:26:00      第256批次  工序号:0xFF  第47周

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



void MessageToBuffDis(void)//信息转存到数组并显示
{		
	getTime();	
	if(MessageSetFlag == true) return;
	
	sprintf((char*)MessageBuf,"%4d年%02d月%02d日  %02d:%02d:%02d      第%03d批次  工序号:%#04X  第%02d周 ",\
			calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec,\
			ID.Batch,ID.ProcedureNum,calendar.weeks);	

	Show_Str(0, 0,POINT_COLOR,BACK_COLOR,MessageBuf,16,0);
}

void MessageSetDis(void)//设置模式信息显示
{
	
	getTime();	
	
	if(MessageSetFlag == false) return;
	//闪烁
	if(MessageGlintFlag)
	{
		MessageGlintFlag = false;
		sprintf((char*)MessageBuf,"%04d年%02d月%02d日  %02d:%02d:%02d      第%03d批次  工序号:%#04X  第%2d周 ",\
				calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec,\
				ID.Batch,ID.ProcedureNum,calendar.weeks);	
	}
	else
	{
		MessageGlintFlag = true;
		switch(MessagePlace)
		{
			case MessagePlace_year:
				sprintf((char*)MessageBuf,"    年%02d月%02d日  %02d:%02d:%02d      第%03d批次  工序号:%#04X  第%2d周 ",\
						calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec,\
						ID.Batch,ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_month:
				sprintf((char*)MessageBuf,"%04d年  月%02d日  %02d:%02d:%02d      第%03d批次  工序号:%#04X  第%2d周 ",\
						calendar.year,calendar.date,calendar.hour,calendar.min,calendar.sec,\
						ID.Batch,ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_date:
				sprintf((char*)MessageBuf,"%04d年%02d月  日  %02d:%02d:%02d      第%03d批次  工序号:%#04X  第%2d周 ",\
						calendar.year,calendar.month,calendar.hour,calendar.min,calendar.sec,\
						ID.Batch,ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_hour:
				sprintf((char*)MessageBuf,"%04d年%02d月%02d日    :%02d:%02d      第%03d批次  工序号:%#04X  第%2d周 ",\
						calendar.year,calendar.month,calendar.date,calendar.min,calendar.sec,\
						ID.Batch,ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_min:
				sprintf((char*)MessageBuf,"%04d年%02d月%02d日  %02d:  :%02d      第%03d批次  工序号:%#04X  第%2d周 ",\
						calendar.year,calendar.month,calendar.date,calendar.hour,calendar.sec,\
						ID.Batch,ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_sec:
				sprintf((char*)MessageBuf,"%04d年%02d月%02d日  %02d:%02d:        第%03d批次  工序号:%#04X  第%2d周 ",\
						calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,\
						ID.Batch,ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_batch:
				sprintf((char*)MessageBuf,"%04d年%02d月%02d日  %02d:%02d:%02d      第   批次  工序号:%#04X  第%2d周 ",\
						calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec,\
						ID.ProcedureNum,calendar.weeks);	
			break;
			case MessagePlace_procedure:
				sprintf((char*)MessageBuf,"%04d年%02d月%02d日  %02d:%02d:%02d      第%03d批次  工序号:      第%2d周 ",\
						calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec,\
						ID.Batch,calendar.weeks);	
			break;
			default:break;
		}
	}
	
	MessageOverTimeCnt++;
	if(MessageOverTimeCnt>=MessageOverTime)//超时退出设置模式
	{
		MessageOverTimeCnt = 0;
		MessageSetFlag = false;
		//SdFatfsInit();//设置完打开新文件

		//RTC_Set(calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec);
		//RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒中断		
		
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

void MessageUpDownQuick(void)	//长按快速加减
{
	if(MessageSetFlag != true) return;
	
	if(KeyPress & KEY_UP)
	{
		MessageUpDownCnt++;
		if(MessageUpDownCnt>=10)//长按时间
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

