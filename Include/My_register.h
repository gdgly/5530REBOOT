/******************** (C) COPYRIGHT 2014 KUNKIN********************
 * ÎÄ¼şÃû  £ºmy_register
 * ×÷Õß    £ºÀîÕñ
 * ÃèÊö    £º¿âº¯Êı°æ
 * ÄÚÈİ    £ºDefine register¶¨ÒåÏµÍ³Ê¹ÓÃµÄ¼Ä´æÆ÷£¬È«¾Ö±äÁ¿
 * ×¢Òâ		 £ºÍ·ÎÄ¼şÖĞÖ»ÄÜÓĞÉùÃ÷£¬²»ÄÜÓĞ¶¨Òå
 * ĞŞ¸ÄÈÕÆÚ£º2014-12-9
*********************************************************************/

#include "STM32F4xx.h"
#ifndef __my_register_h__
#define __my_register_h__
//=============================================================================

//=============================================================================
#define NOP   __nop();	  //¿ÕÖ¸Áî

//============================================================================
extern struct bitDefine flagA,flagB,flagC,flagD,flagE,flagF,flagG;
#define   Flag_ADC_Full             (flagA.bit1)//DMA´«ÊäÍê³É±êÖ¾Î» 
#define   Flag_BEEP						    	(flagA.bit2)//·äÃùÆ÷´¥·¢±êÖ¾
#define   Flag_Key_Scan							(flagA.bit3)//°´¼ü¶¨Ê±É¨Ãè±êÖ¾
#define   UART_SEND_flag						(flagA.bit4)//´®¿Ú·¢ËÍÍê³É±êÖ¾
#define   UART_Buffer_Rece_flag			(flagA.bit5)//´®¿Ú½ÓÊÕÍê³É±êÖ¾
#define   Flag_Swtich_ON      			(flagA.bit6)//Æô¶¯°´¼ü´¦ÓÚON±êÖ¾
#define   Flag_0_5S      		      	(flagA.bit7)//ÏÔÊ¾ÉèÖÃµçÑ¹ÉÁË¸±êÖ¾

#define   Flag_1S      		         	(flagB.bit0)//ÏÔÊ¾ÉèÖÃµçÁ÷ÉÁË¸±êÖ¾
#define   Flag_5S      		        	(flagB.bit1)//ÍË³öÉèÖÃ×´Ì¬±êÖ¾
#define   Flag_Long                 (flagB.bit2)//°´¼ü³¤°´±êÖ¾
#define   Flag_DAC_OFF              (flagB.bit3)//æ˜¯å¦è½¬æ¢DACæ§åˆ¶å€¼æ ‡å¿—
#define   Flag_SetA                 (flagB.bit5)//½øÈëÉèÖÃµçÁ÷±êÖ¾Î»
#define   Flag_SetV                 (flagB.bit6)//½øÈëÉèÖÃµçÑ¹±êÖ¾Î» 
#define   Flag_DisSet               (flagB.bit7)//ÏÔÊ¾ÉèÖÃµçÑ¹ºÍµçÁ÷±êÖ¾Î»

#define   Flag_GE                   (flagC.bit0)//¸öÎ»ÉÁË¸±êÖ¾
#define   Flag_SHI                  (flagC.bit1)//Ê®Î»ÉÁË¸±êÖ¾
#define   Flag_BAI                  (flagC.bit2)//°ÙÎ»ÉÁË¸±êÖ¾
#define   Flag_QIAN                 (flagC.bit3)//Ç§Î»ÉÁË¸±êÖ¾
#define   Flag_Beep_OFF             (flagC.bit6)//·äÃùÆ÷¹Ø±Õ±êÖ¾

