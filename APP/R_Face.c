/******************************************************************/
/* 名称：                                                  				*/
/* 效果：                                                        */
/* 内容：内阻仪界面                                              */
/* 作者：zhan                                                  */
/* 联系方式QQ:363116119                                        */


#include "MainTask.h"
#include  "gui.h"
#include "DIALOG.h"
#include "my_register.h" 
#include "ff.h"			/* FatFS文件系统模块*/
#include "usbh_bsp_msc.h"


extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ12;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontSU30;
extern USB_OTG_CORE_HANDLE   USB_OTG_Core;
extern USBH_HOST             USB_Host;
WM_HWIN hWinR;
float DISS_Voltage;//负载电压
float DISS_POW_Voltage;//稳压电源电压
float DISS_Current;//负载电流
float DISS_POW_Current;//稳压电源电流
float DISS_R;//内阻
u8 page;
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_1     (GUI_ID_USER + 0x12)
#define ID_TEXT_0     	(GUI_ID_USER + 0x13)
#define ID_TEXT_1     	(GUI_ID_USER + 0x14)
#define ID_TEXT_3     	(GUI_ID_USER + 0x15)
#define ID_TEXT_4     	(GUI_ID_USER + 0x16)
#define ID_TEXT_6     	(GUI_ID_USER + 0x17)
#define ID_BUTTON_12    (GUI_ID_USER + 0x18)
#define ID_BUTTON_13    (GUI_ID_USER + 0x19)
#define ID_BUTTON_14    (GUI_ID_USER + 0x1A)
#define ID_BUTTON_15   	(GUI_ID_USER + 0x1B)
#define ID_BUTTON_16   	(GUI_ID_USER + 0x1C)
#define ID_BUTTON_17    (GUI_ID_USER + 0x1D)

#define ID_TimerTime    1
// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "R", ID_WINDOW_1, 0, 0, 480, 272, 0, 0x0, 0 },
//  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 30, 30, 100, 30, 0, 0x64, 0 },
//  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 30, 70, 330, 30, 0, 0x64, 0 },
//  { TEXT_CreateIndirect, "Text", ID_TEXT_3, 30, 103, 32, 32, 0, 0x0, 0 },
//  { TEXT_CreateIndirect, "Text", ID_TEXT_4, 30, 104, 32, 32, 0, 0x0, 0 },
//	{ TEXT_CreateIndirect, "Text", ID_TEXT_6, 182, 152, 32, 20, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "Button", ID_BUTTON_12, 6, 231, 55, 39, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "Button", ID_BUTTON_13, 85, 232, 55, 39, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "Button", ID_BUTTON_14, 162, 232, 55, 39, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "Button", ID_BUTTON_15, 245, 231, 55, 39, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "Button", ID_BUTTON_16, 330, 232, 55, 39, 0, 0x0, 0 },
//  { BUTTON_CreateIndirect, "Button", ID_BUTTON_17, 414, 232, 55, 39, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	
//	DIR DirInf;
//	uint32_t bw;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
		
	case WM_PAINT:
	GUI_SetColor(GUI_ORANGE);//设置画笔颜色
	GUI_FillRect(0,0,480,30);//画一个矩形
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_FontSU30);
	GUI_UC_SetEncodeUTF8();
	GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明
	GUI_DispStringAt("固件升级", 208, 3);
	GUI_SetColor(0x00FFFFFF);//设置前景色为红色
	GUI_DispStringAt("请插入U盘", 30,30);
	GUI_DispStringAt("按确认键升级", 30,60);
	GUI_DispStringAt("升级时请勿触摸按键或断开电源", 30,90);
	GUI_SetFont(GUI_FONT_24_1);//
	GUI_DispStringAt("1.", 10,30+5);
	GUI_DispStringAt("2.", 10,60+5);
	GUI_DispStringAt("3.", 10,90+5);
	
