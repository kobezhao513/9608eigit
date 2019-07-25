/*
 * record.h
 *
 *  Created on: 2012-7-31
 *      Author: Administrator
 */

#ifndef RECORD_MANAGE_H_
#define RECORD_MANAGE_H_

//------------------------------------- include -------------------------------------

#include "global.h"
//------------------------------------- define --------------------------------------bdata mode_browse_record;									//记录浏览模式

/*#define	RECORD_BROWSE_TIME			0
#define	RECORD_BROWSE_TYPE			1
#define	RECORD_BROWSE_AREA          2
#define	RECORD_BROWSE_LAN           3

#define	RECORD_BROWSE_LOCAL			0
#define	RECORD_BROWSE_CONTROLLER	1
*/
#define	LOCAL_RECORD_BROWSE_TIME			0
#define	LOCAL_RECORD_BROWSE_TYPE			1
#define	CONTR_RECORD_BROWSE_TIME		    2
#define	CONTR_RECORD_BROWSE_TYPE		    3



//----------------
//利达
//联动：控制器发来的预警、火警
//操作：开机、复位、自动、手动、启动、停动、现场紧急启动、现场紧急停动、声光启动、声光停动
//动作：延时、起瓶（启动喷洒）、喷洒（反馈）---------需上报
//故障：钢瓶线、反馈线、声光线、启停线----------需上报
//----------------
//海湾
//故障、动作、启动控制、延时、启动喷洒、反馈、联动
//----------------

#define	MESSAGE_TYPE_MAX_NUM			8

// 记录类型的宏定义
#define RECORD_FIRE							0x00
#define RECORD_MONITER					    0x01
#define RECORD_START						0x02
#define RECORD_ANSWER						0x03
#define RECORD_DELAY						0x04
#define RECORD_FAULT						0x05
#define RECORD_SHIELD						0x06
#define RECORD_EARLYFIRE				    0x07
#define	RECORD_RESET						0x08      // 复位(火警等所有消息)

#define RECORD_FREE         		        0x09      // 释放(监管)
#define RECORD_STOP							0x0A      // 停止(启动)
#define RECORD_CLEAR        		        0x0B      // 清除(反馈)
#define	RECORD_CANCEL						0x0C      // 取消(延时)
#define	RECORD_ELIMINATE				    0x0D      // 排除(故障)

#define	RECORD_RELEASE					    0x0E      // 解除(屏蔽)
#define RECORD_RESUME       		        0x0F      // 恢复 

#define	RECORD_POWERONOFF					0x10      //开关机
#define	RECORD_SETUP					    0x11      //设置(含定义、注册)

#define	REOCRD_SELFCHECK				    0x12     //自检
#define	REOCRD_MODE				            0x13     // 模式(手自动)

#define RECORD_LOCALCOMMAND        		    0x14     // 本地命令
#define RECORD_NETCOMMAND                   0x15     //网络命令
#define	RECORD_OTHERS						0x16      // 其他
#define	RECORD_MISC							0x17


#define RECORD_MAX              (RECORD_OTHERS)

#define	RECORD_STA_BUS_DN 			0x01

#define RECORD_MANNU_FIRE           0x01
#define RECORD_CONTROL_FIRE        0x02

// 记录模式的宏定义
#define	RMODE_UC							0x00
#define	RMODE_CONTR_UC						0x01
//============ 记录状态定义=========================================================

#define	RECORD_STA_INIT 								0xFF
#define	RECORD_STA_SUCC									0xE0//成功


//记录传输标志
#define  RECORD_TRAN_INIT   			0XFF
#define  RECORD_TRAN_SUC 				0x00

//记录显示标志
#define  RECORD_SHOW       				0XFF
#define  RECORD_NO_SHOW 				0x00

//记录回路设备
//#define	IC_DEV_SYS  				0x00		//控制器(自身系统)
//#define	IC_DEV_LCD					0x01		//液晶
//#define	IC_POWER					0xC8FB		// 机内电源地址

#define IC_LOOP_CONTROL     		0xFF	  //控制器
#define IC_LOOP_SYS     			0xF0	  //系统设备
#define IC_DEV_CONTROL              0xFF      //控制器设备
#define	IC_DEV_SYS  				0x00	  //系统设备
#define IC_DEV_MPOWER      			0x01      //主电
#define IC_DEV_BAT          		0x02      //备电
#define IC_DEV_OUT_POW          	0x03      //输出电源
#define	IC_DEV_EXT_FLASH			0x04	  //外部flash
#define	IC_DEV_NET_LINE     		0x05	  //网络链路
#define	IC_DEV_NET_COM	    		0x06	  //网络通讯
#define	IC_DEV_SIMCARD   	        0x07	  //sim卡
#define	IC_DEV_OTHERS	    		0x08	  //系统设备其他


