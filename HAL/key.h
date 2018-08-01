#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

extern vu16 KeyPress;
extern vu16 isKeyClicked;
extern vu16 KeyDown, KeyUp;
extern vu16 isKeyPressLong;	

#define KeyStar 		PCin(3) //升降台接触到fs标志
#define KeyPneumatic 	PCin(2) //扫码完成后，按一下开始测试按钮，自动测试开始
#define KeyScann		PDin(13)	//开始扫描


//电机
//#define MotorDetLimiDown_Pin	PCin(1)//下限位
//#define MotorDetLimiUp_Pin		PCin(0)//上限位

//暂时不用
#define KeyModeTest PGin(8)
#define KeyModeProg PGin(9)
#define KeyModeCali PGin(10)


//5向开关
#define KeyU		PGin(5)//上	
#define KeyD		PGin(3)	//下
#define KeyL		PGin(4)	//左	
#define KeyR		PGin(6)//右	
#define KeyT		PGin(7) //顶


//每个按键对应一个bit
#define KEY_START    	0x0001
#define KEY_MODETEST 	0x0002
#define KEY_MODEPROG 	0x0004
#define KEY_MODECALI 	0x0008
#define KEY_UP			0x0010
#define KEY_DOWN		0x0020
#define KEY_LEFT		0x0040
#define KEY_RIGHT		0x0080
#define KEY_TOP			0x0100
#define KEY_MOTORDETLIMIDOWN	0x0200
#define KEY_MOTORDETLIMIUP		0x0400
#define KEY_PNEUMATIC		    0x0800
#define KEY_SCANNING		    0x1000 //开始扫描


void KEY_Init(void);
void TASK_KeyScan(void);
#endif
