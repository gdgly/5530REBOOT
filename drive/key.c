/******************** (C) COPYRIGHT 2015 AVER********************
 * �ļ���  ��KEY.C
 * ����    ������
 * ����    ��3*2�������ɨ��
 * ����    ��ɨ����ɺ����ֵ
 * Ӳ�����ӣ�һ·3*2�������  һ·��������������
 * �޸����ڣ�2015-08-18
********************************************************************/
#include "my_register.h"
#include "beep.h"
#include "flash.h"
#include "gpio.h"
#include "key.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include  "gui.h"
#include "MainTask.h"
#include "ff.h"			/* FatFS�ļ�ϵͳģ��*/
#include "usbh_bsp_msc.h"
#include "flash_if.h"

extern WM_HWIN CreateR(void);
extern WM_HWIN CreateWindow2(void);
extern WM_HWIN CreateWindow(void);
//extern WM_HWIN hWinWind;
extern WM_HWIN hWinR;
//extern WM_HWIN load_wind;//负载界面句柄
//=================================================================
extern struct bitDefine
{
    unsigned bit0: 1;
    unsigned bit1: 1;
    unsigned bit2: 1;
    unsigned bit3: 1;
    unsigned bit4: 1;
    unsigned bit5: 1;
    unsigned bit6: 1;
    unsigned bit7: 1;
} flagA, flagB,flagC,flagD,flagE,flagF,flagG;
//====================================================================
FRESULT result;
FATFS fs;
FIL file;
//IAP
u8 Jumpflag=0;

UINT fnum;            					  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
static  u8    filedatabuf[2048];
static  UINT   readcount;

vu8 Flag_SETVGE;
vu8 Flag_SETVSHI;
vu8 Flag_SETVBAI;
vu8 Flag_SETVQIAN;
vu8 Flag_SETAGE;
vu8 Flag_SETASHI;
vu8 Flag_SETABAI;
vu8 Flag_SETAQIAN;

#define MAX_VOL  3200		
#define MAX_CUR  5100	
vu8 t_KeyScan;
vu8 NewKey;
vu8 t_wei;
vu8 t_beep;
vu8 t_Str;
vu8 t_lock;
vu8 t_onoff;
vu8 t_doub;
vu8 t_key_step;//键盘扫描计数器
vu16 Key1,Key2,Key3,Key4,Key5,Key6,Key7,Key8;
vu32 Key9;
vu16 KeyCounter;
vu16 MAX_limit;//�Զ���������ֵ
vu16 MIN_limit;//�Զ���������ֵ
vu16 Test_Time;
vu16 Test_Time1;
vu16 Time_sw;//��ʱ
vu16 Test_Daley;//�Զ������жϵȴ�ʱ�䣬��ʱ���û�������
vu16 Test_C_Door;//�Զ������ż�����
vu8 OFFSET_Zore;//���ֵ
vu32 Key_Scan(void);
void Lift_Move(void);
void Right_Move(void);
void Setvalue_Add(void);
/* 键值定义 */
#define        KEY_1                		 0X4E     //
#define        KEY_2                		 0X56     //
#define        KEY_3                		 0X5E     //
#define        KEY_4                		 0X57      //
#define        KEY_5                		 0X5F      //
#define        KEY_6                		 0X76      //
#define        KEY_7                		 0X67     //
#define        KEY_8                		 0X6F      //
#define        KEY_9                		 0X77     //
#define        KEY_0                		 0X5D      //
#define        KEY_dian              		 0X65     //
#define        KEY_ESC              		 0X6D     //

#define        ENTER                     0X4D    //
#define        PUSH_Lift                 0X45     //
#define        PUSH_Right                0X55      //
#define        PUSH_Up                   0X46      //
#define        PUSH_Down                 0X74      //
#define        PUSH_DISP                 0X47      //
#define        PUSH_SETUP                0X4F     //

#define        KEY_Lock                  0X75     //
#define        KEY_BIAS                  0X6E      //
#define        KEY_Reset                 0X66      //
#define        KEY_TRIG                  0X75      //

#define        KEY_Powe                  0X6C     //
#define        KEY_Face1                 0X64      //
#define        KEY_Face2                 0X5c      //
#define        KEY_Face3                 0X54      //
#define        KEY_Face4                 0X4c      //
#define        KEY_Face5                 0X44      //

