//传感器协议处理
#include "protocol_fs.h"
#include "usart.h"
#include "CRC.h"
#include "rtc.h" 	
#include "delay.h"
#include "message.h"
#include "serial.h"
#include <asc2bcd.h>

DATA_FS FS[NUM_CHANNELS];
//u8  burning_check_ok=0;
u8 idReceive2ScanningBuff[20];

//上位机通讯

u8  burning_check_ok=0;
u8  UartSendData_PC[200]={0x0B, 0x01, 0x00, 0x00, 0x80, 0x05, 0x04, 0x00, 0x07, 0xE2, 0x01, 0x19, 0x08, 0x01, 0x00, 0x12, 0x01, 0x01, 0x00, 0x01, 0x1F, 0x12, 0x01, 0x01, 0x00, 0x02, 0x1F, 0x12, 0x01, 0x01, 0x00, 0x03, 0x1F, 0x12, 0x01, 0x01, 0x00, 0x04, 0x1F, 0x00, 0x01, 0x12, 0x01, 0x01, 0x00, 0x01, 0x1F, 0x01, 0xF4, 0x01, 0x4A, 0x00, 0x1E, 0x00, 0x96, 0x00, 0x5A, 0x00, 0x01, 0x32, 0x32, 0x00, 0x00, 0x02, 0x12, 0x01, 0x01, 0x00, 0x02, 0x1F, 0x01, 0xF4, 0x01, 0x4A, 0x00, 0x1E, 0x00, 0x96, 0x00, 0x5A, 0x00, 0x01, 0x32, 0x32, 0x00, 0x00, 0x03, 0x12, 0x01, 0x01, 0x00, 0x03, 0x1F, 0x01, 0xF4, 0x01, 0x4A, 0x00, 0x1E, 0x00, 0x96, 0x00, 0x5A, 0x00, 0x01, 0x32, 0x32, 0x00, 0x00, 0x04, 0x12, 0x01, 0x01, 0x00, 0x04, 0x1F, 0x01, 0xF4, 0x01, 0x4A, 0x00, 0x1E, 0x00, 0x96, 0x00, 0x5A, 0x00, 0x01, 0x32, 0x32, 0x00, 0x00, 0xCE, 0x3D, 0x30};	

//串口选通模式：0:FS模式  1：扫码模式  2:PID校准模式
vu8 uartMode=0;
	
bool IDstartSendFlag;
bool IDCheckOk;
u16 FS_SerialBuff[NUM_CHANNELS] = {1,2,3,4};//要分配的产品序列号
u16 FS_SerialReceiveBuff[NUM_CHANNELS] = {0,0,0,0};//接收到的产品序列号

/*******************************************************************************
* Function : USART2_IRQHandler(void) 
* brief    : Recieve dust data and deal with 
* param    : None
* retval   : None
*******************************************************************************/
void USART2_IRQHandler(void)   //FS3   
{
	if(USART_GetITStatus(UART_FS3, USART_IT_RXNE) != RESET)	  
	{	
		switch (uartMode)
		{	
			case 0 :			
				FS[CHANNEL_FS3].RecieveBuff[FS[CHANNEL_FS3].RecieveNum++] = USART_ReceiveData(UART_FS3);		
				if(FS[CHANNEL_FS3].RecieveBuff[0] != ADDRESS_SLAVE)
				{
					FS[CHANNEL_FS3].RecieveNum = 0;			
				}
			break;
		
			case 1 :	
				FS[CHANNEL_FS3].RecieveBuff[FS[CHANNEL_FS3].RecieveNum++] = USART_ReceiveData(UART_FS3);		
				if(FS[CHANNEL_FS3].RecieveBuff[0] != ADDRESS_SCANNING)
				{
					FS[CHANNEL_FS3].RecieveNum = 0;			
				}
				
			break;

			case 2 :	
				FS[CHANNEL_FS3].RecieveBuff[FS[CHANNEL_FS3].RecieveNum++] = USART_ReceiveData(UART_FS3);		
				
			   if(FS[CHANNEL_FS3].RecieveBuff[0] != 0x4D )
				{
					FS[CHANNEL_FS3].RecieveNum = 0;			
				}
				
			break;
				
			default: break;
		
		}
		//Clear ReceiveFlag
		USART_ClearITPendingBit(UART_FS3, USART_IT_RXNE);	
	}	
}
/*******************************************************************************
* Function : USART2_IRQHandler(void) 
* brief    : Recieve dust data and deal with 
* param    : None
* retval   : None
*******************************************************************************/
void USART3_IRQHandler(void)   //FS4  
{
	if(USART_GetITStatus(UART_FS4, USART_IT_RXNE) != RESET)	  
	{		
		
		
		switch (uartMode)
		{	
			case 0 :			
				FS[CHANNEL_FS4].RecieveBuff[FS[CHANNEL_FS4].RecieveNum++] = USART_ReceiveData(UART_FS4);		
				if(FS[CHANNEL_FS4].RecieveBuff[0] != ADDRESS_SLAVE)
				{
					FS[CHANNEL_FS4].RecieveNum = 0;			
				}
			break;
		
			case 1 :	
				FS[CHANNEL_FS4].RecieveBuff[FS[CHANNEL_FS4].RecieveNum++] = USART_ReceiveData(UART_FS4);		
				if(FS[CHANNEL_FS4].RecieveBuff[0] != ADDRESS_SCANNING)
				{
					FS[CHANNEL_FS4].RecieveNum = 0;			
				}			
			break;

			case 2 :	
				FS[CHANNEL_FS4].RecieveBuff[FS[CHANNEL_FS4].RecieveNum++] = USART_ReceiveData(UART_FS4);		
				if(FS[CHANNEL_FS4].RecieveBuff[0] != 0x4D )
				{
					FS[CHANNEL_FS4].RecieveNum = 0;			
				}			
			break;
				
			default: break;
		
		}
		
		
		//Clear ReceiveFlag
		USART_ClearITPendingBit(UART_FS4, USART_IT_RXNE);	
	}	
}

