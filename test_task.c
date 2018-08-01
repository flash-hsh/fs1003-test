#include "test_task.h"

#include "sys.h"
#include "gpio.h"
#include "delay.h"
#include "key.h"

#include "lcd.h"
#include "gui_lcd.h"
#include "usart.h"
#include "timer.h"
#include "pwm.h"
#include "adc.h"
#include "dma.h"
#include "rtc.h" 

#include "malloc.h"  

#include "ff.h"  
#include "exfuns.h"
#include "fattester.h"	
#include "CRC.h"
#include <stdlib.h>

#include "fsm.h"
#include "protocol_fs.h"
#include "display.h"
#include "motor.h"
#include "message.h"
#include "serial.h"
#include "fatfs.h"
#include "buzzer.h"
#include "string.h"


vu16 TimerCount = 0;

TaskHandle_t TestTask_Handler;


int g_max_num;  //状态表里含有的状态个数
int event = ERROR_EVENT; //事件转移——>状态转移

#define MIN_caliVot 0x32	//微调电压

u8 UpdateFirmwareEnFlag = 1;//0:不允许更新烧录固件

void TASK_KeyDeal(void);
void DataInit(void);
void DataServe(void);//数据存储
void Scan_QR_Codes_Function(void);
void Burning_Program_Function(void);
void PID_Calibration_Function(void);
void Clean_Filter_Calibration_Function(void);
void Dirty_Filter_Calibration_Function(void);
void Half_Filter_Test_Function(void);
void Termination_Test_Function(void);
void Idle_State_Function(void);


/*状态机注册*/
void FSM_Regist(FSM_t* pFsm, FsmTable_t* pTable)
{
	pFsm->FsmTable = pTable;
}

/*状态迁移*/
void FSM_StateTransfer(FSM_t* pFsm, int state)
{
	pFsm->curState = state;
}


/*事件处理*/
void FSM_EventHandle(FSM_t* pFsm, int event)
{
	FsmTable_t* pActTable = pFsm->FsmTable;
	void (*eventActFun)() = NULL;  //函数指针初始化为空
	int NextState;
	int CurState = pFsm->curState;
	int flag = 0; //标识是否满足条件

	/*获取当前动作函数*/
	for (int i = 0; i<g_max_num; i++)
	{
		//当且仅当当前状态下来个指定的事件，我才执行它
		if (event == pActTable[i].event && CurState == pActTable[i].CurState)
		{
			flag = 1;
			eventActFun = pActTable[i].eventActFun;
			NextState = pActTable[i].NextState;
			break;
		}
	}


	if (flag) //如果满足条件了
	{
		/*动作执行*/
		if (eventActFun)
		{
			eventActFun();
		}

		//跳转到下一个状态
		FSM_StateTransfer(pFsm, NextState);
	}
	else
	{
		// do nothing
	}
}





FsmTable_t TestDeviceTable[] =
{
	//{到来的事件，当前的状态，将要要执行的函数，下一个状态}
	{ KEY_SCAN_TRIGGER,  				IDLE_STATE,  				Scan_QR_Codes_Function,       		SCAN_QR_CODES },
	{ KEY_START_PRESS,   				SCAN_QR_CODES, 				Burning_Program_Function,    		BURNING_PROGRAM },
	{ BURNING_PROGRAM_OK,  				BURNING_PROGRAM,    		PID_Calibration_Function,    		PID_CALIBRATION },
	{ PID_CALIBRATION_OK,  				PID_CALIBRATION,     		Clean_Filter_Calibration_Function,  CLEAN_FILTER_CALIBRATION },
	{ CLEAN_FILTER_CALIBRATION_OK,  	CLEAN_FILTER_CALIBRATION,   Dirty_Filter_Calibration_Function,  DIRTY_FILTER_CALIBRATION },
	{ DIRTY_FILTER_CALIBRATION_OK,  	DIRTY_FILTER_CALIBRATION,   Half_Filter_Test_Function,       	HALF_FILTER_TEST },
	{ HALF_FILTER_TEST_OK,  			HALF_FILTER_TEST,   		Termination_Test_Function,       	IDLE_STATE },
	{ IDLE_EVENT,  						IDLE_STATE,   				Idle_State_Function,       			IDLE_STATE },	
	{ ERROR_EVENT,  					SCAN_QR_CODES,   			Termination_Test_Function,       	IDLE_STATE },
	{ ERROR_EVENT,  					BURNING_PROGRAM,   			Termination_Test_Function,       	IDLE_STATE },
	{ ERROR_EVENT,  					PID_CALIBRATION,   			Termination_Test_Function,       	IDLE_STATE },
	{ ERROR_EVENT,  					HALF_FILTER_TEST,   		Termination_Test_Function,       	IDLE_STATE },
	{ ERROR_EVENT,  					IDLE_STATE,  				Idle_State_Function,       			IDLE_STATE },
	
	//add your codes here
};

