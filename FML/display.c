//显示
#include "display.h"
#include "stdlib.h"
#include "sys.h"
#include "ILI9486.h"
#include "gui_lcd.h"
#include "protocol_fs.h"
#include "fsm.h" 
#include <asc2bcd.h>


bool DisplayUpdataEnable = true;//使能数据更新
extern bool MessageSetFlag;	//信息设置标志	true：设置状态

u8 Tp1_1[5];
u8 Tp1_2[5];
u8 Tp1_3[5];
u8 Tp1_4[5];

u8 Tp2_1[5];
u8 Tp2_2[5];
u8 Tp2_3[5];
u8 Tp2_4[5];

u8 V33_1[5];
u8 V33_2[5];
u8 V33_3[5];
u8 V33_4[5];

u8 Vmax_1[5];
u8 Vmax_2[5];
u8 Vmax_3[5];
u8 Vmax_4[5];

u8 Vmin_1[5];
u8 Vmin_2[5];
u8 Vmin_3[5];
u8 Vmin_4[5];

u8 BlockingRate_1[4];
u8 BlockingRate_2[4];
u8 BlockingRate_3[4];
u8 BlockingRate_4[4];//BlockingRate

void VoltChange(u8* tp,u16 buff)
{
	tp[0] = '0' + buff/1000;
	tp[1] = '.';
	tp[2] = '0' + buff%1000/100;
	tp[3] = '0' + buff%1000%100/10;
	tp[4] = 0;
}

void VoltChange1(u8* tp,u16 buff)
{
	tp[0] = '0' + buff/100;
	tp[1] = '.';
	tp[2] = '0' + buff/10%10;
	tp[3] = '0' + buff%10;
	tp[4] = 0;
}


void BlockingRateChange(u8* tp,u16 buff)
{
	tp[0] = '0' + buff/100;
	tp[1] = '0' + buff/10%10;
	tp[2] = '0' + buff%10;
	tp[3] = 0;
}

