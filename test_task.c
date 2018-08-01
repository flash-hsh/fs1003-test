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


int g_max_num;  //״̬���ﺬ�е�״̬����
int event = ERROR_EVENT; //�¼�ת�ơ���>״̬ת��

#define MIN_caliVot 0x32	//΢����ѹ

u8 UpdateFirmwareEnFlag = 1;//0:�����������¼�̼�

void TASK_KeyDeal(void);
void DataInit(void);
void DataServe(void);//���ݴ洢
void Scan_QR_Codes_Function(void);
void Burning_Program_Function(void);
void PID_Calibration_Function(void);
void Clean_Filter_Calibration_Function(void);
void Dirty_Filter_Calibration_Function(void);
void Half_Filter_Test_Function(void);
void Termination_Test_Function(void);
void Idle_State_Function(void);


/*״̬��ע��*/
void FSM_Regist(FSM_t* pFsm, FsmTable_t* pTable)
{
	pFsm->FsmTable = pTable;
}

/*״̬Ǩ��*/
void FSM_StateTransfer(FSM_t* pFsm, int state)
{
	pFsm->curState = state;
}


/*�¼�����*/
void FSM_EventHandle(FSM_t* pFsm, int event)
{
	FsmTable_t* pActTable = pFsm->FsmTable;
	void (*eventActFun)() = NULL;  //����ָ���ʼ��Ϊ��
	int NextState;
	int CurState = pFsm->curState;
	int flag = 0; //��ʶ�Ƿ���������

	/*��ȡ��ǰ��������*/
	for (int i = 0; i<g_max_num; i++)
	{
		//���ҽ�����ǰ״̬������ָ�����¼����Ҳ�ִ����
		if (event == pActTable[i].event && CurState == pActTable[i].CurState)
		{
			flag = 1;
			eventActFun = pActTable[i].eventActFun;
			NextState = pActTable[i].NextState;
			break;
		}
	}


	if (flag) //�������������
	{
		/*����ִ��*/
		if (eventActFun)
		{
			eventActFun();
		}

		//��ת����һ��״̬
		FSM_StateTransfer(pFsm, NextState);
	}
	else
	{
		// do nothing
	}
}





