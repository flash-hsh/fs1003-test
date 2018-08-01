#ifndef __GPIO_H
#define __GPIO_H	 
#include "sys.h"

#define ProgKey1 PEout(0)
#define ProgKey2 PEout(1)
#define ProgKey3 PEout(2)
#define ProgKey4 PEout(3)


#define uartModePin PFout(2)	//���1��ɨ��ģʽ��0��FS����

#define PNEUMATIC_VALVE		PFout(15)	//��ѹ��  0: ��  1����

void GpioConfiguration(void);
#endif
