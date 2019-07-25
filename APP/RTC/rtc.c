#include  <string.h>

//#include "config.h"
//#include "system.h" 
//#include "delay.h"
#include "basic.h"
#include "record_manage.h"
#include "rtc.h" 

#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */

float temp_offset = 0;
//RTC_STRUCT	RTC_timer;			//时钟结构体 	
UINT8 time_string[7];		//他日将年修改为16位的，其可以与RTC_timer合并
const UINT8 mon_table[12] ={31,28,31,30,31,30,31,31,30,31,30,31};//平年的月份日期表
//UINT8 calibr_mode = TRUE;
//UINT8 calibr_time	= 0;
extern UINT8 soft_reset_flag;
RTC_InitTypeDef  RTC_InitStructure;
RTC_TimeTypeDef  RTC_TimeStructure;
RTC_DateTypeDef  RTC_DateStructure;
//RTC_TimeTypeDef  RTC_TimeStampStructure;
//RTC_DateTypeDef  RTC_TimeStampDateStructure;
__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

/******************************************************************
* Function Name: RTC_NVIC_Config()
* Description  : RTC中断配置.
* Arguments    : None
* Return Value : None
********************************************************************/
//void RTC_NVIC_Config(void)
//{	
//    NVIC_InitTypeDef NVIC_InitStructure;
//	
//	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;				//RTC全局中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//先占优先级1位,从优先级3位
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//先占优先级0位,从优先级4位
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能该通道中断
//	NVIC_Init(&NVIC_InitStructure);								//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
//}

/******************************************************************
* Function Name: void RTC_Configuration(void)
* Description  : 配置RTC，设置晶振、时钟源.同时检测时钟
				 是否工作正常BKP->DR1用于保存是否第一次配置的设置
* Arguments    : None
* Return Value : None
********************************************************************/

void RTC_TimeRegulate(UINT8 year,UINT8 month,UINT8 day,UINT8 week,UINT8 hour,UINT8 minute,UINT8 second)
{
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours = hour;
    RTC_TimeStructure.RTC_Minutes = minute;
    RTC_TimeStructure.RTC_Seconds = second;
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		//Show_String(3,72,"系统初始化中，请稍后...");
	} 
	else
	{
		PWR_BackupAccessCmd(ENABLE);
		RTC_WriteBackupRegister(RTC_BKP_DR0,RTC_INIT_FLAG);
	}

	  RTC_DateStructure.RTC_WeekDay = week;
    RTC_DateStructure.RTC_Date = day;
    RTC_DateStructure.RTC_Month = month;

    RTC_DateStructure.RTC_Year = year;
	/* Configure the RTC date register */
	if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
	{
		;
	} 
	else
	{
		/* Indicator for the RTC configuration */
		PWR_BackupAccessCmd(ENABLE);
		RTC_WriteBackupRegister(RTC_BKP_DR0, RTC_INIT_FLAG);
	}

}
/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{
	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* Enable the LSE OSC */
	RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
	}

	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	SynchPrediv = 0xFF;
	AsynchPrediv = 0x7F;

	/* Enable the RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro();

	/* Enable The TimeStamp */
	RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE);   
	RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
	RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
   
	/* Check on RTC init */
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
//207		Record_Save_Local(RECORD_FAULT);
	}
	/* Configure the time register */
	//RTC_TimeRegulate(14,10,10,5,8,8,8);   
}

/*********************************************************************************
* 函数名称 : RTC_Init
* 函数描述 : 上电初始化RTC，如果备份区数据是先前写入的RTC_INIT_FLAG则，
			 不重新配置RTC，反之重新配置RTC.
* 输入参数 : None
* 输出参数 : None
* 返 回 值 : None
* 修改日期 ：2013-5
*********************************************************************************/
void RTC_User_Init(void)
{
//	RTC_NVIC_Config();
	
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != RTC_INIT_FLAG)	//从后备寄存器中读出数据:判断是否第一次写配置时钟
	{	 			
		RTC_Config();
	}
	else
	{
		/* Check if the Power On Reset flag is set */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			RCC_ClearFlag();
		}
		else if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
		{
			RCC_ClearFlag();
		}
		/* Check if the Pin Reset flag is set */
		/*else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
		    //i = 2;
			RCC_ClearFlag();
		}*/
		/* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();

		/* Clear the RTC Alarm Flag */
		RTC_ClearFlag(RTC_FLAG_ALRAF);

		/* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
		EXTI_ClearITPendingBit(EXTI_Line17);
	}
//	RTC_Calibration(100);					//RTC校准
}

/*********************************************************************************
* 函数名称 : RTC_IRQHandler
* 函数描述 : RTC中断服务函数
* 输入参数 : None
* 输出参数 : None
* 返 回 值 : None
* 修改日期 ：2013-5
*********************************************************************************/
//void RTC_IRQHandler(void)
//{							    
//	if(RTC->CRL&0x0001)			//秒钟中断
//	{							
//		RTC_Get();				//更新时间 	 
//	}
//	if(RTC->CRL&0x0002)			//闹钟中断
//	{
//		RTC->CRL&=~(0x0002);	//清闹钟中断
//	} 				  								 
//	RTC->CRL&=0X0FFA;         	//清除溢出，秒钟中断标志
//	while(!(RTC->CRL&(1<<5)));	//等待RTC寄存器操作完成		   							 	   	 
//}

