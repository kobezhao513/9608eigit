#ifndef __GT_GK603_H__
#define __GT_GK603_H__


#include "global.h"
#include "controller.h"

//帧分析
//24 24 len(7D) data(121BYTE) cs(校验和)
//帧长度: 为帧的总长度，包括帧头及帧尾校验
//cs               : 对前124字节进行校验，累加和

//#define FRAME_MIN_LEN_EI2000            7
//#define FRAME_MAX_LEN_EI2000            250

#define FRAME_HEAD_GM647               	0x24  //帧头
#define FRAME_HEAD_LEN_GM647           	0x02  //帧头长度
#define FRAME_LEN_OFFSET_GM647         	0x02  //帧长度偏移位置
#define FRAME_LEN_BYTE_NUM_GM647       	0x01        
#define FRAME_LEN_TOTAL_GM647			0x7D  //帧总长度
#define FRAME_CS_LEN_GM647             	0x01  //cs长度

#define FRAME_CS_INIT_VALUE_GM647		0x00//校验初始值

#define FRAME_DEV_TYPE_OFFSET1_GM647		0x48//非盘类设备类型偏移
#define FRAME_DEV_TYPE_OFFSET2_GM647		0x35//盘类设备类型偏移
#define FRAME_EV_OFFSET_GM647				0x54//事件类型偏移
#define FRAME_CONRE_NUM_GM647				0x23//控制器机号偏移
#define FRAME_LOOP_NUM_GM647				0x27//回路号偏移
#define FRAME_LA_NUM_GM647					0x2D//地址号偏移


//事件类型转换
enum GT_EVENT
{
	GT_POWEROFF = 0,				//关机
	GT_POWERON,						//开机
	GT_FIRE,						//火警
	GT_REG_DEV_OFFLINE,				//设备掉线
	GT_REG_DEV_ONLINE,				//设备上线
	GT_CONTR_RESET,					//复位
	GT_MAINPOWER_VOLTAGE_L,			//主电欠压
	GT_MAINPOWER_FAULT,				//主电故障
	GT_MAINPOWER_RECOVER,			//主电恢复
	GT_MAINPOWER_VOLTAGE_RECOVER,	//主电欠压恢复
	GT_BATTERY_VOLTAGE_L,			//备电欠压
	GT_BATTERY_FAULT,				//备电故障
	GT_BATTERY_VOLTAGE_RECOVER,		//备电欠压恢复
	GT_BATTERY_RECOVER,				//备电恢复
	GT_MANUAL_ALLOW,				//手动允许
	GT_MANUAL_FORBID,				//手动禁止
	GT_AUTO_ALLOW,					//自动允许
	GT_AUTO_FORBID,					//自动禁止
	GT_FIRE_RESTORE,				//预警恢复
	GT_MONITOR,						//监管
	GT_MONITOR_CANCLE,				//监管撤销
	GT_START,						//启动
	GT_STOP,						//停动
	GT_FEEDBACK,					//反馈
	GT_FEEDBACK_CANCLE,				//停止反馈
	GT_SHIELD,						//屏蔽
	GT_SHIELD_CANCLE,				//屏蔽释放

};


//设备类型
enum GT_DEVICE
{
	GT_BUTTON = 0,			//手报
	GT_SMOKE,				//感烟
	GT_TEMPERATURE,			//感温
	GT_INOUT,				//输入输出模块
	GT_BUSDISK,				//总线盘
};


typedef struct
{
	UINT8 gt_type;
	UINT8 ts_type;
}GT_Device_Type;


UINT8 GT603_Device_Get(CONTR_IF * buf);

void Com_Task_GT603_SF_ZJ(CONTR_IF *buf);


#endif
