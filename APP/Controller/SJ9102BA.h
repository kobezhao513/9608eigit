#ifndef __SJ9102BA_H__
#define __SJ9102BA_H__


#include "global.h"
#include "controller.h"

//帧分析
//7E 01 contr_num 01 len data csH csL 7E
//帧长度: 为去掉包头4字节，校验和帧尾
//cs               : 对去掉包头4字节的数据校验

//#define FRAME_MIN_LEN_EI2000            7
//#define FRAME_MAX_LEN_EI2000            250

#define FRAME_HEAD_SJ9102BA             0x7E  //帧头
#define FRAME_HEAD_LEN_SJ9102BA         0x01  //帧头长度
#define FRAME_LEN_OFFSET_SJ9102BA       0x04  //帧长度偏移位置
#define FRAME_LEN_BYTE_NUM_SJ9102BA     0x01        
#define FRAME_CS_LEN_SJ9102BA           0x02  //cs长度
#define FRAME_TAIL_SJ9102BA             0x7E  //帧尾
#define FRAME_TAIL_LEN_SJ9102BA         0x01  //帧尾长度
//#define FRAME_OTHER_LEN_EI2000               0x00
#define FRAME_EXCEPT_DATA_LEN_SJ9102BA  0x08//(FRAME_HEAD_LEN_EI2000+FRAME_LEN_BYTE_NUM_EI2000+FRAME_CS_LEN_EI2000+FRAME_TAIL_LEN_EI2000)

#define FRAME_CS_INIT_VALUE_SJ9102BA    0xFFFF  //校验初始化值
//#define FRAME_CS_START_OFFSET_EI2000         0x04  //校验起始偏移位置

//#define EI_VERSION            0x10        //主动发送巡检时使用

//掉线监测时间
//#define EI_CONTR_OFFLINE_TIMER     240    //240S=1S*240 

//巡检时间
//#define EI_POLL_PERIOD25S       3125  //25s==3125*8
//#define EI_POLL_PERIOD30S       3750  //30s==3750*8
//#define EI_POLL_PERIOD40S       5000  //40s==5000*8

#define SJ_EV_OFFSET					0x27//事件类型偏移
#define SJ_EV_TYPE_OFFSET				0x05//事件分类偏移
#define SJ_CONTR_NUM_OFFSET				0x06//机号偏移
#define SJ_LOOP_OFFSET					0x08//回路号偏移
#define SJ_LA_OFFSET					0x0A//LA偏移
#define SJ_DEV_OFFSET					0x0C//设备类型偏移


//通讯命令
#define SJ_COMMAND_POLL					0x01//巡检
#define SJ_COMMAND_RESET				0x0C//复位
#define SJ_COMMAND_POWERON				0x0A//开机
#define SJ_COMMAND_FAULT				0x06//故障
#define SJ_COMMAND_FIRE					0x02//火警
#define SJ_COMMAND_START				0x03//启动
#define SJ_COMMAND_SHIELD				0x08//屏蔽

//事件类型转换
#define SJ_EV_BATTERRY_OPEN				0x5E//备电断路
#define SJ_EV_DEV_OFFLINE				0x62//设备掉线
#define SJ_EV_DEV_START					0x5A//启动
#define SJ_EV_FIRE						0x58//火警
#define SJ_EV_SHIELD					0x5D//设备屏蔽
#define SJ_EV_LOOP_OPEN					0x6F//总线断路


