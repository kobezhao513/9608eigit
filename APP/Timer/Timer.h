
#ifndef __TIMER_H__
#define __TIMER_H__

#include "global.h"

//==================系统时间定义========================//
typedef struct 											
{	
	UINT32 Tick;							
	UINT8 Flag1Ms;	
	UINT8 Count1Ms;
	UINT8 Count8Ms;
	UINT8 Count1S;
	UINT8 Sec;										
	UINT8 Min;
	UINT8 Hour;										
	UINT8 Month;										
	UINT8 Data;										
	UINT8 Year;	 
}SYS_TIMER;	//用于系统内定时
extern SYS_TIMER  SysTimer;

/*==================RT1用定时时间定义========================*/

#define		C_TIME_1S              		999 			//系统时间1mS中断产生
#define		C_TIME_1MIN             	59				//系统时间1Min由1S产生
#define		C_TIME_1HOUR              59 				//系统时间1Hour由1Min产生
#define		C_TIME_1DATA              23 				//系统时间1DATA由1Hour产生
#define		C_TIME_1MONTH             30				//系统时间1MONTH由1DATA产生
#define		C_TIME_1YEAR              11 				//系统时间1YEAR 由1MONTH产生

void Timer14_Init(UINT16 arr,UINT16 psc);

UINT32  get_systemtick(void);
void Monitor_Task(void);

#endif