void VoltChanges(void)
{
	VoltChange(Tp1_1,FS[CHANNEL_FS1].Tp1Volt);
	//Tp1_1,FS[CHANNEL_FS1].Tp1Volt
	VoltChange(Tp1_2,FS[CHANNEL_FS2].Tp1Volt);
	VoltChange(Tp1_3,FS[CHANNEL_FS3].Tp1Volt);
	VoltChange(Tp1_4,FS[CHANNEL_FS4].Tp1Volt);

	VoltChange(Tp2_1,FS[CHANNEL_FS1].Tp2Volt);
	VoltChange(Tp2_2,FS[CHANNEL_FS2].Tp2Volt);
	VoltChange(Tp2_3,FS[CHANNEL_FS3].Tp2Volt);
	VoltChange(Tp2_4,FS[CHANNEL_FS4].Tp2Volt);

	VoltChange(V33_1,FS[CHANNEL_FS1].V33Volt);
	//printf("FS[CHANNEL_FS1].V33Volt=%d \r\n",FS[CHANNEL_FS1].V33Volt);
	VoltChange(V33_2,FS[CHANNEL_FS2].V33Volt);
	VoltChange(V33_3,FS[CHANNEL_FS3].V33Volt);
	VoltChange(V33_4,FS[CHANNEL_FS4].V33Volt);

	VoltChange(Vmax_1,FS[CHANNEL_FS1].MaxVolt);
	VoltChange(Vmax_2,FS[CHANNEL_FS2].MaxVolt);
	VoltChange(Vmax_3,FS[CHANNEL_FS3].MaxVolt);
	VoltChange(Vmax_4,FS[CHANNEL_FS4].MaxVolt);

	VoltChange(Vmin_1,FS[CHANNEL_FS1].MinVolt);
	VoltChange(Vmin_2,FS[CHANNEL_FS2].MinVolt);
	VoltChange(Vmin_3,FS[CHANNEL_FS3].MinVolt);
	VoltChange(Vmin_4,FS[CHANNEL_FS4].MinVolt);

	BlockingRateChange(BlockingRate_1,FS[CHANNEL_FS1].BlockingRate);
	BlockingRateChange(BlockingRate_2,FS[CHANNEL_FS2].BlockingRate);
	BlockingRateChange(BlockingRate_3,FS[CHANNEL_FS3].BlockingRate);
	BlockingRateChange(BlockingRate_4,FS[CHANNEL_FS4].BlockingRate);
}
void DisDrawLine(void)
{
	PointColor = POINT_COLOR;
	BackColor = BACK_COLOR;
	LCD_DrawLine(0,yStar3V3,lcddev.width-1,yStar3V3);
	LCD_DrawLine(0,yStarTp1,lcddev.width-1,yStarTp1);
	LCD_DrawLine(0,yStarTp2,lcddev.width-1,yStarTp2);
	LCD_DrawLine(0,yStarProg,lcddev.width-1,yStarProg);
	LCD_DrawLine(0,yStarVmax,lcddev.width-1,yStarVmax);
	LCD_DrawLine(0,yStarVmin,lcddev.width-1,yStarVmin);
	LCD_DrawLine(0,yStarBlockingRate,lcddev.width-1,yStarBlockingRate);
	LCD_DrawLine(0,yStarJudge,lcddev.width-1,yStarJudge);
	LCD_DrawLine(xStarFS1,yStar3V3,xStarFS1,lcddev.height);
	LCD_DrawLine(xStarFS2,yStar3V3,xStarFS2,lcddev.height);
	LCD_DrawLine(xStarFS3,yStar3V3,xStarFS3,lcddev.height);
	LCD_DrawLine(xStarFS4,yStar3V3,xStarFS4,lcddev.height);
}
void DisDrawUI(void)
{
	PointColor = POINT_COLOR;
	BackColor = BACK_COLOR;
	LCD_Fill(0,yStar3V3,lcddev.width-1,lcddev.height-1,BackColor);	
	DisDrawLine();
	Show_Str(xOffset, yStar3V3+yOffset,PointColor,BackColor, "3V3",32,0);
	Show_Str(xOffset, yStarTp1+yOffset,PointColor,BackColor, "TP1",32,0);
	Show_Str(xOffset, yStarTp2+yOffset,PointColor,BackColor, "TP2",32,0);
	Show_Str(xOffset, yStarProg+yOffset,PointColor,BackColor, "PROG",32,1);
	Show_Str(xOffset, yStarVmax+yOffset,PointColor,BackColor, "Vmax",32,1);
	Show_Str(xOffset, yStarVmin+yOffset,PointColor,BackColor, "Vmin",32,1);
	Show_Str(xOffset, yStarBlockingRate+yOffset,PointColor,BackColor,"堵率",32,1);
	Show_Str(xOffset, yStarJudge+yOffset,PointColor,BackColor, "OK/NG",32,1);
}
void DisJudge(u16 x, u16 y, u16 bc, u8 *str)//显示判定结果，单个 OK/NG
{
	LCD_Fill(x+1,y+1,x+99,y+37,bc);
	Show_Str(x+xOffset+34,y+yOffset,PointColor,bc, str,32,0);	
}
void DisVolt(u16 x, u16 y,u16 bc, u8 *str)//显示单个电压值
{
	LCD_Fill(x+1,y+1,x+99,y+37,bc);
	Show_Str(x+xOffset+16,y+yOffset,PointColor,bc,str,32,0);
}
void DisTestVolt(void)//显示硬件电压值
{
//*V33**************************************************************************************	
	if((FS[CHANNEL_FS1].V33Volt > MIN_V33) && (FS[CHANNEL_FS1].V33Volt < MAX_V33))
	{
		DisVolt(xStarFS1, yStar3V3, BACK_COLOR, V33_1);	
	}else
	{
		//FS[CHANNEL_FS1].JudgeFlag = false;
		DisVolt(xStarFS1, yStar3V3, BACK_COLOR_ERR, V33_1);		
	}
	
	if((FS[CHANNEL_FS2].V33Volt > MIN_V33) && (FS[CHANNEL_FS2].V33Volt < MAX_V33))
	{
		DisVolt(xStarFS2, yStar3V3, BACK_COLOR, V33_2);
	}else
	{
		//FS[CHANNEL_FS2].JudgeFlag = false;
		DisVolt(xStarFS2, yStar3V3, BACK_COLOR_ERR, V33_2);	
	}
	
	if((FS[CHANNEL_FS3].V33Volt > MIN_V33) && (FS[CHANNEL_FS3].V33Volt < MAX_V33))
	{
		DisVolt(xStarFS3, yStar3V3, BACK_COLOR, V33_3);
	}else
	{
		//FS[CHANNEL_FS3].JudgeFlag = false;
		DisVolt(xStarFS3, yStar3V3, BACK_COLOR_ERR, V33_3);
	}
	
	if((FS[CHANNEL_FS4].V33Volt > MIN_V33) && (FS[CHANNEL_FS4].V33Volt < MAX_V33))
	{
		DisVolt(xStarFS4, yStar3V3, BACK_COLOR, V33_4);
	}else
	{
		//FS[CHANNEL_FS4].JudgeFlag = false;
		DisVolt(xStarFS4, yStar3V3, BACK_COLOR_ERR, V33_4);
	}
//*TP1**************************************************************************************
	if(FS[CHANNEL_FS1].Tp1Volt < MAX_TP1)
	{
		DisVolt(xStarFS1, yStarTp1, BACK_COLOR, Tp1_1);
	}else
	{
		//FS[CHANNEL_FS1].JudgeFlag = false;
		DisVolt(xStarFS1, yStarTp1, BACK_COLOR_ERR, Tp1_1);
	}
	
	if(FS[CHANNEL_FS2].Tp1Volt < MAX_TP1)
	{
		DisVolt(xStarFS2, yStarTp1, BACK_COLOR, Tp1_2);
	}else
	{
		//FS[CHANNEL_FS2].JudgeFlag = false;
		DisVolt(xStarFS2, yStarTp1, BACK_COLOR_ERR, Tp1_2);
	}
	
	if(FS[CHANNEL_FS3].Tp1Volt < MAX_TP1)
	{
		DisVolt(xStarFS3, yStarTp1, BACK_COLOR, Tp1_3);
	}else
	{
		//FS[CHANNEL_FS3].JudgeFlag = false;
		DisVolt(xStarFS3, yStarTp1, BACK_COLOR_ERR, Tp1_3);
	}
	
	if(FS[CHANNEL_FS4].Tp1Volt < MAX_TP1)
	{
		DisVolt(xStarFS4, yStarTp1, BACK_COLOR, Tp1_4);
	}else
	{
		//FS[CHANNEL_FS4].JudgeFlag = false;
		DisVolt(xStarFS4, yStarTp1, BACK_COLOR_ERR, Tp1_4);
	}
//*TP2**************************************************************************************
	if((FS[CHANNEL_FS1].Tp2Volt > MIN_TP2) && (FS[CHANNEL_FS1].Tp2Volt < MAX_TP2))
	{
		DisVolt(xStarFS1, yStarTp2, BACK_COLOR, Tp2_1);
	}else
	{
		//FS[CHANNEL_FS1].JudgeFlag = false;
		DisVolt(xStarFS1, yStarTp2, BACK_COLOR_ERR, Tp2_1);
	}
	
	if((FS[CHANNEL_FS2].Tp2Volt > MIN_TP2) && (FS[CHANNEL_FS2].Tp2Volt < MAX_TP2))
	{
		DisVolt(xStarFS2, yStarTp2, BACK_COLOR, Tp2_2);
	}else
	{
		//FS[CHANNEL_FS2].JudgeFlag = false;
		DisVolt(xStarFS2, yStarTp2, BACK_COLOR_ERR, Tp2_2);
	}
	
	if((FS[CHANNEL_FS3].Tp2Volt > MIN_TP2) && (FS[CHANNEL_FS3].Tp2Volt < MAX_TP2))
	{
		DisVolt(xStarFS3, yStarTp2, BACK_COLOR, Tp2_3);
	}else
	{
		//FS[CHANNEL_FS3].JudgeFlag = false;
		DisVolt(xStarFS3, yStarTp2, BACK_COLOR_ERR, Tp2_3);
	}
	
	if((FS[CHANNEL_FS4].Tp2Volt > MIN_TP2) && (FS[CHANNEL_FS4].Tp2Volt < MAX_TP2))
	{
		DisVolt(xStarFS4, yStarTp2, BACK_COLOR, Tp2_4);	
	}else
	{
		//FS[CHANNEL_FS4].JudgeFlag = false;
		DisVolt(xStarFS4, yStarTp2, BACK_COLOR_ERR, Tp2_4);	
	}
}

