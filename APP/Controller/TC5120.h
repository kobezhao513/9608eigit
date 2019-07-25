#ifndef __tc5120_H__
#define __tc5120_H__


#include "global.h"
#include "controller.h"

//帧分析
//68 len len 68   self 0 goal 0 10 ctr num data   cs 16

#define FRAME_MIN_LEN_EI2000            7
#define FRAME_MAX_LEN_EI2000            250

#define FRAME_HEAD_TC5120_PRT           0x0D  //帧头
#define FRAME_HEAD_LEN_TC5120_PRT       0x01  //帧头长度
#define FRAME_TYPE1_LEN_TC5120_PRT		0x05//消息类型1收完的长度
#define FRAME_TYPE2_LEN_TC5120_PRT		0x3F//消息类型2收完的长度
#define FRAME_LEN1_TC5120_PRT			0x13//短帧帧长
#define FRAME_LEN2_TC5120_PRT			0x19//中等长度帧的帧长
#define FRAME_LEN3_TC5120_PRT			0x55//次长帧的帧长
#define FRAME_LEN4_TC5120_PRT			0x58//最长帧的帧长
#define FRAME_TAIL_TC5120_PRT           0x0A  //帧尾
#define FRAME_TAIL_LEN_TC5120_PRT       0x01  //帧尾长度

#define FRAME_LOOP_LOC1_TC5120_PRT		0x48//火警、启动、停动、反馈等事件回路信息获取地址
#define FRAME_LOOP_LOC2_TC5120_PRT		0x45//故障等事件回路信息获取地址
#define FRAME_LA_LOC1_TC5120_PRT		0x4B//火警等事件LA信息获取地址
#define FRAME_LA_LOC2_TC5120_PRT		0x48//故障等事件LA信息获取地址




//设备类型
#define DEV_SMOKE  			0x2A//感烟
#define DEV_TEMPERATURE  	0x1F//感温
#define DEV_INPUT  			0x55//输入模块
#define DEV_OUTPUT  		0x56//输出模块
#define DEV_INOUT  			0x57//输入输出模块
#define DEV_BUTTON  		0x17//手报
#define DEV_ALARM  			0x7D//声光

#define DEV_UNKNOWN  		0xFF//未知设备


void Com_Task_Tc5120_Prt_JT(CONTR_IF *buf);


#endif
