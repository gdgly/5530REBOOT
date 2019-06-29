/******************************************************************/
/* ÂêçÁß∞ÔºöADC0                                                    */
/* ÊïàÊûúÔºö                                                        */
/* ÂÜÖÂÆπÔºö                                                       */
/* ‰ΩúËÄÖÔºözhan                                                  */
/* ËÅîÁ≥ªÊñπÂºèQQ:363116119                                        */
/******************************************************************/
#include "adc.h"
#include "my_register.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
/*****************************************************************/
#define ADC1_DR_Address       0x4001204C
vu16 NTC_value;
vu16 Imon_value;
vu16 Imon1_value;
vu16 Vmon_value;
vu16 Vmon1_value;
vu16 Rmon_value;
vu16 Contr_Voltage;//ÁîµÂéãDACÂÄº
vu16 Contr_Current;//ÁîµÊµÅDACÂÄº
vu16 Contr_Laod;//Ë¥üËΩΩDACÂÄº
vu16 Contr_Voltage_Laod;//Ë¥üËΩΩÁîµÂéãDACÂÄº
vu16 Contr_Current_Laod;//Ë¥üËΩΩÁîµÊµÅDACÂÄº
vu16 ADC1_Buffer[300];
vu16 ADC_Vmon_Filt[50];
vu16 ADC_Vmon1_Filt[50];
vu16 ADC_Imon_Filt[50];
vu16 ADC_Imon1_Filt[50];
vu16 ADC_Rmon_Filt[50];
vu16 ADC_NTC_Filt[50];
vu16 Imon_Filt_Doul[20];
vu16 Vmon_Filt_Doul[40];
vu16 Vmon1_Filt_Doul[20];
vu16 Rmon_Filt_Doul[20];
vu16 Imon1_Filt_Doul[20];
/*****************************************************************/
void ADC1_DMA_Init(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;
	
		/* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  /* DMA1 clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  /* Configure PA.0  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);			// 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//
  /* DMA1 Channel1 Config */
  DMA_InitStructure.DMA_Channel = DMA_Channel_0; 
  DMA_InitStructure.DMA_PeripheralBaseAddr = (vu32)ADC1_DR_Address;
  DMA_InitStructure.DMA_Memory0BaseAddr = (vu32)&ADC1_Buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize =300;//¡¨–¯◊™ªª250¥Œ
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  	
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  
  /* DMA1 Channel1 enable */
  DMA_Cmd(DMA2_Stream0, ENABLE);
	/* ÂºÄÂêØDAM1ÈÄöÈÅì1‰∏≠Êñ≠ */
	DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
  DAM1_ADC_NVIC();//DMA÷–∂œ≈‰÷√
  ADC_DMACmd(ADC1, ENABLE);  // πƒ‹DMA
  
  /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;			//
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 				//
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;			//
	ADC_InitStructure.ADC_ExternalTrigConv =ADC_ExternalTrigConvEdge_None;	//
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//
	ADC_InitStructure.ADC_NbrOfConversion = 6;	 								//
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*≈‰÷√ADC ±÷”*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_84Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 4, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 6, ADC_SampleTime_3Cycles);
  /* ADC Calibration */
  ADC_Cmd(ADC1, ENABLE);// πƒ‹ADC
	ADC_SoftwareStartConv(ADC1);   //ø™ º◊™ªª
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
}
/***************************************************************/
void DAM1_ADC_NVIC(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the adc1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************/
void ADC_CH_Scan(void)//Êª§Ê≥¢
{
	vu16 i;
	static vu8 a,b,c,d,e,f;
	vu8 count;
	static vu8 t_Filt;
	static vu8 t_dsoI,t_dsoV,t_dsoR,t_dsoI1,t_dsoV1;
	vu32 sum;
	for(i=0;i<300;i++)
	{
		if ((i % 6) == 0)
		{
			ADC_Vmon_Filt[a++]=ADC1_Buffer[i];
		}
		else if((i % 6) ==1 )
		{
			ADC_NTC_Filt[b++]=ADC1_Buffer[i];
		}
		else if((i % 6) ==2)
		{
			ADC_Imon_Filt[c++]=ADC1_Buffer[i];
		}	
		else if((i % 6) ==3)
		{
			ADC_Vmon1_Filt[d++]=ADC1_Buffer[i];
		}	
		else if((i % 6) ==4)
		{
			ADC_Rmon_Filt[e++]=ADC1_Buffer[i];
		}	
		else if((i % 6) ==5)
		{
			ADC_Imon1_Filt[f++]=ADC1_Buffer[i];
		}
	}
	a=0;
	b=0;
	c=0;
	d=0;
	e=0;
	f=0;
	sum=0;
/*************VMON**********************/
	for(count=0;count<50;count++)
	{
		sum +=ADC_Vmon_Filt[count];
	}
	if(t_dsoV<40)
	{
		Vmon_Filt_Doul[t_dsoV]=sum/50;
		sum=0;//
	}
	else
	{
		sum=0;
		for(count=1;count<39;count++)
		{
			sum +=Vmon_Filt_Doul[count];
		}
		Vmon_value=sum/39;
		sum=0;//
		t_dsoV=0;
	} 
	t_dsoV++;
/************IMON*********************/
	for(count=0;count<50;count++)
	{
		sum +=ADC_Imon_Filt[count];
	}
	if(t_dsoI<20)
	{
		Imon_Filt_Doul[t_dsoI]=sum/50;
		sum=0;//
	}
	else
	{
		sum=0;
		for(count=1;count<19;count++)
		{
			sum +=Imon_Filt_Doul[count];
		}
		Imon_value=sum/18;
		sum=0;//
		t_dsoI=0;
	}
	t_dsoI++;
/************IMON1*********************/
	for(count=0;count<50;count++)
	{
		sum +=ADC_Imon1_Filt[count];
	}
	if(t_dsoI1<20)
	{
		Imon1_Filt_Doul[t_dsoI1]=sum/50;
		sum=0;//
	}
	else
	{
		sum=0;
		for(count=1;count<19;count++)
		{
			sum +=Imon1_Filt_Doul[count];
		}
		Imon1_value=sum/19;
		sum=0;//
		t_dsoI1=0;
	}
	t_dsoI1++;
/************RMON*********************/
	for(count=0;count<50;count++)
	{
		sum +=ADC_Rmon_Filt[count];
	}
	if(t_dsoR<20)
	{
		Rmon_Filt_Doul[t_dsoR]=sum/50;
		sum=0;//
	}
	else
	{
		sum=0;
		for(count=1;count<19;count++)
		{
			sum +=Rmon_Filt_Doul[count];
		}
		Rmon_value=sum/19;
		sum=0;//
		t_dsoR=0;
	}
	t_dsoR++;
/*************VMON1**********************/
	for(count=0;count<50;count++)
	{
		sum +=ADC_Vmon1_Filt[count];
	}
	if(t_dsoV1<20)
	{
		Vmon1_Filt_Doul[t_dsoV1]=sum/50;
		sum=0;//
	}
	else
	{
		sum=0;
		for(count=1;count<19;count++)
		{
			sum +=Vmon1_Filt_Doul[count];
		}
		Vmon1_value=sum/18;
		sum=0;//
		t_dsoV1=0;
	} 
	t_dsoV1++;
/**********NTC**********************/
	for(count=0;count<50;count++)
	{
		sum +=ADC_NTC_Filt[count];
	}
	NTC_value=sum/50;
}