#if 1
void Display(int *event)
{
	//if(MessageSetFlag == true) return;
	if(DisplayUpdataEnable != true) return;
	
	DisplayUpdataEnable = false;
	if(*event == KEY_START_PRESS)
	{
		DisDrawUI();
		//*event = STATUS_2;
		return;
	}
	
	VoltChanges();
	DisTestVolt();
//*program**********************************************************************************
	if(*event == BURNING_PROGRAM_OK)//烧录结果
	{
		if(FS[CHANNEL_FS1].ChannelConnect == true)
		{
			DisJudge(xStarFS1,yStarProg,BACK_COLOR,"OK");
		}
		else
		{
			FS[CHANNEL_FS1].JudgeFlag = false;
			DisJudge(xStarFS1,yStarProg,BACK_COLOR_ERR, "NG");
		}
		
		if(FS[CHANNEL_FS2].ChannelConnect == true)
		{
			DisJudge(xStarFS2,yStarProg,BACK_COLOR,"OK");
		}
		else
		{
			FS[CHANNEL_FS2].JudgeFlag = false;
			DisJudge(xStarFS2,yStarProg,BACK_COLOR_ERR, "NG");
		}
		
		if(FS[CHANNEL_FS3].ChannelConnect == true)
		{
			DisJudge(xStarFS3,yStarProg,BACK_COLOR,"OK");
		}
		else
		{
			FS[CHANNEL_FS3].JudgeFlag = false;
			DisJudge(xStarFS3,yStarProg,BACK_COLOR_ERR, "NG");
		}
		
		if(FS[CHANNEL_FS4].ChannelConnect == true)
		{
			DisJudge(xStarFS4,yStarProg,BACK_COLOR,"OK");
		}
		else
		{
			FS[CHANNEL_FS4].JudgeFlag = false;
			DisJudge(xStarFS4,yStarProg,BACK_COLOR_ERR, "NG");
		}	
	}
//*校准1电压**********************************************************************************
	else if(*event == CLEAN_FILTER_CALIBRATION_OK)
	{
		if((FS[CHANNEL_FS1].MaxVolt > MIN_V_MAX) && (FS[CHANNEL_FS1].MaxVolt < MAX_V_MAX))
		{
			DisVolt(xStarFS1, yStarVmax, BACK_COLOR, Vmax_1);
		}else
		{
			FS[CHANNEL_FS1].JudgeFlag = false;
			DisVolt(xStarFS1, yStarVmax, BACK_COLOR_ERR, Vmax_1);
		}

		if((FS[CHANNEL_FS2].MaxVolt > MIN_V_MAX) && (FS[CHANNEL_FS2].MaxVolt < MAX_V_MAX))
		{
			DisVolt(xStarFS2, yStarVmax, BACK_COLOR, Vmax_2);
		}else
		{
			FS[CHANNEL_FS2].JudgeFlag = false;
			DisVolt(xStarFS2, yStarVmax, BACK_COLOR_ERR, Vmax_2);
		}

		if((FS[CHANNEL_FS3].MaxVolt > MIN_V_MAX) && (FS[CHANNEL_FS3].MaxVolt < MAX_V_MAX))
		{
			DisVolt(xStarFS3, yStarVmax, BACK_COLOR, Vmax_3);
		}else
		{
			FS[CHANNEL_FS3].JudgeFlag = false;
			DisVolt(xStarFS3, yStarVmax, BACK_COLOR_ERR, Vmax_3);
		}

		if((FS[CHANNEL_FS4].MaxVolt > MIN_V_MAX) && (FS[CHANNEL_FS4].MaxVolt < MAX_V_MAX))
		{
			DisVolt(xStarFS4, yStarVmax, BACK_COLOR, Vmax_4);	
		}else
		{
			FS[CHANNEL_FS4].JudgeFlag = false;
			DisVolt(xStarFS4, yStarVmax, BACK_COLOR_ERR, Vmax_4);	
		}
	}
//*校准2电压************************************************************
	else if(*event == DIRTY_FILTER_CALIBRATION_OK)
	{
		if((FS[CHANNEL_FS1].MinVolt > MIN_V_MIN) && (FS[CHANNEL_FS1].MinVolt < MAX_V_MIN))
		{
			DisVolt(xStarFS1, yStarVmin, BACK_COLOR, Vmin_1);
		}else
		{
			FS[CHANNEL_FS1].JudgeFlag = false;
			DisVolt(xStarFS1, yStarVmin, BACK_COLOR_ERR, Vmin_1);
		}

		if((FS[CHANNEL_FS2].MinVolt > MIN_V_MIN) && (FS[CHANNEL_FS2].MinVolt < MAX_V_MIN))
		{
			DisVolt(xStarFS2, yStarVmin, BACK_COLOR, Vmin_2);
		}else
		{
			FS[CHANNEL_FS2].JudgeFlag = false;
			DisVolt(xStarFS2, yStarVmin, BACK_COLOR_ERR, Vmin_2);
		}

		if((FS[CHANNEL_FS3].MinVolt > MIN_V_MIN) && (FS[CHANNEL_FS3].MinVolt < MAX_V_MIN))
		{
			DisVolt(xStarFS3, yStarVmin, BACK_COLOR, Vmin_3);
		}else
		{
			FS[CHANNEL_FS3].JudgeFlag = false;
			DisVolt(xStarFS3, yStarVmin, BACK_COLOR_ERR, Vmin_3);
		}

		if((FS[CHANNEL_FS4].MinVolt > MIN_V_MIN) && (FS[CHANNEL_FS4].MinVolt < MAX_V_MIN))
		{
			DisVolt(xStarFS4, yStarVmin, BACK_COLOR, Vmin_4);
		}else
		{
			FS[CHANNEL_FS4].JudgeFlag = false;
			DisVolt(xStarFS4, yStarVmin, BACK_COLOR_ERR, Vmin_4);
		}
	}
//*校准3电压************************************************************
	else if(*event == HALF_FILTER_TEST_OK )//校准3或者发生异常事件时判断NG的需要重新更换
	{
		
		if((FS[CHANNEL_FS1].BlockingRate >= MIN_BlockingRate) && (FS[CHANNEL_FS1].BlockingRate <= MAX_BlockingRate) 
			&& (FS[CHANNEL_FS1].PwmDuty >= MIN_BlockingRate) && (FS[CHANNEL_FS1].PwmDuty <= MAX_BlockingRate))
		{
			
		   DisVolt(xStarFS1+16, yStarBlockingRate, BACK_COLOR, BlockingRate_1);
		
		}else
		{
			FS[CHANNEL_FS1].JudgeFlag = false;
		    DisVolt(xStarFS1+16, yStarBlockingRate, BACK_COLOR_ERR, BlockingRate_1);
		
		}

		if((FS[CHANNEL_FS2].BlockingRate >= MIN_BlockingRate) && (FS[CHANNEL_FS2].BlockingRate <= MAX_BlockingRate)			
			&& (FS[CHANNEL_FS2].PwmDuty >= MIN_BlockingRate) && (FS[CHANNEL_FS2].PwmDuty <= MAX_BlockingRate))
		{
			
		   DisVolt(xStarFS2+16, yStarBlockingRate, BACK_COLOR, BlockingRate_2);
		
		}else
		{
			FS[CHANNEL_FS2].JudgeFlag = false;
		    DisVolt(xStarFS2+16, yStarBlockingRate, BACK_COLOR_ERR, BlockingRate_2);
		
		}

		if((FS[CHANNEL_FS3].BlockingRate >= MIN_BlockingRate) && (FS[CHANNEL_FS3].BlockingRate <= MAX_BlockingRate)
			&& (FS[CHANNEL_FS3].PwmDuty >= MIN_BlockingRate) && (FS[CHANNEL_FS3].PwmDuty <= MAX_BlockingRate))
		{
			
		   DisVolt(xStarFS3+16, yStarBlockingRate, BACK_COLOR, BlockingRate_3);
		
		}else
		{
			FS[CHANNEL_FS3].JudgeFlag = false;
		    DisVolt(xStarFS1+16, yStarBlockingRate, BACK_COLOR_ERR, BlockingRate_3);
		
		}

		if((FS[CHANNEL_FS4].BlockingRate >= MIN_BlockingRate) && (FS[CHANNEL_FS4].BlockingRate <= MAX_BlockingRate)
			&& (FS[CHANNEL_FS4].PwmDuty >= MIN_BlockingRate) && (FS[CHANNEL_FS4].PwmDuty <= MAX_BlockingRate))
		{
			
		   DisVolt(xStarFS4+16, yStarBlockingRate, BACK_COLOR, BlockingRate_4);
		
		}else
		{
			FS[CHANNEL_FS4].JudgeFlag = false;
		    DisVolt(xStarFS4+16, yStarBlockingRate, BACK_COLOR_ERR, BlockingRate_4);
		
		}
		
		
//		DisVolt(xStarFS1+16, yStarBlockingRate, BACK_COLOR, BlockingRate_1);
//		DisVolt(xStarFS2+16, yStarBlockingRate, BACK_COLOR, BlockingRate_2);
//		DisVolt(xStarFS3+16, yStarBlockingRate, BACK_COLOR, BlockingRate_3);
//		DisVolt(xStarFS4+16, yStarBlockingRate, BACK_COLOR, BlockingRate_4);

		if(FS[CHANNEL_FS1].JudgeFlag == true)
		{
			DisJudge(xStarFS1,yStarJudge, BACK_COLOR,"OK");		
		}else
		{
			DisJudge(xStarFS1,yStarJudge, BACK_COLOR_ERR,"NG");		
		}

		if(FS[CHANNEL_FS2].JudgeFlag == true)
		{
			DisJudge(xStarFS2,yStarJudge, BACK_COLOR,"OK");
		}else
		{
			DisJudge(xStarFS2,yStarJudge, BACK_COLOR_ERR,"NG");
		}

		if(FS[CHANNEL_FS3].JudgeFlag == true)
		{
			DisJudge(xStarFS3,yStarJudge, BACK_COLOR,"OK");
		}else
		{
			DisJudge(xStarFS3,yStarJudge, BACK_COLOR_ERR,"NG");
		}

		if(FS[CHANNEL_FS4].JudgeFlag == true)
		{
			DisJudge(xStarFS4,yStarJudge, BACK_COLOR,"OK");
		}else
		{
			DisJudge(xStarFS4,yStarJudge, BACK_COLOR_ERR,"NG");
		}
	}

#if 1
	 else if(*event == ERROR_EVENT)
	{

		//FS[CHANNEL_FS1].JudgeFlag = false;

		if(FS[CHANNEL_FS1].JudgeFlag == true)
		{
			DisJudge(xStarFS1,yStarJudge, BACK_COLOR,"OK"); 	
		}else
		{
			DisJudge(xStarFS1,yStarJudge, BACK_COLOR_ERR,"NG"); 	
		}
		
		if(FS[CHANNEL_FS2].JudgeFlag == true)
		{
			DisJudge(xStarFS2,yStarJudge, BACK_COLOR,"OK");
		}else
		{
			DisJudge(xStarFS2,yStarJudge, BACK_COLOR_ERR,"NG");
		}
		
		if(FS[CHANNEL_FS3].JudgeFlag == true)
		{
			DisJudge(xStarFS3,yStarJudge, BACK_COLOR,"OK");
		}else
		{
			DisJudge(xStarFS3,yStarJudge, BACK_COLOR_ERR,"NG");
		}
		
		if(FS[CHANNEL_FS4].JudgeFlag == true)
		{
			DisJudge(xStarFS4,yStarJudge, BACK_COLOR,"OK");
		}else
		{
			DisJudge(xStarFS4,yStarJudge, BACK_COLOR_ERR,"NG");
		}




	}

#endif	 
	DisDrawLine();
}

