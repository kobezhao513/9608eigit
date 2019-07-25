#ifndef __EI2000_H__
#define __EI2000_H__


#include "global.h"
#include "controller.h"

//帧分析
//68 len len 68   self 0 goal 0 10 ctr num data   cs 16
//帧长度: 为去掉包头4字节，校验和帧尾
//cs               : 对去掉包头4字节的数据校验

#define FRAME_MIN_LEN_EI2000            7
#define FRAME_MAX_LEN_EI2000            250

#define FRAME_HEAD_EI2000               0x68  //帧头
#define FRAME_HEAD_LEN_EI2000           0x02  //帧头长度
#define FRAME_LEN_OFFSET_EI2000         0x01  //帧长度偏移位置
#define FRAME_LEN_BYTE_NUM_EI2000       0x02        
#define FRAME_CS_LEN_EI2000             0x01  //cs长度
#define FRAME_TAIL_EI2000               0x16  //帧尾
#define FRAME_TAIL_LEN_EI2000           0x01  //帧尾长度
//#define FRAME_OTHER_LEN_EI2000               0x00
#define FRAME_EXCEPT_DATA_LEN_EI2000    0x06//(FRAME_HEAD_LEN_EI2000+FRAME_LEN_BYTE_NUM_EI2000+FRAME_CS_LEN_EI2000+FRAME_TAIL_LEN_EI2000)

#define FRAME_CS_INIT_VALUE_EI2000           0x00  //校验初始化值
#define FRAME_CS_START_OFFSET_EI2000         0x04  //校验起始偏移位置

#define EI_VERSION            0x10        //主动发送巡检时使用

//掉线监测时间
#define EI_CONTR_OFFLINE_TIMER     240    //240S=1S*240 

//巡检时间
#define EI_POLL_PERIOD25S       3125  //25s==3125*8
#define EI_POLL_PERIOD30S       3750  //30s==3750*8
#define EI_POLL_PERIOD40S       5000  //40s==5000*8


//通讯命令
#define EI_COMMAND_TYPE_ALARM           135
#define EI_COMMAND_TYPE_FAULT           138
#define EI_COMMAND_TYPE_RESET           139
#define EI_COMMAND_TYPE_POLL_RETURN     140
#define EI_COMMAND_TYPE_OTHER_ALARM     141
#define EI_Refresh_CONTR                200
#define EI_POLL_CONTR                   206
#define EI_COMMAND_TYPE_ANSWER          250
#define EI_COMMAND_TYPE_UPLOAD_INF      0x3F //巡检上报


//事件类型转换
#define EI_EV_TYPE_ALARM                0x01   //火警
#define EI_EV_TYPE_SMOKE_ALARM          0x16   //感烟火警
#define EI_EV_TYPE_TEMPERATURE_ALARM    0x17   //感温火警

#define EI_EV_TYPE_THEFT_ALARM          0x02   //盗警
#define EI_EV_TYPE_PRE_ALARM            0x03   //预警
#define EI_EV_TYPE_DETECTOR_FAULT       0x04   //探头故障
#define EI_EV_TYPE_AGE_FAULT            0x05   //老化故障
#define EI_EV_TYPE_SHORT_ISULATE_FAULT  0x07   //短路隔离故障
#define EI_EV_TYPE_NO_FAULT             0x08   //未编故障

#define EI_EV_TYPE_ACTION               0x0A   //动作
#define EI_EV_TYPE_ISOLATOR             0x0B   //隔离
#define EI_EV_TYPE_START                0x0C   //启动 (总线模块 )
#define EI_EV_TYPE_LEAKAGE              0x0D   //泄露
#define EI_EV_TYPE_INPUT_ALARM          0x0E   //输入模块火警
#define EI_EV_TYPE_CONTROL_DELAY        0x0F   //控制模块正在延时
#define EI_EV_TYPE_SMOKE_SENSE_FAULT    0x18   //烟传感故障
#define EI_EV_TYPE_TEMPERATURE_FAULT    0x19   //温传感故障
#define EI_EV_TYPE_ADDR_REPEAT_FAULT    0x1A   //地址重复
#define EI_EV_TYPE_24V_OPEN_FAULT       0x1B   //24V断线
#define EI_EV_TYPE_INPUT_OR_OUTPUT_OVERSTEP_FAULT    0x2A    //输入输出口越界
#define EI_EV_TYPE_SIGNAL_OPEN_FAULT    0x2B   //信号线断线
#define EI_EV_TYPE_MONITOR_ALARM        0x30   //监管报警

#define EI_EV_TYPE_MUL_START            0x34   //多线启动


#define EI_EV_TYPE_BUS_LOOP_SHORT_FAULT    0x10  //回路短路故障
#define EI_EV_TYPE_BUS_LOOP_OPEN_FAULT     0x11  //回路开路故障

#define EI_EV_MULTI_PANEL_OUTPUT_OPEN_FAULT    0x13   //直接按键输出断路
#define EI_EV_MULTI_PANEL_OUTPUT_SHORT_FAULT   0x14   //直接按键输出短路
#define EI_EV_24VPOWER_OUTPUT_SHORT_FAULT      0x15   //24V电源输出短路
#define EI_EV_LCD_PANEL_FAULT                  0x1C   //液晶显示盘主板故障
#define EI_EV_LINKAGE_MAIN_PANEL_FAULT         0x1F   //联动键盘主板故障
#define EI_EV_DIRRECT_PANEL_COM_FAULT          0x20   //直接按键板通讯故障
#define EI_EV_LINKAGE_PANEL_COM_FAULT          0x21   //联动键盘板通讯故障
#define EI_EV_MAIN_POWER_FAULT                 0x28   //主电故障
#define EI_EV_BACKUP_POWET_FAULT               0x29   //备电故障

#define EI_EV_AREA_ALARM                       0x33   //区域火警
#define EI_EV_MUL_START_SHORT_FAULT            0x36   //多线启动短路
#define EI_EV_MUL_START_OPEN_FAULT             0x37   //多线启动断路
#define EI_EV_MUL_STOP_SHORT_FAULT             0x38   //多线停止短路
#define EI_EV_MUL_STOP_OPEN_FAULT              0x39   //多线停止断路

#define EI_EV_SG_COM_FAULT                     0x3B   //声光通讯故障
#define EI_EV_BUS_BOARD_FAULT                  0x3C   //总线板故障
#define EI_EV_SG_START                         0x3E   //声光启动
#define EI_EV_SG_SHORT_FAULT                   0x3F   //声光短路
#define EI_EV_SG_OPEN_FAULT                    0x40   //声光断路




UINT16 Change_Event_Type_EI(UINT8 src_event,UINT8 *record_type);
void Save_Component_Info_EI2000(UINT8 *buf);
void Save_Loop_Fault_EI2000(UINT8 *buf);
void Save_Other_Info_EI2000(UINT8 *buf);
void EI_Save_Reset(void);

void Send_Answer_Frame_EI2000(CONTR_IF *buf,UINT8 type);
void Send_Poll_Contr_EI2000(CONTR_IF *buf);
UINT8 Analyse_EI2000(CONTR_IF *buf);
void Handle_EI2000_ZJ(CONTR_IF *buf);
void Com_Task_EI2000_Zj(CONTR_IF *buf);


#endif
