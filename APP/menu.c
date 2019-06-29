/******************************************************************/
/* 名称：                                                  */
/* 效果：                                                        */
/* 内容：电源界面                                                       */
/* 作者：zhan                                                  */
/* 联系方式QQ:363116119                                        */
/******************************************************************/
#include "MainTask.h"
#include  "gui.h"
#include "my_register.h" 

WM_HWIN hWinWind;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ16;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontHZ12;
extern WM_HWIN CreateR(void);
#define ID_WINDOW_0      	(GUI_ID_USER + 0x00)
#define ID_BUTTON_0     	(GUI_ID_USER + 0x01)
#define ID_BUTTON_1     	(GUI_ID_USER + 0x02)
#define ID_BUTTON_2     	(GUI_ID_USER + 0x03)
#define ID_BUTTON_3     	(GUI_ID_USER + 0x04)
#define ID_BUTTON_4     	(GUI_ID_USER + 0x05)
#define ID_BUTTON_5     	(GUI_ID_USER + 0x06)
#define ID_TEXT_9    	 		(GUI_ID_USER + 0x07)
#define ID_TEXT_10     		(GUI_ID_USER + 0x08)
#define ID_TimerTime2    3

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 272, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 6, 231, 55, 39, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 85, 232, 55, 39, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_2, 162, 232, 55, 39, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_3, 245, 231, 55, 39, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_4, 330, 232, 55, 39, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_5, 414, 232, 55, 39, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_9, 8, 54, 32, 16, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_10, 8, 112, 32, 16, 0, 0x64, 0 },

  // USER START (Optionally insert additional widgets)
  // USER END
};


static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {

	case WM_PAINT://无效化重绘
		GUI_SetColor(0x0003FCF5);
		GUI_FillRect(0,0,480,30);
		GUI_SetColor(GUI_BLACK);
		GUI_SetFont(&GUI_FontHZ16);
		GUI_UC_SetEncodeUTF8();
		GUI_SetTextMode(GUI_TM_TRANS);//设置文本模式为底色透明
		GUI_DispStringAt("程控电源", 200, 7);//SET
		GUI_SetColor(GUI_RED);//设置前景色为红色
		GUI_SetFont(&GUI_FontD24x32);//设置字体尺寸
		GUI_GotoXY(50,72);//设置显示坐标
		GUI_DispFloatFix(DISS_POW_Voltage,5,2);//显示电压值
		GUI_GotoXY(50,140);//设置显示坐标
		GUI_DispFloatFix(DISS_POW_Current,5,3);//显示电流值
		break;
	case WM_TIMER://定时模块消息
	if(WM_GetTimerId(pMsg->Data.v) == ID_TimerTime2)
	{
		WM_InvalidateWindow(hWinWind);//无效化窗口
		WM_RestartTimer(pMsg->Data.v, 500);//复位定时器数值越大刷新时间越短
	}
	case WM_KEY://接受按键消息来处理按键功能
		switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
		{ 
			case GUI_KEY_TAB:

			break;
			
			case GUI_KEY_ENTER:
						WM_DeleteWindow(hWinWind);
				CreateR();
			break;
		}
	break;
  case WM_INIT_DIALOG://重绘外观
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, 0x004A4655);
		WM_CreateTimer(hItem,ID_TimerTime2,20,0);//创建本窗口定时器
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetTextColor(hItem,0,GUI_BLACK);//设置字体颜色为黑色
		BUTTON_SetFont      (hItem,    &GUI_FontHZ12);//设定按钮文本字体
		GUI_UC_SetEncodeUTF8();
		BUTTON_SetText(hItem,"开关");
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetTextColor(hItem,0,GUI_BLACK);//设置字体颜色为黑色
		BUTTON_SetFont      (hItem,    &GUI_FontHZ12);//设定按钮文本字体
		GUI_UC_SetEncodeUTF8();
    BUTTON_SetText(hItem, "内阻测试");
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetTextColor(hItem,0,GUI_BLACK);//设置字体颜色为黑色
		BUTTON_SetFont      (hItem,    &GUI_FontHZ12);//设定按钮文本字体
		GUI_UC_SetEncodeUTF8();
    BUTTON_SetText(hItem, "程控负载");
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetTextColor(hItem,0,GUI_BLACK);//设置字体颜色为黑色
		BUTTON_SetFont      (hItem,    &GUI_FontHZ12);//设定按钮文本字体
		GUI_UC_SetEncodeUTF8();
    BUTTON_SetText(hItem, "充放电");
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
    BUTTON_SetTextColor(hItem,0,GUI_BLACK);//设置字体颜色为黑色
		BUTTON_SetFont      (hItem,    &GUI_FontHZ12);//设定按钮文本字体
		GUI_UC_SetEncodeUTF8();
    BUTTON_SetText(hItem, "曲线");
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);
    BUTTON_SetTextColor(hItem,0,GUI_BLACK);//设置字体颜色为黑色
		BUTTON_SetFont      (hItem,    &GUI_FontHZ12);//设定按钮文本字体
		GUI_UC_SetEncodeUTF8();
    BUTTON_SetText(hItem, "系统设置");
		
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_9);
		TEXT_SetTextColor(hItem, 0x00FFFFFF);//设置字体颜色
    TEXT_SetFont(hItem,&GUI_FontHZ16);//设定文本字体
		GUI_UC_SetEncodeUTF8();
		TEXT_SetText(hItem,"电压");
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_10);
		TEXT_SetTextColor(hItem, 0x00FFFFFF);//设置字体颜色
    TEXT_SetFont(hItem,&GUI_FontHZ16);//设定文本字体
		GUI_UC_SetEncodeUTF8();
		TEXT_SetText(hItem,"电流");
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_1: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
				
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_4: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_5: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

WM_HWIN CreateWindow(void) {

  hWinWind = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWinWind;
}