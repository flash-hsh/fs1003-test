#include "key.h"
#include "motor.h"
//按键全局变量
vu16 KeyCurrent = 0;		//当前按键有效
vu16 KeyOld = 0;			//保留旧的按键状态值
vu16 KeyNoChangedTime = 0;	//滤波时间
vu16 KeyPress = 0;			//按键被按下
vu16 KeyDown = 0;			//按键下降沿
vu16 KeyUp = 0;				//按键上升沿
vu16 KeyLast = 0;
vu16 isKeyClicked = 0;		//按键单击标志
vu16 isKeyPressLong = 0;	//按键长按标志

vu16 KeyPressTimeCnt_top = 0;	//长按时间计数 10ms
#define KeyPress_5s 	400		//5s

//按键初始化函数
void KEY_Init(void) 
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOA,GPIOE时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //KEY0 KEY1 KEY2对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3; //KEY0 KEY1 KEY2对应引脚
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //KEY0 KEY1 KEY2对应引脚
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
}
//按键扫描函数
void TASK_KeyScan(void)
{
    (!KeyStar)		?(KeyCurrent |= 0x0001):(KeyCurrent &= 0xFFFE);
    (!KeyModeTest)	?(KeyCurrent |= 0x0002):(KeyCurrent &= 0xFFFD);
    (!KeyModeProg)	?(KeyCurrent |= 0x0004):(KeyCurrent &= 0xFFFB);
    (!KeyModeCali)	?(KeyCurrent |= 0x0008):(KeyCurrent &= 0xFFF7);
    (!KeyU)			?(KeyCurrent |= 0x0010):(KeyCurrent &= 0xFFEF);
    (!KeyD)			?(KeyCurrent |= 0x0020):(KeyCurrent &= 0xFFDF);
    (!KeyL)			?(KeyCurrent |= 0x0040):(KeyCurrent &= 0xFFBF);
    (!KeyR)			?(KeyCurrent |= 0x0080):(KeyCurrent &= 0xFF7F);
    (!KeyT)			?(KeyCurrent |= 0x0100):(KeyCurrent &= 0xFEFF);
    (!MotorDetLimiDown_Pin)			?(KeyCurrent |= 0x0200):(KeyCurrent &= 0xFDFF);
    (!MotorDetLimiUp_Pin)			?(KeyCurrent |= 0x0400):(KeyCurrent &= 0xFBFF);
    (!KeyPneumatic)					?(KeyCurrent |= 0x0800):(KeyCurrent &= 0xF7FF);
    (!KeyScann)						?(KeyCurrent |= 0x1000):(KeyCurrent &= 0xEFFF);

    if(KeyCurrent!=KeyOld)
    {
        KeyNoChangedTime = 0;
        KeyOld = KeyCurrent;
        return;
    }
    else
    {
        KeyNoChangedTime++;
        if(KeyNoChangedTime>=2)		//滤波
        {
            KeyNoChangedTime = 2;
            KeyPress = KeyOld;
            KeyDown |= (~KeyLast)&(KeyPress);
            KeyUp   |= KeyLast&(~KeyPress);
            KeyLast = KeyPress;
        }
    }
//*按键单击***************************************
    if((KEY_START & KeyUp) && (KEY_START & KeyDown))
    {
        KeyUp &= (~KEY_START);
        KeyDown &= (~KEY_START);
        isKeyClicked |= KEY_START;
        return;
    }

    if((KEY_MODETEST & KeyUp) && (KEY_MODETEST & KeyDown))
    {
        KeyUp &= (~KEY_MODETEST);
        KeyDown &= (~KEY_MODETEST);
        isKeyClicked |= KEY_MODETEST;
        return;
    }   

    if((KEY_MODEPROG & KeyUp) && (KEY_MODEPROG & KeyDown))
    {
        KeyUp &= (~KEY_MODEPROG);
        KeyDown &= (~KEY_MODEPROG);
        isKeyClicked |= KEY_MODEPROG;
       return;
    } 

    if((KEY_MODECALI & KeyUp) && (KEY_MODECALI & KeyDown))
    {
        KeyUp &= (~KEY_MODECALI);
        KeyDown &= (~KEY_MODECALI);
        isKeyClicked |= KEY_MODECALI;
        return;
    }

    if((KEY_UP & KeyUp) && (KEY_UP & KeyDown))
    {
        KeyUp &= (~KEY_UP);
        KeyDown &= (~KEY_UP);
        isKeyClicked |= KEY_UP;
        return;
    }   

    if((KEY_DOWN & KeyUp) && (KEY_DOWN & KeyDown))
    {
        KeyUp &= (~KEY_DOWN);
        KeyDown &= (~KEY_DOWN);
        isKeyClicked |= KEY_DOWN;
        return;
    } 	

    if((KEY_LEFT & KeyUp) && (KEY_LEFT & KeyDown))
    {
        KeyUp &= (~KEY_LEFT);
        KeyDown &= (~KEY_LEFT);
        isKeyClicked |= KEY_LEFT;
        return;
    } 	

    if((KEY_RIGHT & KeyUp) && (KEY_RIGHT & KeyDown))
    {
        KeyUp &= (~KEY_RIGHT);
        KeyDown &= (~KEY_RIGHT);
        isKeyClicked |= KEY_RIGHT;
        return;
    } 	

    if((KEY_TOP & KeyUp) && (KEY_TOP & KeyDown))
    {
        KeyUp &= (~KEY_TOP);
        KeyDown &= (~KEY_TOP);
        isKeyClicked |= KEY_TOP;
		return;
    } 

    if((KEY_PNEUMATIC & KeyUp) && (KEY_PNEUMATIC & KeyDown))
    {
        KeyUp &= (~KEY_PNEUMATIC);
        KeyDown &= (~KEY_PNEUMATIC);
        isKeyClicked |= KEY_PNEUMATIC;
		return;
    } 

    if((KEY_SCANNING & KeyUp) && (KEY_SCANNING & KeyDown))
    {
        KeyUp &= (~KEY_SCANNING);
        KeyDown &= (~KEY_SCANNING);
        isKeyClicked |= KEY_SCANNING;
		return;
    } 

	
//*按键长按*******************************************************
	if(KeyPress & KEY_TOP)
	{
		KeyPressTimeCnt_top++;
		if(KeyPressTimeCnt_top >= KeyPress_5s)
		{
			KeyPressTimeCnt_top = 0;
			isKeyPressLong |= KEY_TOP;
		}
	}
}	

