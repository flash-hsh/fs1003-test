#ifndef _PROTOCOL_FS_H
#define _PROTOCOL_FS_H
#include "sys.h"	 

#define NUM_CHANNELS  4


//ɨ��ǹ
#define ADDRESS_SCANNING 		0x0C  //ɨ��ǹ�ĵ�ַ
#define SCANNING_DATA_LTH       0x06 //ɨ��ǹ���ݳ���

//��λ��
#define ADDRESS_PC    	0x0B  //PC��λ���ĵ�ַ
#define PC_RD           0x01 //PC�� Read Data				   ��PC��������		
#define PC_CHAL         0x00 //��ʾ��ͬ�Ĵ�����ͨ����0x00 ����㲥ģʽ�����ͻ��߷������д��������ݣ� 0x01~0x04����ͨ��ģʽ����Ӧ�Ĵ�����ͨ����ֻ���ͻ��߷�����Ӧ�Ĵ��������ݣ�	
#define PC_CONSTA       0x00 //��ʾ��������״̬���ڵ�ͨ��ģʽ�£�0x01����δ���ӻ����쳣״̬��0x00����״̬���ã� �ڹ㲥ģʽ�£�һ���ֽڴ���8�����������ݣ����ҵ���ֱ�ÿ1bit����1��������״̬,0 ��������״̬���ã� 1 ����δ���ӻ����쳣״̬����������ֵ0x0D ���� 2�Ŵ������쳣��			
#define PC_DATA_LTH     0x80 //���ݳ��ȣ���ʾ���ֽ�֮��Ĳ����ֵĳ��ȣ�������֡ͷ�������֣�ͨ���ţ�״̬�����ݳ����Լ�CRCУ��λ�ĳ���	





#define WORKER_NUM_H    0x25 //Ա����� 9527
#define WORKER_NUM_L    0x37

#define testSTART_Flag  0x00 //��ʼ���Ա�־		
#define testEND_Flag    0xCE //�������־	

//FS������
#define ADDRESS_SLAVE 		0x0A  //FS�������ĵ�ַ
#define CMD_PID  0X00 //PID����            				PIDУ׼��Ӧ�𣬵���ģʽ			4
#define CMD_RBR  0x01 //Read Blocking Rate				��������			7
#define CMD_RVV  0x02 //Read Voltage Value				��ȡ��ѹ			7
#define CMD_RTV  0x03 //Read Temperture Value			��ȡ�¶�			9
#define CMD_WCV  0x04 //Write Calibration Voltage		дУ׼��ѹ		9
#define CMD_WID  0x05 //Write ID						дID			11
#define CMD_RID  0x06 //Read ID							��ȡID			11
#define CMD_RCV  0x07 //Read Calibration Voltage		��ȡУ׼��ѹ		9
#define CMD_WUC  0x08 //Write User Calibration Voltage	д�û�У׼��ѹ	7
#define CMD_RUC  0x09 //Read User Calibration Voltage	���û�У׼��ѹ	7
#define CMD_RSUC  0x0A //Reset User Calibration Voltage	�����û�У׼��ѹ	7
#define CMD_RPID  0x0B  //Read PID Duty                 ��ȡPWM����/PID




#define RBRIDE		2
#define RVVIDE		2
#define RTVIDE		4
#define RIDIDE		6
#define RCVIDE		4
#define RPIDIDE		4
#define CDWIDE_MAX    25      //Dust recieve buff size

#define CHANNEL_FS1 0
#define CHANNEL_FS2 1
#define CHANNEL_FS3 2
#define CHANNEL_FS4 3

//PID����ģʽ
#define DEBUG_MODE  	0x00
#define PIDCALI_MODE	0x01
#define ANSWER_MODE		0x02




#define START 0X00
#define FS1   0X01
#define FS2   0X02
#define FS3   0X03
#define FS4   0X04


//����
#define UART_FS1 UART4
#define UART_FS2 USART6
#define UART_FS3 USART2
#define UART_FS4 USART3