/*******************************************************************************
* Function : USART2_IRQHandler(void) 
* brief    : Recieve dust data and deal with 
* param    : None
* retval   : None
*******************************************************************************/
void UART4_IRQHandler(void)  //FS1    
{
	    
    if(USART_GetITStatus(UART_FS1, USART_IT_RXNE) != RESET)	  
	{		
		
        
		
		switch (uartMode)
		{	
			case 0 :			
				FS[CHANNEL_FS1].RecieveBuff[FS[CHANNEL_FS1].RecieveNum++] = USART_ReceiveData(UART_FS1);				
				if((FS[CHANNEL_FS1].RecieveBuff[0] != ADDRESS_SLAVE) )
				{
					FS[CHANNEL_FS1].RecieveNum = 0;								  
				}
			break;
		
			case 1 :			
				FS[CHANNEL_FS1].RecieveBuff[FS[CHANNEL_FS1].RecieveNum++] = USART_ReceiveData(UART_FS1);				
				if((FS[CHANNEL_FS1].RecieveBuff[0] != ADDRESS_SCANNING) )
				{
					FS[CHANNEL_FS1].RecieveNum = 0;								  
				}
			break;

			case 2 :			
				FS[CHANNEL_FS1].RecieveBuff[FS[CHANNEL_FS1].RecieveNum++] = USART_ReceiveData(UART_FS1);				
				if((FS[CHANNEL_FS1].RecieveBuff[0] != 0x4D))
				{
					FS[CHANNEL_FS1].RecieveNum = 0;								  
				}
			break;

				
			default: break;
		
		}
		
        
		//Clear ReceiveFlag
		USART_ClearITPendingBit(UART_FS1, USART_IT_RXNE);	
	}	
}
/*******************************************************************************
* Function : USART2_IRQHandler(void) 
* brief    : Recieve dust data and deal with 
* param    : None
* retval   : None
*******************************************************************************/
void USART6_IRQHandler(void)    //FS2  
{
	if(USART_GetITStatus(UART_FS2, USART_IT_RXNE) != RESET)	  
	{		
		
		
		switch (uartMode)
		{	
			case 0 :			
				FS[CHANNEL_FS2].RecieveBuff[FS[CHANNEL_FS2].RecieveNum++] = USART_ReceiveData(UART_FS2);		
				if(FS[CHANNEL_FS2].RecieveBuff[0] != ADDRESS_SLAVE)
				{
					FS[CHANNEL_FS2].RecieveNum = 0;			
				}
			break;
		
			case 1 :			
				FS[CHANNEL_FS2].RecieveBuff[FS[CHANNEL_FS2].RecieveNum++] = USART_ReceiveData(UART_FS2);		
				if(FS[CHANNEL_FS2].RecieveBuff[0] != ADDRESS_SCANNING)
				{
					FS[CHANNEL_FS2].RecieveNum = 0;			
				}
			break;

			case 2 :			
				FS[CHANNEL_FS2].RecieveBuff[FS[CHANNEL_FS2].RecieveNum++] = USART_ReceiveData(UART_FS2);		
				
			    if(FS[CHANNEL_FS2].RecieveBuff[0] != 0x4D)
				{
					FS[CHANNEL_FS2].RecieveNum = 0;			
				}
			break;
				
			default: break;
		
		}
		
		
		//Clear ReceiveFlag
		USART_ClearITPendingBit(UART_FS2, USART_IT_RXNE);	
	}	
}
/*******************************************************************************
* Function : Filter_EnterModeCMD(u8 Mode)
* brief    : The CMD of Entering different mode
* param    : @Mode:Mode Selection, 0:Operation Mode 1:Engineer Mode
* retval   : Dust sensors connection status, 0x00:All the Sensors connect (A bit represents a sensor) 0xFF:Unconnected 
*******************************************************************************/
void Filter_EnterModeCMD(u8 Function)
{
	u8 i=0x00;
	u8 CMDWIDE = 0x09;
	u8 SENDBUFF[CDWIDE_MAX] = {ADDRESS_SLAVE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	switch(Function)
	{
				
		//FS Read Blocking Rate
		case CMD_RBR: CMDWIDE = 0x07; 
				SENDBUFF[1] = CMD_RBR;	 
		break;

		//FS Read Voltage Value
		case CMD_RVV: CMDWIDE = 0x07; 
				SENDBUFF[1] = CMD_RVV;
		break; 

		//FS Read Temperture Value	 									
		case CMD_RTV: CMDWIDE = 0x09; 
			SENDBUFF[1] = CMD_RTV;	
		break;

		//FS Write Calibration Voltage	
		case CMD_WCV: CMDWIDE = 0x09;  
			Filter_WCV();
		break;

		//FS Write ID
		case CMD_WID: CMDWIDE = 0x0B;  	
			Filter_WID();

		break;

		//FS Read ID									
		case CMD_RID: CMDWIDE = 0x0B;  
			SENDBUFF[1] = CMD_RID;	
		break;
		
		//FS Read Calibration Voltage									
		case CMD_RCV: CMDWIDE = 0x09;  
			SENDBUFF[1] = CMD_RCV;	
		break;
		
		case CMD_RPID: CMDWIDE = 0x07;  
			SENDBUFF[1] = CMD_RPID;	
		break;
		
		default:break;
	}
	if((Function==CMD_WCV)||(Function==CMD_WID))return;
	
	SENDBUFF[2] = CMDWIDE - 3;
	SENDBUFF[CMDWIDE-2] = (u8)(CRC16(SENDBUFF, CMDWIDE-2) >> 8);	
	SENDBUFF[CMDWIDE-1] = (u8)(CRC16(SENDBUFF, CMDWIDE-2));
	
	//printf("\r\nFilter_EnterModeCMD %d :",Function);
	for(i=0; i<CMDWIDE; i++)
	{
		USART_SendChar(UART_FS1,SENDBUFF[i]);
		USART_SendChar(UART_FS2,SENDBUFF[i]);
		USART_SendChar(UART_FS3,SENDBUFF[i]);
		USART_SendChar(UART_FS4,SENDBUFF[i]);
		//printf("%d ",SENDBUFF[i]);
	}
}
/*******************************************************************************
* Function : DUST_RecieveDataProcess(u8 ChannelNum,u8 Data)	
* brief    : When Recieving Dust Sensors Data,Deal with data 
* param    : @ChannelNum:Dust Channel Number On A Board(8 Channels)
						 @Data: The Recieved data will be deald with 
* retval   : None
*******************************************************************************/
void Filter_RecieveDataProcess(u8 ChannelNum)
{
	u8 CMDLength = 0, i = 0;
	CMDLength = FS[ChannelNum].RecieveBuff[2]+3;
	FS[ChannelNum].SUMCRC = CRC16(FS[ChannelNum].RecieveBuff, CMDLength-2);
	if((FS[ChannelNum].SUMCRC == (FS[ChannelNum].RecieveBuff[CMDLength-2]<<8) + FS[ChannelNum].RecieveBuff[CMDLength-1])
		&& (FS[ChannelNum].RecieveBuff[0] == ADDRESS_SLAVE))
	{
		//According to the functiong,Choose the way to deal with data 
		switch(FS[ChannelNum].RecieveBuff[1])
		{									
			case CMD_RBR:   
							FS[ChannelNum].ChannelConnect = true;
							for(i=0; i<RBRIDE; i++)
							{
								FS[ChannelNum].BlockingRateBuff[i] = FS[ChannelNum].RecieveBuff[i+3];
							}										
			break;
										
			case CMD_RVV:   
							FS[ChannelNum].ChannelConnect = true;		
							for(i=0; i<RVVIDE; i++)
							{
								FS[ChannelNum].VoltageValueBuff[i] = FS[ChannelNum].RecieveBuff[i+3];
							}			
			break;
										
			case CMD_RTV:  
							FS[ChannelNum].ChannelConnect = true;
							for(i=0; i<RTVIDE; i++) 
							{
								FS[ChannelNum].TempertureValueBuff[i] = FS[ChannelNum].RecieveBuff[i+3];
							}							  
			break;
										
			case CMD_RCV: 	
							FS[ChannelNum].ChannelConnect = true;						
							for(i=0; i<RCVIDE; i++) 
							{
								FS[ChannelNum].CalibrationVoltageBuff[i] = FS[ChannelNum].RecieveBuff[i+3];
							}	

                            //printf("\r\nfunction %d:DUST[%d].RecieveIdBuff[]:",FS[ChannelNum].RecieveBuff[1],ChannelNum);
//							for(i=0;i<RCVIDE;i++)
//							{
//								printf("FS[%d].CalibrationVoltageBuff[i]=%02X \r\n",ChannelNum,FS[ChannelNum].CalibrationVoltageBuff[i]);
//							}
							//printf("\r\n");
			break;
			
			case CMD_RID: 	
							FS[ChannelNum].ChannelConnect = true;				
							for(i=0; i<RIDIDE; i++)
							{
								FS[ChannelNum].RecieveIdBuff[i] = FS[ChannelNum].RecieveBuff[i+3];
							}	
							FS_SerialReceiveBuff[ChannelNum] = FS[ChannelNum].RecieveIdBuff[3];
							FS_SerialReceiveBuff[ChannelNum] <<= 8;
							FS_SerialReceiveBuff[ChannelNum] |= FS[ChannelNum].RecieveIdBuff[4];
							
//							printf("\r\nFS[%d].RecieveIdBuff[]:",ChannelNum);
//							for(i=0;i<RIDIDE;i++)
//							{
//								printf("%02X ",FS[ChannelNum].RecieveIdBuff[i]);
//							}
//							printf("\r\n");
			break;

			case CMD_RPID:
							FS[ChannelNum].ChannelConnect = true;
							FS[ChannelNum].PidDuty= FS[ChannelNum].RecieveBuff[3] * 256 + \
							FS[ChannelNum].RecieveBuff[4];

			break;
							
			default:break;
		}
	}
	/*After recieving  a CMD, Clear index of current dust buff*/
	FS[ChannelNum].RecieveNum = 0;
	FS[ChannelNum].SUMCRC = 0;	
}
//*******************************************************************************
void Filter_WCV(void)//FS Write Calibration Voltage	
{
	u8 i;
	u8 SENDBUFF[9] = {ADDRESS_SLAVE,CMD_WCV,0x06,0,0,0,0,0,0};
	
	SENDBUFF[3] = (u8)((u16)(FS[CHANNEL_FS1].MaxVolt)>>8);
	SENDBUFF[4] = (u8)((u16)(FS[CHANNEL_FS1].MaxVolt));
	SENDBUFF[5] = (u8)((u16)(FS[CHANNEL_FS1].MinVolt)>>8);
	SENDBUFF[6] = (u8)((u16)(FS[CHANNEL_FS1].MinVolt));
	SENDBUFF[7] = (u8)(CRC16(SENDBUFF, 7) >> 8);	
	SENDBUFF[8] = (u8)(CRC16(SENDBUFF, 7));
	for(i=0; i<9; i++)
	{
		USART_SendChar(UART_FS1,SENDBUFF[i]);
	}
	
	SENDBUFF[3] = (u8)((u16)(FS[CHANNEL_FS2].MaxVolt)>>8);
	SENDBUFF[4] = (u8)((u16)(FS[CHANNEL_FS2].MaxVolt));
	SENDBUFF[5] = (u8)((u16)(FS[CHANNEL_FS2].MinVolt)>>8);
	SENDBUFF[6] = (u8)((u16)(FS[CHANNEL_FS2].MinVolt));
	SENDBUFF[7] = (u8)(CRC16(SENDBUFF, 7) >> 8);	
	SENDBUFF[8] = (u8)(CRC16(SENDBUFF, 7));
	for(i=0; i<9; i++)
	{
		USART_SendChar(UART_FS2,SENDBUFF[i]);
	}
	
	SENDBUFF[3] = (u8)((u16)(FS[CHANNEL_FS3].MaxVolt)>>8);
	SENDBUFF[4] = (u8)((u16)(FS[CHANNEL_FS3].MaxVolt));
	SENDBUFF[5] = (u8)((u16)(FS[CHANNEL_FS3].MinVolt)>>8);
	SENDBUFF[6] = (u8)((u16)(FS[CHANNEL_FS3].MinVolt));
	SENDBUFF[7] = (u8)(CRC16(SENDBUFF, 7) >> 8);	
	SENDBUFF[8] = (u8)(CRC16(SENDBUFF, 7));
	for(i=0; i<9; i++)
	{
		USART_SendChar(UART_FS3,SENDBUFF[i]);
	}
	
	SENDBUFF[3] = (u8)((u16)(FS[CHANNEL_FS4].MaxVolt)>>8);
	SENDBUFF[4] = (u8)((u16)(FS[CHANNEL_FS4].MaxVolt));
	SENDBUFF[5] = (u8)((u16)(FS[CHANNEL_FS4].MinVolt)>>8);
	SENDBUFF[6] = (u8)((u16)(FS[CHANNEL_FS4].MinVolt));
	SENDBUFF[7] = (u8)(CRC16(SENDBUFF, 7) >> 8);	
	SENDBUFF[8] = (u8)(CRC16(SENDBUFF, 7));
	for(i=0; i<9; i++)
	{
		USART_SendChar(UART_FS4,SENDBUFF[i]);
	}
}


void Filter_WID(void)//FS Write ID
{
	u8 channel;
	u8 i;
	u8 SENDBUFF[11] = {ADDRESS_SLAVE,CMD_WID,0x08,0,0,0,0,0,0,0,0};//年 周 批次 序列号高 序列号低 工序号

	for(channel=0;channel<NUM_CHANNELS;channel++)
	{
		
 
        SENDBUFF[3] = (u8)FS[channel].IDWriteBuff[0];
        SENDBUFF[4] = (u8)FS[channel].IDWriteBuff[1];
        SENDBUFF[5] = (u8)FS[channel].IDWriteBuff[2];
        SENDBUFF[6] = (u8)FS[channel].IDWriteBuff[3];
        SENDBUFF[7] = (u8)FS[channel].IDWriteBuff[4];    
		SENDBUFF[8] = (u8)FS[channel].IDWriteBuff[5];
		
		SENDBUFF[9] = (u8)(CRC16(SENDBUFF, 9) >> 8);	
		SENDBUFF[10] = (u8)(CRC16(SENDBUFF, 9));	
        
//		printf("\r\nFS[%d].IDWriteBuff[]:",channel);
//		for(i=0;i<RIDIDE;i++)
//		{
//			printf("%02X ",FS[channel].IDWriteBuff[i]);
//		}
//		printf("\r\n");
       
	
		switch(channel)
		{
			case CHANNEL_FS1:
				
				for(i=0; i<11; i++)
				{
					USART_SendChar(UART_FS1,SENDBUFF[i]);
					//USART_SendChar(USART1,SENDBUFF[i]);
				}
			break;
			case CHANNEL_FS2:
				for(i=0; i<11; i++)
				{
					USART_SendChar(UART_FS2,SENDBUFF[i]);
				}
			break;
			case CHANNEL_FS3:
				for(i=0; i<11; i++)
				{
					USART_SendChar(UART_FS3,SENDBUFF[i]);
				}
			break;
			case CHANNEL_FS4:
				for(i=0; i<11; i++)
				{
					USART_SendChar(UART_FS4,SENDBUFF[i]);
				}
			break;
		}
	}
}

void FilterIdCheck(void)//Read ID
{
	Filter_EnterModeCMD(CMD_RID);		
	delay_ms(200);
	Filter_RecieveDataProcess(CHANNEL_FS1);
	Filter_RecieveDataProcess(CHANNEL_FS2);
	Filter_RecieveDataProcess(CHANNEL_FS3);
	Filter_RecieveDataProcess(CHANNEL_FS4);	
}



void SendCMDtoScannDevice(void)
{
	u8 i;

	u8 sendBuff[20]= {0x7E,0x00,0x08,0x01,0x00,0x02,0x01,0xAB,0xCD};

	for(i=0; i<9; i++)
	{
		USART_SendChar(UART_FS1,sendBuff[i]);
		USART_SendChar(UART_FS2,sendBuff[i]);
		USART_SendChar(UART_FS3,sendBuff[i]);
		USART_SendChar(UART_FS4,sendBuff[i]);
		//printf("%d ",SENDBUFF[i]);
	}


}

/************************************************* 
Function:       ReceiveID 
Description:    接收扫码模块发来的ID号 
Input:          FS[ChannelNum].RecieveBuff[0] 
                   
Output:         无 
Return:         返回0：接收成功	1： 接收失败
*************************************************/  

u8 ReceiveID(void)
{

	
#if 1
	u8 i,j;
	u8 ret;
    u8 temp_buf[30];
    u8 ChannelNum;

//	for(ChannelNum=0;ChannelNum<4;ChannelNum++)
//	{
//			printf("\r\nFS[%d].RecieveIdBuff[]:",ChannelNum);
//			for(i=0;i<16;i++)
//			{
//				printf("%02X ",FS[ChannelNum].RecieveBuff[i]);
//			}
//			printf("\r\n");
//	}
#endif	
#if 1	
	IDstartSendFlag = true;	
	for(ChannelNum=0;ChannelNum<4;ChannelNum++)
	{
		
		if(FS[ChannelNum].RecieveBuff[0]==ADDRESS_SCANNING && FS[ChannelNum].RecieveBuff[1]==(ChannelNum+1))
		{
		     j=0;
			for(i=0;i<14;i++)
			{
				
				if(FS[ChannelNum].RecieveBuff[i+2]>=0x30 && FS[ChannelNum].RecieveBuff[i+2]<='F')
				{
				
					temp_buf[j] = FS[ChannelNum].RecieveBuff[i+2];
				     j++;
				   
				}
			
			}
			
			ASC2BCD(FS[ChannelNum].IDWriteBuff,temp_buf,12);  //asciii转BCD码
			if(FS[ChannelNum].IDWriteBuff[5]!=0x1F) return 1;

//			printf("\r\nscan_FS[%d].RecieveIdBuff[]:",ChannelNum);
//			for(i=0;i<RIDIDE;i++)
//			{
//				printf("%02X ",FS[ChannelNum].IDWriteBuff[i]);
//			}
//			printf("\r\n");

		
		
		}
		
		else
		{
		
		  IDstartSendFlag = false;
		  return 1;
		
		}

		

	}

    ret = IDBuff_cmp(FS[CHANNEL_FS1].IDWriteBuff,FS[CHANNEL_FS2].IDWriteBuff,FS[CHANNEL_FS3].IDWriteBuff,FS[CHANNEL_FS4].IDWriteBuff);
	if (ret == 0)		
		 return 0;

	return 1;
		
	
#endif	
	
#if 0	
	u8 CMDLength = 0;
    u8 i = 0;
    u16 idcrc=0,crc;

	
	
	
	
	
	
 
	
	CMDLength = idReceive2ScanningBuff[2]+5;
	idcrc = CRC16(idReceive2ScanningBuff, CMDLength-2);
	crc = (idReceive2ScanningBuff[CMDLength-2]<<8) + idReceive2ScanningBuff[CMDLength-1];
  
   if((idReceive2ScanningBuff[0] == ADDRESS_SCANNING)&&(idcrc==crc))
	{
		//According to the functiong,Choose the way to deal with data 				
		switch(idReceive2ScanningBuff[1])
		{									
			case FS1: 
            
                for(i=0; i<RIDIDE; i++)
                {
                    FS[CHANNEL_FS1].IDWriteBuff[i] = idReceive2ScanningBuff[i+3];
                }	

//                printf("FS[CHANNEL_FS1].IDWriteBuff= ");               
//                for(i=0;i<RIDIDE;i++)
//                    {
//                        printf("%02x ",FS[CHANNEL_FS1].IDWriteBuff[i] );
//                    }
//                printf("\r\n");                            
			
            break;

			case FS2:
            
                for(i=0; i<RIDIDE; i++)
                {
                    FS[CHANNEL_FS2].IDWriteBuff[i] = idReceive2ScanningBuff[i+3];
                }
                
//                printf("FS[CHANNEL_FS2].IDWriteBuff= ");               
//                for(i=0;i<RIDIDE;i++)
//                    {
//                        printf("%02x ",FS[CHANNEL_FS2].IDWriteBuff[i] );
//                    }
//                printf("\r\n");                            
                            
			break;

			case FS3:  
            
                for(i=0; i<RIDIDE; i++)
                {
                    FS[CHANNEL_FS3].IDWriteBuff[i] = idReceive2ScanningBuff[i+3];
                }
//                printf("FS[CHANNEL_FS3].IDWriteBuff= ");               
//                for(i=0;i<RIDIDE;i++)
//                    {
//                        printf("%02x ",FS[CHANNEL_FS3].IDWriteBuff[i] );
//                    }
//                printf("\r\n");                            
                            
			break;

			case FS4:  
            
                for(i=0; i<RIDIDE; i++)
                {
                    FS[CHANNEL_FS4].IDWriteBuff[i] = idReceive2ScanningBuff[i+3];
                }
                
//                printf("FS[CHANNEL_FS4].IDWriteBuff= ");               
//                for(i=0;i<RIDIDE;i++)
//                    {
//                        printf("%02x ",FS[CHANNEL_FS4].IDWriteBuff[i] );
//                    }
//                printf("\r\n");                            
               IDBuff_cmp(FS[CHANNEL_FS1].IDWriteBuff,FS[CHANNEL_FS2].IDWriteBuff,FS[CHANNEL_FS3].IDWriteBuff,FS[CHANNEL_FS4].IDWriteBuff);
            
			break;

			case START: 
//				 if(idReceive2ScanningBuff[3]==0x18 && idReceive2ScanningBuff[4]==0x24 && idReceive2ScanningBuff[8]==0x28)
//				{

					IDstartSendFlag = true;	
					//printf("IDstartSendFlag = true\r\n");
//				}
//				else
//				{
//					IDstartSendFlag = false;	
//					printf("IDstartSendFlag = false\r\n");
//				
//				
//				}
                
			break;

            default:break;
            
          }


 
     for(i=0;i<11;i++)
        {
            idReceive2ScanningBuff[i]=0;
        
        }
 
 
 }
	
 #endif

}


/************************************************* 
Function:       IDBuff_cmp 
Description:    每个ID号比较一下，排除重复的ID号 
Input:          输入ID号数组的首地址 
                   
Output:         无 
Return:         返回0：没有重复的ID号	1： 有重复的ID号
*************************************************/  

u8 IDBuff_cmp(u8 *a,u8 *b,u8 *c,u8 *d)
{


    int ret1,ret2,ret3,ret4,ret5,ret6;
	
    ret1=array_cmp(a, b,6);
    ret2=array_cmp(a, c,6);
    ret3=array_cmp(a, d,6);
    ret4=array_cmp(b, c,6);
    ret5=array_cmp(b, d,6);
    ret6=array_cmp(c, d,6);
    //printf("ret1= %d,ret2=%d,ret3=%d,,ret4=%d,ret5=%d,ret6=%d \r\n",ret1,ret2,ret3,ret4,ret5,ret6);
    if((ret1==0)||(ret2==0)||(ret3==0)||(ret4==0)||(ret5==0)||(ret6==0)||(IDstartSendFlag==false))
	{
		IDCheckOk=false;
		return 1;
	}
    else if(IDstartSendFlag==true)
    {
    
		IDCheckOk=true;
		return 0;
    }

	return 1;



}

/*******************************************************************************
* Function : u8 PID_Calibration_Mode(void)	
* brief    : When Recieving Dust Sensors Data,Deal with data 
* param    : @ChannelNum:Dust Channel Number On A Board(8 Channels)
						 @Data: The Recieved data will be deald with 
* retval   : None
*******************************************************************************/



void Filter_DataInit(void)
{
	u8 i,channel;
    IDstartSendFlag = false;
	for(channel=0;channel<NUM_CHANNELS;channel++)
	{
		for(i=0;i<RBRIDE;i++)
		{
			FS[channel].BlockingRateBuff[i] = 0;	
		}
		for(i=0;i<RVVIDE;i++)
		{
			FS[channel].VoltageValueBuff[i] = 0;	
		}
		for(i=0;i<RTVIDE;i++)
		{
			FS[channel].TempertureValueBuff[i] = 0;	
		}
		for(i=0;i<RCVIDE;i++)
		{
			FS[channel].CalibrationVoltageBuff[i] = 0;	
		}
		for(i=0;i<CDWIDE_MAX*2;i++)
		{
			FS[channel].RecieveBuff[i] = 0;	
		}
		for(i=0;i<RIDIDE;i++)
		{
			FS[channel].RecieveIdBuff[i] = 0;	
		}
//        for(i=0;i<RIDIDE;i++)
//		{
//			FS[channel].IDWriteBuff[i] = 0;	
//		}
		FS[channel].SUMCRC = 0;
		FS[channel].ErrorTimes = 0;
		FS[channel].RecieveNum = 0;
		FS[channel].MaxVolt = 0;
		FS[channel].MinVolt = 0;
		FS[channel].BlockingRate = 0;
		FS[channel].Tp1Volt = 0;
		FS[channel].Tp2Volt = 0;
		FS[channel].V33Volt = 0;
		//FS[channel].TempOut = 0;
		//FS[channel].TempIn = 0;
        //FS[channel].V50Volt_f = 0;
        //FS[channel].V33Volt_f = 0;
        //FS[channel].MinVolt_f = 0;
        //FS[channel].MaxVolt_f = 0;
        //FS[channel].MidVolt_f2 = 0;
        //FS[channel].MidVolt_f1 = 0;
        //FS[channel].PwmDuty_f = 00.0;
        //FS[channel].BlockingRate_f = 0;
        //FS[channel].TempIn_f = 00.0;
        //FS[channel].TempOut_f = 00.0;
        FS[channel].ChannelConnect = false;	                
		FS[channel].JudgeFlag = true;        
		FS_SerialReceiveBuff[channel] = 0;
	}
}

/************************************************* 
Function:       PID_Set_Ok
Description:   	检查PID是否校准OK
Calls:          无
Table Accessed: 无 
Table Updated:  无 
Input:          FS[].JudgeFlag
                  // 用、取值说明及参数间关系。 
Output:         // 对输出参数的说明。 
Return:         返回 0：PID设置成功  			1：PID设置失败 
Others:         // 其它说明 
*************************************************/  

u8 PID_Set_Ok(void)
{
	
	u8 channelNum;

	for (channelNum = 0; channelNum < 4; channelNum++)
	{
		if(FS[channelNum].RecieveBuff[0]==0x4D )
		{
	
			FS[channelNum].ChannelConnect = true;
			FS[channelNum].JudgeFlag = true;
		}
		else
		{
		
			FS[channelNum].JudgeFlag = false;

		}
	
	}

	for(channelNum=0;channelNum<4;channelNum++)
	{

		if(FS[channelNum].JudgeFlag == false)
			return 1;

	}

    return 0;	
	
}


void PID_Calibration_Mode( u8 Function )
{

	u8 i=0x00;
	u8 CMDWIDE = 0x09;
	u8 SENDBUFF[7] = {ADDRESS_SLAVE,CMD_PID,0x04,0x01,0x00,0x00,0x00};//年 周 批次 序列号高 序列号低 工序号

	switch(Function)
	{
				
		//FS DEBUG_MODE
		case DEBUG_MODE: CMDWIDE = 0x07; 
				SENDBUFF[3] = DEBUG_MODE;	 
		break;

		//FS PIDCALI_MODE
		case PIDCALI_MODE: CMDWIDE = 0x07; 
				SENDBUFF[3] = PIDCALI_MODE;
		break; 

		//FS ANSWER_MODE	 									
		case ANSWER_MODE: CMDWIDE = 0x07; 
			SENDBUFF[3] = ANSWER_MODE;	
		break;

		default:break;
	}
	
	SENDBUFF[2] = CMDWIDE - 3;
	SENDBUFF[CMDWIDE-2] = (u8)(CRC16(SENDBUFF, CMDWIDE-2) >> 8);	
	SENDBUFF[CMDWIDE-1] = (u8)(CRC16(SENDBUFF, CMDWIDE-2));
	
	//printf("\r\nFilter_EnterModeCMD %d :",Function);
	for(i=0; i<CMDWIDE; i++)
	{
		USART_SendChar(UART_FS1,SENDBUFF[i]);
		USART_SendChar(UART_FS2,SENDBUFF[i]);
		USART_SendChar(UART_FS3,SENDBUFF[i]);
		USART_SendChar(UART_FS4,SENDBUFF[i]);
		//printf("%d ",SENDBUFF[i]);
	}



}





void UartSendData_toPC(void)
{
	u8 i;
	
     UartSendData_PC[0]=ADDRESS_PC;
     UartSendData_PC[1]=PC_RD;
     UartSendData_PC[2]=PC_CHAL;
     UartSendData_PC[3]=PC_CONSTA;
     UartSendData_PC[4]=PC_DATA_LTH;
     
	 UartSendData_PC[5]=WORKER_NUM_H;
     UartSendData_PC[6]=WORKER_NUM_L;
	
     UartSendData_PC[7]=testSTART_Flag; //开始标志
	
     UartSendData_PC[8]=((calendar.year)>>8);	//年
     UartSendData_PC[9]=calendar.year;
	
     UartSendData_PC[10]=calendar.month;		//月	
     UartSendData_PC[11]=calendar.date;		//日	
     UartSendData_PC[12]=calendar.hour;		//时
     UartSendData_PC[13]=calendar.min;		//分	
     UartSendData_PC[14]=calendar.sec;		//秒
     //FS1  ID
	 UartSendData_PC[15]=FS[CHANNEL_FS1].RecieveIdBuff[0];	 
     UartSendData_PC[16]=FS[CHANNEL_FS1].RecieveIdBuff[1];	
     UartSendData_PC[17]=FS[CHANNEL_FS1].RecieveIdBuff[2];	 
     UartSendData_PC[18]=FS[CHANNEL_FS1].RecieveIdBuff[3];		
     UartSendData_PC[19]=FS[CHANNEL_FS1].RecieveIdBuff[4];	 
     UartSendData_PC[20]=FS[CHANNEL_FS1].RecieveIdBuff[5];
	 
	 //FS2  ID
     UartSendData_PC[21]=FS[CHANNEL_FS2].RecieveIdBuff[0];
     UartSendData_PC[22]=FS[CHANNEL_FS2].RecieveIdBuff[1];
     UartSendData_PC[23]=FS[CHANNEL_FS2].RecieveIdBuff[2];
     UartSendData_PC[24]=FS[CHANNEL_FS2].RecieveIdBuff[3];
     UartSendData_PC[25]=FS[CHANNEL_FS2].RecieveIdBuff[4];
     UartSendData_PC[26]=FS[CHANNEL_FS2].RecieveIdBuff[5];
	 
	 //FS3  ID
     UartSendData_PC[27]=FS[CHANNEL_FS3].RecieveIdBuff[0];
     UartSendData_PC[28]=FS[CHANNEL_FS3].RecieveIdBuff[1];
     UartSendData_PC[29]=FS[CHANNEL_FS3].RecieveIdBuff[2];	 
     UartSendData_PC[30]=FS[CHANNEL_FS3].RecieveIdBuff[3];	 
     UartSendData_PC[31]=FS[CHANNEL_FS3].RecieveIdBuff[4];
     UartSendData_PC[32]=FS[CHANNEL_FS3].RecieveIdBuff[5];
	 
	 //FS4  ID
     UartSendData_PC[33]=FS[CHANNEL_FS4].RecieveIdBuff[0];
     UartSendData_PC[34]=FS[CHANNEL_FS4].RecieveIdBuff[1];
     UartSendData_PC[35]=FS[CHANNEL_FS4].RecieveIdBuff[2];
     UartSendData_PC[36]=FS[CHANNEL_FS4].RecieveIdBuff[3];
     UartSendData_PC[37]=FS[CHANNEL_FS4].RecieveIdBuff[4];
     UartSendData_PC[38]=FS[CHANNEL_FS4].RecieveIdBuff[5];
	 
	 
	 
     UartSendData_PC[39]=burning_check_ok; //烧录结果
	 
	 //FS1  ID
     UartSendData_PC[40]=FS1;
	 UartSendData_PC[41]=FS[CHANNEL_FS1].RecieveIdBuff[0];	 
     UartSendData_PC[42]=FS[CHANNEL_FS1].RecieveIdBuff[1];	
     UartSendData_PC[43]=FS[CHANNEL_FS1].RecieveIdBuff[2];	 
     UartSendData_PC[44]=FS[CHANNEL_FS1].RecieveIdBuff[3];		
     UartSendData_PC[45]=FS[CHANNEL_FS1].RecieveIdBuff[4];	 
     UartSendData_PC[46]=FS[CHANNEL_FS1].RecieveIdBuff[5];
	 
	 //5v
     UartSendData_PC[47]=((FS[CHANNEL_FS1].V50Volt)>>8);
     UartSendData_PC[48]=FS[CHANNEL_FS1].V50Volt;
     //3V3
	 UartSendData_PC[49]=((FS[CHANNEL_FS1].V33Volt)>>8);	 
     UartSendData_PC[50]=FS[CHANNEL_FS1].V33Volt;
	 //100% TP2V
     UartSendData_PC[51]=(u8)((FS[CHANNEL_FS1].MinVolt)>>8);
     UartSendData_PC[52]=(u8)FS[CHANNEL_FS1].MinVolt;
	 //0% 	TP2V 
     UartSendData_PC[53]=(u8)((FS[CHANNEL_FS1].MaxVolt)>>8);
     UartSendData_PC[54]=(u8)FS[CHANNEL_FS1].MaxVolt;
	 //50% TP2V
     UartSendData_PC[55]=(u8)(FS[CHANNEL_FS1].MidVolt_tp2>>8);
     UartSendData_PC[56]=(u8)FS[CHANNEL_FS1].MidVolt_tp2;
	 //50% TP1V
     UartSendData_PC[57]=(u8)(FS[CHANNEL_FS1].MidVolt_tp1>>8);
     UartSendData_PC[58]=(u8)FS[CHANNEL_FS1].MidVolt_tp1;
	 
	 //PWM
     UartSendData_PC[59]=FS[CHANNEL_FS1].PwmDuty; 
	 
	//uart
     UartSendData_PC[60]=FS[CHANNEL_FS1].BlockingRateBuff[0];
	 
	 //温度	 
     UartSendData_PC[61]=FS[CHANNEL_FS1].TempertureValueBuff[0];
     UartSendData_PC[62]=FS[CHANNEL_FS1].TempertureValueBuff[1];
	 
	 //FS2
     UartSendData_PC[63]=FS2;
	 
	 UartSendData_PC[64]=FS[CHANNEL_FS2].RecieveIdBuff[0];	 
     UartSendData_PC[65]=FS[CHANNEL_FS2].RecieveIdBuff[1];	
     UartSendData_PC[66]=FS[CHANNEL_FS2].RecieveIdBuff[2];	 
     UartSendData_PC[67]=FS[CHANNEL_FS2].RecieveIdBuff[3];		
     UartSendData_PC[68]=FS[CHANNEL_FS2].RecieveIdBuff[4];	 
     UartSendData_PC[69]=FS[CHANNEL_FS2].RecieveIdBuff[5];
	 
	 //5v
     UartSendData_PC[70]=((FS[CHANNEL_FS2].V50Volt)>>8);
     UartSendData_PC[71]=FS[CHANNEL_FS2].V50Volt;
     //3V3
	 UartSendData_PC[72]=(FS[CHANNEL_FS2].V33Volt)>>8;	 
     UartSendData_PC[73]=FS[CHANNEL_FS2].V33Volt;
	 //100% TP2V
     UartSendData_PC[74]=(u8)(FS[CHANNEL_FS2].MinVolt)>>8;
     UartSendData_PC[75]=(u8)FS[CHANNEL_FS2].MinVolt;
	 //0% 	TP2V 
     UartSendData_PC[76]=(u8)((FS[CHANNEL_FS2].MaxVolt)>>8);
     UartSendData_PC[77]=(u8)FS[CHANNEL_FS2].MaxVolt;
	 //50% TP2V
     UartSendData_PC[78]=(u8)(FS[CHANNEL_FS2].MidVolt_tp2>>8);
     UartSendData_PC[79]=(u8)FS[CHANNEL_FS2].MidVolt_tp2;
	 //50% TP1V
     UartSendData_PC[80]=(u8)(FS[CHANNEL_FS2].MidVolt_tp1>>8);
     UartSendData_PC[81]=(u8)FS[CHANNEL_FS2].MidVolt_tp1;
	 
	 //PWM
     UartSendData_PC[82]=FS[CHANNEL_FS2].PwmDuty; 
	 
	//uart
     UartSendData_PC[83]=FS[CHANNEL_FS2].BlockingRateBuff[0];
	 
	 //温度	 
     UartSendData_PC[84]=FS[CHANNEL_FS2].TempertureValueBuff[0];
     UartSendData_PC[85]=FS[CHANNEL_FS2].TempertureValueBuff[1];


	 //FS3
     UartSendData_PC[86]=FS3;
	 
	 UartSendData_PC[87]=FS[CHANNEL_FS3].RecieveIdBuff[0];	 
     UartSendData_PC[88]=FS[CHANNEL_FS3].RecieveIdBuff[1];	
     UartSendData_PC[89]=FS[CHANNEL_FS3].RecieveIdBuff[2];	 
     UartSendData_PC[90]=FS[CHANNEL_FS3].RecieveIdBuff[3];		
     UartSendData_PC[91]=FS[CHANNEL_FS3].RecieveIdBuff[4];	 
     UartSendData_PC[92]=FS[CHANNEL_FS3].RecieveIdBuff[5];
	 
	 //5v
     UartSendData_PC[93]=((FS[CHANNEL_FS3].V50Volt)>>8);
     UartSendData_PC[94]=FS[CHANNEL_FS3].V50Volt;
     //3V3
	 UartSendData_PC[95]=((FS[CHANNEL_FS3].V33Volt)>>8);	 
     UartSendData_PC[96]=FS[CHANNEL_FS3].V33Volt;
	 //100% TP2V
     UartSendData_PC[97]=(u8)(FS[CHANNEL_FS3].MinVolt)>>8;
     UartSendData_PC[98]=(u8)FS[CHANNEL_FS3].MinVolt;
	 //0% 	TP2V 
     UartSendData_PC[99]=(u8)((FS[CHANNEL_FS3].MaxVolt)>>8);
     UartSendData_PC[100]=(u8)FS[CHANNEL_FS3].MaxVolt;
	 //50% TP2V
     UartSendData_PC[101]=(u8)(FS[CHANNEL_FS3].MidVolt_tp2>>8);
     UartSendData_PC[102]=(u8)FS[CHANNEL_FS3].MidVolt_tp2;
	 //50% TP1V
     UartSendData_PC[103]=(u8)(FS[CHANNEL_FS3].MidVolt_tp1>>8);
     UartSendData_PC[104]=(u8)FS[CHANNEL_FS3].MidVolt_tp1;
	 
	 //PWM
     UartSendData_PC[105]=FS[CHANNEL_FS3].PwmDuty; 
	 
	//uart
     UartSendData_PC[106]=FS[CHANNEL_FS3].BlockingRateBuff[0];
	 
	 //温度	 
     UartSendData_PC[107]=FS[CHANNEL_FS3].TempertureValueBuff[0];
     UartSendData_PC[108]=FS[CHANNEL_FS3].TempertureValueBuff[1];



	 //FS4
     UartSendData_PC[109]=FS4;
	 
	 UartSendData_PC[110]=FS[CHANNEL_FS4].RecieveIdBuff[0];	 
     UartSendData_PC[111]=FS[CHANNEL_FS4].RecieveIdBuff[1];	
     UartSendData_PC[112]=FS[CHANNEL_FS4].RecieveIdBuff[2];	 
     UartSendData_PC[113]=FS[CHANNEL_FS4].RecieveIdBuff[3];		
     UartSendData_PC[114]=FS[CHANNEL_FS4].RecieveIdBuff[4];	 
     UartSendData_PC[115]=FS[CHANNEL_FS4].RecieveIdBuff[5];
	 
	 //5v
     UartSendData_PC[116]=((FS[CHANNEL_FS4].V50Volt)>>8);
     UartSendData_PC[117]=FS[CHANNEL_FS4].V50Volt;
     //3V3
	 UartSendData_PC[118]=((FS[CHANNEL_FS4].V33Volt)>>8);	 
     UartSendData_PC[119]=FS[CHANNEL_FS4].V33Volt;
	 //100% TP2V
     UartSendData_PC[120]=(u8)(FS[CHANNEL_FS4].MinVolt)>>8;
     UartSendData_PC[121]=(u8)FS[CHANNEL_FS4].MinVolt;
	 //0% 	TP2V 
     UartSendData_PC[122]=(u8)((FS[CHANNEL_FS4].MaxVolt)>>8);
     UartSendData_PC[123]=(u8)FS[CHANNEL_FS4].MaxVolt;
	 //50% TP2V
     UartSendData_PC[124]=(u8)(FS[CHANNEL_FS4].MidVolt_tp2>>8);
     UartSendData_PC[125]=(u8)FS[CHANNEL_FS4].MidVolt_tp2;
	 //50% TP1V
     UartSendData_PC[126]=(u8)(FS[CHANNEL_FS4].MidVolt_tp1>>8);
     UartSendData_PC[127]=(u8)FS[CHANNEL_FS4].MidVolt_tp1;
	 
	 //PWM
     UartSendData_PC[128]=FS[CHANNEL_FS4].PwmDuty; 
	 
	//uart
     UartSendData_PC[129]=FS[CHANNEL_FS4].BlockingRateBuff[0];
	 
	 //温度	 
     UartSendData_PC[130]=FS[CHANNEL_FS4].TempertureValueBuff[0];
     UartSendData_PC[131]=FS[CHANNEL_FS4].TempertureValueBuff[1];

	 
     UartSendData_PC[132]=testEND_Flag;		//结束标识
     
	 
	 UartSendData_PC[133]=(u8)(CRC16(UartSendData_PC, 133) >> 8);
     UartSendData_PC[134]=(u8)(CRC16(UartSendData_PC, 133));
	
		for(i=0;i<135;i++){
		
			//printf("%02X",UartSendData_PC[i]);
			USART_SendChar(USART1,UartSendData_PC[i]);

		}


}


