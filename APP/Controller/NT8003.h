#ifndef __NT8003_H__
#define __NT8003_H__


#include "global.h"
#include "controller.h"

//帧分析
//AA 55 DF(设备类型) XOR CS len DATA
//帧长度: 从DATA开始计算
//cs		: 从DATA开始计算
//XOR		:从DATA开始计算

#define FRAME_HEAD1_NT8003              	0xAA  //帧头
#define FRAME_HEAD2_NT8003					0x55  //帧头
#define FRAME_HEAD_LEN_NT8003           	0x02  //帧头长度
#define FRAME_LEN_OFFSET_NT8003         	0x05  //帧长度偏移位置
#define FRAME_LEN_BYTE_NUM_NT8003       	0x01        
#define FRAME_CS_LEN_NT8003             	0x01  //cs长度
#define FRAME_XOR_LEN_NT8003				0x01  //XOR长度
//#define FRAME_TAIL_EI2000               0x16  //帧尾
//#define FRAME_TAIL_LEN_EI2000           0x01  //帧尾长度
//#define FRAME_OTHER_LEN_EI2000               0x00
#define FRAME_EXCEPT_DATA_LEN_NT8003    	0x06//(FRAME_HEAD_LEN_EI2000+FRAME_LEN_BYTE_NUM_EI2000+FRAME_CS_LEN_EI2000+FRAME_TAIL_LEN_EI2000)

#define FRAME_CS_INIT_VALUE_NT8003      	0x00  //累加校验初始化值
#define FRAME_XOR_INIT_VALUE_NT8003			0x00  //异或校验初始化值

#define FRAME_CS_XOR_START_OFFSET_NT8003    0x06  //校验起始偏移位置
#define FRAME_XOR_OFFSET_NT8003				0x03  //异或和校验值所在位置
#define FRAME_CS_OFFSET_NT8003				0x04  //累加和校验值所在位置
#define FRAME_EV_OFFSET_NT8003				0x06  //事件类型偏移地址
#define FRAME_EV_OTHER_TYPE_OFFSET_NT8003	0x09  //其他类故障事件类型偏移地址
#define FRAME_CONTR_NUM_OFFSET_NT8003		0x07  //控制器机号偏移地址
#define FRAME_LOOP_NUM_OFFSET_NT8003		0x08  //回路号偏移地址
#define FRAME_LA_OFFSET_NT8003				0x09  //地址号偏移地址
#define FRAME_DEV_TYPE_OFFSET_NT8003		0x0A  //设备类型偏移地址
#define FRAME_EV_TIME_OFFSET_NT8003			0x0D  //事件时间偏移地址

#define FRAME_ANSWER_NT8003					0x66  //应答帧内容

//掉线监测时间
#define NT_CONTR_OFFLINE_TIMER     			90    //90S=1S*90 



//事件类型转换
#define NT_EV_POLL					0xE6//巡检
#define NT_EV_RESET					0xE1//控制器复位
#define NT_EV_FIRE					0x01//火警
#define NT_EV_START					0x02//启动
#define NT_EV_FEEDBACK				0x03//反馈
#define NT_EV_MONITOR				0x04//监管
#define NT_EV_COMPONENT_FAULT		0x06//器件故障
#define NT_EV_MODULE_FAULT			0x07//模块故障
#define NT_EV_MODULE_STOP			0x0D//停止
#define NT_EV_FEEDBACK_CANCLE		0x0E//反馈停止
#define NT_EV_MONITOR_CANCLE		0x13//监管取消
#define NT_EV_FAULT_ELIMINALE		0x18//故障恢复
#define NT_EV_OTHER_FAULT			0x2D//其他故障
#define NT_EV_OTHER_FAULT_ELIMINALE	0x2E//其他故障恢复

//其他故障事件类型
#define NT_EV_TRANSMIT				0x09//传输设备的事件
#define NT_EV_MAINPOWER				0x1F//主电故障或恢复
#define NT_EV_MAINPOWER_VOLTAGE_LOW	0x20//主电欠压
#define NT_EV_BACKUP_POWER			0x21//备电故障或恢复

