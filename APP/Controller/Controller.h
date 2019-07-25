#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "global.h"
#include "uart.h"
#include "basic.h"


#define RS485_COMM_TYPE   0x00
#define RS232_COMM_TYPE   0x01


#define CS_NUMBER_H        0x00
#define CS_NUMBER_L        System.uc[0]//0x00
#define CTR_NUMBER_H       0x00
#define CTR_NUMBER_L       BCD2Hex(System.ctr_addr[0])

#define CONTR_OFFLINE_TIMER     93    //93S=1S*93  


extern UINT8 Last_Uc[LEN_REG_UC];
extern UINT16 Last_Event_Type;
extern UINT32 Last_Event_Time;
extern UINT8 CenterCmd_Erease_Flash_Flag;
extern UINT8 CenterCmd_Rst_Flag;



enum COM_TYPE
{
	TS=0,				//鼎信协议
	GST200_CRT_ZJ,		//海湾200 CRT主叫GST200   GST-NNET-02接口卡
	GST500_CRT_ZJ,		//海湾500 CRT主叫GST500  GST5000    GST-NNET-02接口卡
	EI2000_CRT_ZJ,		//依爱控制器主叫EI2000G、EI6000T
	QN5010_CRT_ZJ,		//青鸟5010控制器主叫JBF5010
	GST200_CRT_JT,		//海湾200 CRT监听GST200   GST-NNET-02接口卡
	GST500_CRT_JT,		//海湾500 CRT监听GST500  GST5000    GST-NNET-02接口卡
	LD128EN_CRT_485_ZJ,	//利达LD128EN(M)主叫	LD128EN(M)
	GST200_SF_ZJ,		//海湾200MODBUS协议GST200  GST-INET-03A接口卡
	GST500_SF_ZJ,		//海湾500MODBUS协议GST500  GST5000  GST-INET-03A接口卡
	LD128EN_CRT_JT,		//利达LD128EN(M)监听	LD128EN(M)

	TC5120_PRT_JT,		//营口天成打印机	TC5120
	GT603_SF_ZJ,		//国泰怡安第三方接口  GT603
	SJ9102BA_CRT_ZJ,	//松江CRT主叫	9102BA
	SJ9102BA_CRT_JT,	//松江CRT监听	9102BA
	NT8003_CS_ZJ,		//尼特传输设备主叫		FT8003
	TANDA3004_PRT,		//泰和安打印机		TX3004
	FORSAFE_SF,			//赋安三方接口
	FHSJ9108_PRT,		//泛海三江打印机	9108
	SIMPLEX_CRT_ZJ,		//辛普利斯CRT
	LD128EN_CRT_232_ZJ,	//利达CRT主叫

	QN_193K_SF_ZJ,		//青鸟三方接口 193K接口卡
	QN5010_CRT_JT,		//青鸟5010控制器监听JBF5010
	LD128EN_PRT,		//利达打印机	LD128EN(M)
	TX3607_FW19000_485,	//北京防威TX3607接口卡     FW19000
	TX3607_FW19000_232,	//北京防威TX3607接口卡     FW19000
	N3030_OLD,			//诺蒂菲尔老款3030控制器
	PTW3300_JT,			//保得威尔 3300控制器  、诺蒂菲尔新款3030控制器
	FT8003,
	Parallel_Debug,			//串口数据输出模式
	COM_NULL,
};

enum UART_STA
{
	UART_CLOSE=0,
	UART_REC_INIT,
	UART_REC_NO_INIT,
	UART_SEND_INIT,
	UART_SEND_NO_INIT,
	UART_CLOSE_TXD,
};
enum CONTOR_LED
{
    LED_OFF = 0X00,
	LED_BLINK,
	LED_LIGHT,
};
enum RXD_INFO_ANALYSIS_STATE
{
	FRAME_HEAD=0,
	FRAME_LEN,
	FRAME_LEN_EXP,
	FRAME_DATA,
	FRAME_CS,
};

#define FRAME_LEN_LOC
enum SEND_STA
{
	SEND_IDLE = 0,
	WAITING,
	SENDING,
	//PREPARE,
};

#define CONTR_BUF_LEN   (1024*1)

