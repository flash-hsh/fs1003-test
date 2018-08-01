#include "key.h"
#include "motor.h"
//����ȫ�ֱ���
vu16 KeyCurrent = 0;		//��ǰ������Ч
vu16 KeyOld = 0;			//�����ɵİ���״ֵ̬
vu16 KeyNoChangedTime = 0;	//�˲�ʱ��
vu16 KeyPress = 0;			//����������
vu16 KeyDown = 0;			//�����½���
vu16 KeyUp = 0;				//����������
vu16 KeyLast = 0;
vu16 isKeyClicked = 0;		//����������־
vu16 isKeyPressLong = 0;	//����������־

vu16 KeyPressTimeCnt_top = 0;	//����ʱ����� 10ms
#define KeyPress_5s 	400		//5s

//������ʼ������
void KEY_Init(void) 
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOA,GPIOEʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //KEY0 KEY1 KEY2��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3; //KEY0 KEY1 KEY2��Ӧ����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //KEY0 KEY1 KEY2��Ӧ����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
}
//����ɨ�躯��
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
        if(KeyNoChangedTime>=2)		//�˲�
        {
            KeyNoChangedTime = 2;
            KeyPress = KeyOld;
            KeyDown |= (~KeyLast)&(KeyPress);
            KeyUp   |= KeyLast&(~KeyPress);
            KeyLast = KeyPress;
        }
    }
//*��������***************************************
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

	
//*��������*******************************************************
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