#define   flag_Tim_USART            (flagD.bit0)//´®¿Ú¼ÆËãÇåÁã±êÖ¾
#define   flag_Test                 (flagD.bit1)//×Ô¶¯²âÊÔÆğÍ£±êÖ¾
#define   flag_Test_PASS            (flagD.bit2)//×Ô¶¯²âÊÔPASS±êÖ¾
#define   flag_Test_FAIL            (flagD.bit3)//×Ô¶¯²âÊÔFAIL±êÖ¾
#define   flag_Test_Time            (flagD.bit4)//×Ô¶¯²âÊÔµÈ´ıÊ±¼ä±êÖ¾
#define   flag_Test_Start           (flagD.bit5)//×Ô¶¯²âÊÔ±È½Ï¿ªÊ¼±êÖ¾
#define   flag_Test_Min             (flagD.bit6)//×Ô¶¯²âÊÔÏÂÏŞ±êÖ¾
#define   flag_Test_MAX             (flagD.bit7)//×Ô¶¯²âÊÔÉÏÏŞ±êÖ¾

#define   flag_Test_Door             (flagE.bit0)//×Ô¶¯²âÊÔÃÅ¼÷±êÖ¾
#define   flag_Test_SetTime          (flagE.bit1)//×Ô¶¯²âÊÔÊ±¼ä±êÖ¾ 
#define   flag_OVP                   (flagE.bit2)//OVP±êÖ¾
#define   flag_OCP                   (flagE.bit3)//OCP±êÖ¾
#define   flag_Doub                  (flagE.bit4)//°´¼üË«»÷
#define   flag_t_Doub                (flagE.bit5)//°´¼üË«»÷¼ÆÊ±±êÖ¾
#define   flag_Time_SW               (flagE.bit6)//¶¨Ê±¹¦ÄÜ¿ªÆô±êÖ¾
#define   flag_CC_MODE               (flagE.bit7)//¹¤×÷Ä£Ê½±êÖ¾

#define   flag_Adjust                (flagF.bit0)//æ ¡å‡†æ¨¡å¼æ ‡å¿—ä½
#define   flag_ADJ_ON                (flagF.bit1)//
#define   flag_ADJ_VL                (flagF.bit2)//
#define   flag_ADJ_VH                (flagF.bit3)//
#define   flag_ADJ_ALCC              (flagF.bit4)//
#define   flag_ADJ_AHCC              (flagF.bit5)//
#define   flag_ADJ_ALCV              (flagF.bit6)//
#define   flag_ADJ_AHCV              (flagF.bit7)//

#define   flag_DisVL                 (flagG.bit0)//
#define   flag_OverV                 (flagG.bit1)//
#define   flag_OverACC               (flagG.bit2)//
#define   flag_OverACV               (flagG.bit3)//
#define   flag_Just_Usart            (flagG.bit4)//
#define   flag_FAN_ON                (flagG.bit5)//
#define   flag_ADJ_FAN               (flagG.bit6)//
//=============================================================================
extern vu32 GE_V;
extern vu32 SHI_V;
extern vu32 BAI_V;
extern vu32 QIAN_V;  //ÊıÂë¹ÜÉ¨Ãè¶ÎÂë
extern vu32 GE_A;
extern vu32 SHI_A;
extern vu32 BAI_A;
extern vu32 QIAN_A;
//--------------------------
extern vu8 NewKey;
extern vu8 DAC_Flag;
extern vu8 Beep_Flag;
extern vu8 t_beep;
extern vu8 t_KeyScan;
extern vu8 t_LEDScan;  //¶¨Ê±¼ÆÊıÆ÷
extern vu8 t_KEYON;
extern vu16 t_0_5S;
extern vu16 t_1S;
extern vu16 t_5S;
extern vu16 t_Basket;
extern vu16 Basket_V;
extern vu16 Basket_A;
extern vu16 t_OFF_V;
extern vu16 t_ON_V;
extern vu16 t_OFF_A;
extern vu16 t_ON_A;
extern vu8 t_USART;
extern vu16 ADJ_Write;//Ğ£×¼Ê±ĞèĞ´ÈëµÄÊµ¼ÊÖµ
extern vu8 OFFSET_Zore;//ÁãµãÖµ
extern vu8 ADDR;
extern vu8 t_lock;
extern vu8 t_onoff;
extern vu16 MAX_limit;
extern vu16 MIN_limit;
extern vu16 Test_Time;
extern vu16 Test_Time1;
extern vu16 Time_sw;
extern vu16 Test_Daley;
extern vu16 Test_C_Door;
extern vu16 TEMP_OFFSET;//ÎÂ¶È²¹³¥Öµ
//--------------------------
extern vu8 UART_Buffer_Size;//´®¿Ú½ÓÊÕÊı¾İ³¤¶È
extern vu8 Transmit_BUFFERsize;
//---------------------------
extern vu16 NTC_value;   //ADÖµ
extern vu16 Imon_value;
extern vu16 Imon1_value;
extern vu16 Vmon_value;
extern vu16 Vmon1_value;
extern vu16 Rmon_value;

