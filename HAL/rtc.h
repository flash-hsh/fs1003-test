#ifndef __RTC_H
#define __RTC_H	
#include "stm32f4xx.h"   
//Mini STM32开发板
//RTC实时时钟 驱动代码			 
//正点原子@ALIENTEK
//2010/6/6

//时间结构体
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//公历日月年周
	vu16 year;
	vu8  month;
	vu8  date;
	vu8  week;	//周几	 
	vu8  weeks;	//该年第几周		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//日历结构体

u8 My_RTC_Init(void);						//RTC初始化
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);			//RTC时间设置
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week); 		//RTC日期设置
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec);		//设置闹钟时间(按星期闹铃,24小时制)
void RTC_Set_WakeUp(u32 wksel,u16 cnt);					//周期性唤醒定时器设置
#endif


