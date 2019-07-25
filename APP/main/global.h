#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "string.h"
#include "stdio.h"
#include <stdlib.h>
#include "stm32f0xx.h"

typedef unsigned char       UINT8;
typedef unsigned char       uchar;
typedef unsigned char       uint8;
typedef unsigned short int     UINT16;
typedef unsigned short int     uint16;
typedef unsigned long       UINT32;
typedef unsigned long       uint32;
typedef unsigned char 		UCHAR8; 
typedef unsigned char 		uchar8; 


#define BIT0 				(1u<<0)
#define BIT1 				(1u<<1)
#define BIT2 				(1u<<2)
#define BIT3 				(1u<<3)
#define BIT4 				(1u<<4)
#define BIT5 				(1u<<5)
#define BIT6 				(1u<<6)
#define BIT7 				(1u<<7)

#define		EI() 	__enable_irq()
#define		DI() 	__disable_irq()

#define		INTERNAL_RTC

#define   LEN_REG_LA       			1
#define   LEN_REG_UC       			5
#define   LEN_REG_ID       			12
#define   LEN_REG_CC       			32

#define   LEN_TEL_NUMBER            3

#define   FIRE_EXTINGUISHER_SYSTEM   0x01		//消防
#define   DEVICE_TYPE                0x13		//
#define   TYPE_LEVEL                 0x34		//

#define		DEVICE_TYPE_OF_UC_CODE	 0x85	//联网模块
#define		DEV_TYPE_CONTROL_UC      0x01   //控制器UC

#define 	MAX_LOOP_NUM					  4			//最大回路总数 
#define 	MAX_DEVICE_NUM					64			//回路内最大设备数


#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  1
#endif



typedef union 
{
    UINT8 byte;
    struct 
    {
        UINT8 b0       :1;
        UINT8 b1       :1;
        UINT8 b2       :1;
        UINT8 b3       :1;
        UINT8 b4       :1;
        UINT8 b5       :1;
        UINT8 b6       :1;
        UINT8 b7       :1; 
    }b;
}BIT_CHAR;

typedef struct 
{
    // 工程定义	
   	UINT8 project_name[32];       			//工程名称
    UINT8 project_address[32];			//工程地址
    UINT8 project_linkman[16];			//联系人
    UINT8 project_phone[16];			//联系电话
    UINT8 project_email[32];			//邮件地址


    UINT8 la;	//新协议la位置在此处
    UINT8 id[LEN_REG_ID];
    UINT8 uc[LEN_REG_UC];
    UINT8 cc[LEN_REG_CC];


    // 版本信息
    UINT8 software_ver[3];				//软件版本
    UINT8 hardware_ver[3];				//硬件版本
    UINT8 product_ver[3];				  //产品版本

    //UINT8 calibr_value_rtc;			//rtc频率校准值	
   // UINT16 calibr_value_temp;			//温度校准值,25度时对应的AD值	


	
	UINT8 ctr_type;
	UINT8 ctr_addr[LEN_REG_UC];
	UINT8 ctr_loop_start;			//控制器起始回路号用于MODBUS类读取状态
	UINT8 ctr_loop_end;			//控制器结束回路号
	UINT8 trans_equip_connect;
	UINT8 trans_addr[LEN_REG_UC];
	
	//并口相关
	
		UINT8 parallel_type;

	UINT8 reserve[65]; 				//预留

}CONFIG;
extern CONFIG System;



typedef union  										
{
	UINT8 two[2];
	UINT16 one;
} UWORD_UChAR;

typedef union
{
	UINT16  word;
	UINT8 byte[2];
}split;

typedef struct
{
    BIT_CHAR initialFlag; 					      							                    	    
	BIT_CHAR keyFlag; 					        										                    	    
	BIT_CHAR shieldFlag;
	BIT_CHAR wpFlag; 
    //UINT8 wp_invalid_time10s[3];
    
    UINT8 mode;					      //系统工作模式 0正常 1调试

	UINT8 self_check;					//自检

}RUNSTA;
extern RUNSTA RunSta;

typedef struct
{
	UINT8 Deadline;								//????10S
    UINT8 Random[8];           					//???
    UINT8 PasswordLevel;           				//????,???????????????????
}AUTH_DATA;
extern AUTH_DATA Auth;


typedef struct 
{	
    UINT8 Loop;
    UINT8 Dev;
}IC_STRUCT;
#define   LEN_RECORD_EVENT    58
typedef struct	
{
    UINT8 Sta;		              		// 0xff
    UINT8 Mode;							// 记录模式:高四位: 0:调试模式，1:正常模式
                                        // 低四位: 0:本机记录，1:控制器记录
    UINT8 Time[6];						// 记录时间: 年月日时分秒 	6
    UINT8 Id[LEN_REG_ID];				// ID 12
    UINT8 Uc[LEN_REG_UC];				// 用户码(user code) 5
    UINT8 Cc[LEN_REG_CC];				// 汉字描述(character code) 32
    IC_STRUCT Ic;                       // 2
    UINT8 Type;							// 记录类型,对应ALARM_TYPE_TAB[]
    UINT8 EventLen;
    split EventType;					// 事件类型  4
    UINT8 Event[LEN_RECORD_EVENT];      // 58  第一个字节表示具体事件来源:第一个字节存控制器LA
    UINT8 TranFlag;            			//0xff表示已经传输,0x00表示未传输过
    UINT8 ShowFlag;                     //0xff表示需要显示,0x00表示不需要显示
}RECORD_STRUCT;
extern RECORD_STRUCT Record;


#endif