//	GUI_GotoXY(50,72);//
//	GUI_DispFloatFix(DISS_Voltage,6,3);//显示电压值
//	GUI_DispDecAt(R_VLUE,50,140,4);//显示内阻值
	break;
	case WM_TIMER://定时模块消息
	if(WM_GetTimerId(pMsg->Data.v) == ID_TimerTime)
	{
		page = 1;
		USBH_Process(&USB_OTG_Core, &USB_Host);
		if (HCD_IsDeviceConnected(&USB_OTG_Core))
		{
			GPIO_ResetBits(GPIOD,GPIO_Pin_12);
            TM1650_SET_LED(0x48,0x71);
            TM1650_SET_LED(0x68,0xF2);//PASS灯
		}
		WM_RestartTimer(pMsg->Data.v, 100);
	}
	break;
		
  case WM_INIT_DIALOG:
    //
    // Initialization of 'R'
	
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_BLUE);
		WM_CreateTimer(hItem,ID_TimerTime,20,0);//创建本窗口定时器
    //
//		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_12);
//		BUTTON_SetTextColor(hItem,0,GUI_BLACK);//设置字体颜色为黑色
//		BUTTON_SetFont      (hItem,    &GUI_FontHZ12);//设定按钮文本字体
//		GUI_UC_SetEncodeUTF8();
//		BUTTON_SetText(hItem,"开关");
//	
//	
//		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_13);
//		BUTTON_SetTextColor(hItem,0,GUI_BLACK);//设置字体颜色为黑色
//		BUTTON_SetFont      (hItem,    &GUI_FontHZ12);//设定按钮文本字体
//		GUI_UC_SetEncodeUTF8();
//		BUTTON_SetText(hItem,"程控电源");
//	
//		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_14);
//		BUTTON_SetTextColor(hItem,0,GUI_BLACK);//设置字体颜色为黑色
//		BUTTON_SetFont      (hItem,    &GUI_FontHZ12);//设定按钮文本字体
//		GUI_UC_SetEncodeUTF8();
//		BUTTON_SetText(hItem,"程控负载");
//		
//		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_15);
//		BUTTON_SetTextColor(hItem,0,GUI_BLACK);//设置字体颜色为黑色
//		BUTTON_SetFont      (hItem,    &GUI_FontHZ12);//设定按钮文本字体
//		GUI_UC_SetEncodeUTF8();
//		BUTTON_SetText(hItem,"充放电");
//		
//		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_16);
//		BUTTON_SetTextColor(hItem,0,GUI_BLACK);//设置字体颜色为黑色
//		BUTTON_SetFont      (hItem,    &GUI_FontHZ12);//设定按钮文本字体
//		GUI_UC_SetEncodeUTF8();
//		BUTTON_SetText(hItem,"曲线");
//		
//		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_17);
//		BUTTON_SetTextColor(hItem,0,GUI_BLACK);//设置字体颜色为黑色
//		BUTTON_SetFont      (hItem,    &GUI_FontHZ12);//设定按钮文本字体
//		GUI_UC_SetEncodeUTF8();
//		BUTTON_SetText(hItem,"系统设置");
//		
//    // Initialization of 'Text'
//    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetTextColor(hItem, 0x00FFFFFF);
		TEXT_SetFont(hItem,&GUI_FontSU30);//设定文本字体
		GUI_UC_SetEncodeUTF8();
		 TEXT_SetText(hItem,"");
//    //
//    // Initialization of 'Text'
//    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    GUI_UC_SetEncodeUTF8();
    TEXT_SetText(hItem, "");
    TEXT_SetTextColor(hItem, 0x00FFFFFF);
    TEXT_SetFont(hItem,&GUI_FontSU30);//设定文本字体
//    //
//    // Initialization of 'Text'
    //
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
		TEXT_SetText(hItem, "U");
    TEXT_SetTextColor(hItem, 0x000000FF);
    TEXT_SetFont(hItem, &GUI_Font24_1);
//    //
//    // Initialization of 'Text'
//    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
		GUI_UC_SetEncodeUTF8();
		TEXT_SetText(hItem, "Ω");
    TEXT_SetTextColor(hItem, 0x0000FF00);
		TEXT_SetFont(hItem,&GUI_FontSU30);//设定按钮文本字体
//    //
//    // Initialization of 'Text'
//    //		
//		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_6);
//		TEXT_SetText(hItem, "m");
//    TEXT_SetTextColor(hItem, 0x0000FF00);
//		TEXT_SetFont(hItem, &GUI_Font20_1);
		
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateR
*/
WM_HWIN CreateR(void);
WM_HWIN CreateR(void) {
  hWinR = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWinR;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
