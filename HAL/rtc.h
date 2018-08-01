#ifndef __RTC_H
#define __RTC_H	
#include "stm32f4xx.h"   
//Mini STM32������
//RTCʵʱʱ�� ��������			 
//����ԭ��@ALIENTEK
//2010/6/6

//ʱ��ṹ��
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//������������
	vu16 year;
	vu8  month;
	vu8  date;
	vu8  week;	//�ܼ�	 
	vu8  weeks;	//����ڼ���		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//�����ṹ��

u8 My_RTC_Init(void);						//RTC��ʼ��
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);			//RTCʱ������
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week); 		//RTC��������
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec);		//��������ʱ��(����������,24Сʱ��)
void RTC_Set_WakeUp(u32 wksel,u16 cnt);					//�����Ի��Ѷ�ʱ������
#endif


