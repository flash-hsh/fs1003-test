#include "serial.h"
#include "protocol_fs.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>   
u16 SerialNumBuff[NUM_CHANNELS];//存储SD卡读到的4个序列号，或新生产的序列号
//对输入数组内容重小到大排序，冒泡法
void paixu(u16 input[],u8 len)
{
	u8 i,j,temp;
	for(j=0;j<len;j++) 
	{
		for(i=0;i<len-1-j;i++)
		{
			if(input[i]>input[i+1]) /* 由小到大,由大到小时改为< */
			{
				temp = input[i];
				input[i] = input[i+1];
				input[i+1] = temp;
			}			
		}
	}	
}

/*
根据原始数据和已经使用的数据生产新的数据
input[]:原始数组
del[]: 已经使用的数据，需要删去
*/
void SerialCreatNew(u16 input[],u16 del[],u8 len)
{
	u8 i=0;
	u8 j=1;

//	printf("\r\ninput data old:");
//	for(i=0;i<len;i++)
//	{
//		printf("%d ",input[i]);
//	}
	
	for(i=0;i<len;i++)
	{
		if(input[i]==del[i])
		{
			input[i] = input[len-1] + j;
			j++;
		}
	}
	paixu(input,len);//从小到大排序

//	printf("\r\ninput data new:");
//	for(i=0;i<len;i++)
//	{
//		printf("%d ",input[i]);
//	}
}

//如果返回值<0，则表明str1小于str2

//如果返回值，如果> 0，则表明str2小于str1

//如果返回值= 0，则表明str1等于str2

u8 array_cmp(u8 *a, u8 *b,u8 Length )
{
    return memcmp(a,b,Length); //返回结果。
}


