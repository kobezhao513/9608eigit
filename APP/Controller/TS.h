#ifndef __TS_H__
#define __TS_H__


#include "global.h"
#include "controller.h"

//帧分析
//68 len len 68   self 0 goal 0 10 ctr num data   cs 16
//帧长度: 为去掉帧头和校验，包括长度字节
//cs               : 对去掉包头4字节的数据校验

#define FRAME_MIN_LEN_TS              3 
#define FRAME_MAX_LEN_TS              127
#define FRAME_MAX_LEN_EXP_TS          2048//目前最大使用1100

#define FRAME_HEAD_TS                 0x7F   //帧头
#define FRAME_HEAD_LEN_TS             0x01   //帧头长度
#define FRAME_LEN_OFFSET_TS           0x01   //帧长度偏移位置
#define FRAME_LEN_BYTE_NUM_TS         0x01        
#define FRAME_CS_LEN_TS               0x02   //cs长度
//#define FRAME_TAIL_TS                 //无帧尾
//#define FRAME_TAIL_LEN_TS            //帧尾长度
#define FRAME_EXCEPT_DATA_LEN_TS     (FRAME_HEAD_LEN_TS+FRAME_CS_LEN_TS)

#define FRAME_CS_INIT_VALUE_TS        0xFFFF //校验初始化值
#define FRAME_CS_START_OFFSET_TS      0x00   //校验起始偏移位置
#define TS_POLL_PERIOD90S       11250  	//90s==11250*8
#define DI_CONTROL_INFO			0xC0	//控制器信息

extern UINT8 Contr_Mode;
extern UINT8 Fr_Lastser;

#define COM_232   00
#define COM_485   01


#define GET_EVENT_NOMAL_LEN      46
#define GET_EVENT_SPECIAL_LEN    46+50
#define GET_EVENT_TYPE_LOC       47


enum
{
	EVENT_NO = 0,
	EVENT_FIND = 1,
	EVENT_UPLOAD = 2,
};

enum
{
	SELF_EVENT = 1,
	RESET_EVENT,
	SILENCE_EVENT,
	COMMAND_EVENT,
	CMD_SEND_SUCC,
};


enum TS_FARAWAY_COMM_STATE
{
	TS_DATA_IDLE=0,
	TS_DATA_VALID,
	TS_DATA_SENDING,
	TS_DATA_SEND_SUC,
};

typedef struct
{
  UINT8 Flag;
  UINT8 Buf[256];
}TS_FARAWAY_DATA;

typedef struct
{
  TS_FARAWAY_DATA R;
  TS_FARAWAY_DATA T;

}TS_FARAWAY_COM;
extern TS_FARAWAY_COM TS_Far;



typedef struct
{
	UINT32 addr;			//
	UINT16 diLoc;			//DI位置
	UINT16 dataLoc;			//数据域位置
	UINT8 ser;
}CONTR_COMMAND;
UINT16 Caculate_CRC(UINT16 CS,UINT8 data);

void Controller_Com_Event_Save(CONTR_IF * buf);
UINT8 Find_Info_Upload(void);
void Slave_Prepare_Answer(CONTR_IF * buf,UINT8 len);
void Slave_Info_Send(CONTR_IF * buf,UINT8 info_type);
void TS_Get_Controller_Data(UINT8 *send_buf);
void TS_Get_Center_Data(UINT8 *rec_buf);
UINT8 Analyse_Info_TS(CONTR_IF *buf);
void Handle_Info_TS(CONTR_IF *buf);
void Com_Task_TS(CONTR_IF *buf);

UINT8 Find_Module_Msg(UINT8 *Record_Buffer);
void Module_Msg_Send_Fal(void);
void Module_Msg_Send_Suc(void);



#endif
