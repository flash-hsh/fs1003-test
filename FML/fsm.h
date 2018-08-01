#ifndef __FSM_H
#define __FSM_H		
//#include "sys.h"	 

typedef struct FsmTable_s
{
	int event;   //�¼�
	int CurState;  //��ǰ״̬
	void (*eventActFun)();  //����ָ��
	int NextState;  //��һ��״̬
}FsmTable_t;


typedef struct FSM_s
{
	FsmTable_t* FsmTable;   //ָ���״̬��
	int curState;  //FSM��ǰ������״̬

}FSM_t;



//��װ��״̬
enum
{
	IDLE_STATE,    //����״̬  
	SCAN_QR_CODES,	//ɨ��״̬
	BURNING_PROGRAM,	//��¼
	PID_CALIBRATION,	//PIDУ׼
	CLEAN_FILTER_CALIBRATION,	//�ɾ�����У׼
	DIRTY_FILTER_CALIBRATION,	//ȫ������У׼
	HALF_FILTER_TEST,	//������������
};

//��װ�����¼�
enum
{
	ERROR_EVENT = 1 ,	//�����¼�������ɨ�벻�ɹ�����¼���ɹ���PIDУ׼���ɹ�
	KEY_SCAN_TRIGGER,		//ɨ�밴�������ɿ�
	KEY_START_PRESS,	//���Կ�ʼ��������	
	BURNING_PROGRAM_OK,	//��¼����ɹ�
	PID_CALIBRATION_OK,	//PIDУ׼�ɹ�
	CLEAN_FILTER_CALIBRATION_OK,	//�ɾ�����У׼�ɹ�
	DIRTY_FILTER_CALIBRATION_OK,	//ȫ������У׼�ɹ�
	HALF_FILTER_TEST_OK,	//�����������Գɹ�
	IDLE_EVENT ,	//û���¼�����ʱ���������״̬
	
};



////void InitFsm(FSM_t* pFsm,FsmTable_t* pTable );
//void FSM_EventHandle(FSM_t* pFsm, int event);
//void FSM_Regist(FSM_t* pFsm, FsmTable_t* pTable);
//void FSM_StateTransfer(FSM_t* pFsm, int state);


#endif



