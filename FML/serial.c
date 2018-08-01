#include "serial.h"
#include "protocol_fs.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>   
u16 SerialNumBuff[NUM_CHANNELS];//�洢SD��������4�����кţ��������������к�
//����������������С��������ð�ݷ�
void paixu(u16 input[],u8 len)
{
	u8 i,j,temp;
	for(j=0;j<len;j++) 
	{
		for(i=0;i<len-1-j;i++)
		{
			if(input[i]>input[i+1]) /* ��С����,�ɴ�Сʱ��Ϊ< */
			{
				temp = input[i];
				input[i] = input[i+1];
				input[i+1] = temp;
			}			
		}
	}	
}

/*
����ԭʼ���ݺ��Ѿ�ʹ�õ����������µ�����
input[]:ԭʼ����
del[]: �Ѿ�ʹ�õ����ݣ���Ҫɾȥ
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
	paixu(input,len);//��С��������

//	printf("\r\ninput data new:");
//	for(i=0;i<len;i++)
//	{
//		printf("%d ",input[i]);
//	}
}

//�������ֵ<0�������str1С��str2

//�������ֵ�����> 0�������str2С��str1

//�������ֵ= 0�������str1����str2

u8 array_cmp(u8 *a, u8 *b,u8 Length )
{
    return memcmp(a,b,Length); //���ؽ����
}


