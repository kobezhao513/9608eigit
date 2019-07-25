/*
 * bus_dn.h
 *
 *  Created on: 2012-12-24
 *      Author: zjc
 */

#ifndef XYCNBUS_H_
#define XYCNBUS_H_
//------------------------------------- include -------------------------------------

#include "global.h"


#define 	DATA_LOC_TDID				19					// DATA????
#define 	DATA_LOC_TDLA				8
#define 	DATA_LOC_TDLA_LA			10					// ????????
//--------------------------------ADDR位置定义-----------------------------------
#define TC_LOC_HEAD 	            0				// 帧头位置定义
#define TC_LOC_LEN 		            1				// 长度
#define TC_LOC_CTR 		            2				// 控制码
#define TC_LOC_MMADR	            3				// MMADR 
	
#define    TC_LOC_DATA_LA           7				// DATA位置定义

#define		S_HEAD		   			TC_LOC_HEAD		//帧头
#define		S_LEN		   			TC_LOC_LEN		//长度
#define		S_LEN2		   			0x02			//长度1
#define		S_CTR		   			TC_LOC_CTR		//控制码
#define		S_MMADR		   			TC_LOC_MMADR	//MMADR 位置


//---------------- ADDR 位置定义---------------
#define 	ADDR_LOC_LA				3				//不考虑数据长度地址位置
#define 	ADDR_LOC_MMA_LA				4				//不考虑数据长度地址位置
#define 	ADDR_LOC_MMA_TDLA_LA			5
#define 	ADDR_LOC_MMA_CTRLA_TDLA_LA  		6

#define 	ADDR_LOC_UC				3
#define 	ADDR_LOC_MMA_UC				4
#define 	ADDR_LOC_MMA_TDUC_UC			8
#define 	ADDR_LOC_MMA_CTRUC_TDUC_UC		12

#define 	ADDR_LOC_ID				3
#define 	ADDR_LOC_MMA_ID				4
#define 	ADDR_LOC_MMA_TDID_ID			16
#define 	ADDR_LOC_MMA_CTRID_TDID_ID		28

//---------------- SER 位置定义----------------
#define		S_SER		   			0x03			//顺序号
#define 	SER_LOC_BRO				3
#define 	SER_LOC_MMADR_BRO			4

#define	SER_LOC_LA				    (ADDR_LOC_LA+1)
#define 	SER_LOC_TDLA				(ADDR_LOC_LA+1)
#define 	SER_LOC_TDLA_LA				6
#define 	SER_LOC_CTRLA_TDLA_LA			7

#define 	SER_LOC_TDUC				8
#define 	SER_LOC_TDUC_UC				14
#define 	SER_LOC_CTRUC_TDUC_UC			16

#define 	SER_LOC_TDID				15
#define 	SER_LOC_TDID_ID				28
#define 	SER_LOC_CTRID_TDID_ID			40


//---------------- DI????----------------
#define		DI_LOC_BRO							(SER_LOC_BRO+1)
#define		DI_LOC_MMADR_BRO					(SER_LOC_MMADR_BRO+1)

#define	DI_LOC_LA                   (SER_LOC_LA+1)
#define		DI_LOC_TDLA							(SER_LOC_TDLA+1)
#define		DI_LOC_TDLA_LA						(SER_LOC_TDLA_LA+1)
#define		DI_LOC_CTRLA_TDLA_LA				(SER_LOC_CTRLA_TDLA_LA+1)

#define		DI_LOC_TDUC							(SER_LOC_TDUC+1)
#define		DI_LOC_TDUC_UC						(SER_LOC_TDUC_UC+1)
#define		DI_LOC_CTRUC_TDUC_UC				(SER_LOC_CTRUC_TDUC_UC+1)

#define		DI_LOC_TDID							(SER_LOC_TDID+1)
#define		DI_LOC_TDID_ID						(SER_LOC_TDID_ID+1)
#define		DI_LOC_CTRID_TDID_ID				(SER_LOC_CTRID_TDID_ID+1)


//---------------- DATA位置定义----------------
#define 	DATA_LOC_TDID				19
#define 	DATA_LOC_TDLA				8

//---------------- 中断上报位置定义----------------
#define 	DATA_LOC_INT_TDLA_LA		10


//=============================命令帧头定义 ===========================
#define 	FRAME_HEADER			0x7F			//帧头

#define		LEN_EXPAND_BIT		   	0x80			//长度扩展位
#define		MAX_LEN_1BYTE		   	0x80			//单字节最大长度
#define		MAX_LEN_2BYTE		   	MAX_LEN_1BYTE*256//双字节最大长度


//=============================CTR0 定义 =================================
#define		CTR_ADDR_MODE_BRO		0x07			//广播
#define		CTR_ADDR_MODE_LA		0x06			//LA
#define		CTR_ADDR_MODE_ID		0x05			//ID
#define		CTR_ADDR_MODE_UC		0x04			//UC

#define		CTR_ADDR_MODE			0x07			//地址模式
#define		CTR_RE_OR_WR			0x08			//读写 0:读 1:写
#define		CTR_ANSWER_STA			0x10			//应答位 0:异常应答 1:正常应答
#define		CTR_MMADR				0x20			//多级地址扩展 0:有扩展 1:无扩展
#define		CTR_TANS_DIR			0x40			//传输方向 0:从到主 1:主到从
#define		CTR_EXPAND_CTR			0x80			//扩展控制码 0:有扩展 1:无扩展


#define		CTR_BRO_WR		   		0xE7			//广播写控制码
#define		CTR_BRO_RE		   		0xEF			//广播读控制码
#define		CTR_LA_WR		   		0xE6			//LA写控制码
#define		CTR_LA_RE		   		0xEE			//LA读控制码
#define		CTR_UC_WR		   		0xE4			//UC写控制码
#define		CTR_UC_RE		   		0xEC			//UC读控制码
#define		CTR_ID_WR		   		0xE5			//ID写控制码
#define		CTR_ID_RE		   		0xED			//ID读控制码

#define		CTR_NORMAL_ANS          0x10			//正常应答

#define		CTR_MMADR_BRO_WR		0xC7			//多级广播写控制码
#define		CTR_MMADR_BRO_RE	   	0xCF			//广播读控制码
#define		CTR_MMADR_LA_WR		   	0xC6			//LA写控制码
#define		CTR_MMADR_LA_RE		   	0xCE			//LA读控制码
#define		CTR_MMADR_UC_WR		   	0xC4			//UC写控制码
#define		CTR_MMADR_UC_RE		   	0xCC			//UC读控制码
#define		CTR_MMADR_ID_WR		   	0xC5			//ID写控制码
#define		CTR_MMADR_ID_RE		   	0xCD			//ID读控制码

#define		CTR_MMADR_UC_WR_ANS		0x92			//UC回码

#define		CTR_MMADR_LA_RE_ANS		0x9E			//LA回码,节点信息上报
//#define		CTR_LA_RE_ANS			0xBe			//LA回码，回路信息上报
#define		CTR_LA_RE_ANS			0xBE			//LA回码，回路信息上报
#define		MMADR_INT_ANS		    0x86			//中断上传MMADR


#define		CTR_MASK_DEV_ANS_ERR		0xAF			//设备回码异常应答掩码
#define		CTR_MASK_DEV_ANS_OK			0xBF			//设备回码正常应答掩码

//=============================MMADR 定义 =================================
#define		MMADR_2GRADE		   	0xE7			//MMADR两级传输
#define		MMADR_3GRADE		   	0xD5			//MMADR三级级传输

#define		MMADR_1GRADE_UP		   	0x86			//MMADR一级上行传输

#define		MMADR_HAVE_TRANS_MASK	0x07			//MMADR已传输掩码
#define		MMADR_TO_TRANS_MASK		0x70			//MMADR要传输掩码



//=============================DI 关键子定义 =================================
#define		DI_EXTENSION		   	0xC0			//DI扩展码
#define		DI_EXTEN_MODE_MASK		0xE0			//DI扩展码工作模式掩码

#define		DI_EXTEN_ADDR_MASK		0x003F			//DI扩展码地址掩码