//初始化FSM
void InitFsm(FSM_t* pFsm)
{
	
	g_max_num = sizeof(TestDeviceTable) / sizeof(FsmTable_t);
	pFsm->curState = IDLE_STATE;
	FSM_Regist(pFsm, TestDeviceTable);
	
}


//工装烧录测试任务函数
void test_task(void *pvParameters)
{
	FSM_t fsm;
	InitFsm(&fsm);
	SdFatfsInit();
	DataInit();	

	while(1)
	{
		FSM_EventHandle(&fsm, event);
		if(TimerFlag == 1)//10ms
		{ 	
			TimerFlag=0;
			TimerCount++;
			TASK_KeyDeal();
			Display(&event);			
			motor_calibration();				
		}

		
		
//		if(TimerCount%50==0) 
//		{  //500ms
//		
//			TimerCount=0;
//			ReadVolt();			
//			//MessageSetDis();	
//			MessageToBuffDis();
//			
//		}	
                         //延时1s，也就是1000个时钟节拍	
	}
}


void TASK_KeyDeal(void)
{			
	TASK_KeyScan();	
	//工装压下
	if((KeyDown & KEY_START))
	
	{				
		KeyDown &= (!KEY_START);
		isKeyClicked &=  (~KEY_PNEUMATIC);				
		DataInit();
		event = KEY_START_PRESS;//启动测试				
		DisplayUpdataEnable = true;
	}
	
	//工装抬起 
	if((KeyUp & KEY_START))
	{
		
		KeyUp &= (!KEY_START);
		event = ERROR_EVENT;//只要有工装抬起事件发生，无论在什么状态，触发ERROR_EVENT事件		
	
		//printf("end!\r\n");
	}


	//***********************************
		//if((isKeyClicked & KEY_PNEUMATIC) && (IDCheckOk == true))
	  if(isKeyClicked & KEY_PNEUMATIC)
		{			
			isKeyClicked &=  (~KEY_PNEUMATIC);			
			if(IDCheckOk == true)
			{
				PNEUMATIC_VALVE=1;//气压阀 通电			
				IDCheckOk = false;
				IDstartSendFlag = false;
				uartMode=0;
				uartModePin=0;
			}						
		}

    
	  if(isKeyClicked & KEY_SCANNING)//扫码按钮被按下
		{
		    uartModePin=1; 
			uartMode=1;
			isKeyClicked &=  (~KEY_SCANNING);	   	   	    
			event = KEY_SCAN_TRIGGER;//发生二维码扫描事件，去执行Scan_QR_Codes_Function

		}	

//***********************************
    if(isKeyClicked & KEY_UP)
    {
        isKeyClicked &=  (~KEY_UP);
		printf("KEY_UP isKeyClicked!\r\n");
//		if(MessageSetFlag==true)
//		{
//			MessageOverTimeCnt = 0;
//			MessageUpDownCnt = 0;
//			MessageGlintFlag = true;
//			MessageUpDown(0);		
//		}
    }
	
    if(isKeyClicked & KEY_DOWN)
    {
        isKeyClicked &=  (~KEY_DOWN);
		printf("KEY_DOWN isKeyClicked!\r\n");
		
//		if(MessageSetFlag==true)
//		{
//			MessageOverTimeCnt = 0;
//			MessageUpDownCnt = 0;
//			MessageGlintFlag = true;
//			MessageUpDown(1);				
//		}
    }

    if(isKeyClicked & KEY_LEFT)
    {
        isKeyClicked &=  (~KEY_LEFT);
		printf("KEY_LEFT isKeyClicked!\r\n");
		
//		if(MessageSetFlag==true)
//		{
//			MessageGlintFlag = true;
//            MessageOverTimeCnt = 0;
//			if(MessagePlace!=MessagePlace_year)
//			{
//				MessagePlace--;
//			}
//			else
//			{
//				MessagePlace = MessagePlace_procedure;
//			}
//		}
    }

    if(isKeyClicked & KEY_RIGHT)
    {
        isKeyClicked &=  (~KEY_RIGHT);
		printf("KEY_RIGHT isKeyClicked!\r\n");
		
//		if(MessageSetFlag==true)
//		{
//			MessageOverTimeCnt = 0;
//            MessageGlintFlag = true;
//			if(MessagePlace!=MessagePlace_procedure)
//			{
//				MessagePlace++;
//                

//			}
//			else
//			{
//				MessagePlace = MessagePlace_year;
//			}
//		}
    }

    if(isKeyClicked & KEY_TOP)
    {
        isKeyClicked &=  (~KEY_TOP);
		printf("KEY_TOP isKeyClicked!\r\n");
		
//		if(MessageSetFlag==true)//退出设置模式
//		{
//			MessageSetFlag = false;
//			MessageOverTimeCnt = 0;
//			RTC_Set(calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec);
//			RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒中断		
//			
//			Flash_Buffer[0] = ID.Batch;
//			Flash_Buffer[1] = ID.ProcedureNum;
//			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Buffer,2);
//		}
    }
//***************************************************	
	if(isKeyPressLong & KEY_TOP)//长按
	{
        isKeyPressLong &=  (~KEY_TOP);
        KeyDown &= (~KEY_TOP);		//防止长按弹起置位单击标志
		printf("KEY_TOP isKeyPressLong!\r\n");
		
		
//		MessageSetFlag = true;		//进入日历设置模式
		
//		MessageOverTimeCnt = 0;
//		MessagePlace = MessagePlace_year;
//		RTC_ITConfig(RTC_IT_SEC, DISABLE);	//失能RTC秒中断
		
	}

    if(KeyPress & KEY_MOTORDETLIMIDOWN)
    {       
		cur_position = ZERO_POINT;

    }   

    if(KeyPress & KEY_MOTORDETLIMIUP)
    {        
       
		cur_position = CRITICAL_POINT;


    }   


}



