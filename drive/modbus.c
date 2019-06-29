/******************** (C) COPYRIGHT 2014 KUNKIN********************
 * 文件名  :MODBUS.C
 * 作者   :
 * 描述    :MODBUS?????
 * 内容    :KL220A_Master
 * 硬件连接: 485
 * 修改日期:2014-12-22
********************************************************************/
#include "my_register.h" 
#include "usart.h" 
#include "modbus.h" 
#include "flash.h"
#include "stm32f4xx_gpio.h"
//===================================================================//
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
} flagA,flagB,flagC,flagD,flagE,flagG;
/*************************校准参数************************************/
vu16 ADJ_Write;//校准时需写入的实际值
vu8 DAC_Flag;//DAC是否加载标志
vu16 Modify_A_READ;
vu16 Modify_C_READ;
vu16 Modify_A_ACT;
	
vu16 Modify_B_READ;
vu16 Modify_D_READ;
vu16 Modify_B_ACT;
vu16 Correct_Parametet[13];//校准参数
vu32 Correct_Strong[9];//校准系数
/*************************变量定义***********************************/
vu16 Run_Control[14];
vu8 ADDR;
//============================函数声明===============================//
vu16 Hardware_CRC(vu8 *p_buffer,vu8 count);
//===========================MODBUS协议=============================//
void UART_Action(void)
{//RUT格式：
	//ADDR  命令码  读寄存器的起始地址高   读寄存器的起始地址低  读数据字个数高字节   读数据个数低字节  CRC高 CRC低
	//返回格式：ADDR 命令码 返回数据字节数  数据高  数据低 ..... CRC高  CRC低
	if (UART_Buffer_Rece[0] == ADDR)
	{
		if (UART_Buffer_Rece[1] == (0x03))	//命令3 读数据   
		{																		 
			vu8 i;
			vu16 crc_result;
			crc_result = (UART_Buffer_Rece[6] << 8) + UART_Buffer_Rece[7];
			if ((crc_result == Hardware_CRC(UART_Buffer_Rece,6)) ||(crc_result == 0) )
			{
				if (UART_Buffer_Rece[3] < 0x07)    								//如果寄存器在可读范围内
				{
					if ((UART_Buffer_Rece[3] + UART_Buffer_Rece[5]) < 0x0F)		//如果最后一个读取的寄存器地址在可读范围内
					{							
						UART_Buffer_Send[0] = ADDR;
						UART_Buffer_Send[1] = 0x03;
						UART_Buffer_Send[2] = UART_Buffer_Rece[5]*2;
						for (i=0;i<UART_Buffer_Send[2];i++)
						{
							if ((i % 2) == 0) UART_Buffer_Send[3 + i] = Run_Control[UART_Buffer_Rece[3] + i / 2] >> 8;
							else UART_Buffer_Send[3 + i] = Run_Control[UART_Buffer_Rece[3] + i / 2];														
						}
						crc_result = Hardware_CRC(UART_Buffer_Send,UART_Buffer_Send[2] + 3);
						UART_Buffer_Send[3 + UART_Buffer_Send[2]] = crc_result >> 8;
						UART_Buffer_Send[4 + UART_Buffer_Send[2]] = crc_result;
						Transmit_BUFFERsize = UART_Buffer_Send[2] + 5;
						UART_SEND_flag=1;
					}
				}
			}	
		}
	} 
//===============================写寄存器=================================
	if ((UART_Buffer_Rece[0] == 0) || (UART_Buffer_Rece[0] == ADDR) || (UART_Buffer_Rece[0] == ((ADDR-1)/4+100)))	 
	{
		vu8 var8;
		vu8 a=0;
		vu16 var16;
		vu16 crc_result;
//=========================以下命令6 写单个寄存器===========================
		if (UART_Buffer_Rece[1] == 6)                                 //判断第二个字节是否为命令6
		{
			if (UART_Buffer_Rece[3] < 0x05)							  //判断需要写的地址是否在可写范围内
			{
				crc_result = (UART_Buffer_Rece[6] << 8) + UART_Buffer_Rece[7];
				if ((crc_result == Hardware_CRC(UART_Buffer_Rece,6)) ||(crc_result == 0) )		  //检查CRC
				{
					var16 = (UART_Buffer_Rece[4] << 8) + UART_Buffer_Rece[5];	//第5 6个字节为要写入的数据
					var8 = UART_Buffer_Rece[3];	        						//第3 4个字节为要写入的地址
					Run_Control[var8] = var16;			    //将数据写入指定的地址

					if (UART_Buffer_Rece[0] == ADDR)							//广播模式下不返回数据
					{
						for (a=0;a<8;a++)
						{UART_Buffer_Send[a] = UART_Buffer_Rece[a];}
						Transmit_BUFFERsize = 8;						//原样数据返回，不计算CRC
						UART_SEND_flag=1;
					}
				}
			}
		}
//=======================================以下是命令16，连写寄存器===========================================
//功能码16格式:
//     地址 命令 写入起始地址高  写入起始地址低 写入字数高  写入字数低 写入字节数  数据高 数据低 ......CRC高 CRC低
//返回数据格式:
//     地址 命令 写入起始地址高  写入起始地址低  写入字节数高 写入字节数低  CRC高  CRC低 
		if (UART_Buffer_Rece[1] == 16)										  
		{	
			if ((UART_Buffer_Rece[6] == 6) && (UART_Buffer_Rece[3] == 0x00))	//规定连写寄存器的个数
			{
				crc_result = (UART_Buffer_Rece[13] << 8) + UART_Buffer_Rece[14];
				if ((crc_result == Hardware_CRC(UART_Buffer_Rece,13)) ||(crc_result == 0) )	   //检查CRC
				{												
					for (var8=0;var8<3;var8++) Run_Control[var8] = (UART_Buffer_Rece[var8*2+7] << 8) + UART_Buffer_Rece[var8*2+8];

					if (UART_Buffer_Rece[0] == ADDR)					  //广播模式不返回数据
					{
						UART_Buffer_Send[0] = ADDR;
						UART_Buffer_Send[1] = 16;
						UART_Buffer_Send[2] = UART_Buffer_Rece[2];
						UART_Buffer_Send[3] = UART_Buffer_Rece[3];
						UART_Buffer_Send[4] = UART_Buffer_Rece[4];
						UART_Buffer_Send[5] = UART_Buffer_Rece[5];
						crc_result = Hardware_CRC(UART_Buffer_Send,6);	 //计算CRC码
						UART_Buffer_Send[6] = crc_result>>8;
						UART_Buffer_Send[7] = crc_result;				 
						Transmit_BUFFERsize = 8;					     //设置发送字节数长度
						UART_SEND_flag=1;
					}
				}
			}			 
		}
	}
//===================================================================================
}
//===============================AD值转换成测量值============================================//
void Transformation_ADC(void)  
{
	vu32 var32;
	vu32 var32a;
/*****************************测量电压转换*******************************************/
	var32 = Vmon_value;
	var32 = var32 * REG_CorrectionV;  
	if ((Polar & 0x01) == 0x01)		  
	{
		if (var32 < REG_ReadV_Offset) 
		{
			var32 = 0;
		}
		else var32 = var32 - REG_ReadV_Offset;
	}
	else var32 = var32 + REG_ReadV_Offset;
	var32 = var32 >> 12;
	if (var32 < 5) var32 = 0;				  //40mV以下清零
	Voltage = var32;
	DISS_Voltage=Voltage;
	DISS_Voltage=DISS_Voltage/100;//计算显示电压
	var32 = 0;
/*****************************稳压电源测量电压转换*******************************************/
	var32 = Vmon1_value;
	var32 = var32 * REG_CorrectionV1;  
	if ((Polar3 & 0x01) == 0x01)		  
	{
		if (var32 < REG_ReadV1_Offset) 
		{
			var32 = 0;
		}
		else var32 = var32 - REG_ReadV1_Offset;
	}
	else var32 = var32 + REG_ReadV1_Offset;
	var32 = var32 >> 14;
	if (var32 < 5) var32 = 0;				  //40mV以下清零
	POW_Voltage = var32;
	DISS_POW_Voltage=POW_Voltage;
	DISS_POW_Voltage=DISS_POW_Voltage/100;//计算显示电压
	var32 = 0;
/*****************************内阻值转换*******************************************/
	var32 = Rmon_value;
	var32 = var32 * REG_CorrectionR;  
	if ((Polar1 & 0x04) == 0x04)		  
	{
		if (var32 < REG_ReadR_Offset) 
		{
			var32 = 0;
		}
		else var32 = var32 - REG_ReadR_Offset;
	}
	else var32 = var32 + REG_ReadR_Offset;
	var32 = var32 >> 12;
	if (var32 < 5) var32 = 0;				  //40mV以下清零
	R_VLUE = var32;
	var32 = 0;	
/*****************************稳压电源测量电流转换*******************************************/
	var32 = Imon_value;
	var32 = var32 * CON_CorrectionA;	   
	if ((Polar2 & 0x01) == 0x01)			   
	{
		if (var32 < CON_ReadA_Offset) var32 = 0;
		else var32 = var32 - CON_ReadA_Offset;
	}
	else
	{
		var32 = var32 + CON_ReadA_Offset;
	}	
	var32 = var32 >> 12;
	Current = var32;;
	DISS_POW_Current=Current;
	DISS_POW_Current=DISS_POW_Current/1000;//计算显示电流
/**************************稳压电源设置电压转换******************************************/
	var32 = SET_Voltage;
	var32=var32<<14;   
	if ((Polar1 & 0x01) == 0)			   
	{
		if (var32 < SET_ReadV_Offset) var32 = 0;
		else var32 = var32 - SET_ReadV_Offset;
	}
	else var32 = var32 + SET_ReadV_Offset;
	var32 = var32/SET_CorrectionV;
	var32=var32>>1;
	Contr_Voltage = var32;
	if(SET_Voltage==0)
	{
		Contr_Voltage=0;
	}
	var32 = 0;
/**************************稳压电源设置电流转换**************************************/
	var32 = SET_Current;
	var32=var32<<14;   
	if ((Polar1 & 0x08) == 0)			   
	{
		if (var32 < SET_ReadA_Offset) var32 = 0;
		else var32 = var32 - SET_ReadA_Offset;
	}
	else var32 = var32 + SET_ReadA_Offset;
	var32 = var32/SET_CorrectionA;
	var32=var32>>1;
	Contr_Current = var32;
	if(SET_Current==0)
	{
		Contr_Current=0;
	}
	
	var32 = 0;

/*************************负载电流控制转换**************************************/
	var32 = SET_Current_Laod;
	var32=var32<<12;   
	if ((Polar2 & 0x08) == 0)			   
	{
		if (var32 < SET_ReadA1_Offset) var32 = 0;
		else var32 = var32 - SET_ReadA1_Offset;
	}
	else var32 = var32 + SET_ReadA1_Offset;
	var32 = var32/SET_CorrectionA1;
	var32=var32>>1;
	if(Flag_DAC_OFF==0)
	{
		Contr_Laod = var32;
	}
	if(SET_Current_Laod==0)
	{
		Contr_Laod=0;
	}
	
	var32 = 0;
/*******************负载测量电流转换**************************************/
	var32 = Imon1_value;
	var32 = var32 * REG_CorrectionA1;  
	if ((Polar2 & 0x04) == 0x04)		  
	{
		if (var32 < REG_ReadA1_Offset) 
		{
			var32 = 0;
		}
		else var32 = var32 - REG_ReadA1_Offset;
	}
	else var32 = var32 + REG_ReadA1_Offset;
	var32 = var32 >> 12;
	Laod_Current = var32;
	DISS_Current=Laod_Current;
	DISS_Current=DISS_Current/100;//计算显示电流
	var32 = 0;		
	
}
/********************************************************************************
  * 名称 :   MODBUS_Conrtl
  * 作者 :  zhan
  * 版本 :  V1.0
  * 日期 :  2016-xx-xx
  * 作用 :  MODBUS解析函数
  ******************************************************************************/ 
//-----------------------------CRC检测--------------------------------------------//
vu16 Hardware_CRC(vu8 *p_buffer,vu8 count)    //CRC16
{
	vu16 CRC_Result=0xffff;
	vu8 i;
	if(count==0)
	{
		count=1;
	}
	while(count--)
	{
		CRC_Result^=*p_buffer;
		for(i=0;i<8;i++)
		{
			if(CRC_Result&1)
			{
				CRC_Result>>=1;
				CRC_Result^=0xA001;
			}
			else 
			{
				CRC_Result>>=1;
			}
		}
		p_buffer++;
	}
	return CRC_Result;
}