#define		DI_1BYTE_BIT_COMM		0x40			//DI单字节位命令
#define		DI_2BYTE_BIT_COMM		0x20			//DI双字节位命令

#define		DI_1BYTE_BYTE_COMM		0x80			//DI单字节字节命令
#define		DI_2BYTE_BYTE_COMM		0x40			//DI双字节字节命令

#define		DI_2BYTE_NBYTE_COMM		0x60			//DI双字节N字节命令
#define		DI_3BYTE_NBYTE_COMM		0x90			//DI多字节N字节字节命令

#define		DI_BIT_DATA_MASK_1BYTE	0x20			//位数据掩码1字节

#define		DI_MUl_BLOCK_MASK		0xA0			//多字节块操作
#define		DI_EXTEN_DI_MASK		0x10			//扩展di





//=============================数据域 关键子定义 =================================

#define		DATA_BYTE_MASK_ALL		0xFF			//数据域字节全有效掩码


//=============================特殊命令 定义 ============================
#define		DI_EVENT_INF		   				0x38		//事件信息
#define		DI_DEBUG             				0x39		//调试操作
#define		TYPE_DEBUG_LED						0x0B		//检灯


#define     DI_LA_LOG             		0x3A
#define     DI_UC_LOG      		        0x3B
#define     DI_ID_LOG            		0x3C

#define     DI_POLL_INT            		0x3F 			//巡检及中断上报
#define	    BRO_ADDR_LA 				0xFF				//广播地址
														//一级总线透传命令状态__检测应答
															
#define	  BUS_ANS_TYPE_ERR 				  0					//总线应答错误
#define	  BUS_ANS_TYPE_OK 				  (BUS_ANS_TYPE_ERR+1)			//总线应答正确
#define	  BUS_ANS_TYPE_SPECIAL 			(BUS_ANS_TYPE_OK+1)			//总线应答特殊，通配
#define	  BUS_ANS_TYPE_ABNORMAL 		(BUS_ANS_TYPE_SPECIAL+1)		//总线应答异常
#define	  BUS_ANS_TYPE_BRO 				  (BUS_ANS_TYPE_ABNORMAL+1)		//广播

//=============================位寄存器 定义 20131210============================

#define     REG_BIT_ID_LOG_FLAG     	0x00    
#define     REG_BIT_LA_LOG_FLAG     	0x01  
#define     REG_BIT_UC_LOG_FLAG     	0x02  
#define     REG_BIT_CLEAR_RUN_STA  	 	0x03    

#define     REG_BIT_WP_CC				0x08    		//CC写保护
#define     REG_BIT_WP_LA				0x09    		//LA写保护
#define     REG_BIT_WP_UC				0x0A    		//UC写保护

#define     REG_BIT_CHECK_FLAG     		0x18    		//自检标志
//=============================字节寄存器 定义 ============================

#define		REG_BYTE_INIT				0x00			//初始化
#define		REG_BYTE_PARA_WR_EN			0x01			//参数写使能
#define     REG_BYTE_SHIELD	     		0x02   			//屏蔽
#define     REG_BYTE_KEY_IDENTITY    	0x03  			//关键标识
#define     REG_BYTE_HEART_INTERVAL 	0x04    		//心跳帧间隔
#define     REG_BYTE_SYS_RUN_STA        0x05    		//设备运行状态
#define     REG_BYTE_DUTY_ANSWER_TIME  	0x06    		//查岗应答时间
#define     REG_BYTE_TRANS_EQUIP_CON  	0x07    		//配接传输设备
#define     REG_BYTE_CTR_LOOP_START  	0x08    		//配接控制器起始回路
#define     REG_BYTE_CTR_LOOP_END  		0x09    		//配接控制器结束回路


#define     REG_DETECT_SET1             0X28           //开关量监测电平
#define     REG_DETECT_SET2             0X29           //开关量监测电平
#define     REG_DETECT_SET3             0X2A           //开关量监测电平

#define     REG_INPUT_EVENT1            0X22           //开关量输入事件类型1
#define     REG_INPUT_EVENT2            0X24           //开关量输入事件类型2
#define     REG_INPUT_EVENT3            0X26           //开关量输入事件类型3


#define     REG_RECOVER_FACTORY         0X35         //恢复出厂设置
#define     REG_DEV_CTR     			0x38    	//设备登录控制
#define     REG_BYTE_COM_RESULT			0x335//读通信成功标志

#define		REG_DEBUG	0X39//调试
#define		TYPE_DEBUG_RESET 0X55	//复位
#define		TYPE_DEBUG_UPDATE_START 0XF7	//复位
#define		TYPE_DEBUG_UPDATE 0XFF	//复位
#define		TYPE_DEBUG_UPDATE_OVER 0XFC	//复位
#define     REG_BYTE_CTR_ERASE_RECARD 	 0x48						 //擦除所有记录

#define     REG_MAINTENANCE   0X5A     //维保信息


#define     REG_BYTE_CTR_VERIFY_PERIOD  0x60    	//校验周期
#define     REG_BYTE_CTR_LANGUGE  		0x66    		//语言选择

#define     REG_BYTE_CTR_LCD_CONTRAST  	0x70    							//LCD 对比度
#define     REG_BYTE_CTR_LCD_BRIGHTLESS (REG_BYTE_CTR_LCD_CONTRAST+1)    	//LCD 背光
#define     REG_BYTE_CTR_TIME_CURSOR_UPDATA (REG_BYTE_CTR_LCD_BRIGHTLESS+1)    	//光标更新时间
#define     REG_BYTE_CTR_TIME_EXIT_FUN 	(REG_BYTE_CTR_TIME_CURSOR_UPDATA+2)    	//退出菜单时间
#define     REG_BYTE_CTR_TIME_LOCK_SCREEN 	(REG_BYTE_CTR_TIME_EXIT_FUN+2)    	//锁屏时间
#define		REG_BYTE_CTR_RS232_PBS  	0x80    							//RS232速率
#define		REG_BYTE_CTR_RS485_PBS  	(REG_BYTE_CTR_RS232_PBS+1)   


#define     REG_BYTE_CTR_POLL_CYC  			0x82    						//巡检周期
#define     REG_BYTE_CTR_TTS_SET         	0x8F    							//TTS语音设置
					//脉冲输出时间

#define     REG_BYTE_IP_ACQUIRE_MODE        0XB8//IP地址获取方式
#define     REG_BYTE_MESSAGE_MODE        0XB9//短信发送
#define		REG_BYTE_PROTOCOL_NUM			0xBE//控制器协议号
#define		REG_BYTE_CONTR_UC				0xBF//控制器UC


#define     REG_BYTE_CTR_MIMA1  	0xE0    							//一级密码
#define     REG_BYTE_CTR_MIMA2  	(REG_BYTE_CTR_MIMA1+4)   			//二级密码
#define     REG_BYTE_CTR_MIMA3  	(REG_BYTE_CTR_MIMA2+4)    			//三级密码
#define     REG_BYTE_CTR_MIMA4  	(REG_BYTE_CTR_MIMA3+4)    			//四级密码

												//REG_BYTE_CTR_ERASE_LINK
#define     ERASE_ALL_LINK     		0xFF  			//擦除所有联动公式

													//REG_BYTE_CTR_ERASE_JD_INF
#define		ERASE_UC				0x3F			//擦除UC
#define     ERASE_CC      			0x7F  			//擦除CC
#define     CRT_TYPE     			0xBE  			//配接控制器协议号
#define     CTR_ADDR     			0xBF  			//配接控制器UC
#define     ERASE_UC_CC_PARA     		0xFF  			//擦除UC CC 参数 


#define     REG_RECORD_NUM       		0x110  					//记录数量

#define     REG_BYTE_CTR_OPERATE_AUTH  	0x120    					//操作权限

#define     REG_BYTE_CTR_PRODUCT_PROCESS1  	0x130    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS2  	0x131    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS3  	0x132    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS4  	0x133    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS5  	0x134    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS6  	0x135    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS7  	0x136    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS8  	0x137    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS9  	0x138    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS10  0x139    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS11  0x13a    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS12  0x13b    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS13  0x13c    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS14  0x13d    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS15  0x13e    							//生产工序
#define     REG_BYTE_CTR_PRODUCT_PROCESS16  0x13f    							//生产工序

