#ifndef __BUZZER_H
#define __BUZZER_H	 
#include "sys.h"

#define buzzer_ON   TIM_Cmd(TIM12, ENABLE)
#define buzzer_OFF 	TIM_Cmd(TIM12, DISABLE)

//extern bool BuzzerEn;

void buzzerInit(u32 arr,u32 psc);
//·äÃùÆ÷ÏìµÄ´ÎÊý
void buzzerRingNum(u8 num);	



#endif




