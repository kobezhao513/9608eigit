 
#include "sj9102ba.h"
#include "controller.h"
#include "xycnbus.h"
#include "basic.h"	
#include "record_manage.h"
#include "record_status.h"
#include "sys_init.h"
#include "ts.h"
#include "led.h"
extern const UINT8 auchCRCHi[];
extern const UINT8 auchCRCLo[];


//控制器事件转换表
CHANGE_EVENT_TYPE const C_EVENT_CHANGE_LIST_SJ9102BA[]=
{
//部件类报警
	{SJ_EV_FIRE,			RECORD_FIRE,		INT_TYPE_FIRE_ALARM},//火警
	{SJ_EV_DEV_OFFLINE,		RECORD_FAULT,		INT_TYPE_REG_DEV_OFFLINE},//设备掉线
	{SJ_EV_DEV_START,		RECORD_START,		INT_TYPE_START_SUCESS},//设备启动
	{SJ_EV_SHIELD,			RECORD_SHIELD,		INT_TYPE_SHILED_SUCESS},//设备屏蔽
	
//回路故障
	{SJ_EV_LOOP_OPEN,		RECORD_FAULT,		INT_TYPE_BUS_OPEN_CIRCUIT},//总线断路

//其他故障
	
};

SJ_DEV_TYPE const C_DEV_CHANGE_LIST_SJ9102BA[]=
{
	{SJ_DEV_ALARM_INPUT,					0x00},//报警输入
	{SJ_DEV_SMOKE,							0x2a},//点型感烟
	{SJ_DEV_TEMPERATURE,					0x1f},//点型感温
	{SJ_DEV_PARALLEL_DETECTOR,				0x00},//并联探头
	{SJ_DEV_MANUAL_BUTTON,					0x17},//手动按钮
	{SJ_DEV_FIRE_HYDRANT,					0x8c},//消火栓
	{SJ_DEV_LINEAR_TEMPERATURE,				0x22},//线性感温
	{SJ_DEV_FIRE_DETECTOR,					0x3d},//火焰探测器
	{SJ_DEV_LINEAR_LIGHT,					0x2b},//线性光束
	{SJ_DEV_MONITOR_INPUT,					0x00},//监管输入
	{SJ_DEV_PRESSURE_SWITCH,				0x63},//压力开关
	{SJ_DEV_COMBUSTIBLE_GAS,				0x0b},//可燃气体
	{SJ_DEV_WATER_FLOW_INDICATOR,			0x60},//水流指示
	{SJ_DEV_RESIDUAL_CURRENT,				0x11},//剩余电流
	{SJ_DEV_ELECTRIC_TEMPERATURE,			0x12},//电气测温
	{SJ_DEV_SIGNAL_VALVE,					0x61},//信号蝶阀
	{SJ_DEV_FIRE_DAMPER_IN,					0xb2},//防火阀入
	{SJ_DEV_POWER_MODULE,					0x00},//电源模块
	{SJ_DEV_DISPLAY_PANEL,					0x16},//显示盘
	{SJ_DEV_SPRAY_WARNING,					0x7b},//喷洒警告
	{SJ_DEV_BOTTLE_VALVE,					0xc6},//瓶头阀
	{SJ_DEV_START_VALVE,					0x00},//启动阀
	{SJ_DEV_CONTROLL_PANEL,					0x8f},//控制盘
	{SJ_DEV_BLEED_INDICATION,				0x00},//放气指示
	{SJ_DEV_SPRAY_START,					0xa6},//喷洒启动
	{SJ_DEV_SPRAY_FEEFBACK,					0x00},//启动反馈
	{SJ_DEV_VOLTAGE_MEASURE,				0x13},//电压检测
	{SJ_DEV_CURRENT_MEASURE,				0x14},//电流检测
	{SJ_DEV_INOUT,							0x57},//输入输出
	{SJ_DEV_SOUND_LIGHT_ALARM,				0x7d},//声光警报
	{SJ_DEV_BROADCAST_MODULE,				0x00},//广播模块
	{SJ_DEV_ALARM_BELL,						0x00},//消防警铃
	{SJ_DEV_SMOKE_DAMPER,					0xc5},//排烟阀
	{SJ_DEV_FIRE_DAMPER,					0xb2},//防火阀
	{SJ_DEV_POSITIVE_PRESSURE_AIR_VALVE,	0x00},//正压风阀
	{SJ_DEV_ROLLING_DOOR_DOWN,				0xb4},//卷帘全降
	{SJ_DEV_ROLLING_DOOR_HARF,				0xb3},//卷帘半降
	{SJ_DEV_FIRE_DOOR,						0x66},//防火门
	{SJ_DEV_NEW_FAN,						0xc2},//新风机
	{SJ_DEV_SMOKE_EXHAUST_FAN,				0x6e},//排烟风机
	{SJ_DEV_POSITIVE_PRESSURE_AIR_FAN,		0x6f},//正压风机
	{SJ_DEV_LIGHT_INTENSITY_CUTTING,		0x98},//照明强切
	{SJ_DEV_STIMULUS_CUTTING,				0x99},//动力强切
	{SJ_DEV_FIRE_PUMP,						0x5b},//消防泵
	{SJ_DEV_SPRAY_PUMP,						0x5f},//喷淋泵
	{SJ_DEV_ELEVATOR_CRASH,					0x77},//电梯迫降
	{SJ_DEV_AIR_CONDITION_CUTTING,			0x00},//空调切断
	{SJ_DEV_FIRE_CURTAIN,					0xb9},//消防水幕
	{SJ_DEV_DULUGE_VALVE,					0xba},//雨淋阀
	{SJ_DEV_SMOKE_SCREEN,					0x74},//挡烟垂壁
	{SJ_DEV_EMERGENCY_LIGHTING,				0xb5},//应急照明
	{SJ_DEV_ENTRANCE_GUARD,					0xbb},//门禁系统
	{SJ_DEV_FIRE_WATER_MONITOR,				0x00},//消防水炮
	{SJ_DEV_WATER_MIST,						0x00},//细水雾
	{SJ_DEV_ELECTROMAGNETISM_RELEASE,		0x00},//电磁释放
	{SJ_DEV_ELECTRIC_DOOR_CLOSER,			0x6c},//电动闭门
	{SJ_DEV_ELECTROMAGNETIC_DOOR_HOLDER,	0x00},//电磁门吸
	{SJ_DEV_DOORSENSOR,						0x6b},//门磁开关
	{SJ_DEV_MULTILINE_MODULE,				0x00},//多线模块
	{SJ_DEV_FEEDBACK_INPUT,					0x00},//输入反馈
	{SJ_DEV_LOOP,							0xa3},//总线回路
	{SJ_DEV_POWER,							0x1a},//主机电源
};