/*********************************************************************************
* 函数名称 : Is_Leap_Year
* 函数描述 : 判断是否是闰年函数：闰年必须能被4整除，但是不能被400整除
* 输入参数 : year:年份
* 输出参数 : None
* 返 回 值 : 该年份是不是闰年.1,是.0,不是
* 修改日期 ：2013-5
*********************************************************************************/
/*UINT8 Is_Leap_Year(UINT16 year)
{			  
	if(year%4 == 0) 			//能被4整除
	{ 
		if(year%100 == 0) 
		{ 
			if(year%400 == 0)	//如果以00结尾,还要能被400整除 
				return 1;	   
			else
				return 0;   
		}
		else
			return 1;   
	}
	else
		return 0;	
}	*/


#ifdef INTERNAL_RTC
/*********************************************************************************
* 函数名称 : Set_RTC_Time
* 函数描述 : 时钟设置：年为UINT8
* 输入参数 : rec：年月日+星期+时分秒.
* 输出参数 : None
* 返 回 值 : None
* 修改日期 ：2013-5
*********************************************************************************/
void Set_RTC_Time(UINT8 *rec)
{
	RTC_TimeRegulate(BCD2Hex(rec[6]),BCD2Hex(rec[5]),BCD2Hex(rec[4]),1,BCD2Hex(rec[2]),BCD2Hex(rec[1]),BCD2Hex(rec[0]));
}
/*********************************************************************************
* 函数名称 : Get_RTC_Time
* 函数描述 : 时钟设置：年为UINT8
* 输入参数 : rec：年月日+星期+时分秒.
* 输出参数 : None
* 返 回 值 : None
* 修改日期 ：2013-5
*********************************************************************************/
void Get_RTC_Time(UINT8 *rec)
{
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	rec[2] = Hex2BCD(RTC_TimeStructure.RTC_Hours);
	rec[1] = Hex2BCD(RTC_TimeStructure.RTC_Minutes);
	rec[0] = Hex2BCD(RTC_TimeStructure.RTC_Seconds);   
	
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	rec[6] = Hex2BCD(RTC_DateStructure.RTC_Year);
	rec[5] = Hex2BCD(RTC_DateStructure.RTC_Month);
	rec[4] = Hex2BCD(RTC_DateStructure.RTC_Date);
	rec[3] = Hex2BCD(RTC_DateStructure.RTC_WeekDay);
}      




/*********************************************************************************
* 函数名称 : RTC_Calibration
* 函数描述 : RTC时钟校准；
* 输入参数 : FastSecPer30days：每30天时钟偏差的秒数;
* 输出参数 : None;
* 返 回 值 : None
* 修改日期 ：2013-12
*********************************************************************************/
void RTC_Calibration(uint16 FastSecPer30days)
{
//	float Deviation = 0.0;
//	UINT8 CalibStep = 0;
//	
//    Deviation = FastSecPer30days * PPM_PER_SEC;//得到ppm误差
//	Deviation /= PPM_PER_STEP; 					//得到校准值的浮点数
//	CalibStep = (UINT8)Deviation; 				// 得到校准值的整形数
//	if(Deviation >= (CalibStep + 0.5))
//    CalibStep  +=  1; //四舍五入
//	if(CalibStep > 127) 
//    CalibStep = 127; 							// 校准值应在0—127之间
//    BKP_SetRTCCalibrationValue(CalibStep);  	//调用库函数
}

/*********************************************************************************
* 函数名称 : RTC_Save_PowerOff_Time
* 函数描述 : 存储关机时间，每秒更新一次，更新于Per_Second_Event_Deal()；
* 输入参数 : None;
* 输出参数 : None;
* 返 回 值 : None
* 修改日期 ：2013-12
*********************************************************************************/
void RTC_Save_PowerOff_Time(void)
{
	UINT16 temp;
	

	PWR_BackupAccessCmd(ENABLE);						//使能RTC和后备寄存器访问 
	
	temp =  RTC_TimeStructure.RTC_Minutes << 8;
	temp += RTC_TimeStructure.RTC_Seconds;
	RTC_WriteBackupRegister(RTC_BKP_DR2,temp);
	temp =  RTC_DateStructure.RTC_Date << 8;
	temp += RTC_TimeStructure.RTC_Hours;
	RTC_WriteBackupRegister(RTC_BKP_DR3,temp);	
	temp =  RTC_DateStructure.RTC_Year << 8;
	temp += RTC_DateStructure.RTC_Month;
	RTC_WriteBackupRegister(RTC_BKP_DR4,temp);	
}
/*********************************************************************************
* 函数名称 : RTC_Save_PowerOff_Time
* 函数描述 : 存储关机时间，每秒更新一次；
* 输入参数 : None;
* 输出参数 : None;
* 返 回 值 : None
* 修改日期 ：2013-12
*********************************************************************************/
void RTC_Get_PowerOff_Time(UINT8 *time_buf)
{
	UINT16 temp;
	
	PWR_BackupAccessCmd(ENABLE);						//使能RTC和后备寄存器访问 
	
	temp = RTC_ReadBackupRegister(RTC_BKP_DR2);
	time_buf[0] = Hex2BCD((UINT8)temp);
	time_buf[1] = Hex2BCD((UINT8)(temp >> 8));
	temp = RTC_ReadBackupRegister(RTC_BKP_DR3);
	time_buf[2] = Hex2BCD((UINT8)temp);
	time_buf[3] = Hex2BCD((UINT8)(temp >> 8));	
	temp = RTC_ReadBackupRegister(RTC_BKP_DR4);
	time_buf[4] = Hex2BCD((UINT8)temp);
	time_buf[5] = Hex2BCD((UINT8)(temp >> 8));	
}
#endif	//end of #ifdef INTERNAL_RTC

