#ifndef __GST200_H__
#define __GST200_H__

#include "global.h"
#include "controller.h"
#include "record_manage.h"


//帧分析
//帧格式:0x5x len data cs
//帧长度: 长度为所有数据长度
//cs               : 对所有数据校验

#define FRAME_MIN_LEN_GST200            5
#define FRAME_MAX_LEN_GST200            51

#define FRAME_HEAD_GST200               CTR_NUMBER_L  //帧头:0x5+控制器地址
#define FRAME_HEAD_LEN_GST200           0x01  //帧头长度
#define FRAME_LEN_OFFSET_GST200         0x01  //帧长度偏移位置;帧长度包括所有数据以及校验
#define FRAME_LEN_BYTE_NUM_GST200       0x01     
#define FRAME_CS_LEN_GST200             0x01  //cs长度
//#define FRAME_TAIL_GST200                        //无帧尾
//#define FRAME_TAIL_LEN_GST200                 0x00  //帧尾长度
//#define FRAME_OTHER_LEN_GST200            0x00
#define FRAME_EXCEPT_DATA_LEN_GST200    0 //(FRAME_HEAD_LEN_GST200+FRAME_LEN_OFFSET_GST200+FRAME_CS_LEN_GST200)

#define FRAME_CS_INIT_VALUE_GST200    0x00  //校验初始化值
#define FRAME_CS_START_OFFSET_GST200  0x00  //校验起始偏移位置,从帧头开始


//发送数据类型
#define GST200_CONTR_ANSWER           0x50
#define GST200_CONTR_POLL             0x30

#define GST_POLL_PERIOD300MS          37  //1000/8
#define GST_POLL_PERIOD600MS          75


//GST事件类型
#define GST200_FIRE          0X15   //火警
#define GST200_ANSWER        0X22   //动作
#define GST200_RESUME        0X11   //恢复
#define GST200_START         0X02   //启动
#define GST200_STOP          0X03   //停动
#define GST200_FAULT         0X21   //故障
#define GST200_SHIELD        0XDE   //隔离
#define GST200_RELEASE       0XDF   //释放
#define GST200_DELAY         0X23   //延时
#define GST200_CANCEL        0X24   //延时结束
#define GST200_ELIMINATE     0X20   //排除


#define GST200_CTRSELF       0X31   //控制器自身
#define GST200_RESET         0X04   //控制器复位
#define GST200_SILENCE       0X61   //控制器消音