#define     REG_FIRE_ID       			0x162			//消防身份信息
#define 	REG_BYTE_RANDON_DATA       	0X169
#define 	REG_BYTE_AUTH_DATA       	0X0171
#define     REG_RTC_SS       			0x179  
#define     REG_BYTE_ID       			0x180   		//ID
#define     REG_BYTE_UC       			0x18c   
#define     REG_BYTE_CC       			0x191   
#define     REG_BYTE_LA       				0x1B1 
#define 	REG_BYTE_SOFT_VER_REG       	0X01B2			
#define   	REG_BYTE_HARD_VER_REG       	0X01B5
#define   	REG_BYTE_PROTOCOL_VER_REG    	0X01B8	
#define     REG_BYTE_CTR_INFO        		0x01C0


#define     REG_BYTE_PROJECT_NAME		0x210   										//工程信息
#define     LEN_PROJECT_NAME			32 
#define     REG_BYTE_PROJECT_ADDR		(REG_BYTE_PROJECT_NAME+LEN_PROJECT_NAME)		//工程地址
#define     LEN_PROJECT_ADDR			32
#define     REG_BYTE_PROJECT_CONTACT	(REG_BYTE_PROJECT_ADDR+LEN_PROJECT_ADDR)		//联络人
#define     LEN_PROJECT_CONTACT			16 
#define     REG_BYTE_PROJECT_TEL		(REG_BYTE_PROJECT_CONTACT+LEN_PROJECT_CONTACT)	//联络电话
#define     LEN_PROJECT_TEL				16 
#define     REG_BYTE_PROJECT_EMAIL		(REG_BYTE_PROJECT_TEL+LEN_PROJECT_TEL)			//邮件
#define     LEN_PROJECT_EMAIL			32  
//===================================传输装置系统配置=======================================
#define     REG_BYTE_COM_MODE			0x83

#define     REG_BYTE_IP_ADDR_SERVER		0xA0
#define     REG_BYTE_PORT_SERVER		0xA4

#define     REG_BYTE_IP_ADDR_LOCAL		0xA6
#define     REG_BYTE_NETMASK_LOCAL		0xBA
#define     REG_BYTE_GATEWAY_LOCAL		0xAE
#define     REG_BYTE_PORT_LOCAL			0x29D
#define     REG_BYTE_MAC_LOCAL			0xB2

#define     REG_EMG_TEL_NUM1            0X290
#define     REG_EMG_TEL_NUM2            0X29B
#define     REG_EMG_TEL_NUM3            0X2A6

#define     REG_BYTE_ALARM_DELAY_AUTO	0x2B1
#define     REG_BYTE_ALARM_DELAY_MANUAL	0x2B2
#define     REG_BYTE_APN_MODE       	0x2B3
#define     REG_BYTE_RATE_MODE       	0x087
#define     REG_BYTE_SERVER_IP       	0x2B5
#define     REG_BYTE_SERVER_PORT       	0x2B9


#define     REG_RECORD       			0x400000		//事件记录
#define    	RECORD_LEN 					ALEN_FLASH_RECORD				//信息长度
#define    	RECORD_NUM 					MAXN_FLASH_RECORD			//记录数量
#define  	RELADR_RECORD_DATA 			0x2f			//记录数据

#define    	REG_END       				(REG_RECORD+(RECORD_LEN*RECORD_NUM))	//寄存器结束

#define  	FLAG_DATA_BLOCK_VAILD 		0xaa			//数据块有效标志
#define  	FLAG_DATA_BLOCK_INVAILD 	0x55			//数据块无效标志



//=============================操作类型 定义 =================================
#define		OPSTYPE_SPECIAL_COMM		0x00			//特殊命令
#define		OPSTYPE_BIT			0x01			//位操作
#define		OPSTYPE_BYTE			0x02			//字节操作
#define		OPSTYPE_MITI_BYTE		0x03			//多字节操作

//==================帧寄存器地址范围 定义 ============================

#define		BIT_ADDR_RANGE_1BYTE		32				//单字节位地址范围
#define		BIT_ADDR_RANGE_2BYTE		1024			//双字节位地址范围

#define		BYTE_ADDR_RANGE_1BYTE		64				//单字节字节地址范围
#define		BYTE_ADDR_RANGE_2BYTE		2048			//双字节字节地址范围

#define		NBYTE_ADDR_RANGE_2BYTE		2048			//双节字节地址范围
#define		NBYTE_ADDR_RANGE_3BYTE		65535			//三字节字节地址范围
#define		NBYTE_ADDR_RANGE_4BYTE		8388608			//四字节字节地址范围



//=============================控制寄存器 定义 ============================
#define		REG_STA_OFF		   		0x00			//寄存器状态-停止
#define		REG_STA_ON		   		0xC0			//寄存器状态-启动
#define		REG_STA_ON_MK	   		0xC1			//寄存器状态-启动
#define		REG_STA_PULSE		   	0x80			//寄存器状态-闪烁

#define		REG_STA_SHEILD		   				(REG_STA_ON+1)		//寄存器状态-屏蔽
#define		REG_STA_RESUM				    	(REG_STA_SHEILD+1)	//寄存器状态-非屏蔽

#define		REG_SG_STA_ON		   	REG_STA_ON//0xC0+1

//===========================异常应答类型字=================================
#define	NAS_ERR_LIMIT_CONDITION				0xFE			//异常应答-条件受限
#define	NAS_ERR_NOT_SUPORT				0xFD			//异常应答-不支持
#define	NAS_ERR_TIMEOUT					0xFC			//异常应答-从站超时
#define	NAS_ERR_CRC_ERR					0xFB			//异常应答-从站CRC错
#define COMMUNICATION_FAULT         0xc5			   
#define	ANSWER_NO_SUPPORT       	0x01
#define	ANSWER_CONDITION_LIMIT  	0x03
#define		ANSWER_COM_ATTACK        	0x1F

//===========================信息上传类型码命令类型字=======================



//以下为新事件类型表20150409更新
//===========================事件类型=======================
//人工控制实时事件优先级0

//---------------------------按键-----------------------------------//
//              按键被按下时，按键载体产生此事件。                  //
// -----------------------------------------------------------------//
#define     INT_TYPE_KEY_ACTION 				0x0000				//按键动作
//---------------------------请求-----------------------------------//
//           人员操作设备进行某项操作请求时，设备产生此事件。         //
//------------------------------------------------------------------//
#define    	INT_TYPE_MANUAL_ALARM     	 		0x0100    			//手动报警
//---------------------------设置------------------------------------//
//			  人员操作设备进行某项设置时，设备产生此事件。           //
//-------------------------------------------------------------------//
#define    	INT_TYPE_SET_TIME		         	0x0200    			//设置时间
#define    	INT_TYPE_SET_MANUAL_ENABLE   		0x0210 				//设置手动允许
#define    	INT_TYPE_SET_MANUAL_DISEN   		0x0211 				//设置手动禁止
#define    	INT_TYPE_SET_AUTO_ENABLE    		0x0220				//设置自动允许
#define    	INT_TYPE_SET_AUTO_DISABLE	     	0x0221				//设置自动禁止
#define    	INT_TYPE_SET_HALF_AUTO_ENABLE	    0x0230				//设置部分自动允许
#define    	INT_TYPE_SET_HALT_AUTO_DISEN	    0x0231				//设置部分自动禁止
#define    	INT_TYPE_SET_SPRAY_ENABLE    		0x0240				//设置喷洒允许
#define    	INT_TYPE_SET_SPRAY_DISEN    		0x0241				//设置喷洒禁止
#define     INT_TYPE_SET_PRINT_FIRE				0X0250				//打印火警
#define		INT_TYPE_SET_PRINT_FIRE_AND_FEEDBACK 0x0251				//打印火警和反馈
#define		INT_TYPE_SET_PINTT_ALL_EVENT		0x0252				//打印所有信息
#define		INT_TYPE_SET_PRINT_DISEN			0x0253				//禁止打印
#define    	INT_TYPE_SET_PRINTER        		0x0254				//打印浓度设置
#define		INT_TYPE_SET_LCD_BACKGROUND_LIGHT	0x0255				//液晶背光设置
#define    	INT_TYPE_SET_PREALARM_ENABLE    	0x0260				//预警功能允许
#define    	INT_TYPE_SET_PREALARM_DISEN    		0x0261				//预警功能禁止
#define    	INT_TYPE_SET_PREALARM_RELATION    	0x0262				//预警关联设置
#define    	INT_TYPE_SET_STARTUP_DIRECT_VIA_FIRE 0x0270				//火警直接启动
#define    	INT_TYPE_SET_STARTUP_VIA_LINKAGE    0x0271				//联动公式启动
#define    	INT_TYPE_SET_NETWORK_ENABLE    		0x0280				//网络通信开启
#define    	INT_TYPE_SET_NETWORK_DISEN    		0x0281				//网络通信关闭
#define    	INT_TYPE_SET_ADD_LINKAGE            0x0290				//新建联动公式
#define    	INT_TYPE_SET_EDIT_LINKAGE    		0x0291				//修改联动公式
#define    	INT_TYPE_SET_DELETE_LINKAGE         0x0292				//删除联动公式

