/*BCD 与 ASCII码转换*/  
#ifndef __ASC2BCD_H
#define __ASC2BCD_H	 
 
#include<sys.h> 
  
  
  
  
/******************************************************************* 
函数名:  asc2bcd  
功能描述:将ascii码转换为bcd码 
参数:     
         bcd:转换后的BCD码 
         asc:需转换的ASCII码串 
         len:需转换的ascii码串长度 
 
返回值:  uint32  
                              0：成功 
                              其他:失败 
********************************************************************/  
uint32_t ASC2BCD(uint8_t *bcd, const uint8_t *asc, uint32_t len);  
  
/******************************************************************* 
函数名: bcd2asc  
功能描述:将bcd码转换为ascii码串 
参数:     
         asc:转换的ASCII码串 
         bcd:需转换的BCD码 
         len:需转换的BCD码长度 
 
返回值:  uint32  
                              0：成功 
                              其他:失败 
********************************************************************/  
uint32_t BCD2ASC (uint8_t *asc, const uint8_t *bcd, uint32_t len);  

#endif

