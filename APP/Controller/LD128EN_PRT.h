
#ifndef __LD128EN_PRT_H
#define __LD128EN_PRT_H

#include "global.h"
#include "controller.h"

//帧分析
//第一条信息以1B 36 开头，最后一条信息以0A 0A 结束 打印内容以ASCII码传输

#define FRAME_MIN_LEN_LE128EN_PRT              4 
#define FRAME_MAX_LEN_LE128EN_PRT              127


#define FRAME_HEAD_LE128EN_PRT                 0x1B   //帧头
#define FRAME_HEAD2_LE128EN_PRT                0x36   //帧头
#define FRAME_HEAD_LEN_LE128EN_PRT             0x02   //帧头长度
#define FRAME_TAIL_LE128EN_PRT                 0X0A		//帧尾
#define FRAME_TAIL2_LE128EN_PRT                0X0A		//帧尾
#define FRAME_TAIL_LEN_LE128EN_PRT             0X02		//帧尾长度


typedef struct 
{
	UINT8 const * const src_type;
	UINT8 dest_type;
}LD_PRT_Device_Type;


void Com_Task_LE128EN_PRT(CONTR_IF *buf);


#endif
