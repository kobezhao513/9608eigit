#ifndef __FORSAFE_H__
#define __FORSAFE_H__


#include "global.h"
#include "controller.h"

//帧分析
//F0 LA len data   cs 16
//帧长度: 为data域的长度
//cs               : 从LA到data的数据校验

#define FRAME_DATA_MAX_LEN_FORSAFE		0x28  //每帧中数据域最大长度

#define FRAME_HEAD_FORSAFE              0xF0  //帧头
#define FRAME_HEAD_LEN_FORSAFE			0x01  //帧头长度
#define FRAME_LA_LEN_FORSAFE			0x01  //LA长度
#define FRAME_LEN_BYTE_LEN_FORSAFE		0x01  //长度字节长度
#define FRAME_LEN_OFFSET_FORSAFE        0x02  //帧长度偏移位置
#define FRAME_CS_LEN_FORSAFE            0x02  //cs长度
#define FRAME_CS_INIT_VALUE_FORSAFE     0x0000//校验初始化值
#define FRAME_CS_START_OFFSET_FORSAFE   0x01  //校验起始偏移位置
#define FRAME_EXCEPT_DATA_LEN_FORSAFE	0x05  //每帧除去数据域后的长度(head+la+len+cs的长度)

#define FRAME_EV_SOURCE_LOC_FORSAFE		0x05  //信息类型位置
#define FRAME_EV_TYPE_LOC_FORSAFE		0x06  //事件类型位置
#define FRAME_EV_CONTR_NUM_LOC_FORSAFE	0x08  //机号位置
#define FRAME_EV_LOOP_NUM_LOC_FORSAFE	0x0F  //回路位置
#define FRAME_EV_LA_LOC_FORSAFE			0x10  //LA位置
#define FRAME_DEV_TYPE_LOC_FORSAFE		0x15  //设备类型位置


//掉线监测时间
#define FORSAFE_CONTR_OFFLINE_TIMER     90    //240S=1S*240 


//通讯命令
#define FORSAFE_COMMAND_LOC				0x03//命令字符位置
#define FORSAFE_COMMAND_RESET			0xA1//控制器命令传输设备复位，不响应该命令
#define FORSAFE_COMMAND_POLL			0xA2//巡检，需应答
#define FORSAFE_COMMAND_TIME			0xA4//校时命令，不响应该命令
#define FORSAFE_COMMAND_EVENT			0xA5//事件命令，无需应答