/* 定义按键动作 */
#define        KEY_NULL_VALUE             0X00    //�޶���
#define        KEY_PRESS                  0X10    //����
#define        KEY_LONG                   0X20    //����
#define        KEY_CONTINUE               0X40    //����
#define        KEY_UP                     0X80    //����̧��
/*定义按键处理步骤*/
#define        KEY_INIT_STATE               0      //��̬
#define        KEY_WOBBLE_STATE             1      //����
#define        KEY_PRESS_STATE              2      //��������
#define        KEY_LONG_STATE               3      //��������
#define        KEY_CONTINUE_STATE           4      //��������
#define        KEY_RELEASE_STATE            5      //�ָ���̬
/* 长按时间 */
#define         KEY_LONG_PERIOD             50                /* ����ʱ��1S */
#define        KEY_CONTINUE_PERIOD          10                /* ˫��ʱ��500ms */

/************************************************************************************************************************/
vu32 Key_Scan(void)
{
	static vu8 KeyState = KEY_INIT_STATE;
	static vu8 IsKeyRelease = 1;

	static vu8 KeyValueTemp = 0;
	vu8 KeyValue ;
	KeyValue= KEY_NULL_VALUE;
	
	if((NewKey>0x37)&&(IsKeyRelease==1))
	{
		if(KeyState == KEY_INIT_STATE)                    /*�ް�������*/
		{
			KeyState = KEY_WOBBLE_STATE;
			return KEY_NULL_VALUE;
		}
		else if(KeyState == KEY_WOBBLE_STATE)                /* ���� */
		{
			KeyState = KEY_PRESS_STATE;
			return KEY_NULL_VALUE;
		}
		else if(KeyState == KEY_PRESS_STATE)                /* �а������£����ذ���ֵ */
		{
			KeyValue = NewKey;
			KeyValueTemp = KeyValue;
			KeyState = KEY_CONTINUE_STATE;
			return KeyValue;
		}
		else if(KeyState == KEY_CONTINUE_STATE)           /*˫������*/
		{
			KeyCounter ++;
			if(KeyCounter == KEY_CONTINUE_PERIOD)
			{
				 KeyCounter = 0;
				 KeyState = KEY_LONG_STATE;
			}
		}
		else if(KeyState == KEY_LONG_STATE)                        /* �������� */
		{
			KeyCounter ++;
			if(KeyCounter == KEY_LONG_PERIOD)
			{
				KeyCounter = 0;
				Flag_Long=1; //����������־λ
			}
		}
	}
	else if(NewKey<=0x37)
	{
		KeyState = KEY_INIT_STATE;        /* �󴥷������ص���ʼ״̬ */
		IsKeyRelease = 1;
		Flag_Long=0;
	}
	return KEY_NULL_VALUE;
}

void GetFlashSize(void)
{
    static u16 Stm32_Flash_Size;
    
    Stm32_Flash_Size = (*(__IO u16*)(0x1FFF7A22));//���������Ĵ���
    
    printf("оƬ����������СΪ%dK\r\n",Stm32_Flash_Size);

}