#endif


void id_DisDrawLine(void)
{
	PointColor = POINT_COLOR;
	BackColor = BACK_COLOR;
	LCD_DrawLine(0,Y_IDENTIFIER,lcddev.width-1,Y_IDENTIFIER);
	LCD_DrawLine(0,Y_FS1,lcddev.width-1,Y_FS1);
	LCD_DrawLine(0,Y_FS2,lcddev.width-1,Y_FS2);
	LCD_DrawLine(0,Y_FS3,lcddev.width-1,Y_FS3);
	LCD_DrawLine(0,Y_FS4,lcddev.width-1,Y_FS4);
	LCD_DrawLine(0,Y_CONCLISION,lcddev.width-1,Y_CONCLISION);
	
	LCD_DrawLine(X_IDENTIFIER,Y_IDENTIFIER,X_IDENTIFIER,lcddev.height);
	LCD_DrawLine(X_MID,Y_IDENTIFIER,X_MID,lcddev.height);
	
	LCD_DrawLine(X_FS1,Y_CONCLISION,X_FS1,lcddev.height);
	LCD_DrawLine(X_FS2,Y_CONCLISION,X_FS2,lcddev.height);
	LCD_DrawLine(X_FS3,Y_CONCLISION,X_FS3,lcddev.height);
	LCD_DrawLine(X_FS4,Y_CONCLISION,X_FS4,lcddev.height);


}	

