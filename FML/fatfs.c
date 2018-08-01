//文件系统
#include "fatfs.h"
#include <stdlib.h>
#include <string.h>
#include "usart.h"
#include "delay.h"
#include "rtc.h" 
#include "malloc.h"  
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"
#include "fattester.h"	 
#include "protocol_fs.h"
#include "CRC.h"

#include "message.h"
#include "rtc.h" 
#include "gui_lcd.h"
#include "lcd.h"
//#include "stmflash.h"
#include "key.h"

char FilenameBuff[100];				//文件名
char SDSaveDataBuf[200];    //需要保存的数据


char FileDirBuff[]="0:/Test Data"; //SD卡路径
char SerialBuff[] = "Latest serial number:";//21
char SerialBuff2[] ="00001 00002 00003 00004";//23
//保存的数据类型
char SDSaveDataType[] ="2018/01/08 11:37:30 1 18 48 01 0001 1F 5.000 3.300 0.500 2.500 1.500 2.000 100.0% 100.0% 100.0% 00.0 00.0\r\n";//
/*
1.是否有文件夹
2.是否有当天文件
3.有则正常读写
4.
5.
*/
void SdFatfsInit(void)//sd卡初始化并打开文件
{
	u32 total,free;
	u8 i;
	u8 res = 0;

 	while(SD_Init())//检测不到SD卡
	{
		delay_ms(500);
		printf("SD Card  Error!\r\n");	

	}
	printf("SD Card  success!\r\n");	
 	exfuns_init();							//为fatfs相关变量申请内存				 
  	f_mount(fs[0],"0:",1); 					//挂载SD卡 
	printf("SD Card f_mount success!\r\n");	
	while(exf_getfree("0",&total,&free))	//得到SD卡的总容量和剩余容量
	{
		printf("SD Card Fatfs Error!\r\n");	
		delay_ms(200);
	}													  			    	
	printf("SD Total Size:%d      MB",total);	
	printf("SD  Free Size:%d     MB",free);	

	while(f_opendir(&dir,FileDirBuff))//打开文件夹
	{
		printf("f_opendir %s文件夹错误\r\n",FileDirBuff);	
		f_mkdir(FileDirBuff);//创建目录
	}
	printf("f_opendir %s success!\r\n",FileDirBuff);
	
	sprintf((char*)FilenameBuff,"%s/%4d%02d%02d.txt",FileDirBuff,calendar.year,calendar.month,calendar.date);//文件名	
	while(f_open(file,(char *)FilenameBuff,FA_WRITE | FA_READ))	//打开文件
	{
		printf("f_open %s error!\r\n",FilenameBuff);
		f_open(file,(char *)FilenameBuff,FA_CREATE_NEW);			//创建文件	
	}
	//printf("f_open %s success!\r\n",FilenameBuff);
//*****************************************************************************************************	
//	printf("strlen SerialBuff:%d\r\n",strlen(SerialBuff));
//	printf("strlen SerialBuff2:%d\r\n",strlen(SerialBuff2));
//	printf("strlen SDSaveDataType:%d\r\n",strlen(SDSaveDataType));
	//判断新日期
	f_lseek(file,0);//定位到页首
	f_read(file,fatbuf,strlen(SerialBuff) + strlen(SerialBuff2),&br);//读取上次分配的最新序列号
	for(i=0;i<strlen(SerialBuff);i++)//判断新文件
	{
		if(fatbuf[i]!=SerialBuff[i])
		{
			res = 1;//新文件
		}
	}
	if(res)
	{
		FS_SerialBuff[CHANNEL_FS1] = 1;
		FS_SerialBuff[CHANNEL_FS2] = 2;
		FS_SerialBuff[CHANNEL_FS3] = 3;
		FS_SerialBuff[CHANNEL_FS4] = 4;
		//printf("1.%s:%05d %05d %05d %05d\r\n",SerialBuff,FS_SerialBuff[CHANNEL_FS1],FS_SerialBuff[CHANNEL_FS2],FS_SerialBuff[CHANNEL_FS3],FS_SerialBuff[CHANNEL_FS4]);
	}
	else
	{
		f_lseek(file,0);//定位到页首
		f_read(file,fatbuf,(strlen(SerialBuff) + strlen(SerialBuff2)),&br);//读取上次分配的最新序列号
		//printf("%s",fatbuf);
		//printf("\r\n");
		FS_SerialBuff[CHANNEL_FS1] = (fatbuf[strlen(SerialBuff)+0]-'0')*10000 + (fatbuf[strlen(SerialBuff)+1]-'0')*1000 + (fatbuf[strlen(SerialBuff)+2]-'0')*100 + (fatbuf[strlen(SerialBuff)+3]-'0')*10 + (fatbuf[strlen(SerialBuff)+4]-'0');
		FS_SerialBuff[CHANNEL_FS2] = (fatbuf[strlen(SerialBuff)+6]-'0')*10000 + (fatbuf[strlen(SerialBuff)+7]-'0')*1000 + (fatbuf[strlen(SerialBuff)+8]-'0')*100 + (fatbuf[strlen(SerialBuff)+9]-'0')*10 + (fatbuf[strlen(SerialBuff)+10]-'0');
		FS_SerialBuff[CHANNEL_FS3] = (fatbuf[strlen(SerialBuff)+12]-'0')*10000 + (fatbuf[strlen(SerialBuff)+13]-'0')*1000 + (fatbuf[strlen(SerialBuff)+14]-'0')*100 + (fatbuf[strlen(SerialBuff)+15]-'0')*10 + (fatbuf[strlen(SerialBuff)+16]-'0');
		FS_SerialBuff[CHANNEL_FS4] = (fatbuf[strlen(SerialBuff)+18]-'0')*10000 + (fatbuf[strlen(SerialBuff)+19]-'0')*1000 + (fatbuf[strlen(SerialBuff)+20]-'0')*100 + (fatbuf[strlen(SerialBuff)+21]-'0')*10 + (fatbuf[strlen(SerialBuff)+22]-'0');
		//printf("%d %d %d %d %d |%d %d %d %d %d |%d %d %d %d %d |%d %d %d %d %d\r\n",\
			fatbuf[strlen(SerialBuff)+1],fatbuf[strlen(SerialBuff)+2],fatbuf[strlen(SerialBuff)+3],fatbuf[strlen(SerialBuff)+4],fatbuf[strlen(SerialBuff)+5],\
			fatbuf[strlen(SerialBuff)+6],fatbuf[strlen(SerialBuff)+7],fatbuf[strlen(SerialBuff)+8],fatbuf[strlen(SerialBuff)+9],fatbuf[strlen(SerialBuff)+10],\
			fatbuf[strlen(SerialBuff)+12],fatbuf[strlen(SerialBuff)+13],fatbuf[strlen(SerialBuff)+14],fatbuf[strlen(SerialBuff)+15],fatbuf[strlen(SerialBuff)+16],\
			fatbuf[strlen(SerialBuff)+18],fatbuf[strlen(SerialBuff)+19],fatbuf[strlen(SerialBuff)+20],fatbuf[strlen(SerialBuff)+21],fatbuf[strlen(SerialBuff)+22]);
		//printf("2.%s%05d %05d %05d %05d\r\n",SerialBuff,FS_SerialBuff[CHANNEL_FS1],FS_SerialBuff[CHANNEL_FS2],FS_SerialBuff[CHANNEL_FS3],FS_SerialBuff[CHANNEL_FS4]);
	}


}
void save(void)
{
//	u8 res;
    //u8 i;
	u8 channel;
	u8 buff[100];
	
	f_lseek(file,0);//定位到页首
	sprintf((char*)buff,"Latest serial number:%05d %05d %05d %05d",FS_SerialBuff[0],FS_SerialBuff[1],FS_SerialBuff[2],FS_SerialBuff[3]);
	f_write(file,buff,strlen(SerialBuff)+strlen(SerialBuff2),&bw);//存储最新分配的序列号
          
	f_lseek(file,f_size(file));//定位到页尾
	for(channel=0;channel<NUM_CHANNELS;channel++)
	{
		if(FS[channel].ChannelConnect)
		{

			//                                            5V     3.3V    
 //2017/12/11 11:37:30 || 17 48 01 0001 1F || 5.000V 3.300V 0.500V 2.500V 1.500V 2.000V 50.0% 50.0% 00.0C 00.0C
			sprintf((char*)SDSaveDataBuf,"\r\n%04d/%02d/%02d %02d:%02d:%02d %d %02X %02X %02X %02X%02X %02X %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f %05.1f%% %05.1f%% %05.1f%% %04.1f %04.1f",\
				calendar.year,calendar.month,calendar.date,calendar.hour,calendar.min,calendar.sec,(channel+1),\
				FS[channel].RecieveIdBuff[0],FS[channel].RecieveIdBuff[1],FS[channel].RecieveIdBuff[2],\
                FS[channel].RecieveIdBuff[3],FS[channel].RecieveIdBuff[4],FS[channel].RecieveIdBuff[5],\
				(float)(FS[channel].V50Volt)/1000,(float)(FS[channel].V33Volt)/1000,(float)(FS[channel].MinVolt)/1000,(float)(FS[channel].MaxVolt)/1000,\
                (float)(FS[channel].MidVolt_tp2)/1000,(float)(FS[channel].MidVolt_tp1)/1000,(float)FS[channel].PwmDuty,(float)(FS[channel].BlockingRateBuff[0]),\
                (float)(FS[channel].PidDuty)/10,(float)FS[channel].TempertureValueBuff[0],(float)FS[channel].TempertureValueBuff[1]);
			f_write(file,SDSaveDataBuf,strlen(SDSaveDataType),&bw);
            //printf("\r\nstrlen(SDSaveDataType)=%d \r\n",strlen(SDSaveDataType));
			//printf("%s \r\n",SDSaveDataBuf);
            
		}

	}
	
	f_sync(file);//刷新，即保存

}