////控制器命令
//---------------------------记录------------------------------------//
//操作设备进行某项操作请求时，控制器产生此事件，用来记录设备产生        //      
//了对应的操作请求                                                   //
//-------------------------------------------------------------------//
#define    	INT_TYPE_REQ_START		         	0x0300    			//启动请求
#define    	INT_TYPE_REQ_STOP     				0x0301  			//停动请求
#define    	INT_TYPE_REQ_SHIELD  				0x0310 				//屏蔽请求
#define    	INT_TYPE_REQ_RELEASE     			0x0311				//解除屏蔽请求
#define    	INT_TYPE_REQ_RESET					0x0320				//复位请求
#define    	INT_TYPE_REQ_SILENCE				0x0340				//消音请求
#define    	INT_TYPE_PARA_MODIFY 				0x0330				//参数修改请求
#define    	INT_TYPE_HELP_INF           		0x0350     			//求助信息 
#define    	INT_TYPE_LAN_SET_TIME           	0x0360     			//网络校时
#define    	INT_TYPE_LAN_READ_TIME        		0x0361     			//网络读取时间


////命令及应答事件（优先级1）：
////基本通信命令
#define   INT_TYPE_IDLOG_DATA           		0x2000     	//ID登录命令数据
#define   INT_TYPE_LALOG_DATA	             	0x2001		//LA登录命令数据
#define   INT_TYPE_UCLOG_DATA           		0x2002	    //UC登录命令数据
#define   INT_TYPE_TRANSPARENT_TRANS    		0x2100     	//透传信息
#define   INT_TYPE_LINK_FORMULA_OUT	    		0x2200     	//联动公式成立联动输出
#define   INT_TYPE_LINK_OUT    	       			0x2201   	//关联的联动公式输出
#define   INT_TYPE_UC_FIND_LC           		0x2300		//二次码查询一次码
#define   INT_TYPE_MULTI_ADDRCOMM_RES			0x2400     	//多级地址命令应答
#define   INT_TYPE_INNER_UP_MSG				    0x2401		//内部上报信息:上报信息:气体灭火、广播、电话
#define   INT_TYPE_WILDCOMM_EXECUTE_RIGHT     	0x2402		//通配命令正确执行
#define   INT_TYPE_WAN_UP_MSG				    0x2403		//广域网上报信息

//#define   INT_TYPE_DEVICE_SYNCH               	0x2500		//节点信息同步


#define   INT_TYPE_EVENT_SYNCH               	0x2500		//事件信息同步
#define   INT_TYPE_RESET_SYNCH					0x2501		//事件信息复位
#define   INT_TYPE_MONITOR_PDC_SEND_DATA		0x2600      //监控PDC总线发码数据
#define   INT_TYPE_MONITOR_PDC_RECE_DATA     	0x2601 		//监控PDC总线收码数据
#define   INT_TYPE_MONITOR_IMTMPI_SEND_DATA   	0x2602  	//监控IMT/MPI总线发码数据
#define   INT_TYPE_MONITOR_IMTMPI_RECE_DATA   	0x2603  	//监控IMT/MPI总线收码数
#define	  INT_TYPE_MONITOR_NODE_ID				0x2604		//监控节点ID
#define	  INT_TYPE_MONITOR_BROCAST_SEND_DATA	0x2610		//监控广播总线发码
#define	  INT_TYPE_MONITOR_BROCAST_RECE_DATA	0x2611		//监控广播总线收码




//---------------------------报警------------------------------------//
//		具有自动报警功能的设备检测到对应警报时,设备产生此事件	   	     //
//-------------------------------------------------------------------//
#define   INT_TYPE_FIRE_ALARM                	0x4000      	//火焰报警
#define   INT_TYPE_TEMPERATURE_ALARM         	0x4001 			//温度报警
#define   INT_TYPE_SMOKE_ALARM               	0x4002  		//烟雾报警
#define   INT_TYPE_CURRENT_LEAKAGE_ALARM     	0x4003        	//漏电报警
#define   INT_TYPE_GAS_LEAKAGE_ALARM         	0x4004          //燃气泄漏
#define   INT_TYPE_THEFT_ALARM               	0x4005  	    //盗警
#define   INT_TYPE_TIMEOUT_ALARM             	0x4006    		//超时报警
#define   INT_TYPE_ALARM                        0x400F          //火警报警 
//---------------------------预警------------------------------------//
//具有自动报警功能的设备，若其报警等级不止一级，检测到低级别警报时，    //
//设备产生此事件                                          	   	     //
//-------------------------------------------------------------------//
#define   INT_TYPE_PRE_FIRE_ALARM				0x4100      	//火警预警
#define   INT_TYPE_PRE_TEMPERATURE_ALARM        0x4101   		//温警预警
#define   INT_TYPE_PRE_SMOKE_ALARM              0x4102       	//烟警预警
#define   INT_TYPE_PRE_THEFT_ALARM              0x4105       	//盗警预警
#define   INT_TYPE_PRE_ALARM                    0x410F          //火警预警

//---------------------------监控-----------------------------------//
//2016-7-19增加监控事件xt
//-------------------------------------------------------------------//
#define   INT_TYPE_MONITOR_FIRE_ALARM           0x4200			//监控火焰报警
#define   INT_TYPE_MONITOR_TEMPERATURE_ALARM	0x4201			//监控温度报警
#define   INT_TYPE_MONITOR_SMOKE_ALARM			0x4202			//监控烟雾报警
#define   INT_TYPE_MONITOR_CURRENT_LEAKAGE_ALARM 0x4203			//监控漏电报警
#define   INT_TYPE_MONITOR_GAS_LEAKAGE_ALARM	0x4204			//监控燃气泄漏报警
#define   INT_TYPE_MONITOR_THEFT_ALARM			0x4205			//监控盗警报警
#define   INT_TYPE_MONITOR_TIMEOUT_ALARM		0x4206			//监控超时报警
#define   INT_TYPE_MONITOR_ALARM		        0x420F			//监管报警
#define   INT_TYPE_MONITOR_HIGH_VOLTAGE			0x4210			//监控过压报警
#define   INT_TYPE_MONITOR_LOW_VOLTAGE			0x4211			//监控欠压报警
#define   INT_TYPE_MONITOR_HIGH_CURRENT			0x4212			//监控过流报警
#define   INT_TYPE_MONITOR_LOSE_PHASE			0x4213			//监控缺相报警
#define   INT_TYPE_MONITOR_FAULT_PHASE			0x4214			//监控错相报警
#define   INT_TYPE_MONITOR_POWER_INTERRUPT		0x4215			//监控供电中断报警
#define   INT_TYPE_MONITOR_FIRE_OUTPUT          0x422E          //监控火警输出
#define   INT_TYPE_MONITOR_FAULT_OUTPUT         0x422F          //监控故障输出

	

