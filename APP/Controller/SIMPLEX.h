#ifndef __SIMPLEX_H__
#define __SIMPLEX_H__


#include "global.h"
#include "controller.h"

//帧分析
//1C seq data 17 cs1 cs2
//校验范围为帧头与帧尾之间的所有数

#define FRAME_HEAD_SIMPLEX              0x1C  //帧头
#define FRAME_HEAD_LEN_SIMPLEX 			0x01  //帧头长度
#define FRAME_TAIL_SIMPLEX              0x17  //帧尾
#define FRAME_SEQ_LEN_SIMPLEX 			0x01  //帧序列号长度
#define FRAME_SEQ_LOC_SIMPLEX			0x01  //帧序列号位置
#define FRAME_CS_LEN_SIMPLEX            0x02  //cs长度
#define FRAME_CS_INIT_VALUE_SIMPLEX     0x0000//校验初始化值
#define FRAME_CS_START_OFFSET_SIMPLEX   0x01  //校验起始偏移位置
#define FRAME_COMMAND_LOC_SIMPLEX		0x02  //命令字位置


//掉线监测时间
#define SIMPLEX_CONTR_OFFLINE_TIMER     240    //240S=1S*240 


//通讯命令字
#define FRAME_COMMAND_ACK_SIMPLEX		0x06//ACK
#define FRAME_COMMAND_POLL_SIMPLEX		0x05//巡检
#define FRAME_COMMAND_ANSWER_SIMPLEX	0x02//巡检应答
#define FRAME_COMMAND_START_SIMPLEX		0x11//开机或复位
#define FRAME_COMMAND_EVENT_SIMPLEX		0x1E//事件上报

//标识
#define FRAME_NEED_ACK_SIMPLEX			0x2D//需要应答ACK，-的ASKII值
#define FRAME_ACKED_SIMPLEX				0x2A//已经回过ACK，*的ASKII值

//事件类型转换
#define SIMPLEX_EV_FIRE					0x56//火警
#define SIMPLEX_EV_FIRE_RESET			0x46//未报火警
#define SIMPLEX_EV_EARLY_ALARM			0x60//预警
#define SIMPLEX_EV_EARLY_ALARM_RESET	0x50//预警恢复
#define SIMPLEX_EV_FAULT				0x64//故障
#define SIMPLEX_EV_FAULT_ELIMINAATE		0x54//故障恢复
#define SIMPLEX_EV_MONITOR				0x63//监管
#define SIMPLEX_EV_MONITOR_RELEASE		0x53//监管释放


UINT16 Change_Event_Type_Simplex(UINT8 src_event,UINT8 *record_type);
void Com_Task_Simplex_Zj(CONTR_IF *buf);


#endif
