/*
 * bus_dn.h
 *
 *  Created on: 2013-8-7
 *      Author: zjc
 */

#ifndef UART_H_
#define UART_H_

#include "global.h"



#define RS485_IRQHandler   USART3_4_IRQHandler
#define RS485_USART  USART3
//#define USART_IRQHandler   USART3_4_IRQHandler
#define RS232_IRQHandler   USART2_IRQHandler
#define RS232_USART  USART2

#define 	BUS_HIGH		      1
#define 	BUS_LOW			      0


#define   UART_BUFFER_LEN			      1200

#define 	UART_BAUD_921600  			52
#define 	UART_BAUD_460800  			104
#define 	UART_BAUD_230400  			208
#define  	UART_BAUD_125000			384
#define		UART_BAUD_115200		    417
#define		UART_BAUD_57600            	833
#define		UART_BAUD_38400            	1250
#define		UART_BAUD_28800            	1667
#define		UART_BAUD_19200            	2500
#define		UART_BAUD_9600            	5000
#define		UART_BAUD_4800             	10000
#define		UART_BAUD_2400             	20000



//============串口通信数据结构定义============================

#define UART_SEND_IDLE		0x00				//空闲
#define UART_CRC_INIT		UART_SEND_IDLE+0x01	//CRC初始化
#define UART_SEND_DATA		UART_CRC_INIT+0x01	//发送数据
#define UART_SEND_CRCL		UART_SEND_DATA+0x01	//发送CRC0
#define UART_SEND_CRCH		UART_SEND_CRCL+0x01	//发送CRC1
#define UART_SEND_OVER		UART_SEND_CRCH+0x01	//发送结束
#define UART_SEND_ERR			0x07				//发送错误

#define UART_REC_IDLE			0x00				//空闲
#define UART_REC_HEARD		    0x01				//CRC初始化
#define UART_REC_LEN			0x02				//接收数据
#define UART_REC_DATA			0x03				//接收数据
#define UART_REC_CRCL			0x04				//接收CRC0
#define UART_REC_CRCH			0x05				//接收CRC1
#define UART_REC_OVER			0x06				//接收结束
#define UART_REC_ERR			0x07				//接收错误

#define UART_REC_LEN2			0x20				//接收长度数据2


#define UART_STA_IDLE			0x11				  //空闲
#define UART_STA_COMM_PROCESS	0x33			//正在通信
#define UART_STA_COMM_OVER		0x55			//通信完毕,且成功
#define UART_STA_COMM_ERR		0xAA				//通信完毕,且故障


#define UART_BUSY				0xAA				  //忙标志

#define CHN_ERR_COUNTER_MAX		0x64			//通道错误计数器限制
#define BPS_ERR_COUNTER_MAX		40				//BPS错误计数器限制

#define BPS_MAX_TYPE			8					    //BPS最大种类
#define BPS_RS232_TYPE_DEFAULT  0				//BPS默认速率类型
#define BPS_RS485_TYPE_DEFAULT  4				//BPS默认速率类型
#define BPS_RS485_MAX_BPS  		2					//485通道最大波特率
#define BPS_RS232_MAX_BPS  		0					//232通道最大波特率



typedef struct 
{
	UINT8 Status;
	UINT8 RepeatNum;					//重复次数
	split Crc;
	UINT16 TotalLen;					//总长度
	UINT16 ActualLen;					//实际已发送长度
	UINT8 ByteInterval;	
	UINT8 DiLoc;					  	//Di位置 对于需要根据di做相应处理的函数进行处理	
	//UINT8 Buff[UART_BUFFER_LEN];
	UINT8 Buff[256];
}UART_SEND;


typedef struct
{
	UINT8 Status;
	UINT8 ByteInterval;					//字节间隔
	split Crc;
	UINT16 TotalLen;						//总长度
	UINT16 ActualLen;  					//实际已接收的数据长度
	UINT16 WaitTime;
	UINT8 Buff[UART_BUFFER_LEN];
}UART_RECEIVE;

typedef struct 
{
	UART_SEND Send;					//串口数据发送缓存
	UART_RECEIVE Rec;				//串口数据接收缓存
	UINT8 FrameTimer;				//BUS帧定时器
	UINT8 FrameIntervalTime;			//串口发送帧间隔
	UINT8 AnsFlag;					//应答标识 TRUE 应答 FALSE 不应答
	UINT8 FrameNum;					//串口发送帧序号 在组帧是使用，切循环累加
UINT8 RunSta;					//UART的运行状态  见UART_BUFFER.RunSta定义
}UART_BUFFER;
//============ 总线任务变量类型 ===========================



extern UART_BUFFER Contr_Pro;

//============串口重发次数============================
#define UART_REPEAT_TIMES		2					    //命令最重发次数	

//============串口接收字节间隔============================
#define BUS_RECEIVE_INTERVAL	10				    //总线数据接收间隔 单位mS	

#define BUS_MAX_FRAME_INTERVAL	125 //			//250*8=2000ms,总线帧间隔  单位mS	

#define SLAVE_REC_BYTE_INTERVAL	10 				//从设备字节间隔


//============  UART_BUFFER.RunSta运行状态定义  ============================

#define RUN_STA_IDLE			0x00				    //运行状态 空闲
#define RUN_STA_TXD			  RUN_STA_IDLE+1	//运行状态 发
#define RUN_STA_RXD			  RUN_STA_TXD+1		//运行状态 收
#define RUN_STA_LOG			  RUN_STA_RXD+1		//运行状态 登录
#define RUN_STA_DEAL			RUN_STA_LOG+1		//运行状态 处理
#define RUN_STA_WAIT			RUN_STA_DEAL+1	//运行状态 等待


//============UART运行状态====================================
#define UART_STA_CLOSE    		0x00          		//串口关闭状态
#define UART_STA_RECE_NO_INT    0x33          		//串口无中断接收状态
#define UART_STA_RECE_INT     	0x55            	//串口中断接收状态
#define UART_STA_SEND_NO_INT	0xaa        	  	//串口无中断发送状态
#define UART_STA_SEND_INT       0xcc         	  	//串口中断发送状态

void Make_Crc(split *crc ,uchar data);


void Contr_485Uart_Init(UINT8 BAND);
void RS485_Uart_Init(UINT8 type);
void  Contr_485Uart_Sta_Switch(UINT8 type) ;

void Contr_232Uart_Init(UINT8 BAND);
void RS232_Uart_Init(UINT8 type);
void Contr_232Uart_Sta_Switch(UINT8 type);

void Monitor_Master_Timer_Run(UART_BUFFER *buff);

void Monitor_Master_Uart(UART_BUFFER  *buff);

void Master_Uart_Over_With_Err(UART_BUFFER *buff);
void Master_Uart_Over_With_Succ(UART_BUFFER  *buff);


void Master_Uart_Rec_Data(UART_BUFFER *buff, USART_TypeDef *uart);
void Master_Uart_Send_Data(UART_BUFFER *buff, USART_TypeDef *uart);



#endif 



