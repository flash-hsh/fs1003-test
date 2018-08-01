#ifndef __FSM_H
#define __FSM_H		
//#include "sys.h"	 

typedef struct FsmTable_s
{
	int event;   //事件
	int CurState;  //当前状态
	void (*eventActFun)();  //函数指针
	int NextState;  //下一个状态
}FsmTable_t;


typedef struct FSM_s
{
	FsmTable_t* FsmTable;   //指向的状态表
	int curState;  //FSM当前所处的状态

}FSM_t;



//工装的状态
enum
{
	IDLE_STATE,    //空闲状态  
	SCAN_QR_CODES,	//扫码状态
	BURNING_PROGRAM,	//烧录
	PID_CALIBRATION,	//PID校准
	CLEAN_FILTER_CALIBRATION,	//干净滤网校准
	DIRTY_FILTER_CALIBRATION,	//全脏滤网校准
	HALF_FILTER_TEST,	//半脏滤网测试
};

//工装触发事件
enum
{
	ERROR_EVENT = 1 ,	//错误事件包括：扫码不成功，烧录不成功，PID校准不成功
	KEY_SCAN_TRIGGER,		//扫码按键按下松开
	KEY_START_PRESS,	//测试开始按键按下	
	BURNING_PROGRAM_OK,	//烧录程序成功
	PID_CALIBRATION_OK,	//PID校准成功
	CLEAN_FILTER_CALIBRATION_OK,	//干净滤网校准成功
	DIRTY_FILTER_CALIBRATION_OK,	//全脏滤网校准成功
	HALF_FILTER_TEST_OK,	//半脏滤网测试成功
	IDLE_EVENT ,	//没有事件触发时，进入空闲状态
	
};



////void InitFsm(FSM_t* pFsm,FsmTable_t* pTable );
//void FSM_EventHandle(FSM_t* pFsm, int event);
//void FSM_Regist(FSM_t* pFsm, FsmTable_t* pTable);
//void FSM_StateTransfer(FSM_t* pFsm, int state);


#endif



