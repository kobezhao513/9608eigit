
#ifndef _NF_FT8003_PRT_
#define _NF_FT8003_PRT_

#include "global.h"
#include "controller.h"


//帧分析
//第一条信息以1B 38 开头，最后一条信息以0D 0D结束 打印内容以ASCII码传输

#define FRAME_MIN_LEN_FT8003_PRT              4
#define FRAME_MAX_LEN_FT8003_PRT              127


#define FRAME_HEAD_FT8003_PRT                 0x1B   	//帧头
#define FRAME_HEAD2_FT8003_PRT                0x38   	//帧头
#define FRAME_HEAD_LEN_FT8003_PRT             0x02   	//帧头长度
#define FRAME_TAIL_FT8003_PRT                 0X0D		//帧尾
#define FRAME_TAIL2_FT8003_PRT                0X0D		//帧尾
#define FRAME_TAIL_LEN_FT8003_PRT             0X02		//帧尾长度



void Com_Task_FT8003_PRT(CONTR_IF *buf);

#endif