//---------------------------成功------------------------------------//
//		       此事件用来表征设备状态改变成功		                     //
//-------------------------------------------------------------------//
#define   INT_TYPE_START_SUCESS                 0x6000      	//设备启动
#define   INT_TYPE_STOP_SUCESS                  0x6001      	//设备停动
#define   INT_TYPE_SHILED_SUCESS 	    	    0X6002			//设备屏蔽
#define   INT_TYPE_RELEASE_SUCESS            	0x6003			//设备解除屏蔽

#define   INT_TYPE_PHONE_CALL					0x6004			//电话呼叫
#define   INT_TYPE_PHONE_CONNECT				0x6005			//电话接通
#define   INT_TYPE_PHONE_HUNGUP					0x6006			//电话挂断

#define   INT_TYPE_RESET_SUCESS                 0x6007	    	//设备复位
#define   INT_TYPE_SET_RARA_SUCESS              0x6008	    	//参数修改成功
#define   INT_TYPE_DEVICE_SILENCE               0x6009      	//设备消音


#define   INT_TYPE_STARTED                      0x600A      	//设备已启动
#define   INT_TYPE_STOPPED                      0x600B      	//设备已停动
#define   INT_TYPE_SHILEDED                     0x600C      	//设备已屏蔽
#define   INT_TYPE_RELEASED                     0x600D      	//设备已解除屏蔽

#define INT_TYPE_UPDATA_SUCC                  0x600E		//升级成功



//---------------------------失败------------------------------------//
//		       此事件用来表征设备状态改变失败		                     //
//-------------------------------------------------------------------//
#define   INT_TYPE_START_FAIL                   0x6100      	//设备启动失败
#define   INT_TYPE_STOP_FAIL                    0X6101        	//设备停动失败
#define   INT_TYPE_SHILED_FAIL 			        0X6102 			//现场设备屏蔽失败
#define   INT_TYPE_RESUME_FAIL                  0X6103 			//现场设备解除屏蔽失败
#define   INT_TYPE_PHONE_CALL_FAIL            	0x6104			//电话呼叫失败
#define   INT_TYPE_PHONE_CONNECT_FAIL          	0x6105			//电话接通失败
#define   INT_TYPE_PHONE_HUNGUP_FAIL            0x6106			//电话挂断失败
#define   INT_TYPE_RESET_FAIL                   0X6107 			//复位失败
#define   INT_TYPE_SET_RARA_FAIL	            0X6108 			//参数修改失败
#define   INT_TYPE_SILENCE_FAIL          		0x6109	    	//设备消音失败
#define INT_TYPE_UPDATA_FAIL                  0x610A		//升级失败

//---------------------------允许------------------------------------//
//		       此事件用来表征设备状态改为允许		                 //
//-------------------------------------------------------------------//
#define   INT_TYPE_MANUAL_CONTROL_ALLOW         0x6200      	//手动控制允许
#define   INT_TYPE_AUTO_CONTROL_ALLOW           0x6201     		//自动控制允许
#define   INT_TYPE_SPRAY_ALLOW			        0x6202      	//喷洒允许
#define   INT_TYPE_PARTIAL_AUTO_CONTROL_ALLOW      0x6203     		//部分自动控制允许
//---------------------------禁止------------------------------------//
//		       此事件用来表征设备状态改为禁止		                 //
//-------------------------------------------------------------------//
#define   INT_TYPE_MANUAL_CONTROL_FORBID        0x6300		     //手动控制禁止
#define   INT_TYPE_AUTO_CONTROL_FORBID      	0x6301			 //自动控制禁止
#define   INT_TYPE_SPRAY_FORBID       		    0x6302			 //喷洒禁止
//---------------------------建立------------------------------------//
//		       此事件用来表征设备某种连接建立		                 //
//-------------------------------------------------------------------//
#define   INT_TYPE_CHANNEL_SETUP			    0x6400	    	//通道建立
#define   INT_TYPE_STS_PHONE_SETUP				0x6401	    	//电话建立	
#define   INT_TYPE_LINK_SETUP					0x6402	    	//链路建立	

//---------------------------手自动广播------------------------------//
//		       	                     //
//-------------------------------------------------------------------//
#define   INT_TYPE_EMERGENCY_BRODCAST_AUTO		0x6500			//自动应急广播
#define   INT_TYPE_EMERGENCY_BRODCAST_MANUAL	0x6501			//手动应急广播
#define   INT_TYPE_EMERGENCYT_OVER_AUTO			0x6502	    	//自动应急结束
#define   INT_TYPE_EMERGENCYT_OVER_MANUAL		0x6503	    	//手动广播结束

#define   INT_TYPE_ON_SCENCE_COMMANDER			0x6510	    	//指挥开始
#define   INT_TYPE_ON_SCENCE_COMMANDER_OVER		0x6511	    	//指挥结束
#define   INT_TYPE_EXSOUND_BEGIN				0x6520	    	//外音播放
#define   INT_TYPE_EXSOUND_OVER			        0x6521	    	//外音结束
#define   INT_TYPE_EXSOUND_PAUSE			    0x6522	    	//外音暂停
#define   INT_TYPE_EXSOUND_RECOVER				0x6523	    	//外音恢复
#define   INT_TYPE_SDCARD_INSERT				0x6530	    	//SD卡插入
#define   INT_TYPE_SDCARD_EXTRACT				0x6531	    	//SD卡拔出
#define   INT_TYPE_UDISK_INSERT				    0x6540	    	//U盘插入
#define   INT_TYPE_UDISK_EXTRACT				0x6541	    	//U盘拔出
#define   INT_TYPE_SOUNDRECORD_BEGIN			0x6550	    	//录音开始
#define   INT_TYPE_SOUNDRECORD_OVER				0x6551	    	//录音结束
#define   INT_TYPE_SOUNDPLAY_BEGIN				0x6560	    	//放音播放
#define   INT_TYPE_SOUNDPLAY_OVER			    0x6561	    	//放音结束
#define   INT_TYPE_SOUNDPLAY_PAUSE			    0x6562	    	//放音暂停
#define   INT_TYPE_SOUNDPLAY_RECOVER		    0x6563	    	//放音恢复
#define   INT_TYPE_AUDIO_LENGTH					0x6564			//放音时间长度

//---------------------------断开------------------------------------//
//		       此事件用来表征设备某种连接断开		                 //
//-------------------------------------------------------------------//
#define   INT_TYPE_CHANNEL_BREAK                0x6600	   	 	//通道断开
#define   INT_TYPE_STS_PHONE_HUNGUP			    0x6601	  	    //电话挂断
#define   INT_TYPE_LINK_BREAK                   0x6602	   	 	//链路断开
//		       此事件用来表征设备处于维护状态		                 //
//-------------------------------------------------------------------//
#define   INT_TYPE_BACKUP_POWER_MAINTENANCE     0x6610         	//备电维护
//---------------------------工作------------------------------------//
//		       此事件用来表征设备供电状态		                     //
//-------------------------------------------------------------------//
#define   INT_TYPE_MAINPOW_ON				   0x6700	    	//主电工作
#define   INT_TYPE_BACKPOW_ON 				   0x6701     		//备电工作
//---------------------------修改------------------------------------//
//		       此事件用来表征设备特性的修改		                     //
//-------------------------------------------------------------------//
#define   INT_TYPE_SOFTWARE_UPGRADE			   0x6800	    	//程序升级
#define   INT_TYPE_DEVICE_ADDRESS_MODIFY	   0x6801			//设备地址修改
#define   INT_TYPE_SYSTEM_MODE_MODIFY		   0x6802	    	//系统模式修改

//---------------------------其他------------------------------------//
//		       设备一些其他状态的表征   		                     //
//-------------------------------------------------------------------//
#define   INT_TYPE_POWER_ON                    0x6900      	//设备开机
#define   INT_TYPE_POWER_OFF                   0x6901      	//设备关机
#define   INT_TYPE_SELFCHECK_CANCEL            0x6902		//设备自检取消
#define   INT_TYPE_SELFCHECK_OVER              0x6903		//设备自检完成
#define   INT_TYPE_DEVICE_SELFCHECK            0x6904		//设备自检

