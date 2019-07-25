#ifndef __tanda3004_H__
#define __tanda3004_H__


#include "global.h"
#include "controller.h"

//帧分析
//事件类型3A  20 机号0A 年/月/日20 时: 分
//事件类型3A  二次码20 设备类型0A 年/月/日20 时3A 分

#define FRAME_FLAG1_TX3004_PRT			0x3A//打印内容中的冒号
#define FRAME_FLAG2_TX3004_PRT			0x0A//打印中的换行
#define FRAME_FLAG3_TX3004_PRT			0x2F//打印中的斜杠
#define FRAME_FLAG4_TX3004_PRT			0x20//打印中的空格
#define FRAME_ROW_NUM_TX3004_PRT		0x02//一条消息的行数
#define ROW0_MAX_LENGTH_TX3004_PRT		0x29//第一行最长长度
#define ROW1_LENGTH_TX3004_PRT			0x11//第二行长度


//事件类型
enum TANDA_PRT_EVENT
{
	TANDA_POWEROFF = 0,				//关机
	TANDA_POWERON,					//开机
	TANDA_FIRE,						//火警
	TANDA_START,					//启动
	TANDA_STOP,						//停止
	TANDA_SHIELD,					//屏蔽
	TANDA_RESET,					//复位
	TANDA_MAINPOWER_FAULT,			//主电故障
	TANDA_BATTERY_FAULT,			//备电故障
	TANDA_DEV_OFFLINE,				//设备掉线
	TANDA_MANUAL_FORBID,			//禁止联动
	TANDA_MANUAL_ALLOW,				//手动允许
	TANDA_AUTO_ALLOW,				//自动允许
	TANDA_SHIELD_RELEASE,			//屏蔽解除
	TANDA_OUTPUT_FAULT,				//输出线故障
	TANDA_DEV_ONLINE,				//设备上线
};


//设备类型
enum TANDA_PRT_DEVICE
{
	TANDA_TEMPERATURE,				//感温
	TANDA_SMOKE,					//感烟
	TANDA_BUTTON,					//手报
	TANDA_INOUT,					//输入输出模块
	TANDA_ALARM,					//声光
};

typedef struct
{
	UINT8 tanda_type;
	UINT8 ts_type;
}TANDA_DEV_TYPE;

void Com_Task_Tanda3004_Prt(CONTR_IF *buf);



#endif