typedef struct{
	u8 BlockingRateBuff[RBRIDE];//����
	u8 VoltageValueBuff[RVVIDE]; //��TP2��ѹֵ�������
	u8 TempertureValueBuff[RTVIDE];//�ⲿ�¶� //�ڲ��¶�	
	u8 CalibrationVoltageBuff[RCVIDE];//��ȡ��У׼��ѹֵ
	u8 RecieveBuff[CDWIDE_MAX*2];//���ݽ��ճ�
	u8 RecieveIdBuff[RIDIDE];					//�洢��ȡ����ID��Ϣ���� �� ���� ���кŸ� ���кŵ� �����
	u8 IDWriteBuff[RIDIDE];  //��ɨ��ǹ���յ�ID���ݣ�д��FS
    u8 ErrorTimes;
	u16  SUMCRC;  
	u16  RecieveNum;   
	u16  Tp1Volt;		//ʵʱ��ѹ
	u16  Tp2Volt;		//ʵʱ��ѹ
	u16  V33Volt;		//ʵʱ��ѹ
	u16  V50Volt;		//ʵʱ��ѹ
    u16  PidDuty;		//��ȡ����PIDռ�ձ�
	u8   PwmDuty;   	//��PWM���Ե��ȡ�����Ե����PW��50%��������ռ�ձ������PWM���     ��Χ��0.0%-100.0%
	u16  MinVolt;   	//��FS��ȡ��У׼�͵��ѹ��ȫ������TP2��ѹֵ   ��Χ��  0.100V-0.700V    
	u16  MaxVolt;   	//��FS��ȡ��У׼�ߵ��ѹ���ɾ�����TP2��ѹֵ   ��Χ�� 1.000V-3.000V
    u16  MidVolt_tp2; 	//��FS��ȡ�����Ե��ѹ��50%������TP2��ѹֵ   ��Χ�� 1.000V-3.000V
    u16  MidVolt_tp1; 	//��TP1���Ե��ȡ�����Ե��ѹ��50%������TP1��ѹֵ   ��Χ�� 1.900V-2.300V
	u16  BlockingRate;	//��PWM���Ե��ȡ�����Ե����PW��50%��������ռ�ձ������PWM���     ��Χ��0.0%-100.0%
	bool ChannelConnect;
	bool JudgeFlag;		//������¼У׼����ж� true:OK	false:NG
}DATA_FS;


#if 1

#define TEST_FS1003A_V06  0 	//1:ʹ�ܲ���
#define TEST_FS1003A_V07  1		//1:ʹ�ܲ��ԣ�V06��V07ֻ��ѡ��һ���汾����

#if TEST_FS1003A_V06

#define VREF			3010 	//ADC�ο���ѹ
#define MIN_V33			3130		//3.23V
#define MAX_V33			3500	//3.36V		
//#define MinTp1			0		//0V
#define MAX_TP1			1900		//0.03V		
#define MIN_TP2			200		//0.4V
#define MAX_TP2			1900		//1.9V

#define MIN_V_MAX			1400		//1.4V  �ɾ�ʱ����Сֵ
#define MAX_V_MAX			1900		//1.9V	�ɾ�ʱ�����ֵ
#define MIN_V_MIN			200		//0.2V  ����ʱ����Сֵ
#define MAX_V_MIN			700		//0.7V  ����ʱ�����ֵ	

#define MAX_BlockingRate  58  //50%�����Ķ�����
#define MIN_BlockingRate  52  //50%�����Ķ�����
#endif

#if TEST_FS1003A_V07

#define VREF			3010 	//ADC�ο���ѹ
#define MIN_V33			3230		//3.23V
#define MAX_V33			3360		//3.36V		
//#define MinTp1			0		//0V
#define MAX_TP1			3100		//0.03V		
#define MIN_TP2			200		//0.4V
#define MAX_TP2			2990		//1.9V

#define MIN_V_MAX			2700		//1.0V  �ɾ�ʱ����Сֵ
#define MAX_V_MAX			2850		//1.9V	�ɾ�ʱ�����ֵ
#define MIN_V_MIN			200		//0.2V  ����ʱ����Сֵ
#define MAX_V_MIN			1000		//0.7V  ����ʱ�����ֵ		

#define MAX_BlockingRate  58  //50%�����Ķ�����
#define MIN_BlockingRate  52  //50%�����Ķ�����
#endif

#endif


extern DATA_FS FS[];
extern u16 FS_SerialBuff[NUM_CHANNELS];
extern u16 FS_SerialReceiveBuff[NUM_CHANNELS];
extern u8  burning_check_ok;
extern u8 idReceive2ScanningBuff[20];
extern vu8 uartMode;
extern bool IDstartSendFlag;  
extern bool IDCheckOk;

/*----------------------------------------------------------------------------
 *        Functiong declaration
 *----------------------------------------------------------------------------*/

void PID_Calibration_Mode(u8 Function);
u8 PID_Set_Ok(void);

void Filter_EnterModeCMD(u8 Function);
void Filter_RecieveDataProcess(u8 ChannelNum);

void SendCMDtoScannDevice(void);
u8 ReceiveID(void);
u8 IDBuff_cmp(u8 *a,u8 *b,u8 *c,u8 *d);


void Filter_WCV(void);
void Filter_WID(void);
void FilterIdCheck(void);
void Filter_DataInit(void);
void UartSendData_toPC(void);

#endif