//设备类型转换
UINT8 SJ9102BA_Change_DeviceType(UINT8 sj_device)
{
	UINT8 i,device_num,ts_type;

	device_num = sizeof(C_DEV_CHANGE_LIST_SJ9102BA)/sizeof(SJ_DEV_TYPE);
	for(i=0;i<device_num;i++)
	{
		if(C_DEV_CHANGE_LIST_SJ9102BA[i].sj_dev_type== sj_device)
		{
			ts_type = C_DEV_CHANGE_LIST_SJ9102BA[i].ts_type;
			break;
		}
	}

	if(i >= device_num)//未知设备
	{
		ts_type = 0x00;
	}

	return ts_type;
}


//以下为特殊程序:不同接口程序存储有区别

void Save_Component_Info_SJ9102BA(UINT8 *buf)
{
	UINT8 uc[3];
	UINT8 dev_type;
	RECORD_STRUCT record;
		
	

	memset(&record.Sta, 0xFF, sizeof(record));

	dev_type = SJ9102BA_Change_DeviceType(*(buf+SJ_DEV_OFFSET));
	record.EventType.word = Change_Event_Type_SJ9102BA(*(buf+SJ_EV_OFFSET),&record.Type);
	if(record.EventType.word != INT_TYPE_NULL) 
	{
		//record.Type = Get_EventTypeGroup(record.EventType.word);		

		uc[0] = *(buf+SJ_CONTR_NUM_OFFSET);     //机号
		uc[1] = *(buf+SJ_LOOP_OFFSET);  //回路 
		uc[2] = *(buf+1+SJ_LA_OFFSET);//地址号
		Hex_To_BasicUC(&record.Uc[1],&uc[0],3);
		MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);
		record.Uc[0] = dev_type;

		Record_Controler_If_Event(&record);
	}			
}