void DataServe(void)//写入ID，数据存储
{
    FilterIdCheck();
	FilterIdCheck();
	save();//数据存储
	UartSendData_toPC();
	
}
void DataInit(void)
{
	TimerFlag = 0;//	
	TimerCount=0;	
	DisplayUpdataEnable = true;
	MotorOff();
	DisDrawUI();
	Filter_DataInit();

}


void Scan_QR_Codes_Function()
{
	// do something
	//printf("Scan_QR_Codes_Function!\r\n");
		u8 ret;
		int cnt = 10;
#if 1

		SendCMDtoScannDevice(); //发送扫码指令
		do
		{
			delay_ms(500);
			ret =	ReceiveID();
			cnt--;
			
		}
		while (ret&&cnt);

	if (ret == 1) //ID接收失败
		event = ERROR_EVENT;
	
#endif

}
	
void Burning_Program_Function()
{
	// do something
	//printf("Burning_Program_Function!\r\n");
#if 1
	FilterIdCheck();
	if(UpdateFirmwareEnFlag==0)//不允许更新固件
	{
		if(FS_SerialReceiveBuff[CHANNEL_FS1]==0)	ProgKey1 = 1;//未分配ID的通道启动烧录
		if(FS_SerialReceiveBuff[CHANNEL_FS2]==0)	ProgKey2 = 1;
		if(FS_SerialReceiveBuff[CHANNEL_FS3]==0)	ProgKey3 = 1;
		if(FS_SerialReceiveBuff[CHANNEL_FS4]==0)	ProgKey4 = 1;
	}
	else//允许更新固件
	{
		ProgKey1 = 1;//烧录器按键弹起
		ProgKey2 = 1;
		ProgKey3 = 1;
		ProgKey4 = 1;	
	}
	delay_ms(100);//延时弹起
	ProgKey1 = 0;//烧录器按键弹起
	ProgKey2 = 0;
	ProgKey3 = 0;
	ProgKey4 = 0;	
	delay_ms(1000);//延时等待烧录结束
	delay_ms(1200);//对72M条件下,nms<=1864 
	//printf("FilterIdCheck 2\r\n");
    delay_ms(1000);
	FilterIdCheck();//烧录完再次检查
	delay_ms(20);
	FilterIdCheck();//烧录完再次检查
	delay_ms(20);
	FilterIdCheck();//烧录完再次检查
	delay_ms(20);
   
	Filter_EnterModeCMD(CMD_WID);//写入ID
    delay_ms(20);
    Filter_EnterModeCMD(CMD_WID);//写入ID
	delay_ms(200);
	Filter_RecieveDataProcess(CHANNEL_FS1);
	Filter_RecieveDataProcess(CHANNEL_FS2);
	Filter_RecieveDataProcess(CHANNEL_FS3);
	Filter_RecieveDataProcess(CHANNEL_FS4);	
	
	event = BURNING_PROGRAM_OK; 
	DisplayUpdataEnable = true;//使能数据更新	

#endif
	
}
	
