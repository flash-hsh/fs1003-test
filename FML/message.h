#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include "sys.h"



typedef struct 
{	
	u8 Batch;		//批次号
	u16 SerialNum;	//产品序列号
	u8 ProcedureNum;//工装@工序号	
}IDTypDef;

extern IDTypDef ID;

typedef enum
{
	MessagePlace_year,
	MessagePlace_month,
	MessagePlace_date,
	MessagePlace_hour,
	MessagePlace_min,
	MessagePlace_sec,
	MessagePlace_batch,	//批次
	MessagePlace_procedure,//工序号
}MessagePlaceTypDef;
extern MessagePlaceTypDef MessagePlace;
extern u16 MessageOverTimeCnt;
extern u8 MessageUpDownCnt;
extern u16 Flash_Buffer[];
extern bool MessageGlintFlag;
extern bool MessageSetFlag;

#define MessageOverTime 20				//无操作延时退出设置模式 10s
void getTime(void);
void MessageSetDis(void);
void MessageUpDown(u8 updown);//0:up	!0:down
void MessageUpDownQuick(void);
#endif