void Save_Loop_Fault_SJ9102BA(UINT8 *buf)  
{
	UINT8 uc[2];
	UINT8 dev_type;
	RECORD_STRUCT record;
	
	memset(&record.Sta, 0xFF, sizeof(record));

	dev_type = SJ9102BA_Change_DeviceType(*(buf+SJ_DEV_OFFSET));
	record.EventType.word = Change_Event_Type_SJ9102BA(*(buf+SJ_EV_OFFSET),&record.Type);	
	if(record.EventType.word != INT_TYPE_NULL)
	{
		record.Type = Get_EventTypeGroup(record.EventType.word);
				
		uc[0] = *(buf+SJ_CONTR_NUM_OFFSET);	//机号 
		uc[1] = *(buf+SJ_LOOP_OFFSET);//回路/板号
		Hex_To_BasicUC(&record.Uc[1],&uc[0],2);
		MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);
		record.Uc[0] = dev_type;
						
		Record_Controler_If_Event(&record);
	}			
}




//以下为接口程序基本配置函数
//===============================================================================
//修改日期: 2017-07
//函数作用: 快速转换事件
//参数            : src_event控制器传递的事件 ;record_type转换后记录大类;
//返回值      : 为详细事件类型
//修改人员: 刘芳
//===============================================================================

UINT16 Change_Event_Type_SJ9102BA(UINT8 src_event,UINT8 *record_type)
{
	UINT16 dest_event = 0xffff;
	UINT16 k,eventNum;

	eventNum = sizeof(C_EVENT_CHANGE_LIST_SJ9102BA)/sizeof(CHANGE_EVENT_TYPE);
	for(k=0; k < eventNum; k++)
	{
		if(C_EVENT_CHANGE_LIST_SJ9102BA[k].SrcEventType == src_event)
		{
			dest_event = C_EVENT_CHANGE_LIST_SJ9102BA[k].DestEventType;
			break;
		}
	}
	if(k == eventNum)
	{ 
		dest_event = INT_TYPE_NULL;
	}
	else
	{
		(*record_type) = C_EVENT_CHANGE_LIST_SJ9102BA[k].RecordType;//Get_EventTypeGroup(dest_event);
	}
	return dest_event;
}