#define   INT_TYPE_EARLY_ALARM_RESET		   0x6905	    //预警恢复
#define   INT_TYPE_ALARM_FROM_EARLYALM	       0x6906		//预警变火警
#define   INT_TYPE_CACHE_OUT_RESET			   0x6907	    //通道恢复空闲

#define   INT_TYPE_NET_ON_LINE                  0x6A00      //值班应答在岗
#define   INT_TYPE_NET_OFF_LINE                 0x6A01      //值班应答查岗
#define   INT_TYPE_NET_ON_LINE_AGAIN            0x6A02      //值班人员复岗

#define	  INT_TYPE_MONITOR_ALARM_FREE			0x6B00			//监管报警释放

//---------------------------启动------------------------------------//
//设备被启动时，产生此事件，反馈设备启动状态                         //                                                       
//-----------------------------------------------------------------//
#define   INT_TYPE_DEVICE_START_FEED_SUC        0x8000  	//启动反馈成功 INT_TYPE_DEVICE_START_FEED_SUC
#define   INT_TYPE_DEVICE_START_FEED_FAIL       0x8001		//启动反馈失败 INT_TYPE_DEVICE_START_FEED_FAIL
//---------------------------停动------------------------------------//
//设备被停动时，产生此事件，反馈设备停动状态                         //                                                       
//-------------------------------------------------------------------//
#define   INT_TYPE_DEVICE_STOP_FEED_SUC         0x8100  	//停止反馈成功 INT_TYPE_DEVICE_STOP_FEED_SUC
#define   INT_TYPE_DEVICE_STOP_FEED_FAIL	    0x8101		//停止反馈失败 INT_TYPE_DEVICE_STOP_FEED_FAIL
//---------------------------同步------------------------------------//
//设备被同步时，产生此事件，反馈设备同步状态                         //                                                       
//-------------------------------------------------------------------//
#define   INT_TYPE_SYSC_DATA_SUC                 0x8200  		//信息同步成功
#define   INT_TYPE_SYSC_DATA_FAIL                0x8201  		//信息同步失败
#define   INT_TYPE_COMPARE_OVER                  0x8210        //设备核对完成
#define   INT_TYPE_COMPARE_FAIL                  0x8211        //设备核对失败
//---------------------------延时------------------------------------//
//具有延时功能的设备，产生此事件，反馈设备延时的状态                 //                                                       
//-------------------------------------------------------------------*/
#define   INT_TYPE_MANUAL_DELAY_START   		0x8300      //延时开始
#define   INT_TYPE_MANUAL_DELAY_CANCEL      	0X8301		//延时取消
#define   INT_TYPE_DELAY_SUC     				0X8302		//延时成功
//---------------------------输入------------------------------------//
//具有输入指示功能的设备，产生此事件，反馈设备输入的状态             //                                                       
//-------------------------------------------------------------------//
#define   INT_TYPE_SWITCH_INPUT_ON            	0x8400       //开输入开关量
#define   INT_TYPE_SWITCH_INPUT_OFF           	0x8401       //关输入开关量
#define   INT_TYPE_ANALOG_INPUT_VOLTAGE       	0x8410       //电压模拟量输入
#define   INT_TYPE_ANALOG_INPUT_CURRENT		  	0x8411		 //电流模拟量输入
//---------------------------校准------------------------------------//
//具有参数自校准功能的设备，产生此事件，反馈设备校准的状态           //                                                       
//-------------------------------------------------------------------//
#define   INT_TYPE_PARA1_CHECK_SUC				0x9000      //参数1校准成功
#define   INT_TYPE_PARA1_CHECK_FAIL      		0x9001 		//参数1校准失败
#define   INT_TYPE_PARA2_CHECK_SUC    			0x9010 		//参数2校准成功
#define   INT_TYPE_PARA2_CHECK_FAIL     		0x9011 		//参数2校准失败
#define   INT_TYPE_PARA3_CHECK_SUC     			0x9020 		//参数3校准成功
#define   INT_TYPE_PARA3_CHECK_FAIL    			0x9021 		//参数3校准失败
#define   INT_TYPE_PARA4_CHECK_SUC    			0x9030 		//参数4校准成功
#define   INT_TYPE_PARA4_CHECK_FAIL     		0x9031 		//参数4校准失败
#define   INT_TYPE_PARA5_CHECK_SUC     			0x9040 		//参数5校准成功
#define   INT_TYPE_PARA5_CHECK_FAIL     		0x9041 		//参数5校准失败
#define   INT_TYPE_ACTION_FEEDBACK     		    0x90F0 		//动作反馈

#define   INT_TYPE_AUTO_DELAY_START     		0x9110      //联动引起设备动作延时开始
#define   INT_TYPE_AUTO_DELAY_CANCEL   			0x9111      //联动引起设备动作延时中止


//传感器实时信息
#define   INT_TYPE_FIRE_STATE           		0xA000      //火焰强度
#define   INT_TYPE_TEMPE_STATE         			0xA001      //温度
#define   INT_TYPE_SMOKE_STATE          		0xA002      //烟浓度
#define   INT_TYPE_GAS_STATE            		0xA003      //燃气浓度
#define   INT_TYPE_THEFT_STATE          		0xA004      //盗警
#define   INT_TYPE_HUMIDITY_STATE       		0xA005      //湿度
#define   INT_TYPE_PRESSURE_STATE       		0xA006      //压力
#define   INT_TYPE_CURRENT_STATE        		0xA007 		//电流
#define   INT_TYPE_VOLTAGE_STATE        		0xA008 		//电压
#define   INT_TYPE_CURRENT_A_STATE        		0xA009 		//A相电流
#define   INT_TYPE_CURRENT_B_STATE        		0xA00A 		//B相电流
#define   INT_TYPE_CURRENT_C_STATE        		0xA00B 		//C相电流
#define   INT_TYPE_VOLTAGE_A_STATE        		0xA00C 		//A相电压
#define   INT_TYPE_VOLTAGE_B_STATE        		0xA00D 		//B相电压
#define   INT_TYPE_VOLTAGE_C_STATE        		0xA00E 		//C相电压
#define   INT_TYPE_VOLTAGE_CURRENT_STATE        0xA00F 		//电压电流
#define   INT_TYPE_COMMUNICATION_SUCCESSRATE    0xA100 		//通讯成功率
#define   INT_TYPE_AMPLIFIER_INFORM        		0xA101 		//功放信息
//故障类事件（优先级6）
//设备自身硬件故障类
//---------------------------操作------------------------------------//
//		       此事件用来表征设备操作相关的故障		                 //
//-------------------------------------------------------------------//
#define   INT_TYPE_EEPROM_RW_FAU           		0xC000	    //EEPROM读写故障
#define   INT_TYPE_RAM_RW_FAU              		0xC001      //RAM读写故障
#define   INT_TYPE_FLASH_RW_FAU            		0xC002      //FLASH读写故障
#define   INT_TYPE_UDISK_R_FAU            		0xC003      //U盘读错误
#define   INT_TYPE_UDISK_W_FAU            		0xC004      //U盘写错误
#define   INT_TYPE_EEPROM_ERASURE_FAU   	 	0xC010		//EEPROM擦除故障
#define   INT_TYPE_FLASH_ERASURE_FAU       		0xC011    	//FLASH擦除故障
#define   INT_TYPE_GSM_REG_FAIL      		 	0xC020		//GSM NET注册失败
#define   INT_TYPE_GPRS_REG_FAIL           		0xC021    	//GPRS NET注册失败
#define   INT_TYPE_SIM_LOST	             		0xC030      //SIM卡缺失
#define   INT_TYPE_GPRS_NACK               		0xC031	    //GPRS模块通信无响应
#define   INT_TYPE_PRINTER_NO_PAPER      	 	0xC040      //打印机缺纸
#define   INT_TYPE_PRINTER_PAPER_JAM       		0xC041    	//打印机卡纸
#define   INT_TYPE_PRINTER_OVER_TEMPE      		0xC042      //打印机超温
#define   INT_TYPE_KEY_FAU              	 	0xC050    	//按键故障
#define   INT_TYPE_HOST_KEY_FAU            		0xC051    	//主机键盘故障
#define   INT_TYPE_SPEAKER_FAU          	 	0xC060      //扬声器故障
#define   INT_TYPE_AMPLIFIER_FAU        	 	0xC061      //功放故障
#define   INT_TYPE_MIC_FAU        		 		0xC062		//MIC故障
#define   INT_TYPE_BLACKBOX_FAU	         		0xC070      //数据存储单元故障
#define   IND_TYPE_RTC_CLOCK_FAU                0xC0B0      //RTC时钟故障
#define   IND_TYPE_ETHERNET_CONTR_FAU           0xC0C0      //以太网控制器故障