extern float DISS_Voltage;
extern  float DISS_POW_Voltage;
extern float DISS_Current;
extern float DISS_POW_Current;
extern float DISS_R;

extern vu16 Run_Control[14];	
#define onoff_ch                    Run_Control[0]  //ON/OFF
#define SET_Voltage 	    	        Run_Control[1]  //ç”µæºè®¾ç½®ç”µå‹
#define SET_Current	    	    		  Run_Control[2]  //ç”µæºè®¾ç½®ç”µæµ
#define SET_Voltage_Laod 					  Run_Control[3]  //ç”µå­è´Ÿè½½è®¾ç½®ç”µå‹
#define SET_Current_Laod					  Run_Control[4]  //ç”µå­è´Ÿè½½è®¾ç½®ç”µæµ
#define LOCK_KEY								    Run_Control[5]  //
#define BEEP_KEY								    Run_Control[6]  //
#define STRONG_KEY							    Run_Control[7]  //

#define MODE_Woke							   	  Run_Control[8]  //
#define POW_Voltage	    	          Run_Control[9]  //
#define Voltage	    	              Run_Control[10]  //
#define Current	    	    		      Run_Control[11]  //
#define Laod_Current	    		      Run_Control[12]  //
#define R_VLUE											Run_Control[13]  //


extern vu16 Contr_Voltage;//
extern vu16 Contr_Current;//
extern vu16 Contr_Laod;//
extern vu16 Correct_Parametet[13];
#define  REG_CorrectionV   Correct_Parametet[0]
#define  REG_CorrectionA   Correct_Parametet[1]
#define  REG_CorrectionA1  Correct_Parametet[2]
#define  REG_CorrectionR   Correct_Parametet[3]
#define  Polar             Correct_Parametet[4]
#define  SET_CorrectionV   Correct_Parametet[5]
#define  SET_CorrectionA   Correct_Parametet[6]
#define  SET_CorrectionA1  Correct_Parametet[7]
#define  CON_CorrectionA   Correct_Parametet[8]
#define  Polar1            Correct_Parametet[9]
#define  Polar2						 Correct_Parametet[10]
#define  Polar3						 Correct_Parametet[11]
#define  REG_CorrectionV1  Correct_Parametet[12]


extern vu32 Correct_Strong[9];//Ğ£×¼ÏµÊı
#define  REG_ReadV_Offset   Correct_Strong[0]
#define  REG_ReadA_Offset   Correct_Strong[1]
#define  REG_ReadA1_Offset  Correct_Strong[2]
#define  REG_ReadR_Offset   Correct_Strong[3]
#define  SET_ReadV_Offset   Correct_Strong[4]
#define  SET_ReadA_Offset   Correct_Strong[5]
#define  SET_ReadA1_Offset  Correct_Strong[6]
#define  CON_ReadA_Offset   Correct_Strong[7]
#define  REG_ReadV1_Offset  Correct_Strong[8]



//---------------------------
extern vu16 ADC1_Buffer[300];//ADC×ª»»»º³åÊı×é
extern vu16 ADC_NTC_Filt[50];
extern vu16 ADC_Vmon_Filt[50];
extern vu16 ADC_Imon_Filt[50];
extern vu8 UART_Buffer_Rece[16];
extern vu8 UART_Buffer_Send[20];
//============================================================================= 
#define Receive_BUFFERSIZE   10
//=============================================================================
#endif
/******************* (C) COPYRIGHT 2015 KUNKIN *****END OF FILE*************************/