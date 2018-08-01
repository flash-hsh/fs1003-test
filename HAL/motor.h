#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "sys.h"

#define MotorDir_Pin 		PFout(12)	//���ת��
#define MotorEn_Pin 		PFout(13)	//ʹ�ܵ��

#define MotorDetLimiUp_Pin		PCin(0)//����λ
#define MotorDetLimiDown_Pin	PCin(1)//����λ

#define MotorOff() 	{MotorEn_Pin = 1;TIM_Cmd(TIM1,DISABLE);}
#define MotorOn() 	{MotorEn_Pin = 0;TIM_Cmd(TIM1,ENABLE);}
#define MotorFwd()	(MotorDir_Pin = 0)
#define MotorRev()	(MotorDir_Pin = 1)

#define 	BanquanPulse	800	//��Ȧ������,��8��Ƶ��1600����/Ȧ

#define  DEVICE_NUM  2   //ѡ��װ���

#if  DEVICE_NUM==1	//��װ1�ĵ������

#define 	STANDCARD1      4
#define 	STANDCARD2		12
#define 	STANDCARD3		12

#elif DEVICE_NUM==2	//��װ2�ĵ������

#define 	STANDCARD1      4		//����λ���׼��1���룬������
#define 	STANDCARD2		25		//����λ���׼��2���룬������
#define 	STANDCARD3		25		//����λ���׼��3���룬������

#endif


typedef enum
{
	ZERO_POINT = 0,			//���λ�ã�����λ����λ��
	CLEAN_FILTER_POSITION,	//�ɾ�����λ��
	DIRTY_FILTER_POSITION,	//ȫ������λ��
	HALF_FILTER_POSITION,	//50%����λ��
	CRITICAL_POINT,			//�ٽ�㣬����λ����λ��	
}Position_e;				//λ�ü�¼

typedef struct
{
	__IO int 	  pulse_count; 					//���������һ�������������1 
	u32		      zero2_cleanFilter; 			//��㵽�ɾ�������������
	u32		      cleanFilter2_dirtyFilter; 	//�ɾ�������ȫ��������������
	u32		      dirtyFilter2_halfFilter;		//ȫ��������50%������������
	u32		      halfFilter2_cleanFilter;		//50%�����ع鵽�ɾ�������������
	u32		      back2_zeroPoint;				//�ڴ�������λ�ûع�����������
	
}Pulse_t;										//����������

typedef enum
{
	FWD = 0,					//��ת
	REV ,						//��ת
}Rotation_Diretion_e;			//��ת����



extern volatile Position_e cur_position;
extern  volatile Pulse_t	pulse;	


void TIM1_PWM_Init(u16 arr,u16 psc);//����Ƶ�ʳ�ʼ��
void MOTOR_Init(void);//�ⲿ�жϵȳ�ʼ��
void motor_drive(Position_e next_position,int pulseNum);
void motor_calibration(void);

#endif