void PID_Calibration_Function()
{
	// do something
	u8 ret;
	int cnt = 4;
	
	//printf("PID_Calibration_Function!\r\n");
#if 1
	uartMode=2;//串口接收改为PID校准模式

	
	do
	{
	
		PID_Calibration_Mode(PIDCALI_MODE);//PID校准模式
		delay_ms(4000);
		ret = PID_Set_Ok();
		cnt--;
	}while(ret&&cnt);

	PID_Calibration_Mode(ANSWER_MODE);	//停止校准
	
	uartMode=0;//串口接收改为FS测试模式
	if (ret == 0)//PID校准OK
		{
			event = PID_CALIBRATION_OK;	

		
		}
	else
		{//PID校准失败
		
			event = ERROR_EVENT; 

		}
	
  	DisplayUpdataEnable = true;//使能数据更新
	

#endif
}
	
void Clean_Filter_Calibration_Function()
{
	// do something
	//printf("Clean_Filter_Calibration_Function!\r\n");

			u8 i;

			for(i=0;i<2;i++)
			{
				Filter_EnterModeCMD(CMD_RPID);//读PID占空比
				delay_ms(50);
				Filter_RecieveDataProcess(CHANNEL_FS1);
				Filter_RecieveDataProcess(CHANNEL_FS2);
				Filter_RecieveDataProcess(CHANNEL_FS3);
				Filter_RecieveDataProcess(CHANNEL_FS4);
			}
				
			for(i=0;i<3;i++)
			{

				Filter_EnterModeCMD(CMD_RVV);//读干净滤网TP2电压值
				delay_ms(50);
				Filter_RecieveDataProcess(CHANNEL_FS1);
				Filter_RecieveDataProcess(CHANNEL_FS2);
				Filter_RecieveDataProcess(CHANNEL_FS3);
				Filter_RecieveDataProcess(CHANNEL_FS4);
				FS[CHANNEL_FS1].MaxVolt = (FS[CHANNEL_FS1].VoltageValueBuff[0]<<8)| FS[CHANNEL_FS1].VoltageValueBuff[1];
				FS[CHANNEL_FS2].MaxVolt = (FS[CHANNEL_FS2].VoltageValueBuff[0]<<8)| FS[CHANNEL_FS2].VoltageValueBuff[1];
				FS[CHANNEL_FS3].MaxVolt = (FS[CHANNEL_FS3].VoltageValueBuff[0]<<8)| FS[CHANNEL_FS3].VoltageValueBuff[1];
				FS[CHANNEL_FS4].MaxVolt = (FS[CHANNEL_FS4].VoltageValueBuff[0]<<8)| FS[CHANNEL_FS4].VoltageValueBuff[1];

			}
						
			event = CLEAN_FILTER_CALIBRATION_OK; 						
			DisplayUpdataEnable = true;//使能LCD更新	

}
	
