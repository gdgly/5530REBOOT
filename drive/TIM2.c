/******************************************************************/
/* Ãû³Æ£HTIM3 PWM                                                */
/* Ð§¹û£º                                                        */
/* ÄÚÈÝ£º²úÉúÒ»¸ö200HZ ÕýÕ¼¿Õ±È£º60.9% ¸ºÕ¼¿Õ±È£º30.9%µÄPWM      */
/* ×÷Õß£ºÀîÕñ                                                    */
/* ÁªÏµ·½Ê½£ºQQ:363116119                                        */
/******************************************************************/
#include "my_register.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
/*****************************************************************/
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
} flagA,flagB,flagC,flagD;

vu16 TEMP_OFFSET;//ÎÂ¶È²¹³¥Öµ
/*****************************************************************/
void TIM2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//µ÷ÓÃGPIO½á¹¹Ìå
  /* ÅäÖÃGPIO¹Ü½Å¸´ÓÃ*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//½«ÉèÖÃÊäÈëº¯Êý
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//½«ÉèÖÃÊäÈëº¯Êý
}
/*****************************************************************/
void TIM2_PWM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* TIM3 µÄÅäÖÃ ---------------------------------------------------
   TIM3 ÊäÈëÊ±ÖÓ(TIM3CLK) ÉèÖÃÎª APB2 Ê±ÖÓ (PCLK2)    
    => TIM3CLK = PCLK2 = SystemCoreClock
   TIM3CLK = SystemCoreClock, Prescaler = 0, TIM3 counter clock = SystemCoreClock
   SystemCoreClock Îª48 MHz */
  /* TIM14 Ê±ÖÓÊ¹ÄÜ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM2_Config();

  /* Time ¶¨Ê±»ù´¡ÉèÖÃ*/
  TIM_TimeBaseStructure.TIM_Prescaler = 6;//Ê±ÖÓÔ¤·ÖÆµ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time ¶¨Ê±ÉèÖÃÎªÉÏÉýÑØ¼ÆËãÄ£Ê½*/
  TIM_TimeBaseStructure.TIM_Period = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

  /* ÆµµÀ1µÄPWM Ä£Ê½ÉèÖÃ */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = 0x0f;//Ê¹ÄÜÆµµÀ1ÅäÖÃ
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_SetAutoreload(TIM2, 0x7530);//è®¾ç½®PWMåˆ†è¾¨çŽ‡ 
	TIM_ARRPreloadConfig(TIM2, ENABLE);
  /* TIM14 ä½¿èƒ½*/
  TIM_Cmd(TIM2, ENABLE);
	
  /* TIM14 å¼€å§‹è¾“å‡º */
  TIM_CtrlPWMOutputs(TIM2, ENABLE);	
}
/**************************************************************************************/
void Temp_Comapre(void)	  //ÎÂ¶ÈÀ´¿ØÖÆ·çÉÈ
{
	if(NTC_value<=3350)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_2);//¿ªÆô·çÉÈ
		flag_FAN_ON=1;
	}
	else if(NTC_value>=3400)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);//¹Ø±Õ·çÉÈ
		flag_FAN_ON=0;
	}
}