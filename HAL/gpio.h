#ifndef __GPIO_H
#define __GPIO_H	 
#include "sys.h"

#define ProgKey1 PEout(0)
#define ProgKey2 PEout(1)
#define ProgKey3 PEout(2)
#define ProgKey4 PEout(3)


#define uartModePin PFout(2)	//输出1：扫码模式，0：FS测试

#define PNEUMATIC_VALVE		PFout(15)	//气压阀  0: 关  1：开

void GpioConfiguration(void);
#endif
