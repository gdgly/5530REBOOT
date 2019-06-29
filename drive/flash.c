/******************** (C) COPYRIGHT 2014 KUNKIN********************
 * 文件名  ：FLASH.C
 * 作者    ：李振
 * 描述    ：STM8L151K4T6 FLASH应用函数
 * 内容    ：
 * 硬件连接：
 * 修改日期：2014-12-22
********************************************************************/
#include "my_register.h" //NOP指令在这
#include "AT24C01.h"
//================================================================//
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
} flagA, flagB,flagC,flagD;
//==================================================================//
void Flash_Write_all (void)
{
	vu8 data_8bit;
/***********************测量数据存储****************************/
  data_8bit = REG_ReadV_Offset >> 24;
	EEPROM_WriteByte(0x01, data_8bit);
	data_8bit = REG_ReadV_Offset >> 16;
	EEPROM_WriteByte(0x02, data_8bit);
	data_8bit = REG_ReadV_Offset >> 8;
	EEPROM_WriteByte(0x03, data_8bit);
	EEPROM_WriteByte(0x04, REG_ReadV_Offset);//测量电压
	
	data_8bit = REG_ReadA_Offset >> 24;
	EEPROM_WriteByte(0x05, data_8bit);
	data_8bit = REG_ReadA_Offset >> 16;
	EEPROM_WriteByte(0x06, data_8bit);
	data_8bit = REG_ReadA_Offset >> 8;
	EEPROM_WriteByte(0x07, data_8bit);
	EEPROM_WriteByte(0x08, REG_ReadA_Offset);//电源电流
	
	data_8bit = REG_ReadA1_Offset >> 24;
	EEPROM_WriteByte(0x33, data_8bit);
	data_8bit = REG_ReadA1_Offset >> 16;
	EEPROM_WriteByte(0x34, data_8bit);
	data_8bit = REG_ReadA1_Offset >> 8;
	EEPROM_WriteByte(0x35, data_8bit);
	EEPROM_WriteByte(0x36, REG_ReadA1_Offset);//负载电流
	
	data_8bit = REG_ReadR_Offset >> 24;
	EEPROM_WriteByte(0x09, data_8bit);
	data_8bit = REG_ReadR_Offset >> 16;
	EEPROM_WriteByte(0x0A, data_8bit);
	data_8bit = REG_ReadR_Offset >> 8;
	EEPROM_WriteByte(0x0B, data_8bit);
	EEPROM_WriteByte(0x0C, REG_ReadR_Offset);//内阻
	
	data_8bit = REG_ReadV1_Offset >> 24;
	EEPROM_WriteByte(0x40, data_8bit);
	data_8bit = REG_ReadV1_Offset >> 16;
	EEPROM_WriteByte(0x41, data_8bit);
	data_8bit = REG_ReadV1_Offset >> 8;
	EEPROM_WriteByte(0x42, data_8bit);
	EEPROM_WriteByte(0x43, REG_ReadV1_Offset);//电源电压*
	
	data_8bit=REG_CorrectionV>>8;
	EEPROM_WriteByte(0x0D, data_8bit);
	EEPROM_WriteByte(0x0E, REG_CorrectionV);
	
	data_8bit=REG_CorrectionA>>8;
	EEPROM_WriteByte(0x0F, data_8bit);
	EEPROM_WriteByte(0x10, REG_CorrectionA);
	
	data_8bit=REG_CorrectionA1>>8;
	EEPROM_WriteByte(0x37, data_8bit);
	EEPROM_WriteByte(0x38, REG_CorrectionA1);
	
	data_8bit=REG_CorrectionR>>8;
	EEPROM_WriteByte(0x11, data_8bit);
	EEPROM_WriteByte(0x12, REG_CorrectionR);
	
	data_8bit=REG_CorrectionV1>>8;
	EEPROM_WriteByte(0x44, data_8bit);
	EEPROM_WriteByte(0x45, REG_CorrectionV1);//电源电压*
	
	EEPROM_WriteByte(0x13,Polar);
	EEPROM_WriteByte(0x46,Polar3);//电源电压*
/***********************设置数据存储****************************/
	data_8bit = SET_ReadV_Offset >> 24;
	EEPROM_WriteByte(0x14, data_8bit);
	data_8bit = SET_ReadV_Offset >> 16;
	EEPROM_WriteByte(0x15, data_8bit);
	data_8bit = SET_ReadV_Offset >> 8;
	EEPROM_WriteByte(0x16, data_8bit);
	EEPROM_WriteByte(0x17, SET_ReadV_Offset);
	
	data_8bit = SET_ReadA_Offset >> 24;
	EEPROM_WriteByte(0x18, data_8bit);
	data_8bit = SET_ReadA_Offset >> 16;
	EEPROM_WriteByte(0x19, data_8bit);
	data_8bit = SET_ReadA_Offset >> 8;
	EEPROM_WriteByte(0x1A, data_8bit);
	EEPROM_WriteByte(0x1B, SET_ReadA_Offset);
	
	data_8bit = SET_ReadA1_Offset >> 24;
	EEPROM_WriteByte(0x39, data_8bit);
	data_8bit = SET_ReadA1_Offset >> 16;
	EEPROM_WriteByte(0x3A, data_8bit);
	data_8bit = SET_ReadA1_Offset >> 8;
	EEPROM_WriteByte(0x3B, data_8bit);
	EEPROM_WriteByte(0x3C, SET_ReadA1_Offset);
	
	data_8bit=SET_CorrectionV>>8;
	EEPROM_WriteByte(0x1C, data_8bit);
	EEPROM_WriteByte(0x1D, SET_CorrectionV);
	
	data_8bit=SET_CorrectionA>>8;
	EEPROM_WriteByte(0x1E, data_8bit);
	EEPROM_WriteByte(0x1F, SET_CorrectionA);
	
	data_8bit=SET_CorrectionA1>>8;
	EEPROM_WriteByte(0x3D, data_8bit);
	EEPROM_WriteByte(0x3E, SET_CorrectionA1);
	
	EEPROM_WriteByte(0x20,Polar1);
/***********************************************************/
	data_8bit=CON_CorrectionA>>8;
	EEPROM_WriteByte(0x21, data_8bit);
	EEPROM_WriteByte(0x22, CON_CorrectionA);
	
	data_8bit = CON_ReadA_Offset >> 24;
	EEPROM_WriteByte(0x23, data_8bit);
	data_8bit = CON_ReadA_Offset >> 16;
	EEPROM_WriteByte(0x24, data_8bit);
	data_8bit = CON_ReadA_Offset >> 8;
	EEPROM_WriteByte(0x25, data_8bit);
	EEPROM_WriteByte(0x26, CON_ReadA_Offset);
	
	EEPROM_WriteByte(0x27,Polar2);
	
	data_8bit=TEMP_OFFSET>>8;
	EEPROM_WriteByte(0x28, data_8bit);
	EEPROM_WriteByte(0x29, TEMP_OFFSET);
}
//===========================================================================//
void EEPROM_READ_Coeff(void)
{
/****************测量参数读取*********************/
	REG_ReadV_Offset=EEPROM_READ_Byte(0x01);
	REG_ReadV_Offset=REG_ReadV_Offset<<8;
	REG_ReadV_Offset=REG_ReadV_Offset+EEPROM_READ_Byte(0x02);
	REG_ReadV_Offset=REG_ReadV_Offset<<8;
	REG_ReadV_Offset=REG_ReadV_Offset+EEPROM_READ_Byte(0x03);
	REG_ReadV_Offset=REG_ReadV_Offset<<8;
	REG_ReadV_Offset=REG_ReadV_Offset+EEPROM_READ_Byte(0x04);
	
	REG_ReadV1_Offset=EEPROM_READ_Byte(0x40);
	REG_ReadV1_Offset=REG_ReadV1_Offset<<8;
	REG_ReadV1_Offset=REG_ReadV1_Offset+EEPROM_READ_Byte(0x41);
	REG_ReadV1_Offset=REG_ReadV1_Offset<<8;
	REG_ReadV1_Offset=REG_ReadV1_Offset+EEPROM_READ_Byte(0x42);
	REG_ReadV1_Offset=REG_ReadV1_Offset<<8;
	REG_ReadV1_Offset=REG_ReadV1_Offset+EEPROM_READ_Byte(0x43);
	
	REG_ReadA_Offset=EEPROM_READ_Byte(0x05);
	REG_ReadA_Offset=REG_ReadA_Offset<<8;
	REG_ReadA_Offset=REG_ReadA_Offset+EEPROM_READ_Byte(0x06);
	REG_ReadA_Offset=REG_ReadA_Offset<<8;
	REG_ReadA_Offset=REG_ReadA_Offset+EEPROM_READ_Byte(0x07);
	REG_ReadA_Offset=REG_ReadA_Offset<<8;
	REG_ReadA_Offset=REG_ReadA_Offset+EEPROM_READ_Byte(0x08);
	
	REG_ReadA1_Offset=EEPROM_READ_Byte(0x33);
	REG_ReadA1_Offset=REG_ReadA1_Offset<<8;
	REG_ReadA1_Offset=REG_ReadA1_Offset+EEPROM_READ_Byte(0x34);
	REG_ReadA1_Offset=REG_ReadA1_Offset<<8;
	REG_ReadA1_Offset=REG_ReadA1_Offset+EEPROM_READ_Byte(0x35);
	REG_ReadA1_Offset=REG_ReadA1_Offset<<8;
	REG_ReadA1_Offset=REG_ReadA1_Offset+EEPROM_READ_Byte(0x36);
	
	REG_ReadR_Offset=EEPROM_READ_Byte(0x09);
	REG_ReadR_Offset=REG_ReadR_Offset<<8;
	REG_ReadR_Offset=REG_ReadR_Offset+EEPROM_READ_Byte(0x0A);
	REG_ReadR_Offset=REG_ReadR_Offset<<8;
	REG_ReadR_Offset=REG_ReadR_Offset+EEPROM_READ_Byte(0x0B);
	REG_ReadR_Offset=REG_ReadR_Offset<<8;
	REG_ReadR_Offset=REG_ReadR_Offset+EEPROM_READ_Byte(0x0C);
	
	REG_CorrectionV=EEPROM_READ_Byte(0x0D);
	REG_CorrectionV=REG_CorrectionV<<8;
	REG_CorrectionV=REG_CorrectionV+EEPROM_READ_Byte(0x0E);
	
	REG_CorrectionV1=EEPROM_READ_Byte(0x44);
	REG_CorrectionV1=REG_CorrectionV1<<8;
	REG_CorrectionV1=REG_CorrectionV1+EEPROM_READ_Byte(0x45);
	
	REG_CorrectionA=EEPROM_READ_Byte(0x0F);
	REG_CorrectionA=REG_CorrectionA<<8;
	REG_CorrectionA=REG_CorrectionA+EEPROM_READ_Byte(0x10);
	
	REG_CorrectionA1=EEPROM_READ_Byte(0x37);
	REG_CorrectionA1=REG_CorrectionA1<<8;
	REG_CorrectionA1=REG_CorrectionA1+EEPROM_READ_Byte(0x38);
	
	REG_CorrectionR=EEPROM_READ_Byte(0x11);
	REG_CorrectionR=REG_CorrectionR<<8;
	REG_CorrectionR=REG_CorrectionR+EEPROM_READ_Byte(0x12);
	
	Polar=EEPROM_READ_Byte(0x13);
	Polar3=EEPROM_READ_Byte(0x46);
/*******************设置参数读取****************************/
	SET_ReadV_Offset=EEPROM_READ_Byte(0x14);
	SET_ReadV_Offset=SET_ReadV_Offset<<8;
	SET_ReadV_Offset=SET_ReadV_Offset+EEPROM_READ_Byte(0x15);
	SET_ReadV_Offset=SET_ReadV_Offset<<8;
	SET_ReadV_Offset=SET_ReadV_Offset+EEPROM_READ_Byte(0x16);
	SET_ReadV_Offset=SET_ReadV_Offset<<8;
	SET_ReadV_Offset=SET_ReadV_Offset+EEPROM_READ_Byte(0x17);
	
	SET_ReadA_Offset=EEPROM_READ_Byte(0x18);
	SET_ReadA_Offset=SET_ReadA_Offset<<8;
	SET_ReadA_Offset=SET_ReadA_Offset+EEPROM_READ_Byte(0x19);
	SET_ReadA_Offset=SET_ReadA_Offset<<8;
	SET_ReadA_Offset=SET_ReadA_Offset+EEPROM_READ_Byte(0x1A);
	SET_ReadA_Offset=SET_ReadA_Offset<<8;
	SET_ReadA_Offset=SET_ReadA_Offset+EEPROM_READ_Byte(0x1B);
	
	SET_ReadA1_Offset=EEPROM_READ_Byte(0x39);
	SET_ReadA1_Offset=SET_ReadA1_Offset<<8;
	SET_ReadA1_Offset=SET_ReadA1_Offset+EEPROM_READ_Byte(0x3A);
	SET_ReadA1_Offset=SET_ReadA1_Offset<<8;
	SET_ReadA1_Offset=SET_ReadA1_Offset+EEPROM_READ_Byte(0x3B);
	SET_ReadA1_Offset=SET_ReadA1_Offset<<8;
	SET_ReadA1_Offset=SET_ReadA1_Offset+EEPROM_READ_Byte(0x3C);
	
	SET_CorrectionV=EEPROM_READ_Byte(0x1C);
	SET_CorrectionV=SET_CorrectionV<<8;
	SET_CorrectionV=SET_CorrectionV+EEPROM_READ_Byte(0x1D);
	
	SET_CorrectionA=EEPROM_READ_Byte(0x1E);
	SET_CorrectionA=SET_CorrectionA<<8;
	SET_CorrectionA=SET_CorrectionA+EEPROM_READ_Byte(0x1F);
	
	SET_CorrectionA1=EEPROM_READ_Byte(0x3D);
	SET_CorrectionA1=SET_CorrectionA1<<8;
	SET_CorrectionA1=SET_CorrectionA1+EEPROM_READ_Byte(0x3E);
	
	
	Polar1=EEPROM_READ_Byte(0x20);
	
	CON_CorrectionA=EEPROM_READ_Byte(0x21);
	CON_CorrectionA=CON_CorrectionA<<8;
	CON_CorrectionA=CON_CorrectionA+EEPROM_READ_Byte(0x22);
	
	CON_ReadA_Offset=EEPROM_READ_Byte(0x23);
	CON_ReadA_Offset=CON_ReadA_Offset<<8;
	CON_ReadA_Offset=CON_ReadA_Offset+EEPROM_READ_Byte(0x24);
	CON_ReadA_Offset=CON_ReadA_Offset<<8;
	CON_ReadA_Offset=CON_ReadA_Offset+EEPROM_READ_Byte(0x25);
	CON_ReadA_Offset=CON_ReadA_Offset<<8;
	CON_ReadA_Offset=CON_ReadA_Offset+EEPROM_READ_Byte(0x26);
	Polar2=EEPROM_READ_Byte(0x27);
	
/*****************读取关机前设置值*************************/
	//SET_Voltage=EEPROM_READ_Byte(0x2A);
	//SET_Voltage=SET_Voltage<<8;
	//SET_Voltage=SET_Voltage+EEPROM_READ_Byte(0x2B);
	
	//SET_Current=EEPROM_READ_Byte(0x2C);
	//SET_Current=SET_Current<<8;
	//SET_Current=SET_Current+EEPROM_READ_Byte(0x2D);
	
	TEMP_OFFSET=EEPROM_READ_Byte(0x28);
	TEMP_OFFSET=TEMP_OFFSET<<8;
	TEMP_OFFSET=TEMP_OFFSET+EEPROM_READ_Byte(0x29);

/*****************读取蜂鸣器状态*************************/
	//Beep_Flag=EEPROM_READ_Byte(0x2E);
	//t_beep=EEPROM_READ_Byte(0x2F);
	//if(Beep_Flag==0xff)
	//{
		//Flag_Beep_OFF=1;
	//}
	///else
	//{
		//Flag_Beep_OFF=0;
	//}
	t_lock=EEPROM_READ_Byte(0x30);
	/***************读本机地址*********************************/
	ADDR=EEPROM_READ_Byte(0x31);
	OFFSET_Zore=EEPROM_READ_Byte(0x32);
	if(OFFSET_Zore>10)
	{
		OFFSET_Zore=0;
	}
}
/****************************************************************************/
void Wrtite_Zore(void)
{
	EEPROM_WriteByte(0x32, OFFSET_Zore);
}
void Wrtite_SetVA(void)
{
	vu8 data_8bit;
  /*********************************************/
	data_8bit = SET_Voltage >> 8;
	EEPROM_WriteByte(0x2A, data_8bit);
	EEPROM_WriteByte(0x2B, SET_Voltage);
	
	data_8bit = SET_Current >> 8;
	EEPROM_WriteByte(0x2C, data_8bit);
	EEPROM_WriteByte(0x2D, SET_Current);
	/********************************************/
}
/******************************************************************************/
void Wrtite_Lock(void)
{
	EEPROM_WriteByte(0x30, t_lock);
}
/*****************************************************************************/
void Wrtite_Beep(void)//保存BEEP状态至FLASH中
{
	EEPROM_WriteByte(0x2E, Beep_Flag);
	EEPROM_WriteByte(0x2F, t_beep);
}
/************************************************************************/
void Wite_adress(void)
{
	EEPROM_WriteByte(0x31,ADDR);
}
/******************* (C) COPYRIGHT 2015 AVER *****END OF FILE*************************/