typedef struct
{
    UINT16 Clev;       //接收缓存处理位置
	UINT16 Head;       //接收缓存接收位置
	UINT16 FrameEndLoc;//找到帧头和长度后，计算出该帧结束位置;
	                   //确认收到一帧数据时使用           
    UINT16 FrameCount;
	UINT16 ByteInterval;//字节监控
	UINT16 FrameLen;   //找到帧头和长度后赋值;计算校验时使用
	UINT8 Buf[CONTR_BUF_LEN];	
}RECEIVE;
typedef struct 
{
	UINT16 Clev;
	UINT16 Tail;
	UINT8 Buf[256];
	UINT8 Sta;	
	UINT8 RepNum;				//重复次数
	UINT8 RepFlag;              //重复标识
	UINT16 RepCounter;          //重复计时     		
}SEND;

typedef struct 
{
	RECEIVE R;				//串口数据接收缓存
	SEND T;					//串口数据发送缓存
	UINT8 DAT_Return[512];  //解析正确后一帧数据放入该缓存，后续取数以及处理均从这取
	UINT8 AnalyseSta;
	UINT8 PollEnFlag;
	UINT16 PollTimer;
	UINT8  Poll_Ctr;
}CONTR_IF;

extern CONTR_IF RS232;
extern CONTR_IF RS485;

typedef struct
{
    UINT16 Count;  //故障计时
	UINT16 Time;   //故障时间:不同控制器掉线时间按照应用来定
	UINT8 Flag;    //故障标志
}COM_FAULT;
typedef struct
{
	COM_FAULT ComFault;
	UINT8 Type;
	UINT8 Addr;
	UINT8 Led;
	BIT_CHAR Task;
	UINT8 delay;
}CONTR;
extern CONTR Contr;
typedef struct
{	
	COM_FAULT ComFault;
	UINT8 send_flag;
	UINT16 Event_Pc;
}COMPETE_TO_CONTR;
extern COMPETE_TO_CONTR Compete_Event;

typedef struct
{
	UINT8 Led;
	UINT8 Gprs;
}DEBUG;
extern DEBUG Debug;
typedef struct 
{
    UINT8 SrcEventType;
	UINT8 RecordType;
	UINT16 DestEventType;
}CHANGE_EVENT_TYPE;

typedef struct 
{
	UINT8 const * const  Event_Annotaiton; 
	UINT8 RecordType;
	UINT16 EventType;
}GET_EVENT_TYPE_ASCII;


typedef struct 
{
	UINT8 const * const src_type;
	UINT8 dest_type;
}GET_Device_Type_ASCII;


#define B_Upload_Self_Info        Contr.Task.b.b0
#define B_Reset_Ctr               Contr.Task.b.b1
#define B_Silence_Ctr    		  Contr.Task.b.b2
#define B_Poll_Ctr                Contr.Task.b.b3

void Monitor_Controller_Slave_Uart(CONTR_IF *buf);
UINT16 HexToBCD(UINT8 hex);
void Hex_To_BasicUC(UINT8 *uc,UINT8 *hex,UINT8 len);

void Poll_Controller_Init(CONTR_IF *buf);
void Save_Controller_Self_Info(UINT8 record_type,UINT16 detailed_type);
void Controller_Uart_Init(void);
void Controller_Data_Init(void);
void Controller485_Uart_Init(void);
void Controller485_Uart_Sta_Switch(UINT8 type) ;
void Controller232_Uart_Sta_Switch(UINT8 type);
void Monitor_Controller_Offline(void);
void  Monitor_Trans_Equip_Offline(void);
void Monitor_Controller_Fault_Eliminate(void);
void Monitor_Trans_Equip_Fault_Eliminate(void);
void Monitor_Controller_Uart(CONTR_IF *buf);
void Monitor_Controller_Uart_Timer_Run(CONTR_IF *buf);
void Controller_Uart_Rec_Data(CONTR_IF *buf, USART_TypeDef *uart);
void Controller_Uart_Send_Data(CONTR_IF *buf, USART_TypeDef *uart);
void RS485_Task(void);
void RS232_Task(void);
void Controller_Com(void);

#endif