FsmTable_t TestDeviceTable[] =
{
	//{�������¼�����ǰ��״̬����ҪҪִ�еĺ�������һ��״̬}
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

//��ʼ��FSM
void InitFsm(FSM_t* pFsm)
{
	
	g_max_num = sizeof(TestDeviceTable) / sizeof(FsmTable_t);
	pFsm->curState = IDLE_STATE;
	FSM_Regist(pFsm, TestDeviceTable);
	
}


//��װ��¼����������
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
                         //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}


void TASK_KeyDeal(void)
{			
	TASK_KeyScan();	
	//��װѹ��
	if((KeyDown & KEY_START))
	
	{				
		KeyDown &= (!KEY_START);
		isKeyClicked &=  (~KEY_PNEUMATIC);				
		DataInit();
		event = KEY_START_PRESS;//��������				
		DisplayUpdataEnable = true;
	}
	
	//��װ̧�� 
	if((KeyUp & KEY_START))
	{
		
		KeyUp &= (!KEY_START);
		event = ERROR_EVENT;//ֻҪ�й�װ̧���¼�������������ʲô״̬������ERROR_EVENT�¼�		
	
		//printf("end!\r\n");
	}


	//***********************************
		//if((isKeyClicked & KEY_PNEUMATIC) && (IDCheckOk == true))
	  if(isKeyClicked & KEY_PNEUMATIC)
		{			
			isKeyClicked &=  (~KEY_PNEUMATIC);			
			if(IDCheckOk == true)
			{
				PNEUMATIC_VALVE=1;//��ѹ�� ͨ��			
				IDCheckOk = false;
				IDstartSendFlag = false;
				uartMode=0;
				uartModePin=0;
			}						
		}

    
	  if(isKeyClicked & KEY_SCANNING)//ɨ�밴ť������
		{
		    uartModePin=1; 
			uartMode=1;
			isKeyClicked &=  (~KEY_SCANNING);	   	   	    
			event = KEY_SCAN_TRIGGER;//������ά��ɨ���¼���ȥִ��Scan_QR_Codes_Function

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
		
//		if(MessageSetFlag==true)//�˳�����ģʽ
//		{
//			MessageSetFlag = false;
//			MessageOverTimeCnt = 0;
//			RTC_Set(calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec);
//			RTC_ITConfig(RTC_IT_SEC, ENABLE);	//ʹ��RTC���ж�		
//			
//			Flash_Buffer[0] = ID.Batch;
//			Flash_Buffer[1] = ID.ProcedureNum;
//			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Buffer,2);
//		}
    }
//***************************************************	
	if(isKeyPressLong & KEY_TOP)//����
	{
        isKeyPressLong &=  (~KEY_TOP);
        KeyDown &= (~KEY_TOP);		//��ֹ����������λ������־
		printf("KEY_TOP isKeyPressLong!\r\n");
		
		
//		MessageSetFlag = true;		//������������ģʽ
		
//		MessageOverTimeCnt = 0;
//		MessagePlace = MessagePlace_year;
//		RTC_ITConfig(RTC_IT_SEC, DISABLE);	//ʧ��RTC���ж�
		
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



void DataServe(void)//д��ID�����ݴ洢
{
    FilterIdCheck();
	FilterIdCheck();
	save();//���ݴ洢
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

		SendCMDtoScannDevice(); //����ɨ��ָ��
		do
		{
			delay_ms(500);
			ret =	ReceiveID();
			cnt--;
			
		}
		while (ret&&cnt);

	if (ret == 1) //ID����ʧ��
		event = ERROR_EVENT;
	
#endif

}
	
void Burning_Program_Function()
{
	// do something
	//printf("Burning_Program_Function!\r\n");
#if 1
	FilterIdCheck();
	if(UpdateFirmwareEnFlag==0)//��������¹̼�
	{
		if(FS_SerialReceiveBuff[CHANNEL_FS1]==0)	ProgKey1 = 1;//δ����ID��ͨ��������¼
		if(FS_SerialReceiveBuff[CHANNEL_FS2]==0)	ProgKey2 = 1;
		if(FS_SerialReceiveBuff[CHANNEL_FS3]==0)	ProgKey3 = 1;
		if(FS_SerialReceiveBuff[CHANNEL_FS4]==0)	ProgKey4 = 1;
	}
	else//������¹̼�
	{
		ProgKey1 = 1;//��¼����������
		ProgKey2 = 1;
		ProgKey3 = 1;
		ProgKey4 = 1;	
	}
	delay_ms(100);//��ʱ����
	ProgKey1 = 0;//��¼����������
	ProgKey2 = 0;
	ProgKey3 = 0;
	ProgKey4 = 0;	
	delay_ms(1000);//��ʱ�ȴ���¼����
	delay_ms(1200);//��72M������,nms<=1864 
	//printf("FilterIdCheck 2\r\n");
    delay_ms(1000);
	FilterIdCheck();//��¼���ٴμ��
	delay_ms(20);
	FilterIdCheck();//��¼���ٴμ��
	delay_ms(20);
	FilterIdCheck();//��¼���ٴμ��
	delay_ms(20);
   
	Filter_EnterModeCMD(CMD_WID);//д��ID
    delay_ms(20);
    Filter_EnterModeCMD(CMD_WID);//д��ID
	delay_ms(200);
	Filter_RecieveDataProcess(CHANNEL_FS1);
	Filter_RecieveDataProcess(CHANNEL_FS2);
	Filter_RecieveDataProcess(CHANNEL_FS3);
	Filter_RecieveDataProcess(CHANNEL_FS4);	
	
	event = BURNING_PROGRAM_OK; 
	DisplayUpdataEnable = true;//ʹ�����ݸ���	

#endif
	
}
	
void PID_Calibration_Function()
{
	// do something
	u8 ret;
	int cnt = 4;
	
	//printf("PID_Calibration_Function!\r\n");
#if 1
	uartMode=2;//���ڽ��ո�ΪPIDУ׼ģʽ

	
	do
	{
	
		PID_Calibration_Mode(PIDCALI_MODE);//PIDУ׼ģʽ
		delay_ms(4000);
		ret = PID_Set_Ok();
		cnt--;
	}while(ret&&cnt);

	PID_Calibration_Mode(ANSWER_MODE);	//ֹͣУ׼
	
	uartMode=0;//���ڽ��ո�ΪFS����ģʽ
	if (ret == 0)//PIDУ׼OK
		{
			event = PID_CALIBRATION_OK;	

		
		}
	else
		{//PIDУ׼ʧ��
		
			event = ERROR_EVENT; 

		}
	
  	DisplayUpdataEnable = true;//ʹ�����ݸ���
	

#endif
}
	
void Clean_Filter_Calibration_Function()
{
	// do something
	//printf("Clean_Filter_Calibration_Function!\r\n");

			u8 i;

			for(i=0;i<2;i++)
			{
				Filter_EnterModeCMD(CMD_RPID);//��PIDռ�ձ�
				delay_ms(50);
				Filter_RecieveDataProcess(CHANNEL_FS1);
				Filter_RecieveDataProcess(CHANNEL_FS2);
				Filter_RecieveDataProcess(CHANNEL_FS3);
				Filter_RecieveDataProcess(CHANNEL_FS4);
			}
				
			for(i=0;i<3;i++)
			{

				Filter_EnterModeCMD(CMD_RVV);//���ɾ�����TP2��ѹֵ
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
			DisplayUpdataEnable = true;//ʹ��LCD����	

}
	
void Dirty_Filter_Calibration_Function()
{
	// do something
	//printf("Dirty_Filter_Calibration_Function!\r\n");

			u8 i;						
			motor_drive(DIRTY_FILTER_POSITION,pulse.cleanFilter2_dirtyFilter);//����л���ȫ������λ��
			delay_ms(3000);


			for (i = 0; i < 3; i++)
			{
				Filter_EnterModeCMD(CMD_RVV);//��ȫ������TP2��ѹֵ
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

			//ȫ���ѹֵ �� ΢����ѹ
			FS[CHANNEL_FS1].MinVolt = FS[CHANNEL_FS1].MinVolt -MIN_caliVot;
			FS[CHANNEL_FS2].MinVolt = FS[CHANNEL_FS2].MinVolt -MIN_caliVot;
			FS[CHANNEL_FS3].MinVolt = FS[CHANNEL_FS3].MinVolt -MIN_caliVot;
			FS[CHANNEL_FS4].MinVolt = FS[CHANNEL_FS4].MinVolt -MIN_caliVot;
	
			
			Filter_EnterModeCMD(CMD_WCV);//д��У׼��ѹ
			delay_ms(50);
			Filter_EnterModeCMD(CMD_WCV);//д��У׼��ѹ
			delay_ms(50);
			Filter_RecieveDataProcess(CHANNEL_FS1);
			Filter_RecieveDataProcess(CHANNEL_FS2);
			Filter_RecieveDataProcess(CHANNEL_FS3);
			Filter_RecieveDataProcess(CHANNEL_FS4);

			event = DIRTY_FILTER_CALIBRATION_OK;
			DisplayUpdataEnable = true;//ʹ��LCD����

}

void Half_Filter_Test_Function()
{
	// do something
			u8 i;
	
			//printf("Half_Filter_Test_Function!\r\n");
			motor_drive(HALF_FILTER_POSITION,pulse.dirtyFilter2_halfFilter);//����л���50%����λ��
			TIM_Cmd(TIM4, ENABLE);//������ʱ������ȡPWM
			delay_ms(4000);		
						  											       
			for(i=0;i<5;i++)
			{
				delay_ms(300);
				Filter_EnterModeCMD(CMD_RBR);//�������ʣ���λ
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
	
			ReadVolt();//������ѹ���ԣ�����һ��DMA�ͽ���һ�ε�ѹת��
														
			for(i=0;i<2;i++)
			{
				Filter_EnterModeCMD(CMD_RVV);//����ѹMidVolt_tp2
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
				FS[CHANNEL_FS1].MidVolt_tp1 = FS[CHANNEL_FS1].Tp1Volt; //TP1��ѹ
				FS[CHANNEL_FS2].MidVolt_tp1 = FS[CHANNEL_FS2].Tp1Volt;
				FS[CHANNEL_FS3].MidVolt_tp1 = FS[CHANNEL_FS3].Tp1Volt;
				FS[CHANNEL_FS4].MidVolt_tp1 = FS[CHANNEL_FS4].Tp1Volt;
			}

			for(i=0;i<4;i++)
			{
				//pwmֵ
				FS[i].PwmDuty=(u8)(pwm_val_fs[i]/10);
			}

			for(i=0;i<2;i++)
			{
				//�¶�ֵ
				FS[CHANNEL_FS1].TempertureValueBuff[i] = 0; 
				FS[CHANNEL_FS2].TempertureValueBuff[i] = 0; 
				FS[CHANNEL_FS3].TempertureValueBuff[i] = 0; 
				FS[CHANNEL_FS4].TempertureValueBuff[i] = 0; 
			}
		
	
	        //PNEUMATIC_VALVE=0;//��ѹ�� ��	
			TIM_Cmd(TIM4, DISABLE);//�ر�pwm��ʱ��
			DataServe();//���ݴ洢
			event = HALF_FILTER_TEST_OK;	//���Խ���
			DisplayUpdataEnable = true;//ʹ�����ݸ���


}
//��ֹ����
void Termination_Test_Function()
{

	//printf("Termination_Test_Function!\r\n");
	buzzerRingNum(2);//����������	
	PNEUMATIC_VALVE=0;//��ѹ�� ��	
	motor_drive(CLEAN_FILTER_POSITION,pulse.halfFilter2_cleanFilter);//���ع�ɾ�����λ��
	event = IDLE_EVENT;	//���Խ�������Ϊ�����¼�
	DisplayUpdataEnable = true;//ʹ�����ݸ���

}

void Idle_State_Function()
{
	// do something
	//printf("Idle_State_Function!\r\n");
#if 0
					
	if(MotorPosition.pulse_count!=MotorPosition.pulseToStand1)//������δ��λ
	{
		MotorOff();
		TIM_Cmd(TIM1,DISABLE);       
		MotorCaliFlag = CALI2;//1:���У׼ģʽ
	}	
	//DisplayUpdataEnable = true;	
	MotorOff();	
#endif

}
	