//设备类型转换
#define NT_DEV_TEMPERATURE						0x01//感温
#define NT_DEV_SMOKE							0x04//感烟
#define NT_DEV_TEMPERATURE_SMOKE				0x05//烟温复合
#define NT_DEV_INFRARED							0x06//点型红外
#define NT_DEV_ULTRAVIOLET						0x07//点型紫外
#define NT_DEV_GAS								0x09//可燃气体
#define NT_DEV_INFRARED_LIGHT					0x0A//红外光束
#define NT_DEV_LINEAR_TEMPERATURE				0x0B//线性感温
#define NT_DEV_INPUT							0x0C//报警输入模块
#define NT_DEV_MANNUAL_BUTTON					0x0D//手报
#define NT_DEV_SIGNAL_INPUT						0x0E//信号输入
#define NT_DEV_PULSE_OUTPUT						0x1F//脉冲输出
#define NT_DEV_VOLTAGE_OUTPUT					0x20//电平输出
#define NT_DEV_BROADCAST						0x23//广播
#define NT_DEV_ALARM							0x24//声光
#define NT_DEV_HYDRANT_PUMP						0x25//消火栓泵
#define NT_DEV_SPRAY_PUMP						0x26//喷淋泵
#define NT_DEV_STABILIZING_PUMP					0x27//稳压泵
#define NT_DEV_WATER_CURTAIN_PUMP				0x28//水幕泵
#define NT_DEV_DELUGE_PUMP						0x29//雨淋泵
#define NT_DEV_FOAM_PUMP						0x2A//泡沫泵
#define NT_DEV_DRAFT_FAN						0x2B//排烟机
#define NT_DEV_FORCED_DRAFT_FAN					0x2C//送风机
#define NT_DEV_NEW_FAN							0x2D//新风机
#define NT_DEV_AIR_PUMP							0x2E//空压机
#define NT_DEV_FIRE_DAMPER						0x2F//防火阀
#define NT_DEV_SMOKE_DAMPER						0x30//排烟阀
#define NT_DEV_BLOW_VALVE						0x31//送风阀
#define NT_DEV_ELECTROMAGNETIC_VALVE			0x32//电磁阀
#define NT_DEV_SMOKE_CONREOL_VALVE				0x33//防排烟阀
#define NT_DEV_WATER_CURTAIN_ELECTROMAGNETIC	0x34//水幕电磁
#define NT_DEV_DELUGE_VALVE						0x35//雨淋阀
#define NT_DEV_ELEVATOR							0x36//电梯
#define NT_DEV_AIR_CONDITION_UNIT				0x37//空调机组
#define NT_DEV_DIESEL_GENERATOR					0x38//柴油发电
#define NT_DEV_LIGHT_DSITRIBUTION				0x39//照明配电
#define NT_DEV_MOTIVE_ELECTRICITY				0x3A//动力配电
#define NT_DEV_FLOOR_LIGHT						0x3B//层号灯
#define NT_DEV_EMERHENCY_LIGHT					0x3C//应急照明
#define NT_DEV_EVACUATION_INSTRUCTION			0x3D//疏散指示
#define NT_DEV_HARF_FIRE_CURTAIN				0x3E//半降卷帘
#define NT_DEV_FIRE_CURTAIN						0x3F//全降卷帘
#define NT_DEV_FIRE_DOOR						0x40//防火门
#define NT_DEV_MULTILINE_DISK					0x41//多线盘
#define NT_DEV_ALARM_OUTPUT						0x42//声光输出
#define NT_DEV_BROADCAST_MODULE					0x43//广播模块
#define NT_DEV_REGULAR_PULSE_MODULE				0x44//常规脉冲
#define NT_DEV_REGULAR_VOLTAGE_MODULE			0x45//常规电平
#define NT_DEV_REGULAR_BROADCAST				0x46//常规广播
#define NT_DEV_START_BUTTON						0x4B//启动按钮
#define NT_DEV_STOP_BUTTON						0x4C//停止按钮
#define NT_DEV_SPRAY_MODULE						0x4D//喷洒模块
#define NT_DEV_GAS_FIRE_EXTINGUISHING_CONTROLL	0x4E//器灭控制盘
#define NT_DEV_BURGLAR_DETECTOR					0x4F//防盗探测器
#define NT_DEV_FIRE_DISPLAY_PANEL				0x50//火灾显示盘
#define NT_DEV_FIRE_HYDRANT_BUTTOON				0x51//消火栓按钮
#define NT_DEV_MONITOR_INPUT_MODULE				0x52//监管输入模块
#define NT_DEV_PRESSURE_SWITCH					0x53//压力开关
#define NT_DEV_WATER_FLOW_INDICATOR				0x54//水流指示
#define NT_DEV_OVERHEAD_WATER_TANK				0x55//高位水箱
#define NT_DEV_SIGNAL_VALVE						0x56//信号蝶阀
#define NT_DEV_FIRE_POWER						0x57//消防电源
#define NT_DEV_CONTR_POWER						0x58//联动电源


typedef struct
{
	UINT8 nt_dev_type;
	UINT8 ts_dev_type;
}NT_DEV_TYPE;


UINT16 Change_Event_Type_NT8003(UINT8 src_event,UINT8 *record_type);
void Com_Task_NT8003_ZJ(CONTR_IF *buf);


#endif
