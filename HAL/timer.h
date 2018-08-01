#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"


extern vu8 TimerFlag;

void TIM2_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
//void TIM4_Int_Init(u16 arr,u16 psc);
//void TIM4_Cap_Init(u16 arr,u16 psc);
void TIM5_Int_Init(u16 arr,u16 psc);
#endif
