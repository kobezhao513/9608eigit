
#ifndef __TX3607_H_
#define __TX3607_H_

#include "controller.h"
//帧分析
//帧格式:0x5x len data cs
//帧长度: 长度为所有数据长度
//cs               : 对所有数据校验

#define FRAME_LEN_TX3607            		32

#define FRAME_HEAD_TX3607               0x40  //帧头:@
#define FRAME_HEAD_LEN_TX3607           0x01  //帧头长度
#define FRAME_LEN_OFFSET_TX3607         0x01  //帧长度偏移位置;帧长度包括所有数据以及校验
#define FRAME_LEN_BYTE_NUM_TX3607       0x01     
#define FRAME_CS_LEN_TX3607             0x06  //cs长度
#define FRAME_TAIL_TX3607               0x23  //#
#define FRAME_TAIL_LEN_TX3607           0x01  //帧尾长度
//#define FRAME_OTHER_LEN_TX3607            0x00
#define FRAME_EXCEPT_DATA_LEN_TX3607    FRAME_HEAD_LEN_TX3607+FRAME_CS_LEN_TX3607+FRAME_TAIL_LEN_TX3607

#define FRAME_REPEAT_TIME_TX3607        5000  //重复帧筛选时间5S



#define FRAME_CS_INIT_VALUE_TX3607    0xFFFF  //校验初始化值
#define FRAME_CS_START_OFFSET_TX3607  25  //校验起始偏移位置,从帧头开始



void Com_Task_TX3607_Jt(CONTR_IF *buf);

#endif
