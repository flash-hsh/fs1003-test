#ifndef _SERIAL_H_
#define _SERIAL_H_
#include "sys.h"

void paixu(u16 input[],u8 len);
void SerialCreatNew(u16 input[],u16 del[],u8 len);
void SerialDeal(void);
u8 array_cmp(u8 *a,  u8 *b, u8 Length );

#endif