void JumpBoot(u8 flag)
{
  	void (*pUserApp)(void);
  uint32_t JumpAddress;
	if(flag==55)
  {		
	__asm("CPSID  I");
        
		JumpAddress = *(volatile uint32_t*) (USER_FLASH_FIRST_PAGE_ADDRESS+4);
		pUserApp = (void (*)(void)) JumpAddress;
		TIM_Cmd(TIM1, DISABLE);	
		TIM_DeInit(TIM1);
		TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE);
		TIM_Cmd(TIM2, DISABLE);	
		TIM_DeInit(TIM2);
		TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
		TIM_Cmd(TIM4,DISABLE);
		TIM_DeInit(TIM4);
		TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
		TIM_Cmd(TIM6, DISABLE);	
	    TIM_DeInit(TIM6);
		TIM_ITConfig(TIM6,TIM_IT_Update,DISABLE);
		USART_DeInit(USART1);
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);		
		USART_Cmd(USART1,DISABLE);
	    USART_DeInit(USART3);
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);		
		USART_Cmd(USART3,DISABLE);
		RCC_DeInit();
		RCC_RTCCLKCmd(DISABLE);
		EXTI_DeInit();
		SysTick->CTRL = 0;
		RTC_DeInit();
		RTC_ITConfig(RTC_IT_WUT,DISABLE);//�ر�WAKE UP ��ʱ���ж�
		RTC_WakeUpCmd( DISABLE);//�ر�WAKE UP ��ʱ����
		__disable_irq();
		NVIC_DisableIRQ(OTG_FS_IRQn);
		NVIC_DisableIRQ(OTG_FS_WKUP_IRQn);
		NVIC_DisableIRQ(OTG_HS_IRQn);
		NVIC_DisableIRQ(OTG_HS_WKUP_IRQn);
		__ASM volatile ("cpsid i");
		/* Initialize user application's Stack Pointer */
		__set_PSP(*(volatile uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
		__set_CONTROL(0);
		__set_MSP(*(volatile uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
		
        
		
//		NVIC_SystemReset();
		pUserApp();
	}
}


uint8_t File_IAP(void)
{
  FRESULT res;
	static u8 signal;
	void (*pUserApp)(void);
  uint32_t JumpAddress;
	u16 i=0;
	u32 wr_state=0;
	static uint32_t fsize;
    result = f_mount(&fs, "2:", 0);
	if(result!=FR_OK)
	{
 //    LCD_ErrLog("f_open->updata.bin->error.\n");
     goto updateerror;
   }
	result=f_open(&file, "2:UPDATE.BIN",FA_OPEN_EXISTING | FA_READ);
	if(result!=FR_OK)
	{
 //    LCD_ErrLog("f_open->updata.bin->error.\n");
     goto updateerror;
  }
//	LCD_UsrLog("f_open->updata.bin->OK!.\n");
	
//	LCD_UsrLog("f_read->updata.bin->..ing.\n");
	fsize=f_size(&file);//unit byte
//	if(fsize>131072)    //128*1024
//	{
//	 // LCD_ErrLog("This firmware is out of limit\n");   
//		while(1)
//		{
//		            //error
//		}
//	}
	//LCD_UsrLog("fsize:%dKB\n",fsize/1024); //��Ϊֻ����������5,����,�̼����ܳ���128KB
	                                      //���и���̼�����,������������̼�ƥ��Ĵ�С
 // LCD_UsrLog(">Erase.....ing\n");
	if(IAP_Start())
	{ 
	//  LCD_ErrLog(">Erase......error\n");
	  goto updateerror;
	}
//	LCD_UsrLog(">Erase.........ok\n");
	
	while(1)
	{
		do{
		 result=f_read(&file,filedatabuf,2048, &readcount);
     if(result!=FR_OK)
		 {
     //  LCD_UsrLog("f_read->updata.bin->error.\n");
			 goto updateerror;
     }
		 if(signal == 0)
		 {
			 TM1650_SET_LED(0x68,0x70);
			 GPIO_ResetBits(GPIOD,GPIO_Pin_12);//���
			 signal = 1;
		 }else{
			 GPIO_ResetBits(GPIOD,GPIO_Pin_12);
                TM1650_SET_LED(0x48,0x71);
                TM1650_SET_LED(0x68,0xF2);//PASS��
			 signal = 0;
		 }
		 
		 wr_state=IAP_Flash_Write(USER_FLASH_FIRST_PAGE_ADDRESS+i*2048,filedatabuf,readcount/4);
		 if(0!=wr_state) 
		 {
			// LCD_UsrLog("Flash.........error\n");
			 goto updateerror;
		 } 
		 i++;
	 }while(f_eof(&file)==0);//�����ļ�ĩβ

	//  LCD_UsrLog("ffok...............%dKB\n",fsize/1024);

   // LCD_UsrLog("ffok............COMPLETE\n");
	 // LCD_UsrLog("Jump to the application...\n");
	  Jumpflag = 55;
//	  I2C_EE_BufferWrite(&Jumpflag, EEP_Firstpage, 1);
	  goto updateerror;
  }

	  updateerror:
	  __asm("CPSID  I");
        
		JumpAddress = *(volatile uint32_t*) (USER_FLASH_FIRST_PAGE_ADDRESS+4);
		pUserApp = (void (*)(void)) JumpAddress;
		TIM_Cmd(TIM1, DISABLE);	
		TIM_DeInit(TIM1);
		TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE);
		TIM_Cmd(TIM2, DISABLE);	
		TIM_DeInit(TIM2);
		TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
		TIM_Cmd(TIM4,DISABLE);
		TIM_DeInit(TIM4);
		TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
		TIM_Cmd(TIM6, DISABLE);	
	    TIM_DeInit(TIM6);
		TIM_ITConfig(TIM6,TIM_IT_Update,DISABLE);
		USART_DeInit(USART1);
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);		
		USART_Cmd(USART1,DISABLE);
	    USART_DeInit(USART3);
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);		
		USART_Cmd(USART3,DISABLE);
		RCC_DeInit();
		RCC_RTCCLKCmd(DISABLE);
		EXTI_DeInit();
		SysTick->CTRL = 0;
		RTC_DeInit();
		RTC_ITConfig(RTC_IT_WUT,DISABLE);//�ر�WAKE UP ��ʱ���ж�
		RTC_WakeUpCmd( DISABLE);//�ر�WAKE UP ��ʱ����
		__disable_irq();
		NVIC_DisableIRQ(OTG_FS_IRQn);
		NVIC_DisableIRQ(OTG_FS_WKUP_IRQn);
		NVIC_DisableIRQ(OTG_HS_IRQn);
		NVIC_DisableIRQ(OTG_HS_WKUP_IRQn);
		__ASM volatile ("cpsid i");
		/* Initialize user application's Stack Pointer */
		__set_PSP(*(volatile uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
		__set_CONTROL(0);
		__set_MSP(*(volatile uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
		
        
		
//		NVIC_SystemReset();
		pUserApp();
		/*************************Jump to User Application area end***************************/
	
    return 0;

}

/***************************************************************************************************************************/
void Key_Funtion(void)
{
	vu32 KeyValue = 0;
	static vu8 t_KEYON;
	static vu8 a,b;
/****************************************************短按功能************************************************************/
	if(Flag_Key_Scan==1)
	{
		Flag_Key_Scan = 0;
		KeyValue = Key_Scan();
		switch(KeyValue)
		{
			case KEY_1 ://
			{
				SET_Voltage=1000;
				SET_Current=4000;
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			
			case KEY_2 ://
			{
				SET_Voltage=420;
				SET_Current=1000;
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			
			case KEY_3 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			
			case KEY_4 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			
			case KEY_5 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;	
			
			case KEY_6 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			
			case KEY_7 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			
			case KEY_8 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			
			case KEY_9 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			
			case KEY_0 ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			
			case KEY_dian ://
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			
			case KEY_ESC :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			
			case PUSH_Lift :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			case PUSH_Right :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			case PUSH_Up :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
				SET_Current_Laod=SET_Current_Laod+1;
			}
			break;
			case PUSH_Down :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
				SET_Current_Laod=SET_Current_Laod-1;
			}
			break;
			
			case ENTER ://
			{
				File_IAP();
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			case PUSH_DISP :
			{
				
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			case PUSH_SETUP :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器

				t_onoff = 1;
			}
			break;
			case KEY_TRIG:
			{
//				t_onoff++;
//				if(t_onoff>1)
//				{
//					t_onoff=0;
//				}
//				if(t_onoff==0)
//				{
//					Flag_Swtich_ON=0;
//					GPIO_SetBits(GPIOC,GPIO_Pin_1);//OFF
//					
//				}
//				else if(t_onoff==1)
//				{
//					Flag_Swtich_ON=1;
//					GPIO_ResetBits(GPIOC,GPIO_Pin_1);//On
//				}
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			case KEY_Reset:
			{
//				static vu8 POW_t;
//				POW_t++;
//				if(POW_t>1)
//				{
//					POW_t=0;
//				}
//				if(POW_t==0)
//				{
//					GPIO_ResetBits(GPIOB,GPIO_Pin_13);//关闭稳压电源输出
//				}
//				else if(POW_t==1)
//				{
//					GPIO_SetBits(GPIOB,GPIO_Pin_13);//打开稳压电源输出
//				}
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			case KEY_Powe :
			{
				
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			case KEY_Face1 :
			{
//				WM_DeleteWindow(hWinWind);
				WM_DeleteWindow(hWinR);
				CreateR();
				Mode_SW_CONT(0x01);//进入内阻测试模式
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			case KEY_Face2 :
			{
//				WM_DeleteWindow(hWinR);
//				WM_DeleteWindow(hWinWind);
//				CreateWindow2();
//				Mode_SW_CONT(0x02);//进入负载模式
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			case KEY_Face3 :
			{
//				WM_DeleteWindow(hWinR);
//				WM_DeleteWindow(hWinWind);
//				CreateWindow();
//				Mode_SW_CONT(0x03);//进入电源模式
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			case KEY_Face4 :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			case KEY_Face5 :
			{
				KeyCounter = 0;
				BEEP_Tiggr();//触发蜂鸣器
			}
			break;
			
			default:
			break;
		}	
	}
}
/***************************************************************************************************************************/
void Lift_Move(void)
{
	t_wei++;
	if(t_wei>3)
	{
		t_wei=0;
	}
	if(t_wei==0)
	{
		Flag_QIAN=1;
		Flag_GE=0;
		Flag_SHI=0;
		Flag_BAI=0;
	}
	else if(t_wei==1)
	{
		Flag_GE=1;
		Flag_QIAN=0;
		Flag_SHI=0;
		Flag_BAI=0;
	}
	else if(t_wei==2)
	{
		Flag_SHI=1;
		Flag_QIAN=0;
		Flag_GE=0;
		Flag_BAI=0;
	}
	else if(t_wei==3)
	{
		Flag_BAI=1;
		Flag_QIAN=0;
		Flag_GE=0;
		Flag_SHI=0;
	}
}
/***********************************************************************************************************************/
void Right_Move(void)
{
	t_wei--;
	Flag_DisSet=1;//��������ֵ��ʾ
	if(t_wei>3)
	{
		t_wei=3;
	}
	if(t_wei==0)
	{
		Flag_QIAN=1;
		Flag_GE=0;
		Flag_SHI=0;
		Flag_BAI=0;
	}
	else if(t_wei==1)
	{
		Flag_GE=1;
		Flag_QIAN=0;
		Flag_SHI=0;
		Flag_BAI=0;
	}
	else if(t_wei==2)
	{
		Flag_SHI=1;
		Flag_QIAN=0;
		Flag_GE=0;
		Flag_BAI=0;
	}
	else if(t_wei==3)
	{
		Flag_BAI=1;
		Flag_QIAN=0;
		Flag_GE=0;
		Flag_SHI=0;
	}
}
/*************************************************************************************************************************/
void Setvalue_Add(void)//���õ�ѹ�͵�������
{
	/********��ѹ����*********************/
	if(Flag_GE==1&&Flag_SetV==1)
	{
		SET_Voltage++;
		if(SET_Voltage>MAX_VOL)
		{
			SET_Voltage=MAX_VOL;
		}
	}
	else if(Flag_SHI==1&&Flag_SetV==1)
	{
		SET_Voltage=SET_Voltage+10;
		if(SET_Voltage>MAX_VOL)
		{
			SET_Voltage=MAX_VOL;//�����ֵ����3000���ε�����Ч
		}
	}
	else if(Flag_BAI==1&&Flag_SetV==1)
	{
		SET_Voltage=SET_Voltage+100;
		if(SET_Voltage>MAX_VOL)
		{
			SET_Voltage=MAX_VOL;//�����ֵ����3000���ε�����Ч
		}
	}
	else if(Flag_QIAN==1&&Flag_SetV==1)
	{
		SET_Voltage=SET_Voltage+1000;
		if(SET_Voltage>MAX_VOL)
		{
			SET_Voltage=SET_Voltage-1000;//�����ֵ����3000���ε�����Ч
		}
	}
/**********��������*******************/
	if(Flag_GE==1&&Flag_SetA==1)
	{
		if(flag_Test_Min==1)//��������ֵ
		{
			MIN_limit++;
			if( MIN_limit>MAX_CUR)
			{
				 MIN_limit=MAX_CUR;
			}
		}
		else if(flag_Test_MAX==1)//��������ֵ
		{
			MAX_limit++;
			if( MAX_limit>MAX_CUR)
			{
				 MAX_limit=MAX_CUR;
			}
		}
		else if(flag_Test_SetTime==1)//���ò���ʱ��
		{
			Test_Daley++;
			if( Test_Daley>9999)
			{
				Test_Daley=9999;
			}
		}
		else if(flag_Test_Door==1)//�����ż�����
		{
			Test_C_Door++;
			if(Test_C_Door>MAX_CUR)
			{
				Test_C_Door=MAX_CUR;
			}
		}
		else if(flag_ADJ_ON==1)//����У׼ģʽ
		{
			ADJ_Write++;
			if(ADJ_Write>MAX_CUR)
			{
				ADJ_Write=MAX_CUR;
			}
		}
		else
		{
			SET_Current++;
			if(SET_Current>MAX_CUR)
			{
				SET_Current=MAX_CUR;
			}
		}
	}
	else if(Flag_SHI==1&&Flag_SetA==1)
	{
		if(flag_Test_Min==1)//��������ֵ
		{
			MIN_limit=MIN_limit+10;
			if( MIN_limit>MAX_CUR)
			{
				 MIN_limit=MIN_limit-10;
			}
		}
		else if(flag_Test_MAX==1)//��������ֵ
		{
			MAX_limit=MAX_limit+10;
			if( MAX_limit>MAX_CUR)
			{
				 MAX_limit=MAX_limit-10;
			}
		}
		else if(flag_Test_SetTime==1)//���ò���ʱ��
		{
			Test_Daley=Test_Daley+10;
			if( Test_Daley>9999)
			{
				Test_Daley=Test_Daley-10;
			}
		}
		else if(flag_Test_Door==1)//�����ż�����
		{
			Test_C_Door=Test_C_Door+10;
			if(Test_C_Door>MAX_CUR)
			{
				Test_C_Door=Test_C_Door-10;
			}
		}
		else if(flag_ADJ_ON==1)//����У׼ģʽ
		{
			ADJ_Write=ADJ_Write+10;
			if(ADJ_Write>MAX_CUR)
			{
				ADJ_Write=MAX_CUR;//�����ֵ����5000���ε�����Ч
			}
		}
		else
		{
			SET_Current=SET_Current+10;
			if(SET_Current>MAX_CUR)
			{
				SET_Current=MAX_CUR;//�����ֵ����5000���ε�����Ч
			}
		}
	}
	else if(Flag_BAI==1&&Flag_SetA==1)
	{
		if(flag_Test_Min==1)//��������ֵ
		{
			MIN_limit=MIN_limit+100;
			if( MIN_limit>MAX_CUR)
			{
				 MIN_limit=MIN_limit-100;
			}
		}
		else if(flag_Test_MAX==1)//��������ֵ
		{
			MAX_limit=MAX_limit+100;
			if( MAX_limit>MAX_CUR)
			{
				 MAX_limit=MAX_limit-100;
			}
		}
		else if(flag_Test_SetTime==1)//���ò���ʱ��
		{
			Test_Daley=Test_Daley+100;
			if( Test_Daley>9999)
			{
				Test_Daley=Test_Daley-100;
			}
		}
		else if(flag_Test_Door==1)//�����ż�����
		{
			Test_C_Door=Test_C_Door+100;
			if(Test_C_Door>MAX_CUR)
			{
				Test_C_Door=Test_C_Door-100;
			}
		}
		else if(flag_ADJ_ON==1)//����У׼ģʽ
		{
			ADJ_Write=ADJ_Write+100;
			if(ADJ_Write>MAX_CUR)
			{
				ADJ_Write=ADJ_Write-100;
			}
		}
		else
		{
			SET_Current=SET_Current+100;
			if(SET_Current>MAX_CUR)
			{
				SET_Current=MAX_CUR;//�����ֵ����5000���ε�����Ч
			}
		}
	}
	else if(Flag_QIAN==1&&Flag_SetA==1)
	{
		if(flag_Test_Min==1)//��������ֵ
		{
			MIN_limit=MIN_limit+1000;
			if( MIN_limit>MAX_CUR)
			{
				 MIN_limit=MIN_limit-1000;
			}
		}
		else if(flag_Test_MAX==1)//��������ֵ
		{
			MAX_limit=MAX_limit+1000;
			if( MAX_limit>MAX_CUR)
			{
				 MAX_limit=MAX_limit-1000;
			}
		}
		else if(flag_Test_SetTime==1)//���ò���ʱ��
		{
			Test_Daley=Test_Daley+1000;
			if( Test_Daley>9999)
			{
				Test_Daley=Test_Daley-1000;
			}
		}
		else if(flag_Test_Door==1)//�����ż�����
		{
			Test_C_Door=Test_C_Door+1000;
			if(Test_C_Door>MAX_CUR)
			{
				Test_C_Door=Test_C_Door-1000;
			}
		}
		else if(flag_ADJ_ON==1)//����У׼ģʽ
		{
			ADJ_Write=ADJ_Write+1000;
			if(ADJ_Write>MAX_CUR)
			{
				ADJ_Write=ADJ_Write-1000;//�����ֵ����5000���ε�����Ч
			}
		}
		else
		{
			SET_Current=SET_Current+1000;
			if(SET_Current>MAX_CUR)
			{
				SET_Current=SET_Current-1000;//�����ֵ����5000���ε�����Ч
			}
		}
	}
}
/***************************************************************************************/
void Setvalue_Reduction(void)//���õ�ѹ�͵����ݼ�
{
	/*********************���õ�ѹ***********************************************/
	if(Flag_GE==1&&Flag_SetV==1)
	{
		SET_Voltage--;
		if(SET_Voltage>MAX_VOL)
		{
			SET_Voltage=0;
		}
	}
	else if(Flag_SHI==1&&Flag_SetV==1)
	{
		SET_Voltage=SET_Voltage-10;
		if(SET_Voltage>MAX_VOL)
		{
			SET_Voltage=SET_Voltage+10;//�����ֵ����3000���εݼ���Ч
		}
	}
	else if(Flag_BAI==1&&Flag_SetV==1)
	{
		SET_Voltage=SET_Voltage-100;
		if(SET_Voltage>MAX_VOL)
		{
			SET_Voltage=SET_Voltage+100;//�����ֵ����3000���εݼ���Ч
		}
	}
	else if(Flag_QIAN==1&&Flag_SetV==1)
	{
		SET_Voltage=SET_Voltage-1000;
		if(SET_Voltage>MAX_VOL)
		{
			SET_Voltage=SET_Voltage+1000;//�����ֵ����3000���εݼ���Ч
		}
	}
/******************************���õ���******************************************/
	if(Flag_GE==1&&Flag_SetA==1)
	{
		if(flag_Test_Min==1)//��������ֵ
		{
			MIN_limit--;
			if( MIN_limit>MAX_CUR)
			{
				 MIN_limit=0;
			}
		}
		else if(flag_Test_MAX==1)//��������ֵ
		{
			MAX_limit--;
			if( MAX_limit>MAX_CUR)
			{
				 MAX_limit=0;
			}
		}
		else if(flag_Test_SetTime==1)//���ò���ʱ��
		{
			Test_Daley--;
			if( Test_Daley>9999)
			{
				Test_Daley=0;
			}
		}
		else if(flag_Test_Door==1)//�����ż�����
		{
			Test_C_Door--;
			if(Test_C_Door<10)
			{
				Test_C_Door=10;
			}
		}
		else if(flag_ADJ_ON==1)//����У׼ģʽ
		{
			ADJ_Write--;//���õ���ֵ
			if(ADJ_Write>MAX_CUR)
			{
				ADJ_Write=0;
			}
		}
		else
		{
			SET_Current--;//���õ���ֵ
			if(SET_Current>MAX_CUR)
			{
				SET_Current=0;
			}
		}
		
	}
	else if(Flag_SHI==1&&Flag_SetA==1)
	{
		if(flag_Test_Min==1)//��������ֵ
		{
			MIN_limit=MIN_limit-10;
			if( MIN_limit>MAX_CUR)
			{
				 MIN_limit=MIN_limit+10;
			}
		}
		else if(flag_Test_MAX==1)//��������ֵ
		{
			MAX_limit=MAX_limit-10;
			if( MAX_limit>MAX_CUR)
			{
				 MAX_limit=MAX_limit+10;
			}
		}
		else if(flag_Test_SetTime==1)//���ò���ʱ��
		{
			Test_Daley=Test_Daley-10;
			if( Test_Daley>9999)
			{
				Test_Daley=Test_Daley+10;
			}
		}
		else if(flag_Test_Door==1)//�����ż�����
		{
			Test_C_Door=Test_C_Door-10;
			if(Test_C_Door>MAX_CUR)
			{
				Test_C_Door=Test_C_Door+10;
			}
		}
		else if(flag_ADJ_ON==1)//����У׼ģʽ
		{
			ADJ_Write=ADJ_Write-10;
			if(ADJ_Write>MAX_CUR)
			{
				ADJ_Write=ADJ_Write+10;//�����ֵ����5000���εݼ���Ч
			}
		}
		else
		{
			SET_Current=SET_Current-10;
			if(SET_Current>MAX_CUR)
			{
				SET_Current=SET_Current+10;//�����ֵ����5000���εݼ���Ч
			}
		}
	}
	else if(Flag_BAI==1&&Flag_SetA==1)
	{
		if(flag_Test_Min==1)//��������ֵ
		{
			MIN_limit=MIN_limit-100;
			if( MIN_limit>MAX_CUR)
			{
				 MIN_limit=MIN_limit+100;
			}
		}
		else if(flag_Test_MAX==1)//��������ֵ
		{
			MAX_limit=MAX_limit-100;
			if( MAX_limit>MAX_CUR)
			{
				 MAX_limit=MAX_limit+100;
			}
		}
		else if(flag_Test_SetTime==1)//���ò���ʱ��
		{
			Test_Daley=Test_Daley-100;
			if( Test_Daley>9999)
			{
				Test_Daley=Test_Daley+100;
			}
		}
		else if(flag_Test_Door==1)//�����ż�����
		{
			Test_C_Door=Test_C_Door-100;
			if(Test_C_Door>MAX_CUR)
			{
				Test_C_Door=Test_C_Door+100;
			}
		}
		else if(flag_ADJ_ON==1)//����У׼ģʽ
		{
			ADJ_Write=ADJ_Write-100;
			if(ADJ_Write>MAX_CUR)
			{
				ADJ_Write=ADJ_Write+100;//�����ֵ����5000���εݼ���Ч
			}
		}
		else
		{
			SET_Current=SET_Current-100;
			if(SET_Current>MAX_CUR)
			{
				SET_Current=SET_Current+100;//�����ֵ����5000���εݼ���Ч
			}
		}
	}
	else if(Flag_QIAN==1&&Flag_SetA==1)
	{
		if(flag_Test_Min==1)//��������ֵ
		{
			MIN_limit=MIN_limit-1000;
			if( MIN_limit>MAX_CUR)
			{
				 MIN_limit=MIN_limit+1000;
			}
		}
		else if(flag_Test_MAX==1)//��������ֵ
		{
			MAX_limit=MAX_limit-1000;
			if( MAX_limit>MAX_CUR)
			{
				 MAX_limit=MAX_limit+1000;
			}
		}
		else if(flag_Test_SetTime==1)//���ò���ʱ��
		{
			Test_Daley=Test_Daley-1000;
			if( Test_Daley>9999)
			{
				Test_Daley=Test_Daley+1000;
			}
		}
		else if(flag_Test_Door==1)//�����ż�����
		{
			Test_C_Door=Test_C_Door-1000;
			if(Test_C_Door>MAX_CUR)
			{
				Test_C_Door=Test_C_Door+1000;
			}
		}
		else if(flag_ADJ_ON==1)//����У׼ģʽ
		{
			ADJ_Write=ADJ_Write-1000;
			if(ADJ_Write>MAX_CUR)
			{
				ADJ_Write=ADJ_Write+1000;//�����ֵ����5000���εݼ���Ч
			}
		}
		else
		{
			SET_Current=SET_Current-1000;
			if(SET_Current>MAX_CUR)
			{
				SET_Current=SET_Current+1000;//�����ֵ����5000���εݼ���Ч
			}
		}
	}
}
/********************************************************************************/
void Mode_SW_CONT(vu8 mode)//ģʽ�л�����
{
	switch(mode)
	{
		case 0x01 ://�������ģʽ
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_2);
			GPIO_ResetBits(GPIOE,GPIO_Pin_3);
		}
		break;
		case 0x02 ://����ģʽ
		{
			GPIO_SetBits(GPIOE,GPIO_Pin_2);
			GPIO_ResetBits(GPIOE,GPIO_Pin_3);
		}
		break;
		case 0x03 ://ֱ����Դģʽ
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_2);//
			GPIO_SetBits(GPIOE,GPIO_Pin_3);//
		}
		break;
		default:
		break;
	}
}
