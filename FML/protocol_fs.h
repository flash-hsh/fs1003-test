#ifndef _PROTOCOL_FS_H
#define _PROTOCOL_FS_H
#include "sys.h"	 

#define NUM_CHANNELS  4


//扫码枪
#define ADDRESS_SCANNING 		0x0C  //扫码枪的地址
#define SCANNING_DATA_LTH       0x06 //扫码枪数据长度

//上位机
#define ADDRESS_PC    	0x0B  //PC上位机的地址
#define PC_RD           0x01 //PC机 Read Data				   向PC发送数据		
#define PC_CHAL         0x00 //表示不同的传感器通道，0x00 代表广播模式，发送或者返回所有传感器数据； 0x01~0x04代表单通道模式下相应的传感器通道，只发送或者返回相应的传感器数据；	
#define PC_CONSTA       0x00 //表示传感器的状态。在单通道模式下，0x01代表未连接或者异常状态；0x00代表状态良好； 在广播模式下，一个字节代表8个二进制数据，从右到左分别每1bit代表1个传感器状态,0 代表传感器状态良好， 1 代表未连接或者异常状态；例：返回值0x0D 代表 2号传感器异常。			
#define PC_DATA_LTH     0x80 //数据长度：表示此字节之后的参数字的长度，不包括帧头、功能字，通道号，状态，数据长度以及CRC校验位的长度	





#define WORKER_NUM_H    0x25 //员工编号 9527
#define WORKER_NUM_L    0x37

#define testSTART_Flag  0x00 //开始测试标志		
#define testEND_Flag    0xCE //测试完标志	

//FS传感器
#define ADDRESS_SLAVE 		0x0A  //FS传感器的地址
#define CMD_PID  0X00 //PID调节            				PID校准，应答，调试模式			4
#define CMD_RBR  0x01 //Read Blocking Rate				读堵塞率			7
#define CMD_RVV  0x02 //Read Voltage Value				读取电压			7
#define CMD_RTV  0x03 //Read Temperture Value			读取温度			9
#define CMD_WCV  0x04 //Write Calibration Voltage		写校准电压		9
#define CMD_WID  0x05 //Write ID						写ID			11
#define CMD_RID  0x06 //Read ID							读取ID			11
#define CMD_RCV  0x07 //Read Calibration Voltage		读取校准电压		9
#define CMD_WUC  0x08 //Write User Calibration Voltage	写用户校准电压	7
#define CMD_RUC  0x09 //Read User Calibration Voltage	读用户校准电压	7
#define CMD_RSUC  0x0A //Reset User Calibration Voltage	重置用户校准电压	7
#define CMD_RPID  0x0B  //Read PID Duty                 读取PWM脉宽/PID




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

//PID调节模式
#define DEBUG_MODE  	0x00
#define PIDCALI_MODE	0x01
#define ANSWER_MODE		0x02




#define START 0X00
#define FS1   0X01
#define FS2   0X02
#define FS3   0X03
#define FS4   0X04


//串口
#define UART_FS1 UART4
#define UART_FS2 USART6
#define UART_FS3 USART2
#define UART_FS4 USART3


typedef struct{
	u8 BlockingRateBuff[RBRIDE];//堵率
	u8 VoltageValueBuff[RVVIDE]; //读TP2电压值，缓存池
	u8 TempertureValueBuff[RTVIDE];//外部温度 //内部温度	
	u8 CalibrationVoltageBuff[RCVIDE];//读取的校准电压值
	u8 RecieveBuff[CDWIDE_MAX*2];//数据接收池
	u8 RecieveIdBuff[RIDIDE];					//存储读取到的ID信息：年 周 批次 序列号高 序列号低 工序号
	u8 IDWriteBuff[RIDIDE];  //将扫码枪接收的ID数据，写到FS
    u8 ErrorTimes;
	u16  SUMCRC;  
	u16  RecieveNum;   
	u16  Tp1Volt;		//实时电压
	u16  Tp2Volt;		//实时电压
	u16  V33Volt;		//实时电压
	u16  V50Volt;		//实时电压
    u16  PidDuty;		//读取到的PID占空比
	u8   PwmDuty;   	//从PWM测试点读取，测试点比例PW—50%脏滤网的占空比输出，PWM输出     范围：0.0%-100.0%
	u16  MinVolt;   	//从FS读取，校准低点电压—全脏滤网TP2电压值   范围：  0.100V-0.700V    
	u16  MaxVolt;   	//从FS读取，校准高点电压—干净滤网TP2电压值   范围： 1.000V-3.000V
    u16  MidVolt_tp2; 	//从FS读取，测试点电压—50%脏滤网TP2电压值   范围： 1.000V-3.000V
    u16  MidVolt_tp1; 	//从TP1测试点读取，测试点电压—50%脏滤网TP1电压值   范围： 1.900V-2.300V
	u16  BlockingRate;	//从PWM测试点读取，测试点比例PW—50%脏滤网的占空比输出，PWM输出     范围：0.0%-100.0%
	bool ChannelConnect;
	bool JudgeFlag;		//测试烧录校准结果判定 true:OK	false:NG
}DATA_FS;


#if 1

#define TEST_FS1003A_V06  0 	//1:使能测试
#define TEST_FS1003A_V07  1		//1:使能测试，V06和V07只能选择一个版本测试

#if TEST_FS1003A_V06

#define VREF			3010 	//ADC参考电压
#define MIN_V33			3130		//3.23V
#define MAX_V33			3500	//3.36V		
//#define MinTp1			0		//0V
#define MAX_TP1			1900		//0.03V		
#define MIN_TP2			200		//0.4V
#define MAX_TP2			1900		//1.9V

#define MIN_V_MAX			1400		//1.4V  干净时的最小值
#define MAX_V_MAX			1900		//1.9V	干净时的最大值
#define MIN_V_MIN			200		//0.2V  脏污时的最小值
#define MAX_V_MIN			700		//0.7V  脏污时的最大值	

#define MAX_BlockingRate  58  //50%滤网的堵塞率
#define MIN_BlockingRate  52  //50%滤网的堵塞率
#endif

#if TEST_FS1003A_V07

#define VREF			3010 	//ADC参考电压
#define MIN_V33			3230		//3.23V
#define MAX_V33			3360		//3.36V		
//#define MinTp1			0		//0V
#define MAX_TP1			3100		//0.03V		
#define MIN_TP2			200		//0.4V
#define MAX_TP2			2990		//1.9V

#define MIN_V_MAX			2700		//1.0V  干净时的最小值
#define MAX_V_MAX			2850		//1.9V	干净时的最大值
#define MIN_V_MIN			200		//0.2V  脏污时的最小值
#define MAX_V_MIN			1000		//0.7V  脏污时的最大值		

#define MAX_BlockingRate  58  //50%滤网的堵塞率
#define MIN_BlockingRate  52  //50%滤网的堵塞率
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