//设备类型转换
#define SJ_DEV_ALARM_INPUT					0x00//报警输入
#define SJ_DEV_SMOKE						0x01//点型感烟
#define SJ_DEV_TEMPERATURE					0x02//点型感温
#define SJ_DEV_PARALLEL_DETECTOR			0x03//并联探头
#define SJ_DEV_MANUAL_BUTTON				0x04//手动按钮
#define SJ_DEV_FIRE_HYDRANT					0x05//消火栓
#define SJ_DEV_LINEAR_TEMPERATURE			0x06//线性感温
#define SJ_DEV_FIRE_DETECTOR				0x07//火焰探测器
#define SJ_DEV_LINEAR_LIGHT					0x08//线性光束
#define SJ_DEV_MONITOR_INPUT				0x14//监管输入
#define SJ_DEV_PRESSURE_SWITCH				0x15//压力开关
#define SJ_DEV_COMBUSTIBLE_GAS				0x16//可燃气体
#define SJ_DEV_WATER_FLOW_INDICATOR			0x17//水流指示
#define SJ_DEV_RESIDUAL_CURRENT				0x18//剩余电流
#define SJ_DEV_ELECTRIC_TEMPERATURE			0x19//电气测温
#define SJ_DEV_SIGNAL_VALVE					0x1A//信号蝶阀
#define SJ_DEV_FIRE_DAMPER_IN				0x1B//防火阀入
#define SJ_DEV_POWER_MODULE					0x1C//电源模块
#define SJ_DEV_DISPLAY_PANEL				0x1D//显示盘
#define SJ_DEV_SPRAY_WARNING				0x1E//喷洒警告
#define SJ_DEV_BOTTLE_VALVE					0x1F//瓶头阀
#define SJ_DEV_START_VALVE					0x20//启动阀
#define SJ_DEV_CONTROLL_PANEL				0x21//控制盘
#define SJ_DEV_BLEED_INDICATION				0x22//放气指示
#define SJ_DEV_SPRAY_START					0x23//喷洒启动
#define SJ_DEV_SPRAY_FEEFBACK				0x24//启动反馈
#define SJ_DEV_VOLTAGE_MEASURE				0x25//电压检测
#define SJ_DEV_CURRENT_MEASURE				0x26//电流检测
#define SJ_DEV_INOUT						0x28//输入输出
#define SJ_DEV_SOUND_LIGHT_ALARM			0x29//声光警报
#define SJ_DEV_BROADCAST_MODULE				0x2A//广播模块
#define SJ_DEV_ALARM_BELL					0x2B//消防警铃
#define SJ_DEV_SMOKE_DAMPER					0x2C//排烟阀
#define SJ_DEV_FIRE_DAMPER					0x2D//防火阀
#define SJ_DEV_POSITIVE_PRESSURE_AIR_VALVE	0x2E//正压风阀
#define SJ_DEV_ROLLING_DOOR_DOWN			0x2F//卷帘全降
#define SJ_DEV_ROLLING_DOOR_HARF			0x30//卷帘半降
#define SJ_DEV_FIRE_DOOR					0x31//防火门
#define SJ_DEV_NEW_FAN						0x32//新风机
#define SJ_DEV_SMOKE_EXHAUST_FAN			0x33//排烟风机
#define SJ_DEV_POSITIVE_PRESSURE_AIR_FAN	0x34//正压风机
#define SJ_DEV_LIGHT_INTENSITY_CUTTING		0x35//照明强切
#define SJ_DEV_STIMULUS_CUTTING				0x36//动力强切
#define SJ_DEV_FIRE_PUMP					0x37//消防泵
#define SJ_DEV_SPRAY_PUMP					0x38//喷淋泵
#define SJ_DEV_ELEVATOR_CRASH				0x39//电梯迫降
#define SJ_DEV_AIR_CONDITION_CUTTING		0x3A//空调切断
#define SJ_DEV_FIRE_CURTAIN					0x3B//消防水幕
#define SJ_DEV_DULUGE_VALVE					0x3C//雨淋阀
#define SJ_DEV_SMOKE_SCREEN					0x3D//挡烟垂壁
#define SJ_DEV_EMERGENCY_LIGHTING			0x3E//应急照明
#define SJ_DEV_ENTRANCE_GUARD				0x3F//门禁系统
#define SJ_DEV_FIRE_WATER_MONITOR			0x40//消防水炮
#define SJ_DEV_WATER_MIST					0x41//细水雾
#define SJ_DEV_ELECTROMAGNETISM_RELEASE		0x42//电磁释放
#define SJ_DEV_ELECTRIC_DOOR_CLOSER			0x43//电动闭门
#define SJ_DEV_ELECTROMAGNETIC_DOOR_HOLDER	0x44//电磁门吸
#define SJ_DEV_DOORSENSOR					0x45//门磁开关
#define SJ_DEV_MULTILINE_MODULE				0x46//多线模块
#define SJ_DEV_FEEDBACK_INPUT				0x47//输入反馈
#define SJ_DEV_LOOP							0x4F//总线回路
#define SJ_DEV_POWER						0x72//主机电源


typedef struct
{
	UINT8 sj_dev_type;
	UINT8 ts_type;
}SJ_DEV_TYPE;


UINT16 Change_Event_Type_SJ9102BA(UINT8 src_event,UINT8 *record_type);
void Com_Task_SJ9102BA_JT(CONTR_IF *buf);



#endif