UINT8 Analyse_SJ9102BA_CRT_JT(CONTR_IF *buf)
{
	UINT16 i,data_loc,fr_len_loc,fr_end_loc,cs_initial_value=0; 
	UINT8 count,data_len,frame_total_len,cs_offset_loc,ope_flag=0;

	if(buf->R.Clev == buf->R.Head) return 0;

	if(buf->R.Clev >= CONTR_BUF_LEN)  
	{
		buf->R.Clev=0;
	}
	i = buf->R.Clev;

	switch(buf->AnalyseSta)
	{
		case FRAME_HEAD:
			Watch_Dog();

			while(i != buf->R.Head)
			{
				if(buf->R.Buf[i] == FRAME_HEAD_SJ9102BA)
				{
					buf->R.Clev = i;
					break;
				}
				if(++i == CONTR_BUF_LEN)
				{
					i = 0;
				}
			}
			//找了一圈，未找到帧头
			if(i == buf->R.Head)
			{
				buf->R.Clev = buf->R.Head;
				buf->R.FrameEndLoc = buf->R.Head;
				break;
			}
						
			buf->AnalyseSta = FRAME_LEN;
			Contr.ComFault.Count = 0;

		case FRAME_LEN:
			//计算帧长度所在位置
			fr_len_loc = buf->R.Clev+FRAME_LEN_OFFSET_SJ9102BA;
			if(fr_len_loc >= CONTR_BUF_LEN)
			{
				fr_len_loc -= CONTR_BUF_LEN;
			}
			//确认收到帧长度
			if(fr_len_loc > buf->R.Clev)
			{
				if((buf->R.Head > buf->R.Clev) && (buf->R.Head <= fr_len_loc)) break ;
			}
			else
			{
				if((buf->R.Head > buf->R.Clev) || (buf->R.Head <= fr_len_loc)) break ;
			}

			//提取帧长度//可加入其它限制
			data_len = buf->R.Buf[fr_len_loc];
			
			//判断该帧是否有转圈
			fr_end_loc = (buf->R.Clev+data_len+FRAME_EXCEPT_DATA_LEN_SJ9102BA);
			if(fr_end_loc >= CONTR_BUF_LEN)
			{
				fr_end_loc -= CONTR_BUF_LEN;
			}

			buf->R.FrameLen = data_len;
			buf->R.FrameEndLoc = fr_end_loc;
			buf->AnalyseSta = FRAME_DATA;

		case FRAME_DATA://确认一帧接收完成
			fr_end_loc = buf->R.FrameEndLoc;
			if(fr_end_loc > buf->R.Clev)//没有转圈
			{
				if((buf->R.Head > buf->R.Clev) && (buf->R.Head < fr_end_loc)) break;
			}
			else
			{
				if((buf->R.Head > buf->R.Clev) || (buf->R.Head < fr_end_loc)) break;
			}
			buf->AnalyseSta = FRAME_CS;

		case FRAME_CS:
			Watch_Dog();
			//计算CRC校验
			cs_initial_value = FRAME_CS_INIT_VALUE_SJ9102BA;
			cs_offset_loc = buf->R.FrameLen + FRAME_EXCEPT_DATA_LEN_SJ9102BA - FRAME_CS_LEN_SJ9102BA - FRAME_TAIL_LEN_SJ9102BA;
			frame_total_len = buf->R.FrameLen+FRAME_EXCEPT_DATA_LEN_SJ9102BA;
			data_loc = buf->R.Clev;

			for(count=0; count<frame_total_len; count++)
			{
				buf->DAT_Return[count] = buf->R.Buf[data_loc];
				if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈

				if(count < cs_offset_loc)
				{
					cs_initial_value = Caculate_CRC(cs_initial_value,buf->DAT_Return[count]);
				}
			}
			if((buf->DAT_Return[cs_offset_loc+1] == (UINT8)(cs_initial_value&0x00FF))
				&& (buf->DAT_Return[cs_offset_loc] == (UINT8)((cs_initial_value&0xFF00)>>8))
				&&(buf->DAT_Return[cs_offset_loc+2] == FRAME_TAIL_SJ9102BA))
			{				
				ope_flag = TRUE;
			}
			else
			{
				buf->R.Clev++;
				memset(buf->DAT_Return,0x00,sizeof(buf->DAT_Return));
			}
			buf->AnalyseSta = FRAME_HEAD;
			break;

		default:
			buf->AnalyseSta = FRAME_HEAD;
			break;
	}
	return ope_flag;
}



void Handle_SJ9102BA_CRT(CONTR_IF *buf)
{
	UINT8 command_type;//命令类型

	command_type = buf->DAT_Return[SJ_EV_TYPE_OFFSET];
	switch(command_type)
	{
		case SJ_COMMAND_FIRE:
		case SJ_COMMAND_SHIELD:
		case SJ_COMMAND_START:
			Save_Component_Info_SJ9102BA(&buf->DAT_Return[0]);
			break;
		
		case SJ_COMMAND_FAULT:
			if(buf->DAT_Return[12] == 0x4F)
			{
				Save_Loop_Fault_SJ9102BA(&buf->DAT_Return[0]);
			}
			else
			{
				Save_Component_Info_SJ9102BA(&buf->DAT_Return[0]);
			}
			break;

		case SJ_COMMAND_RESET:
			Save_Controller_Self_Info(RECORD_RESET,INT_TYPE_RESET_SUCESS);			
			break;

		case SJ_COMMAND_POWERON:
			Save_Controller_Self_Info(RECORD_POWERONOFF,INT_TYPE_POWER_ON);
			break;
			
		case SJ_COMMAND_POLL:
			break;
		
		default:
			break;

	}
	//该帧总长度=数据包中长度+包头+校验+结束符
	buf->R.Clev = buf->R.FrameEndLoc;
	buf->R.FrameLen = 0;

	Contr.ComFault.Count = 0;
}


//===============================================================================
//函数描述: 依爱控制器通讯处理
//输入参数: 无
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================

void Com_Task_SJ9102BA_JT(CONTR_IF *buf)
{
	if(Analyse_SJ9102BA_CRT_JT(buf))
	{
		Handle_SJ9102BA_CRT(buf);
		On_Led_Contor();
	}
}

