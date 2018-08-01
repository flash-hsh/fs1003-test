#include "crc.h"

//º∆À„Modbus CRC16
uint16_t CRC16 (uint8_t *puchMsg, uint8_t u8_Length ) /* The function returns the CRC as a unsigned short type */
{
	uint16_t u16_Reg_CRC = 0xFFFF;
	uint8_t i,j = 0;
	uint8_t u8_Temp_Reg = 0;
	for (i=0;i<u8_Length;i++)
	{
		u16_Reg_CRC ^= *puchMsg++;
		for (j = 0; j < 8; j++)
		{
			if (u16_Reg_CRC & 0x0001)
			{
				u16_Reg_CRC = u16_Reg_CRC >> 1 ^ 0xA001;
			}
			else
			{
				u16_Reg_CRC >>= 1;
			}
		}
	}
	u8_Temp_Reg = u16_Reg_CRC >> 8;
	return (u16_Reg_CRC<<8 | u8_Temp_Reg );
}
