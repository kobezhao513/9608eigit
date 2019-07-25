#ifndef __QN_193K_H__
#define __QN_193K_H__


#include "global.h"
#include "controller.h"


//帧分析
//帧格式: 82 data (22Byte) cs(累加和) 83
//数据域:事件类型+ 机号+ 回路号+ la + 设备类型+ 时间(年月日时分秒)
//帧说明:数据域及校验字节拆分为2个半字节，再加上30
//cs              : 对数据域字节(拆分前)进行累加,先发高字节


#define FRAME_HEAD_QN193K               0x82	//帧头
#define FRAME_CS_LOC_QN193K				0x17	//cs位置
#define FRAME_CSLEN_QN193K				0x1A	//控制器通信帧总长度
#define FRAME_CRC_LOC_QN193K			0x24 	//8位crc位置
#define FRAME_TAIL_QN193K               0x83	//帧尾

#define FRAME_CS_INIT_VALUE_QN193K      0x00	//校验初始化值
#define FRAME_CS_START_OFFSET_QN193K    0x01	//校验起始偏移位置
#define FRAME_EV_OFFSET_QN193k			0x01	//事件类型位置
#define FRAME_CONTR_NUM_OFFSET_QN193k	0x03	//机号位置
#define FRAME_LOOP_NUM_OFFSET_QN193k	0x05	//回路号位置
#define FRAME_LA_OFFSET_QN193k			0x07	//la位置


//事件类型
#define QN193K_EV_POLL_ANSWER			0x09	//巡检应答
#define QN193K_EV_CONTR_FAULT			0xEF	//控制器故障
#define QN193K_EV_CONTR_RESET			0x01	//控制器复位
#define QN193K_EV_CONTR_SILENCE			0x0B	//控制器消音
#define QN193K_EV_FIRE					0x80	//火警
#define QN193K_EV_FIRE1					0x0A	//火警
#define QN193K_EV_FAULT					0x81	//故障
#define QN193K_EV_FAULT_ELIMINATE		0x82	//故障恢复
#define QN193K_EV_LOOP_FAULT			0x87	//回路故障
#define QN193K_EV_LOOP_FAULT_ELIMINATE	0x88	//回路故障恢复
#define QN193K_EV_START_AUTO			0x83	//自动启动
#define QN193K_EV_STOP_AUTO				0x84	//自动停止
#define QN193K_EV_START_MANUAL			0x90	//手动启动
#define QN193K_EV_STOP_MANUAL			0x91	//手动停止
#define QN193K_EV_DEV_ANSWER			0x85	//设备应答
#define QN193K_EV_DEV_ANSWER_STOP		0x86	//设备应答撤销
#define QN193K_EV_SHIELD				0x98	//屏蔽
#define QN193K_EV_SHIELD_RELEASE		0x97	//取消屏蔽
#define QN193K_EV_MULTI_START_MANUAL	0x51	//多线手动启动
#define QN193K_EV_MULTI_STOP_MANUAL		0x52	//多线手动停止
#define QN193K_EV_MULTI_ANSWER			0x53	//多线应答
#define QN193K_EV_MULTI_ANSWER_STOP		0x54	//多线应答停止
#define QN193K_EV_MULTI_START_AUTO		0x55	//多线自动启动
#define QN193K_EV_MULTI_STOP_AUTO		0x56	//多线自动停止
#define QN193K_EV_MULTI_DELAY_START_AUTO	0x57//多线自动启动延时
#define QN193K_EV_MULTI_DELAY_STOP_AUTO		0x58//多线应答缺失
#define QN193K_EV_MULTI_FAULT				0x59//多线线路故障
#define QN193K_EV_MULTI_FAULT_ELIMINATE		0x5A//多线线路故障恢复
#define QN193K_EV_SIMULATED_FIRE			0x8B//模拟火警
#define QN193K_EV_MONITOR					0x8C//监管
#define QN193K_EV_MONITOR_RELEASE			0x8D//监管取消

//多字节组成的事件
#define QN193K_EV_FAULT1					0x80//故障
#define QN193K_EV_FAULT_ELIMINATE1			0x90//故障恢复

#define QN193K_EV_BOARD						0x70//电路板事件
#define QN193K_EV_MAIN_POWER				0x71//主电事件
#define QN193K_EV_BACKUP_POWER				0x72//备电事件


//巡检时间
#define QN193K_POLL_PERIOD1S       125  //1s==125*8

UINT16 Change_Event_Type_QN193k(UINT8 src_event,UINT8 * record_type);
void Com_Task_QN193k_Zj(CONTR_IF *buf);

#endif