void id_DisDrawUI(void)
{
	PointColor = POINT_COLOR;
	BackColor = BACK_COLOR;
	LCD_Fill(0,Y_IDENTIFIER,lcddev.width-1,lcddev.height-1,BackColor);	
	id_DisDrawLine();
	Show_Str(id_xOffset-8, Y_IDENTIFIER+id_yOffset,PointColor,BackColor, "编号",32,1);
	Show_Str(id_xOffset, Y_FS1+id_yOffset,PointColor,BackColor, "FS1",32,1);
	Show_Str(id_xOffset, Y_FS2+id_yOffset,PointColor,BackColor, "FS2",32,1);
	Show_Str(id_xOffset, Y_FS3+id_yOffset,PointColor,BackColor, "FS3",32,1);
	Show_Str(id_xOffset, Y_FS4+id_yOffset,PointColor,BackColor, "FS4",32,1);
	Show_Str(id_xOffset-8, Y_CONCLISION+id_yOffset,PointColor,BackColor, "结论",32,1);

	Show_Str(X_IDENTIFIER+44, Y_IDENTIFIER+id_yOffset,PointColor,BackColor, "芯片 ID",32,1);
	Show_Str(X_MID+27, Y_IDENTIFIER+id_yOffset,PointColor,BackColor, "二维码 ID",32,1);
	


}