//==================	 record   相关参数的存储位置
//相对flash存储的有效标志0xAA
#define FLASH_OFFSET_TIME       3
#define FLASH_OFFSET_TYPE       (UINT8)(&Record.Type-&Record.Sta)
#define FLASH_OFFSET_IC         (UINT8)(&Record.Ic.Loop-&Record.Sta)
#define FLASH_OFFSET_EVENTTYPE  (UINT8)(Record.EventType.byte-&Record.Sta)
#define FLASH_OFFSET_TRANFLAG   (UINT8)(&Record.TranFlag-&Record.Sta)
#define FLASH_OFFSET_SHOWFLAG   (UINT8)(&Record.ShowFlag-&Record.Sta)

//事件产生时，存储的，在record结构体的event数组中的元素位置
#define  LEVEL1_COMM_SOURCE_LOC       0          //命令来源与自身产生
#define  FRAME_ADDR_WAY_LOC           1          //命令帧寻址方式
#define  FRAME_ADDR_UC				        2
//#define  FRAME_SER_LOC				      6          //发送帧的ser，便于控制器收到回码后比对处理

#define  ADDR0_LOC                    3          //设备寄存器地址
#define  ADDR1_LOC                    4          //设备寄存器地址
#define  HANDLE_WAY_LOC               5          //寄存器操作方式
#define  DATA_LEN_LOC                 6          //设备寄存器数据长度
#define  DATA_START_LOC               7          //设备寄存器数据



//#define  DELAY_TIME0_LOC            56         //延时时间
//#define  DELAY_TIME1_LOC             57         //延时时间

//上报上来的（2400,2402）信息
//在record结构体的event数组中的元素位置
//#define	 RECORD_EVENT_CTR			0
//#define	 RECORD_EVENT_UC0			1
//#define	 RECORD_EVENT_UC1			2
//#define	 RECORD_EVENT_UC2			3
//#define	 RECORD_EVENT_UC3			4
//#define	 RECORD_EVENT_SER			5
//#define	 RECORD_EVENT_ADDR0			6
//#define	 RECORD_EVENT_ADDR1			7
//#define	 RECORD_EVENT_DATA			8

//#define  M_SOURCE_LOC       0          //命令来源与自身产生
//#define  ADDR_WAY_LOC           1          //命令帧寻址方式
//#define  FRAME_ADDR_UC				  2
//#define  FRAME_SER_LOC				  6          //发送帧的ser，便于控制器收到回码后比对处理

//#define  ADDR0_LOC                    3          //设备寄存器地址
//#define  ADDR1_LOC                    4          //设备寄存器地址
//#define  HANDLE_WAY_LOC               5          //寄存器操作方式
//#define  DATA_LEN_LOC                 6          //设备寄存器数据长度
//#define  DATA_START_LOC               7          //设备寄存器数据


typedef struct 
{	
	//UINT16 Total;			//记录信息总数
	UINT16 Local_RecordSum; //时间浏览记录信息总数
	UINT16 StartPc;			//记录开始偏移量，即最先存进的记录
	UINT16 SavePc;			//记录存储偏移量
//	UINT16 RePc;			       //记录读取指针	
	UINT16 SheildPc;
	UINT16 P_bus;			//从记录中读取需要下发的命令
	UINT16 P_bro; 	        //广播事件指针
	UINT16 P_bus_up;		//上报读取指针
    UINT16 P_link;
    UINT16 Ptr_Box;			//黑匣子读取指针
    UINT16 ttsRecordPc;
}RECORD_INF;
extern RECORD_INF RecordInf;





//extern uchar const uc_default[];
//extern uchar const uc_unknown[];

//extern uchar const * const cc_dummy_c[];

void Record_Browse(uchar type);
UINT16 Get_Event_Type_Number(void);

void Delete_All_Record(void);
void record_browse_show_info(uint16 offset);
void record_browse_detail_control(uchar key);
void record_browse_time_control(uchar key);
UINT8 Get_EventTypeGroup(UINT16 event_type);
void show_record(RECORD_STRUCT * record,uint16 line,uchar flag);
UINT8 get_record(UINT16 offset,RECORD_STRUCT * record_buffer);

void Record_Save(RECORD_STRUCT *record);
void Init_Record(uchar type);
void Record_Power_On_Off(UINT8);

void deal_record_sta(UINT16 p_record,UINT8 type_loc, UINT8 sta);

void record_clear(uint8 type);

UINT8 Record_Get_Event(UINT16 const event_code,UINT8 * str);
void record_tts(void);
UINT8 Record_Type_Is_Sel(UINT8 type,UINT8 sel_setting);
void Record_Change_Fire_Auto_Type(UINT8 tran_type);
UINT8 Record_TTS_Format(UINT8 * str);
void Record_Search_To_TTS(void);
void Record_Updata_Suc(void);



#endif