#define   INT_TYPE_SMOKE_SENSOR_FAU     		0xC100      //烟雾传感器故障
#define   INT_TYPE_TEMPE_SENSOR_FAU     		0xC101      //温度传感器故障
#define   INT_TYPE_UV_SENSOR_FAU        		0xC102      //紫外传感器故障
#define   INT_TYPE_INFRARED_SENSOR_FAU  		0xC103      //红外传感器故障
#define   INT_TYPE_PYROELE_SENSOR_FAU   		0xC104      //热释电传感器故障
#define   INT_TYPE_HUMIDITY_SENSOR_FAU  		0xC105      //湿度传感器故障
#define   INT_TYPE_CURRENT_SENSOR_FAU   		0xC106      //电流传感器故障
#define   INT_TYPE_FLAMEGAS_SENSOR_FAU  		0xC107      //可燃气体传感器故障
#define   INT_TYPE_SWITCH_SENSOR_FAU			0xC108		//开关传感器故障
#define   INT_TYPE_DOOR_FAU						0xC109		//门故障
//--------------------------- 维保-----------------------------------//
//	需要进行维保的设备，产生此事件，告知其需要进行维保				     //
//-------------------------------------------------------------------//
#define   INT_TYPE_REQUEST_MAINTENANCE     		0xC200	   	//请求维保
#define   INT_TYPE_PASSWORD_AGING    		0xC201	   	//请求维保


////--------------------------- 供电------------------------------------//
////                       供电相关的故障      	          		        //
////--------------------------------------------------------------------//
#define   INT_TYPE_INPUT_POWER_SHORT_CIRCUIT    0xC300    	//输入电源短路
#define   INT_TYPE_INPUT_POWER_OPEN_CIRCUIT     0xC301    	//输入电源断路
#define   INT_TYPE_INPUT_POWER_VOLTAGE_H        0xC302    	//输入电源过压
#define   INT_TYPE_INPUT_POWER_VOLTAGE_L        0xC303    	//输入电源欠压
#define   INT_TYPE_INPUT_POWER_CURRENT_H        0xC304    	//输入电源过流
#define   INT_TYPE_INPUT_POWER_FAULT            0xC305    	//输入电源故障
#define   INT_TYPE_OUTPUT_POWER_SHORT_CIRCUIT   0xC310    	//输出电源短路
#define   INT_TYPE_OUTPUT_POWER_OPEN_CIRCUIT    0xC311    	//输出电源断路
#define   INT_TYPE_OUTPUT_POWER_VOLTAGE_H       0xC312    	//输出电源过压
#define   INT_TYPE_OUTPUT_POWER_VOLTAGE_L       0xC313    	//输出电源欠压
#define   INT_TYPE_OUTPUT_POWER_CURRENT_H       0xC314    	//输出电源过流
#define   INT_TYPE_MODULE_POWER_VOLTAGE_H       0xC315    	//电源模块输出过压
#define   INT_TYPE_MODULE_POWER_VOLTAGE_L       0xC316    	//电源模块输出欠压
#define   INT_TYPE_BACKUP_POWER_SHORT_CIRCUIT   0xC320    	//备电短路
#define   INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT    0xC321    	//备电断路
#define   INT_TYPE_BACKUP_POWER_VOLTAGE_H       0xC322    	//备电过压
#define   INT_TYPE_BACKUP_POWER_VOLTAGE_L       0xC323    	//备电欠压
#define   INT_TYPE_BACKUP_POWER_WARNING         0xC324    	//备电警示
#define   INT_TYPE_BACKUP_POWER_FAU             0xC325    	//备电故障
#define   INT_TYPE_BUS_SHORT_CIRCUIT            0xC330    	//总线短路
#define   INT_TYPE_BUS_OPEN_CIRCUIT             0xC331    	//总线断路
#define   INT_TYPE_BUS_VOLTAGE_H                0xC332    	//总线过压
#define   INT_TYPE_BUS_VOLTAGE_L                0xC333    	//总线欠压
#define   INT_TYPE_BUS_CURRENT_H                0xC334    	//总线过流
#define   INT_TYPE_BLC_SHORT_H			        0xC335  	//载波总线短路
#define   INT_TYPE_BLC_OPEN_H			        0xC336   	//载波总线断路

#define   INT_TYPE_24V_OPEN_FAV			        0xC340   	//24V断路故障


#define   INT_TYPE_INPUT1_SHORT_FAU     		0xC400      //输入通道1短路故障
#define   INT_TYPE_INPUT1_OFF_FAU       		0xC401      //输入通道1断路故障
#define   INT_TYPE_INPUT2_SHORT_FAU     		0xC410      //输入通道2短路故障
#define   INT_TYPE_INPUT2_OFF_FAU       		0xC411      //输入通道2断路故障
#define   INT_TYPE_INPUT3_SHORT_FAU     		0xC420      //输入通道3短路故障
#define   INT_TYPE_INPUT3_OFF_FAU       		0xC421      //输入通道3断路故障
#define   INT_TYPE_INPUT4_SHORT_FAU     		0xC430      //输入通道4短路故障
#define   INT_TYPE_INPUT4_OFF_FAU       		0xC431      //输入通道4断路故障

#define   INT_TYPE_OUTPUT1_SHORT_FAU    		0xC440     	 	//输出通道1短路故障
#define   INT_TYPE_OUTPUT1_OFF_FAU      		0xC441      	//输出通道1断路故障
#define   INT_TYPE_OUTPUT2_SHORT_FAU    		0xC450      	//输出通道2短路故障
#define   INT_TYPE_OUTUT2_OFF_FAU       		0xC451      	//输出通道2断路故障
#define   INT_TYPE_OUTPUT3_SHORT_FAU    		0xC460      	//输出通道3短路故障
#define   INT_TYPE_OUTPUT3_OFF_FAU      		0xC461      	//输出通道3断路故障
#define   INT_TYPE_OUTPUT4_SHORT_FAU    		0xC470      	//输出通道4短路故障
#define   INT_TYPE_OUTPUT4_OFF_FAU      		0xC471      	//输出通道4断路故障

#define   INT_TYPE_NET_LINE_FAULT               0xC4F0      	//网络链路故障
#define   INT_TYPE_TCP_CONNECT_OPEN_FAU         0xC4F1      	//TCP连接断开
#define   INT_TYPE_GPRS_CONNECT_OPEN_FAU        0xC4F2      	//GPRS连接断开


//设备通讯故障
#define   INT_TYPE_NODEANS_FAU          		0xC500      //节点无应答
#define   INT_TYPE_NODECOMM_FAU         		0xC501      //节点命令不支持
#define   INT_TYPE_ANSCHECK_FAU         		0xC502      //应答校验错误
#define   INT_TYPE_LIMIT_CONDITION_FAU  		0xC503      //条件受限不能正确执行
#define   INT_TYPE_NODE_REGISTER_FAU    		0xC504      //节点未注册
#define   INT_TYPE_NODE_OFFLINE		      		0xC505      //节点未在线
#define   INT_TYPE_ADDR_OVERRUN         		0xC506      //寄存器地址超限
#define   INT_TYPE_NODE_SHILED_STA      		0xC507      //节点被屏蔽

#define   INT_TYPE_NODE_NUM_OVERFLOW    		0xC508      //节点数量溢出
#define   INT_TYPE_BUFFER_OVERFLOW   			0xC509      //通信缓存溢出
//boot使用
#define   INT_TYPE_DATA_LOST            		0xC50A      //数据帧丢失
#define   INT_TYPE_DATA_CRC_FAU         		0xC50B      //数据帧CRC错误