void id_DisJudge(u16 x, u16 y, u16 bc, u8 *str)//显示判定结果，单个 OK/NG
{
	LCD_Fill(x+5,y+5,x+95,y+45,bc);
	Show_Str(x+34,y+9,PointColor,bc, str,32,1); //显示区域：X*Y=100*50，数字32号字体，长x宽=32x16, Y:50-32)/2=9,X:(100-32)/2=34

}

void id_Display(void)
{
	 
//void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)


	u8 ChannelNum;
	
	u16 y_offset,x_offset;
	
	u8 temp_RecieveIdBuff[30];
	u8 temp_IDWriteBuff[30];
	
	y_offset=Y_FS1+id_yOffset;
	x_offset=X_IDENTIFIER;
	for(ChannelNum = 0;ChannelNum < 4;ChannelNum++)
	{	
		BCD2ASC(temp_RecieveIdBuff,FS[ChannelNum].RecieveIdBuff,6);
		BCD2ASC(temp_IDWriteBuff,FS[ChannelNum].IDWriteBuff,6);
							
		if(FS[ChannelNum].JudgeFlag == true)
		{
			LCD_ShowString(X_IDENTIFIER+4,y_offset,PointColor,BACK_COLOR,temp_RecieveIdBuff,32,0);//显示区域：x*y=200*50,数字32号字体，长x宽=32x16,ID号长度：12*16=192，剩余宽度：200-16*12=8,两遍剩余宽度：8/2=4
			LCD_ShowString(X_MID+4,y_offset,PointColor,BACK_COLOR,temp_IDWriteBuff,32,0);		
			id_DisJudge(x_offset,Y_CONCLISION, BACK_COLOR,"OK"); 	//显示区域：X*Y=100*50，数字32号字体，长x宽=32x16, Y:50-32)/2=9,X:(100-32)/2=34
		
		}
		
		else
		{
			LCD_ShowString(X_IDENTIFIER+4,y_offset,PointColor,BACK_COLOR_ERR,temp_RecieveIdBuff,32,0);//显示区域：x*y=200*50,数字32号字体，长x宽=32x16,ID号长度：12*16=192，剩余宽度：200-16*12=8,两遍剩余宽度：8/2=4
			LCD_ShowString(X_MID+4,y_offset,PointColor,BACK_COLOR_ERR,temp_IDWriteBuff,32,0);		
			id_DisJudge(x_offset,Y_CONCLISION, BACK_COLOR_ERR,"NG"); 	//显示区域：X*Y=100*50，数字32号字体，长x宽=32x16, Y:50-32)/2=9,X:(100-32)/2=34
		}
		
		y_offset+=50;
		x_offset+=100;
	
		

	}

}

