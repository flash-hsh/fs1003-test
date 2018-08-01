#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include "sys.h"



typedef struct 
{	
	u8 Batch;		//���κ�
	u16 SerialNum;	//��Ʒ���к�
	u8 ProcedureNum;//��װ@�����	
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
	MessagePlace_batch,	//����
	MessagePlace_procedure,//�����
}MessagePlaceTypDef;
extern MessagePlaceTypDef MessagePlace;
extern u16 MessageOverTimeCnt;
extern u8 MessageUpDownCnt;
extern u16 Flash_Buffer[];
extern bool MessageGlintFlag;
extern bool MessageSetFlag;

#define MessageOverTime 20				//�޲�����ʱ�˳�����ģʽ 10s
void getTime(void);
void MessageSetDis(void);
void MessageUpDown(u8 updown);//0:up	!0:down
void MessageUpDownQuick(void);
#endif
