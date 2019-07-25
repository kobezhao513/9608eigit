

#ifndef _PTW3300_H_
#define _PTW3300_H_

#include "controller.h"

#define FRAME_LEN_PTW3300				332


//接收均为UNICODE码格式 可能测试不全面，需后续添加

#define FRAME_HEAD_FIRE_PTW3300			0x70	//火警 70 6B 70 7E 62 A5 8B 66 火灾报警
#define FRAME_HEAD_FAULT_PTW3300		0x6D	//故障 6D 88 96 32 65 45 96 9C 消防故障 6D 88 96 32 8B BE 59 07 62 16 53 3A 57 DF 4E 0D 80 FD 5D E5 4F 5C 5D F2 6E 05 96 64 消防设备或区域不能工作已清除(解除屏蔽)
#define FRAME_HEAD_MONITER_PTW3300 	0x76	//监管 76 D1 63 A7 7A D9 76 D1 63 A7 4F E1 53 F7 监控站监控信号
#define FRAME_HEAD_SHIELD_PTW3300		0x97	//屏蔽 97 5E 6D 88 96 32 8B BE 59 07 96 94 79 BB 非消防设备隔离
#define FRAME_HEAD_RESET_PTW3300    0x7C  //复位 7C FB 7E DF 59 0D 4F 4D 系统复位
#define FRAME_HEAD_VERIFY_PTW3300   0x78  //确认 78 6E 8B A4 98 84 8B 66 确认预警
#define FRAME_HEAD_RESUME_PTW3300		0x98	//恢复 98 84 62 A5 8B 66 5D F2 89 E3 96 64 预报警已解除
#define FRAME_HEAD_RELEASE_PTW3300		0x5B	//解除5B F9 97 5E 6D 88 96 32 8B BE 59 07 76 84 96 94 79 BB 5D F2 6E 05 96 64    对非消防设备的隔离已清除

  
#define FRAME_TAIL_PTW3300                0x0A  //帧尾

#define FRAME_TAIL_LEN_PTW3300           0x01  //帧尾长度


void Com_Task_PTW3300_Jt(CONTR_IF *buf);

#endif
