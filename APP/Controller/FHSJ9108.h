#ifndef __fhsj9108_H__
#define __fhsj9108_H__


#include "global.h"
#include "controller.h"

//帧分析
//巡检帧:	1C 2E 1B 40
//数据帧:	1B 40 xxxx.xx.xx 20 20 20 xx:xx:xx 20 20 20 xxxxxxxx 0A 1C 26 事件0D 打印格式1C 26 事件
//				大类 打印格式xxx 打印格式1C 26 事件打印格式0D 0A

#define FRAME_HEAD_POLL_FHSJ9108_PRT		0x1C//巡检帧帧头
#define FRAME_DATA1_POLL_FHSJ9108_PRT		0x2E//巡检帧数据
#define FRAME_DATA2_POLL_FHSJ9108_PRT		0x1B//巡检帧数据
#define FRAME_TAIL_POLL_FHSJ9108_PRT		0x40//巡检帧帧尾
#define FRAME_POLL_LENGTH_FHSJ9108_PRT		0x04//巡检帧长度
#define FRAME_HEAD_DATA_FHSJ9108_PRT		0x1B//数据帧帧头
#define FRAME_FLAG1_FHSJ9108_PRT			0x3A//打印内容中的冒号
#define FRAME_FLAG2_FHSJ9108_PRT			0x0A//打印中的换行
#define FRAME_FLAG3_FHSJ9108_PRT			0x2E//打印中的点号
#define FRAME_ROW_NUM_FHSJ9108_PRT			0x02//一条消息的行数
#define ROW1_LENGTH_FHSJ9108_PRT			0x23//第二行长度
#define FRAME_FORM_PRINT1_FHSJ9108_PRT		0x1C//确认打印格式
#define FRAME_FORM_PRINT2_FHSJ9108_PRT		0x26//确认打印格式
#define FRAME_FORM_PRINT3_FHSJ9108_PRT		0x2E//打印格式
#define FRAME_UC_LOC_FHSJ9108_PRT			0x1A//UC起始位置

#define POLL_FLAG_FHSJ9108_PRT				0x02//收到巡检帧的标识

//事件类型
enum FHSJ_PRT_EVENT
{
	FHSJ9108_PRT_FIRE = 0,			//火警
	FHSJ9108_PRT_FAULT,				//故障
	FHSJ9108_PRT_FAULT_ELIMINATE,	//故障消除
	FHSJ9108_PRT_START,				//启动
	FHSJ9108_PRT_STOP,				//停止
	
};


//设备类型

typedef struct
{
	UINT8 fhsj_type;
	UINT8 ts_type;
}FHSJ_DEV_TYPE;

void Com_Task_Fhsj9108_Prt(CONTR_IF *buf);



#endif
