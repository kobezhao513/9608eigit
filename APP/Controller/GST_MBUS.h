#ifndef __GST_MBUS_H__
#define __GST_MBUS_H__

#include "global.h"
#include "controller.h"

//帧分析
//CTR_NUMBER_L  03 len data   cs0 CS1
//帧长度: 为去掉包头4字节，校验和帧尾
//cs               : 对去掉包头4字节的数据校验

#define FRAME_MIN_LEN_MBUS             2
#define FRAME_MAX_LEN_MBUS             210

#define FRAME_HEAD_MBUS                CTR_NUMBER_L  //帧头
#define FRAME_HEAD_LEN_MBUS            0x01  //帧头长度
#define FRAME_LEN_OFFSET_MBUS          0x02  //帧长度偏移位置
#define FRAME_LEN_BYTE_NUM_MBUS        0x01        
#define FRAME_CTR_LEN_MBUS             0x01
#define FRAME_CS_LEN_MBUS              0x02  //cs长度
//#define FRAME_TAIL_MBUS                          //无帧尾
//#define FRAME_TAIL_LEN_MBUS                  //帧尾长度
//#define FRAME_OTHER_LEN_MBUS            0x01
#define FRAME_EXCEPT_DATA_LEN_MBUS     0x05//(FRAME_HEAD_LEN_MBUS +FRAME_LEN_OFFSET_MBUS +FRAME_LEN_BYTE_NUM_MBUS +FRAME_CS_LEN_MBUS)

#define FRAME_CS_INIT_VALUE_MBUS            0xFFFF  //校验初始化值
#define FRAME_CS_START_OFFSET_MBUS          0x00  //校验起始偏移位置




#define Selsct_Count          100  //????????
#define Selsct_Total_GST200   241  //??????
#define Selsct_Total_GST5000  (System.ctr_loop_end+1)*240  //??????
#define Selsct_Start_GST200   0  //??????
#define Selsct_Start_GST5000  System.ctr_loop_start*240   //??????

extern UINT16 Selsct_Sta; //??????


void Com_Task_GST_Modbus(CONTR_IF *buf);
void GST_Modbus_Event_Sum(uint16*event_num);
UINT16 GST_Modbus_Event_Sta(uint16 start_loc,uint16 sum,uint8*event_buff);

#endif