void Dirty_Filter_Calibration_Function()
{
	// do something
	//printf("Dirty_Filter_Calibration_Function!\r\n");

			u8 i;						
			motor_drive(DIRTY_FILTER_POSITION,pulse.cleanFilter2_dirtyFilter);//马达切换到全脏滤网位置
			delay_ms(3000);


			for (i = 0; i < 3; i++)
			{
				Filter_EnterModeCMD(CMD_RVV);//读全脏滤网TP2电压值
				delay_ms(50);
				Filter_RecieveDataProcess(CHANNEL_FS1);
				Filter_RecieveDataProcess(CHANNEL_FS2);
				Filter_RecieveDataProcess(CHANNEL_FS3);
				Filter_RecieveDataProcess(CHANNEL_FS4);
				FS[CHANNEL_FS1].MinVolt = (FS[CHANNEL_FS1].VoltageValueBuff[0]<<8)| FS[CHANNEL_FS1].VoltageValueBuff[1];
				FS[CHANNEL_FS2].MinVolt = (FS[CHANNEL_FS2].VoltageValueBuff[0]<<8)| FS[CHANNEL_FS2].VoltageValueBuff[1];
				FS[CHANNEL_FS3].MinVolt = (FS[CHANNEL_FS3].VoltageValueBuff[0]<<8)| FS[CHANNEL_FS3].VoltageValueBuff[1];
				FS[CHANNEL_FS4].MinVolt = (FS[CHANNEL_FS4].VoltageValueBuff[0]<<8)| FS[CHANNEL_FS4].VoltageValueBuff[1];
			}

			//全脏电压值 — 微调电压
			FS[CHANNEL_FS1].MinVolt = FS[CHANNEL_FS1].MinVolt -MIN_caliVot;
			FS[CHANNEL_FS2].MinVolt = FS[CHANNEL_FS2].MinVolt -MIN_caliVot;
			FS[CHANNEL_FS3].MinVolt = FS[CHANNEL_FS3].MinVolt -MIN_caliVot;
			FS[CHANNEL_FS4].MinVolt = FS[CHANNEL_FS4].MinVolt -MIN_caliVot;
	
			
			Filter_EnterModeCMD(CMD_WCV);//写入校准电压
			delay_ms(50);
			Filter_EnterModeCMD(CMD_WCV);//写入校准电压
			delay_ms(50);
			Filter_RecieveDataProcess(CHANNEL_FS1);
			Filter_RecieveDataProcess(CHANNEL_FS2);
			Filter_RecieveDataProcess(CHANNEL_FS3);
			Filter_RecieveDataProcess(CHANNEL_FS4);

			event = DIRTY_FILTER_CALIBRATION_OK;
			DisplayUpdataEnable = true;//使能LCD更新

}

