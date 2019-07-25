#ifndef TYPE_DEFINE_H
#define	TYPE_DEFINE_H

#include "global.h"

typedef enum  
{
	DEV_FUN_NULL		= 0x00,							//无功能
	DEV_FUN_IN 			= 0x01,							//输入功能
	DEV_FUN_OUT  		= 0x02,							//输出功能
	DEV_FUN_OUT_IN  	= 0x03,							//输入输出功能
	DEV_FUN_ANS  		= 0x04,							//反馈功能
	DEV_FUN_OUT_ANS  	= 0x06,							//反馈功能
}E_DEV_BASE_FUNCTION;


//============================数据类型定义====================================
typedef enum
{
	TYPE_CRT_POLL = 1,			//1
	TYPE_CRT_ALARM,				//2
	TYPE_CRT_START,				//3
	TYPE_CRT_ANSWER,			//4
	TYPE_CRT_MONITER,			//5
	TYPE_CRT_FAULT,				//6
	TYPE_CRT_FAULT_RECOVER,		//7
	TYPE_CRT_SHIELD,			//8
	TYPE_CRT_SHIELD_RESUME,		//9
	TYPE_CRT_POWERON,			//10
	TYPE_CRT_POWEROFF,			//11
	TYPE_CRT_RESET,				//12
	TYPE_CRT_SELFCHECK,			//13
	TYPE_CRT_FAULT_MAINPOW,		//14
	TYPE_CRT_FAULT_BATPOW,		//15
	TYPE_CRT_FAULT_MAIN_RECOVER,//16
	TYPE_CRT_FAULT_BAT_RECOVER,	//17
	TYPE_CRT_FAULT_LINE,		//18
	TYPE_CRT_FAULT_LINE_RECOVER,//19
    TYPE_CRT_DELAY,				//20
	TYPE_CRT_EARLY_ALARM,		//21
	TYPE_CRT_RESERVE,			//20-127
	
}TYPE_CRT_EVENT;



typedef struct 
{
	UINT8                DevType;
	const UINT8 *        BriefDescribe;
	const UINT8 *        DetailDescribe;
	UINT16               QuiescentDissipation;       //单位是uA
	UINT16               DynamicDissipation;         //单位是uA
	E_DEV_BASE_FUNCTION  InOutAttribute;
	UINT8 const*         Ico;						
	UINT16 const * const EventType; 
}S_DEV_TYPE_INF;


typedef struct event_type
{
	UINT16               EventType;
	UINT8        		 generic_type;
	UINT8        		 Crt_type;
	UINT8 const * const  Event_Annotaiton; 
}S_EVENT_TYPE_INF;


typedef struct 
{
	UINT8               ClassType;
	UINT8        		Tts_Type;
	UINT8 const * const Class_Tab;
	UINT8 const*        Ico;
}S_EVENT_CLASS_INF;



#endif

