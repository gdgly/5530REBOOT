/******************************************************************/
/* 名称：main                                                    */
/* 效果：                                                        */
/* 内容：                                                       */
/* 作者：zhan                                                  */
/* 联系方式QQ:363116119                                        */
/******************************************************************/
#include "my_register.h"
#include "stm32f4xx.h"
#include "clock.h"
#include "tim6.h"
#include "tim1.h"
#include "tim2.h"
#include "dac.h"
#include "adc.h"
#include "clock.h"
#include "gpio.h"
#include "bsp_SysTick.h"
#include "usart.h"
#include "ssd1963.h"
#include "iwdg.h"
#include "MainTask.h"
#include "IIC_24C01.h"
#include "flash.h"
#include "tm1650.h"
#include "stdio.h"
struct bitDefine
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

    #if 1  
    #pragma import(__use_no_semihosting)               
    //标准库需要的支持函数                   
    struct __FILE   
    {   
        int handle;   
        /* Whatever you require here. If the only file you are using is */   
        /* standard output using printf() for debugging, no file handling */   
        /* is required. */   
    };   
    /* FILE is typedef’ d in stdio.h. */   
    FILE __stdout;         
    //定义_sys_exit()以避免使用半主机模式      
    _sys_exit(int x)   
    {   
        x = x;   
    }   
    //重定向fputc函数  
    //printf的输出，指向fputc，由fputc输出到串口  
    //这里使用串口1(USART1)输出printf信息  
    int fputc(int ch, FILE *f)  
    {        
//        while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成    
//        USART1->DR = (u8) ch;        //写DR,串口1将发送数据  
        return ch;  
    }  
    #endif  
	

int main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);//开启CRC时钟，STEMWIN授权使用
//	RCC_Configuration();
	SysTick_Init();
	GPIO_Conf();
//	TIM1_PWM_Config();
//	TIM2_PWM_Config();
//	TIM6_Config();
//	ADC1_DMA_Init();
//	DAC_Config();
//	USART_Configuration();
//	i2c_CfgGpio();//24C01引脚初始化
	TM1650_GPIO_INT();		/*TM1650引脚初始化 */
	LCD_Initializtion();//液晶屏初始化
//	EEPROM_READ_Coeff();//读取校准参数
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);//关闭风扇
		flag_FAN_ON=0;
	//IWDG_Inte();
	MainTask();
}


/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