void Half_Filter_Test_Function()
{
	// do something
			u8 i;
	
			//printf("Half_Filter_Test_Function!\r\n");
			motor_drive(HALF_FILTER_POSITION,pulse.dirtyFilter2_halfFilter);//马达切换到50%滤网位置
			TIM_Cmd(TIM4, ENABLE);//开启定时器，读取PWM
			delay_ms(4000);		
						  											       
			for(i=0;i<5;i++)
			{
				delay_ms(300);
				Filter_EnterModeCMD(CMD_RBR);//读堵塞率，档位
				delay_ms(50);
				Filter_RecieveDataProcess(CHANNEL_FS1);
				Filter_RecieveDataProcess(CHANNEL_FS2);
				Filter_RecieveDataProcess(CHANNEL_FS3);
				Filter_RecieveDataProcess(CHANNEL_FS4);
				FS[CHANNEL_FS1].BlockingRate = FS[CHANNEL_FS1].BlockingRateBuff[0];
				FS[CHANNEL_FS2].BlockingRate = FS[CHANNEL_FS2].BlockingRateBuff[0];
				FS[CHANNEL_FS3].BlockingRate = FS[CHANNEL_FS3].BlockingRateBuff[0];
				FS[CHANNEL_FS4].BlockingRate = FS[CHANNEL_FS4].BlockingRateBuff[0]; 
		 
		   }
	
			ReadVolt();//启动电压测试，启动一次DMA就进行一次电压转换
														
			for(i=0;i<2;i++)
			{
				Filter_EnterModeCMD(CMD_RVV);//读电压MidVolt_tp2
				delay_ms(50);
				Filter_RecieveDataProcess(CHANNEL_FS1);
				Filter_RecieveDataProcess(CHANNEL_FS2);
				Filter_RecieveDataProcess(CHANNEL_FS3);
				Filter_RecieveDataProcess(CHANNEL_FS4);
				FS[CHANNEL_FS1].MidVolt_tp2 = (FS[CHANNEL_FS1].VoltageValueBuff[0]<<8)| (FS[CHANNEL_FS1].VoltageValueBuff[1]);
				FS[CHANNEL_FS2].MidVolt_tp2 = (FS[CHANNEL_FS2].VoltageValueBuff[0]<<8)| (FS[CHANNEL_FS2].VoltageValueBuff[1]);
				FS[CHANNEL_FS3].MidVolt_tp2 = (FS[CHANNEL_FS3].VoltageValueBuff[0]<<8)| (FS[CHANNEL_FS3].VoltageValueBuff[1]);
				FS[CHANNEL_FS4].MidVolt_tp2 = (FS[CHANNEL_FS4].VoltageValueBuff[0]<<8)| (FS[CHANNEL_FS4].VoltageValueBuff[1]);
			}
				
		  
			for(i=0;i<2;i++)
			{			
				FS[CHANNEL_FS1].MidVolt_tp1 = FS[CHANNEL_FS1].Tp1Volt; //TP1电压
				FS[CHANNEL_FS2].MidVolt_tp1 = FS[CHANNEL_FS2].Tp1Volt;
				FS[CHANNEL_FS3].MidVolt_tp1 = FS[CHANNEL_FS3].Tp1Volt;
				FS[CHANNEL_FS4].MidVolt_tp1 = FS[CHANNEL_FS4].Tp1Volt;
			}

			for(i=0;i<4;i++)
			{
				//pwm值
				FS[i].PwmDuty=(u8)(pwm_val_fs[i]/10);
			}

			for(i=0;i<2;i++)
			{
				//温度值
				FS[CHANNEL_FS1].TempertureValueBuff[i] = 0; 
				FS[CHANNEL_FS2].TempertureValueBuff[i] = 0; 
				FS[CHANNEL_FS3].TempertureValueBuff[i] = 0; 
				FS[CHANNEL_FS4].TempertureValueBuff[i] = 0; 
			}
		
	
	        //PNEUMATIC_VALVE=0;//气压阀 关	
			TIM_Cmd(TIM4, DISABLE);//关闭pwm定时器
			DataServe();//数据存储
			event = HALF_FILTER_TEST_OK;	//测试结束
			DisplayUpdataEnable = true;//使能数据更新


}
//终止测试
void Termination_Test_Function()
{

	//printf("Termination_Test_Function!\r\n");
	buzzerRingNum(2);//启动蜂鸣器	
	PNEUMATIC_VALVE=0;//气压阀 关	
	motor_drive(CLEAN_FILTER_POSITION,pulse.halfFilter2_cleanFilter);//马达回归干净滤网位置
	event = IDLE_EVENT;	//测试结束，置为空闲事件
	DisplayUpdataEnable = true;//使能数据更新

}

void Idle_State_Function()
{
	// do something
	//printf("Idle_State_Function!\r\n");
#if 0
					
	if(MotorPosition.pulse_count!=MotorPosition.pulseToStand1)//如果马达未归位
	{
		MotorOff();
		TIM_Cmd(TIM1,DISABLE);       
		MotorCaliFlag = CALI2;//1:马达校准模式
	}	
	//DisplayUpdataEnable = true;	
	MotorOff();	
#endif

}
	






