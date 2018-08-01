#ifndef __DISPLAY_H
#define __DISPLAY_H		
#include "sys.h"	

extern bool DisplayUpdataEnable;

//*********************************
#define xStarFS1 		80
#define xStarFS2 		180
#define xStarFS3 		280
#define xStarFS4 		380

#define yStar3V3 		16
#define yStarTp1 		54
#define yStarTp2 		92
#define yStarProg 		130
#define yStarVmax 		168
#define yStarVmin 		206
#define yStarBlockingRate 	244
#define yStarJudge		282

//#define yStar3V3 		0
//#define yStarTp1 		40
//#define yStarTp2 		80
//#define yStarProg 		120
//#define yStarVmax 		160
//#define yStarVmin 		200
//#define yStarBlockingRate 	240
//#define yStarJudge		280

#define X_IDENTIFIER 		80
#define X_MID 				280

#define X_FS1 		80
#define X_FS2 		180
#define X_FS3 		280
#define X_FS4 		380

#define Y_IDENTIFIER 	20
#define Y_FS1 			70
#define Y_FS2			120
#define Y_FS3 			170
#define Y_FS4 			220
#define Y_CONCLISION 	270

#define id_xOffset			16
#define id_yOffset			9



#define xOffset			2
#define yOffset			3
//************************
void VoltChange(u8* tp,u16 buff);
void VoltChanges(void);
void VoltChanges1(void);
void DisWaiting(void);
void DisDrawLine(void);
void DisDrawUI(void);
void Display(int *event);
void DisJudge(u16 x, u16 y, u16 bc, u8 *str);//显示判定结果，单个 OK/NG

void id_DisDrawLine(void);
void id_DisDrawUI(void);
void id_Display(void);
void id_DisJudge(u16 x, u16 y, u16 bc, u8 *str);//显示判定结果，单个 OK/NG


#endif
