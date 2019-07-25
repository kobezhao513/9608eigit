

#ifndef _N3030_OLD__
#define _N3030_OLD__

#include "controller.h"

//帧分析
//帧格式: dd data cs bb
//帧长度: 随不同数据长度固定
//cs  :仅对数据校验
#define FRAME_LEN_N3030_OLD				164

#define FRAME_HEAD_FIRE_N3030_OLD		0x6B	//火警 6B 7E A5 66
#define FRAME_HEAD_FAULT_N3030_OLD		0x45	//故障45 9C
#define FRAME_HEAD_ANSWER_N3030_OLD		0xD1	//反馈D1 A1
#define FRAME_HEAD_SHIELD_N3030_OLD		0xF2	//屏蔽F2 AB 94 BB
//屏蔽解除、 故障消除 等等
//解除屏蔽格式: 05 64 + F2 AB 94 BB
#define FRAME_HEAD_RESUME_N3030_OLD		0x05	//恢复05 64 

  
#define FRAME_TAIL_N3030_OLD                0x0A  //帧尾

#define FRAME_TAIL_LEN_N3030_OLD            0x01  //帧尾长度

void Com_Task_N3030_OLD_Jt(CONTR_IF *buf);

#endif
