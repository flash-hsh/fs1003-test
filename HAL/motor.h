#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "sys.h"

#define MotorDir_Pin 		PFout(12)	//电机转向
#define MotorEn_Pin 		PFout(13)	//使能电机

#define MotorDetLimiUp_Pin		PCin(0)//上限位
#define MotorDetLimiDown_Pin	PCin(1)//下限位

#define MotorOff() 	{MotorEn_Pin = 1;TIM_Cmd(TIM1,DISABLE);}
#define MotorOn() 	{MotorEn_Pin = 0;TIM_Cmd(TIM1,ENABLE);}
#define MotorFwd()	(MotorDir_Pin = 0)
#define MotorRev()	(MotorDir_Pin = 1)

#define 	BanquanPulse	800	//半圈脉冲数,设8分频，1600脉冲/圈

#define  DEVICE_NUM  2   //选择工装编号

#if  DEVICE_NUM==1	//工装1的电机脉冲

#define 	STANDCARD1      4
#define 	STANDCARD2		12
#define 	STANDCARD3		12

#elif DEVICE_NUM==2	//工装2的电机脉冲

#define 	STANDCARD1      4		//下限位距标准件1距离，脉冲数
#define 	STANDCARD2		25		//下限位距标准件2距离，脉冲数
#define 	STANDCARD3		25		//下限位距标准件3距离，脉冲数

#endif


typedef enum
{
	ZERO_POINT = 0,			//零点位置，下限位开关位置
	CLEAN_FILTER_POSITION,	//干净滤网位置
	DIRTY_FILTER_POSITION,	//全脏滤网位置
	HALF_FILTER_POSITION,	//50%滤网位置
	CRITICAL_POINT,			//临界点，上限位开关位置	
}Position_e;				//位置记录

typedef struct
{
	__IO int 	  pulse_count; 					//脉冲计数，一个完整的脉冲加1 
	u32		      zero2_cleanFilter; 			//零点到干净滤网的脉冲数
	u32		      cleanFilter2_dirtyFilter; 	//干净滤网到全脏滤网的脉冲数
	u32		      dirtyFilter2_halfFilter;		//全脏滤网到50%滤网的脉冲数
	u32		      halfFilter2_cleanFilter;		//50%滤网回归到干净滤网的脉冲数
	u32		      back2_zeroPoint;				//在大于零点的位置回归零点的脉冲数
	
}Pulse_t;										//脉冲数类型

typedef enum
{
	FWD = 0,					//正转
	REV ,						//反转
}Rotation_Diretion_e;			//旋转方向



extern volatile Position_e cur_position;
extern  volatile Pulse_t	pulse;	


void TIM1_PWM_Init(u16 arr,u16 psc);//脉冲频率初始化
void MOTOR_Init(void);//外部中断等初始化
void motor_drive(Position_e next_position,int pulseNum);
void motor_calibration(void);

#endif
