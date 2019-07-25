#ifndef __EI_PRINT_COMM__
#define __EI_PRINT_COMM__


#include "global.h"
#include "controller.h"

//打印机报文解帧
#define FRAME_MIN_LINE_LEN_PRINT        5           //每行最小接收长度 5个 作为结束标识
#define FRAME_MAX_LINE_LEN_PRINT        30          //每行最大接收长度 实际28个 预留2个
#define FRAME_HEAD0_PRINT               0x1C        //打印机行首标识0
#define FRAME_HEAD1_PRINT               0x26        //打印机行首标识1
#define FRAME_BLANK_PRINT               0x20        //空格
#define FRAME_TAIL0_PRINT               0x0A        //打印机行尾标识0
#define FRAME_TAIL1_PRINT               0x0D        //打印机行尾标识1

#define Send_Heart_Poll   500

typedef struct
{
	   UINT8 Time[6];					   // 记录时间: 年月日时分秒   6
	   UINT8 Id[LEN_REG_ID];			   // ID 12
	   UINT8 Uc[LEN_REG_UC];			   // 用户码(user code) 5
	   UINT8 Cc[LEN_REG_CC];			   // 汉字描述(character code) 32
	   UINT8 Type;						   // 记录类型,对应ALARM_TYPE_TAB[]
	   UINT8 EventLen;
	   split EventType; 				   // 事件类型	4
	   UINT8 Event[LEN_RECORD_EVENT];  
}UPLOAD_STRUCT;
enum
{
	EI_PRINT_COMM_RECV_IDLE,		                //接收空闲
	EI_PRINT_COMM_RECV_HEAD0,						//接收头0 0x1C
	EI_PRINT_COMM_RECV_HEAD1,                       //接收头1 0x26
	EI_PRINT_COMM_RECV_DATA,                        //接收数据
	EI_PRINT_COMM_RECV_OVER,                        //打印机数据接收完成
	EI_PRINT_COMM_RECV_ERR                          //打印机接收数据错误
};
typedef struct
{
	volatile UINT16 status;							//通信状态
	//volatile UINT16 interval;							//字节间隔
	volatile UINT16 pc;								//数据指针
	UINT16 length;										//数据长度
	UINT8 buff[150];										//通信缓存
	UINT16 crc;									//crc校验结果
	UINT8 cs;
}COMM_BASE;
extern CONTR_IF EI_Comm;

/*typedef struct
{
	COMM_BASE recv;
	UINT8 RunSta;
	//COMM_BASE send;
}COMMUNICATE;*/

void Parallel_Init(void);
void Parall_Uart_Rec_Data(UINT8 dataRec);
void Data_Deal_Task(void);

#endif