//GST设备类型
#define GST_GRAT_TEMPER_MEASURE      1   //光栅测温
#define GST_SPOT_HEAT                2   //点型感温
#define GST_SPOT_SMOKE               3   //点型感烟
#define GST_ALARM_INTERFACE          4   //报警接口
#define GST_COMPOSITE_FLAME          5   //复合火焰
#define GST_BEAM_SMOKE               6   //光束感烟
#define GST_ULTRAVIOLET_FLAME        7   //紫外火焰
#define GST_LINEAR_HEAT              8   //线型感温
#define GST_SUCTION_SMOKE            9   //吸气感烟
#define GST_COMPOSITE_FIRE           10   //复合探测
#define GST_MANUAL_POINT             11   //手动按钮
#define GST_FIRE_BROADCAST           12   //消防广播
#define GST_ACOUSTIC                 13   //讯响器
#define GST_FIRE_TELEPHONE           14   //消防电话
#define GST_FIRE_HYDRANT             15   //消火栓
#define GST_FIRE_HYDRANT_PUMP        16   //消火栓泵
#define GST_SPRAY_PUMP               17   //喷淋泵
#define GST_STABILIZED_PUMP          18   //稳压泵
#define GST_SMOKE_VENTILATOR         19   //排烟机
#define GST_PRESSURE_FAN             20   //送风机
#define GST_NEW_FAN                  21   //新风机
#define GST_FRD                      22   //防火阀
#define GST_SMOKE_EXHAUST_DAMPER     23   //排烟阀
#define GST_BLOW_VALVE               24   //送风阀
#define GST_SOLENOID_VALVE           25   //电磁阀
#define GST_ROLLING_DOOR_MID         26   //卷帘门中
#define GST_ROLLING_DOOR_DOW         27   //卷帘门下
#define GST_FIREPROOF_DOOR           28   //防火门
#define GST_PRESS_SWITCH             29   //压力开关
#define GST_WATER_FLOW_INDICATOR     30   //水流指示
#define GST_ELEVATOR                 31   //电梯
#define GST_AIR_CONDITIONING_UNIT    32   //空调机组
#define GST_DIESEL_GENERATOR         33   //柴油发电
#define GST_LIGHT_DISTRIBUTION       34   //照明配电
#define GST_POWER_DISTRIBUTION       35   //动力配电
#define GST_WATER_ELECTROMAGNETIC    36   //水幕电磁
#define GST_GAS_START                37   //气体启动
#define GST_GAS_STOP                 38   //气体停动
#define GST_SLAVE                    39   //从机
#define GST_FIRE_DISPLAY_PANEL       40   //火灾示盘
#define GST_SLUICE_VALVE             41   //闸阀
#define GST_DRY_POWDER               42   //干粉灭火
#define GST_FOAM_PUMP                43   //泡沫泵
#define GST_FIRE_CONTROL_POWER       44   //消防电源
#define GST_EMERGENCY_LIGHTING       45   //紧急照明
#define GST_GUIDANCE_INSTRUCTIONS    46   //疏导指示
#define GST_SPRAYING_INSTRUCTIONS    47   //喷洒指示
#define GST_SECURITY_MODULE          48   //防盗模块
#define GST_SIGNAL_VALVE             49   //信号蝶阀
#define GST_SMOKE_CONTROL_VALVE      50   //防排烟阀
#define GST_WATER_PUMP               51   //水幕泵
#define GST_FLOOR_LAMP               52   //层灯号
#define GST_EQUIPMENT_STOP           53   //设备停动
#define GST_PUMP_FAILURE             54   //泵故障
#define GST_EMERGENCY_START_BUTTON   55   //急启按钮
#define GST_EMERGENCY_STOP_BUTTON    56   //急停按钮
#define GST_RAIN_PUMP                57   //雨淋泵
#define GST_UPPER_COMPUTER           58   //上位机
#define GST_LOOP                     59   //回路
#define GST_AIR_COMPRESSOR           60   //空压机
#define GST_LINKAGE_POWER            61   //联动电源
#define GST_TELEPHONE_JACK           62   //电话插孔
#define GST_PART_EQUIPMENT           63   //部分设备
#define GST_DRENCH_VALVE             64   //雨淋阀
#define GST_HEAT_RODS                65   //感温棒
#define GST_FAULT_OUTPUT             66   //故障输出
#define GST_MANUAL_ALLOW             67   //手动允许
#define GST_AUTO_ALLOW               68   //自动允许
#define GST_COMBUSTIBLE_GAS          69   //可燃气体
#define GST_STANDBY_INDICATOR        70   //备用指示
#define GST_DOOR_LAMP                71   //门灯
#define GST_BACKUP_WORK              72   //备用工作
#define GST_EQUIPMENT_TROUBLE        73   //设备故障
#define GST_EMERGENCY_CALLING        74   //紧急求助
#define GST_CLOCK_POWER              75   //时钟电源
#define GST_SOUND_LIGHT_ALARM        76   //声光警报
#define GST_ALARM_TRANSMISSION       77   //报警传输
#define GST_LOOP_SWITCH              78   //环路开关
#define GST_UNDEFINED                79   //未定义
#define GST_DELAY_OUTPUT             80   //延时输出
#define GST_FIRE_HYDRANT_2           81   //消火栓
#define GST_CABLE_HEAT               82   //缆式感温
#define GST_BREATHE_SMOKE            83   //吸气感烟
#define GST_BREATHE_FIRE             84   //吸气火警
#define GST_BREATHE_WARNING          85   //吸气预警
#define GST_DETECTOR_DIRTY           86   //探测器脏
#define GST_MULTI_WIRE               87   //多线制盘
#define GST_SIMULATR_HEAT            88   //模拟感温
#define GST_ELECTRIC_ALARM           89   //漏电报警
#define GST_BUS                      90   //总线

enum GST200_SEND_COMM
{
	GST200_COM_HEAD=0,
	GST200_COM,
};


enum GST200_CRC_INIT
{
	NO_INIT=0,
	MARK,
	SPACE,
};

typedef struct 
{
  UINT8 src_type;
  UINT8 dest_type;
}Gst_Device_Type;



typedef struct 
{
	UINT8 init_mode;
	UINT8 infor_index;
	UINT8 gst200_com;
    UINT8 gst500_poll_process;
    UINT8 gst500_timeout;
	UINT8 delay;
	
}GST;
extern GST Gst;



void Send_Answer_Frame_GST200(CONTR_IF *buf,UINT8 type);
void Send_Command_Frame_GST200(CONTR_IF *buf,UINT8 type);
UINT16 Change_Event_Type_GST200(UINT8 src_event,UINT8 *record_type);
UINT16 Change_UC_Type_GST200(UINT8 sevent_type);
void Save_Component_Info_GST200(UINT8 *buf);
void Save_Other_Info_GST200(UINT8 *buf);
void GST200_Save_Reset_Infor(UINT8 *buf);
UINT8 Analyse_GST200(CONTR_IF *buf);
void Handle_GST200_Zj(CONTR_IF *buf);
void Handle_GST200_Jt(CONTR_IF *buf);
void Com_Task_GST200_Zj(CONTR_IF *buf);
void Com_Task_GST200_Jt(CONTR_IF *buf);
void GST_Uart_Init_Change(UINT8 type);
#endif