//事件类型转换
#define FORSAFE_EV_DEV_ONLINE					0x00//设备上线 	
#define FORSAFE_EV_DEV_OFFLINE					0x01//设备掉线
#define FORSAFE_EV_DEV_TYPE_FAULT				0x02//设备类型错误
#define FORSAFE_EV_DEV_LA_REPEAT				0x03//设备重码
#define FORSAFE_EV_DEV_START_FAIL				0x04//启动失败
#define FORSAFE_EV_ALARM						0x05//火警
#define FORSAFE_EV_START_REQUEST				0x06//启动请求
#define FORSAFE_EV_START_SUCCEED				0x07//启动成功
#define FORSAFE_EV_DEV_OUTPUT_FAULT				0x08//模块输出故障
#define FORSAFE_EV_POWER_FAULT					0x09//主备电故障
#define FORSAFE_EV_POWER_FAULT_ELIMINATE		0x0A//主备电故障恢复
#define FORSAFE_EV_DEV_OUTPUT_SHORT				0x0B//模块输出短路
#define FORSAFE_EV_DEV_MULTI_SHORT				0x0E//多线短路
#define FORSAFE_EV_DEV_MULTI_CIRCUIT			0x0F//多线开路
#define FORSAFE_EV_DEV_MULTI_FAULT				0x10//多线故障
#define FORSAFE_EV_DEV_MULTI_FAULT_ELIMINATE	0x11//多线故障恢复
#define FORSAFE_EV_DEV_SELF_CHECK_FAULT			0x12//自诊断故障
#define FORSAFE_EV_DEV_POLLUTION				0x13//污染
#define FORSAFE_EV_COMM_FAULT					0x14//通信故障
#define FORSAFE_EV_COMM_FAULT_ELIMINATE			0x15//通信故障恢复
#define FORSAFE_EV_FLASH_FAULT					0x16//flash故障
#define FORSAFE_EV_FLASH_FAULT_ELIMINATE		0x17//flash故障恢复
#define FORSAFE_EV_MULTI_START_REQUEST1			0x18//多线启动请求
#define FORSAFE_EV_MULTI_STOP_REQUEST_1			0x19//多线停止请求
#define FORSAFE_EV_MULTI_START_SUCCEED1			0x1A//多线启动成功
#define FORSAFE_EV_MULTI_STOP_SUCCEED1			0x1B//多线停止成功
#define FORSAFE_EV_MULTI_START_SUCCEED2			0x1C//多线现场启动成功
#define FORSAFE_EV_MULTI_STOP_SUCCEED2			0x1D//多线现场停止成功
#define FORSAFE_EV_MULTI_START_FAIL1			0x1E//多线启动失败
#define FORSAFE_EV_MULTI_STOP_FAIL1				0x1F//多线停止失败
#define FORSAFE_EV_CONTR_POWERON				0x21//开机
#define FORSAFE_EV_CONTR_RESET					0x22//复位
#define FORSAFE_EV_CONTR_AUTO					0x23//自动允许
#define FORSAFE_EV_CONTR_ALARM					0x24//火警确认
#define FORSAFE_EV_CONTR_EARLY_ALARM			0x25//预警确认
#define FORSAFE_EV_CONTR_EARLY_RESET			0x26//预警复位
#define FORSAFE_EV_DEV_SHIELD					0x28//屏蔽
#define FORSAFE_EV_DEV_SHIELD_RELEASE			0x29//屏蔽解除
#define FORSAFE_EV_LOOP_SHIELD					0x2A//回路屏蔽
#define FORSAFE_EV_LOOP_SHIELD_RELEASE			0x2B//回路屏蔽解除
#define FORSAFE_EV_DEV_MANUAL_START				0x2C//手动启动
#define FORSAFE_EV_DEV_MANUAL_STOP				0x2D//手动停止
#define FORSAFE_EV_DEV_LINK_START				0x2E//联动启动
#define FORSAFE_EV_DEV_LINK_DELAY				0x2F//联动延时
#define FORSAFE_EV_LOOP_SHORT					0x30//总线短路
#define FORSAFE_EV_24VLOOP_FAULT				0x31//24V总线异常
#define FORSAFE_EV_24VLOOP_CURRENT_FAULT		0x32//24V总线漏电流异常
#define FORSAFE_EV_LOOP_VOLTAGE_FAULT			0x33//总线参考电压异常
#define FORSAFE_EV_5VLOOP_CURRENT_FAULT			0x34//5V总线漏电流异常
#define FORSAFE_EV_5VLOOP_VOLTAGE_FAULT			0x35//5V总线电压异常
#define FORSAFE_EV_0VLOOP_VOLTAGE_FAULT			0x36//0V总线电压异常
#define FORSAFE_EV_0VLOOP_CURRENT_FAULT			0x37//0V总线漏电流异常
#define FORSAFE_EV_LOOP_HIGHVOLTAGE_CIRCUIT		0x38//高压中断
#define FORSAFE_EV_LOOP_VOLTAGE_LOW				0x39//总线电压低
#define FORSAFE_EV_LOOP_VOLTAGE_HIGH			0x3A//总线电压高
#define FORSAFE_EV_LOOP_CURRENT_FAULT			0x3B//总线电流异常
#define FORSAFE_EV_LOOP_LEAKAGE_CURRENT_FAULT	0x3C//漏电流异常
#define FORSAFE_EV_LOOP_GND_FAULT				0x3D//总线负线异常
#define FORSAFE_EV_LOOP_MAIN_FAULT				0x3E//总线正线异常
#define FORSAFE_EV_LOOP_NO_POWER				0x3F//总线不能上电
#define FORSAFE_EV_LOOP_FAULT_ELIMENATE			0x40//总线故障恢复
#define FORSAFE_EV_EARLY_ALARM					0x41//预警
#define FORSAFE_EV_DEV_FAULT					0x42//设备故障
#define FORSAFE_EV_MULTI_START_REQUEST2			0x43//多线启动请求
#define FORSAFE_EV_MULTI_STOP_REQUEST_2			0x44//多线停止请求
#define FORSAFE_EV_MULTI_START_SUCCEED3			0x45//多线启动成功
#define FORSAFE_EV_MULTI_START_FAIL2			0x46//多线启动失败
#define FORSAFE_EV_MULTI_STOP_SUCCEED3			0x47//多线停止成功
#define FORSAFE_EV_MULTI_STOP_FAIL2				0x48//多线停止失败


//信息类型
#define FORSAFE_EV_SOURCE_DETECTOR				0x01//探头
#define FORSAFE_EV_SOURCE_MODULE				0x02//模块
#define FORSAFE_EV_SOURCE_LOOP					0x03//回路
#define FORSAFE_EV_SOURCE_MAIN_POWER			0x04//主电
#define FORSAFE_EV_SOURCE_BATTERY				0x05//备电
#define FORSAFE_EV_SOURCE_CRT					0x06//计算机
#define FORSAFE_EV_SOURCE_PANEL					0x07//显示盘
#define FORSAFE_EV_SOURCE_MULTI					0x08//多线
#define FORSAFE_EV_SOURCE_CONTR					0x09//控制器
#define FORSAFE_EV_SOURCE_NET_MODULE			0x0A//网络模块
#define FORSAFE_EV_SOURCE_CHARGE				0x0B//充电
#define FORSAFE_EV_SOURCE_SYS_BOARD				0x0C//系统板


UINT16 Change_Event_Type_Forsafe_SF(UINT8 src_event,UINT8 *record_type);
void Com_Task_Forsafe_SF_Zj(CONTR_IF *buf);


#endif
