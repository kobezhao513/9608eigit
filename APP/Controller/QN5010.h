#ifndef __QN5010_H__
#define __QN5010_H__


#include "global.h"
#include "controller.h"


//帧分析
//帧格式: FF FF FF ctr 00 00 6E 01 len data()  cs0 cs1
//帧长度: 为数据长度
//cs               : 对从ctr开始的数据校验

#define FRAME_MIN_LEN_QN5010           0
#define FRAME_MAX_LEN_QN5010            200

#define FRAME_HEAD_QN5010               0xFF
#define FRAME_HEAD_LEN_QN5010           0x03  //帧头长度
#define FRAME_LEN_OFFSET_QN5010         0x08  //帧长度偏移位置
#define FRAME_LEN_BYTE_NUM_QN5010       0x01          
#define FRAME_CS_LEN_QN5010             0x02  //cs长度
//#define FRAME_TAIL_QN5010                //无帧尾
//#define FRAME_TAIL_LEN_QN5010           0x00  //帧尾长度
//#define FRAME_OTHER_LEN_QN5010            0x05

#define FRAME_EXCEPT_DATA_LEN_QN5010     0x0B//(FRAME_LEN_OFFSET_QN5010+FRAME_LEN_BYTE_NUM_QN5010+FRAME_CS_LEN_QN5010)

#define FRAME_CS_INIT_VALUE_QN5010      0x00  //校验初始化值
#define FRAME_CS_START_OFFSET_QN5010    0x03  //校验起始偏移位置


//通讯命令
#define QN_COMMAND_TYPE_ALARM_EV           0x80
#define QN_COMMAND_TYPE_REG_DEV_OTHER_EV   0x0B
#define QN_COMMAND_TYPE_START_OR_STOP      0x0C
#define QN_COMMAND_TYPE_RESET              0x0D
#define QN_COMMAND_TYPE_POLL_RETURN        0x13

//巡检时间
#define QN_POLL_PERIOD1S       125  //1s==125*8

//事件类型转换
#define QN_EV_TYPE_REG_DEV_OFFLINE              0x00
#define QN_EV_TYPE_REG_DEV_ONLINE               0x01

#define QN_EV_TYPE_REG_DEV_SHILED               0x02
#define QN_EV_TYPE_REG_DEV_SHILED_RELEASE       0x03

#define QN_EV_TYPE_MAIN_POWER_FAULT             0x05
#define QN_EV_TYPE_MAIN_POWER_RECOVER           0x06
#define QN_EV_TYPE_BACKUP_POWER_FAULT           0x07
#define QN_EV_TYPE_BACKUP_POWER_RECOVER         0x08
#define QN_EV_TYPE_START_FEEDBACK               0x88
#define QN_EV_TYPE_START_NO_FEEDBACK            0x89
#define QN_EV_TYPE_MANNU_START                  0x90
#define QN_EV_TYPE_MANNU_STOP                   0x91
#define QN_EV_TYPE_AUTO_START                   0x83
#define QN_EV_TYPE_AUTO_STOP                    0x84
#define QN_EV_TYPE_FEEDBACK                    	0x85

void Save_Component_Info_QN5010(UINT8 *buf);
void Save_Other_Info_QN5010(UINT8 *buf);
UINT16 Change_Event_Type_QN5010(UINT8 src_event,UINT8 *record_type);
void Send_Poll_Contr_QN5010(CONTR_IF *buf);
UINT8 Analyse_QN5010(CONTR_IF *buf);
void Handle_QN5010_Zj(CONTR_IF *buf);
void Com_Task_QN5010_Zj(CONTR_IF *buf);
void Com_Task_QN5010_Jt(CONTR_IF *buf);

#endif