#define   INT_TYPE_NET_COMM_FAULT               0xC50C      	//网络通讯故障

//--------------------------- 存储------------------------------------//
//							   存储相关的故障			        	 //
//-------------------------------------------------------------------//
#define   INT_TYPE_CODE_OVERFLOW              0xC600        //用户区代码超界
#define   INT_TYPE_CODE_ILLEGAL               0xC601        //用户区代码不合法
#define   INT_TYPE_LINK_FORMULA_FAU     	  0xC602      	//联动公式错误
#define   INT_TYPE_DEVICE_INF_FAU             0xC603      	//设备信息错误
#define   INT_TYPE_INF_FAU                    0xC604        //回路信息错误
#define   INT_TYPE_USER_PROGRAM_FAU     	  0xC605      	//用户程序错误
#define   INT_TYPE_FLASH_90PERCENT_FAU     	  0xC606      //录音内存不足，已达flash90%
#define   INT_TYPE_FLASH_FULL_FAU     	   	  0xC607      //录音满


//--------------------------- 同步------------------------------------//
//							   同步操作相关的故障					 //
//-------------------------------------------------------------------//
#define   INT_TYPE_SYSC_LINK_FAU        	    0xC700      	//联动公式同步错误
#define   INT_TYPE_SYSC_DEVICE_FAU              0xC701      	//设备信息同步错误

//--------------------------- 误动------------------------------------//
//							   误动作            					 //
//-------------------------------------------------------------------//
#define   INT_TYPE_UNREGGISTE_UPLOAD       	  	0xC800     		//未注册上报
#define   INT_TYPE_SHIELD_UPLOAD              	0xC801      	//屏蔽上报
#define   INT_TYPE_RE_UPLOAD          		  	0xC802     	 	//重复上报
#define   INT_TYPE_RE_ACTION                  	0xC803     	 	//重复动作
#define   INT_TYPE_ALARM_ERR                    0xC805      	//异常报警

//--------------------------- 故障恢复 -------------------------------//
//						  各种故障恢复事件					         //
//--------------------------------------------------------------------//
#define   INT_TYPE_MAINPOW_RECOVER              0xD000    		//主电恢复正常
#define   INT_TYPE_BACKUP_POWER_RECOVER         0xD001    		//备电恢复正常
#define   INT_TYPE_PRINTER_PAPER_RESUM      	0xD002      	//打印机上纸
#define   INT_TYPE_BUFFER_OVERFLOW_RECOVER     	0xD003     	 	//缓存溢出恢复
#define   INT_TYPE_REG_DEV_ONLINE    			0xD004     		//注册设备上线
#define   INT_TYPE_BLC_NORMAL_H			        0xD005      	//载波总线正常
#define   INT_TYPE_NET_LINE_RECOVER_NOMAL       0xD006      	//网络链路恢复正常
#define   INT_TYPE_NET_COM_RECOVER_NOMAL        0xD007      	//网络通信恢复正常
#define   INT_TYPE_DEV_RECOVER_NOMAL			0xD008			//设备恢复
#define   INT_TYPE_TCP_CONNECT_RECOVER          0xD009     	 	//TCP连接恢复
#define   INT_TYPE_GPRS_CONNECT_RECOVER         0xD00A      	//GPRS连接恢复
#define   INT_TYPE_SIM_RECOVER         			0xD00B      //SIM连接恢复

#define   INT_TYPE_SENSOR_RECOVER               0xD00C      	//传感器恢复
#define   INT_TYPE_BUS_RECOVER                  0xD00D      	//信号线恢复
//#define   INT_TYPE_24V_RECOVER                  0xD00F      	//24V恢复
#define   INT_TYPE_OUTPUT_POWER_RECOVER    0xD010    		//输出电源恢复


//组态等记录性事件（优先级7）
#define   INT_TYPE_REG_DEV_OFFLINE          	0xE001      	//注册设备掉线
#define   INT_TYPE_NEW_DEV_ONLINE    			0xE002      	//新增设备上线 !!!
#define   INT_TYPE_NEW_DEV_OFFLINE          	0xE003      	//新增设备掉线
#define   INT_TYPE_DEV_REPLACE          		0xE004     	 	//设备替换     !!!!
#define   INT_TYPE_REPEAT_ID_DEV    			0xE006      	//ID重复
#define   INT_TYPE_REPEAT_UC_DEV    			0xE007      	//UC重复
#define	  INT_TYPE_REFRESH_DELETE				0xE008			//删除设备
#define   INT_TYPE_DEV_RECOVER       			0xE009      	//设备恢复：直控盘掉线设备重新接入
#define   INT_TYPE_DEV_TYPE_ERROR               0xE00A			//设备类型错误
#define   INT_TYPE_DEV_NO_DEFINE                0xE00B			//新增设备未定义
#define   INT_TYPE_DEV_FAULT      			    0xE00F     		//设备故障

#define   INT_TYPE_GET_MAIN_TOKEN      			0xE100      	//索取主站令牌
#define   INT_TYPE_ONLINE_APPLIED      			0xE101      	//申请上线
#define   INT_TYPE_COMMUN_SUCC_RATE     		0xE102      	//设备通信成功率

#define   INT_TYPE_UC_CHANGE            		0xE200      //UC变化
#define   INT_TYPE_ID_CHANGE            		0xE201      //ID变化
#define   INT_TYPE_CC_CHANGE            		0xE202      //CC变化
#define   INT_TYPE_PARAMETER_CHANGE     		0xE203      //参数变化
#define   INT_TYPE_INF_CHANGE           		0xE204      //信息变换

#define   INT_TYPE_DEV_INFLOG_FINISH    		0xE300      //设备信息登录完毕
#define   INT_TYPE_LOG_DEV_INF                  0xE301      //登录的设备信息
#define   INT_TYPE_LA_UNLAW_DEV_INF            0xE302      //LA重复的设备信息
#define   INT_TYPE_LOGIN_OPER                   0xE303      //系统操作员登录
#define   INT_TYPE_LOGIN_ADMIN                  0xE304      //系统管理员登录
#define   INT_TYPE_LOGIN_SUPER                  0xE305      //超级管理员登录
#define   INT_TYPE_REG_RENEW                    0xE306      //设备重新注册
#define   INT_TYPE_REG_REFRESH          	    0xE307		//设备刷新注册
#define   INT_TYPE_REG_AUTO                     0xE308	    //设备自动注册
#define   INT_TYPE_REG_NETWORK                  0xE309	    //网络注册
#define   INT_TYPE_DEFINE_AUTO          	    0xE30A		//设备自动定义
#define   INT_TYPE_DEFINE_ORDER                 0xE30B      //设备继承定义
#define   INT_TYPE_DEFINE_SERIAL                0xE30C	    //设备连续定义
#define   INT_TYPE_FEILD_DEV_DEFINE         	0xE30D     	//现场设备定义
#define   INT_TYPE_BUS_PANEL_DEFINE         	0xE30E     	//总线盘定义
#define   INT_TYPE_MULTI_PANEL_DEFINE           0xE30F      //多线盘盘定义
#define   INT_TYPE_BROCAST_PANEL_DEFINE         0xE310      //广播盘定义
#define   INT_TYPE_CONTROLER_DEFINE             0xE311      //控制器定义
#define   INT_TYPE_PHONEDISTRI_PANEL_DEFINE     0xE312      //电话分配盘定义
#define   INT_TYPE_PRE_ALARM_DEFINE             0xE313      //预警定义
#define   INT_TYPE_LA_CHANGE                    0xE400      //LA修改
#define   INT_TYPE_NODE_NORMAL                  0xE800      //节点正常

#define   INT_TYPE_NULL						    0xFFFF		//无效	



#define   INT_TYPE_COMM_UNSUPPORT               INT_TYPE_NODECOMM_FAU
#define   INT_TYPE_LIMIT_PERMISSIONS            INT_TYPE_LIMIT_CONDITION_FAU

#endif /* BUS_DOWN_H_ */
