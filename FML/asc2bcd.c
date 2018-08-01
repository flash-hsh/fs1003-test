#include <assert.h>
#include <asc2bcd.h>
//#include "utils.h"
//基于查表实现BCD与Ascii之间的转换
static uint8_t bcd2ascii[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static uint8_t ascii2bcd1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
static uint8_t ascii2bcd2[6]  = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

uint32_t ASC2BCD(uint8_t *bcd, const uint8_t *asc, uint32_t len)
{
    uint8_t c = 0;
    uint8_t index = 0;
    uint8_t i = 0;  
    
    len >>= 1;    

    for(; i < len; i++) {
        //first BCD
        if(*asc >= 'A' && *asc <= 'F') {
            index = *asc - 'A'; 
            c  = ascii2bcd2[index] << 4;
        } else if(*asc >= '0' && *asc <= '9') {
            index = *asc - '0';
            c  = ascii2bcd1[index] << 4;
        }
        asc++;

        //second BCD
        if(*asc >= 'A' && *asc <= 'F') {
            index = *asc - 'A'; 
            c  |= ascii2bcd2[index];
        } else if(*asc >= '0' && *asc <= '9') {
            index = *asc - '0';
            c  |= ascii2bcd1[index];
        }
        asc++;

        *bcd++ = c;
    }   

    return 0;
}



uint32_t BCD2ASC (uint8_t *asc, const uint8_t *bcd, uint32_t len)  
{  
    uint8_t c = 0;  
    uint8_t i;  
  
  
    for(i = 0; i < len; i++) {  
        //first BCD  
        c = *bcd >> 4;  
        *asc++ = bcd2ascii[c];  
  
  
        //second  
        c = *bcd & 0x0f;  
        *asc++ = bcd2ascii[c];  
        bcd++;  
    }  
  
  
    return 0;  
}  


