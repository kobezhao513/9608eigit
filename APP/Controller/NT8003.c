 
#include "NT8003.h"
#include "controller.h"
#include "xycnbus.h"
#include "basic.h"	
#include "record_manage.h"
#include "record_status.h"
#include "sys_init.h"
#include "led.h"
//控制器事件转换表
CHANGE_EVENT_TYPE const C_EVENT_CHANGE_LIST_NT8003[]=
{
//部件类报警
	{NT_EV_FIRE,					RECORD_FIRE,				INT_TYPE_FIRE_ALARM},//火警
	{NT_EV_START,					RECORD_START,				INT_TYPE_START_SUCESS},//启动
	{NT_EV_FEEDBACK,				RECORD_ANSWER,				INT_TYPE_DEVICE_START_FEED_SUC},//反馈
	{NT_EV_MONITOR,					RECORD_MONITER,				INT_TYPE_MONITOR_ALARM},//监管
	{NT_EV_COMPONENT_FAULT,			RECORD_FAULT,				INT_TYPE_REG_DEV_OFFLINE},//设备掉线
	{NT_EV_MODULE_FAULT,			RECORD_FAULT,				INT_TYPE_REG_DEV_OFFLINE},//设备掉线
	{NT_EV_MODULE_STOP,				RECORD_STOP,				INT_TYPE_STOP_SUCESS},//停止
	{NT_EV_FEEDBACK_CANCLE,			RECORD_CLEAR,				INT_TYPE_DEVICE_STOP_FEED_SUC},//反馈停止
	{NT_EV_MONITOR_CANCLE,			RECORD_FREE,				INT_TYPE_MONITOR_ALARM_FREE},//监管释放
	{NT_EV_FAULT_ELIMINALE,			RECORD_ELIMINATE,			INT_TYPE_REG_DEV_ONLINE},//设备上线
	
//回路故障
	

//其他故障

};


//控制器设备类型转换表
NT_DEV_TYPE const C_DEV_CHANGE_LIST_NT8003[]=
{
	{NT_DEV_TEMPERATURE,						0x1f},//感温
	{NT_DEV_SMOKE,								0x2a},//感烟
	{NT_DEV_TEMPERATURE_SMOKE,					0x33},//烟温复合
	{NT_DEV_INFRARED,							0x3e},//点型红外
	{NT_DEV_ULTRAVIOLET,						0x3d},//点型紫外
	{NT_DEV_GAS,								0x0b},//可燃气体
	{NT_DEV_INFRARED_LIGHT,						0x2b},//红外光束
	{NT_DEV_LINEAR_TEMPERATURE,					0x22},//线性感温
	{NT_DEV_INPUT,								0x00},//报警输入模块
	{NT_DEV_MANNUAL_BUTTON,						0x17},//手报
	{NT_DEV_SIGNAL_INPUT,						0x55},//信号输入
	{NT_DEV_PULSE_OUTPUT,						0x56},//脉冲输出
	{NT_DEV_VOLTAGE_OUTPUT,						0x56},//电平输出
	{NT_DEV_BROADCAST,							0x94},//广播
	{NT_DEV_ALARM,								0x7d},//声光
	{NT_DEV_HYDRANT_PUMP,						0x5b},//消火栓泵
	{NT_DEV_SPRAY_PUMP,							0x5f},//喷淋泵
	{NT_DEV_STABILIZING_PUMP,					0x64},//稳压泵
	{NT_DEV_WATER_CURTAIN_PUMP,					0xbf},//水幕泵
	{NT_DEV_DELUGE_PUMP,						0xc0},//雨淋泵
	{NT_DEV_FOAM_PUMP,							0x69},//泡沫泵
	{NT_DEV_DRAFT_FAN,							0x6e},//排烟机
	{NT_DEV_FORCED_DRAFT_FAN,					0xc3},//送风机
	{NT_DEV_NEW_FAN,							0xc2},//新风机
	{NT_DEV_AIR_PUMP,							0xbc},//空压机
	{NT_DEV_FIRE_DAMPER,						0xb2},//防火阀
	{NT_DEV_SMOKE_DAMPER,						0xc5},//排烟阀
	{NT_DEV_BLOW_VALVE,							0x00},//送风阀
	{NT_DEV_ELECTROMAGNETIC_VALVE,				0xb6},//电磁阀
	{NT_DEV_SMOKE_CONREOL_VALVE,				0x00},//防排烟阀
	{NT_DEV_WATER_CURTAIN_ELECTROMAGNETIC,		0xb9},//水幕电磁
	{NT_DEV_DELUGE_VALVE,						0xba},//雨淋阀
	{NT_DEV_ELEVATOR,							0x77},//电梯
	{NT_DEV_AIR_CONDITION_UNIT,					0x96},//空调机组
	{NT_DEV_DIESEL_GENERATOR,					0x97},//柴油发电
	{NT_DEV_LIGHT_DSITRIBUTION,					0x98},//照明配电
	{NT_DEV_MOTIVE_ELECTRICITY,					0x99},//动力配电
	{NT_DEV_FLOOR_LIGHT,						0x00},//层号灯
	{NT_DEV_EMERHENCY_LIGHT,					0xb5},//应急照明
	{NT_DEV_EVACUATION_INSTRUCTION,				0x7c},//疏散指示
	{NT_DEV_HARF_FIRE_CURTAIN,					0xb3},//半降卷帘
	{NT_DEV_FIRE_CURTAIN,						0xb4},//全降卷帘
	{NT_DEV_FIRE_DOOR,							0x66},//防火门
	{NT_DEV_MULTILINE_DISK,						0x00},//多线盘
	{NT_DEV_ALARM_OUTPUT,						0x00},//声光输出
	{NT_DEV_BROADCAST_MODULE,					0x00},//广播模块
	{NT_DEV_REGULAR_PULSE_MODULE,				0x57},//常规脉冲
	{NT_DEV_REGULAR_VOLTAGE_MODULE,				0x57},//常规电平
	{NT_DEV_REGULAR_BROADCAST,					0x00},//常规广播
	{NT_DEV_START_BUTTON,						0x00},//启动按钮
	{NT_DEV_STOP_BUTTON,						0x00},//停止按钮
	{NT_DEV_SPRAY_MODULE,						0xa6},//喷洒模块
	{NT_DEV_GAS_FIRE_EXTINGUISHING_CONTROLL,	0x00},//器灭控制盘
	{NT_DEV_BURGLAR_DETECTOR,					0x00},//防盗探测器
	{NT_DEV_FIRE_DISPLAY_PANEL,					0x16},//火灾显示盘
	{NT_DEV_FIRE_HYDRANT_BUTTOON,				0x18},//消火栓按钮
	{NT_DEV_MONITOR_INPUT_MODULE,				0x00},//监管输入模块
	{NT_DEV_PRESSURE_SWITCH,					0x63},//压力开关
	{NT_DEV_WATER_FLOW_INDICATOR,				0x60},//水流指示
	{NT_DEV_OVERHEAD_WATER_TANK,				0x5c},//高位水箱
	{NT_DEV_SIGNAL_VALVE,						0x61},//信号蝶阀
	{NT_DEV_FIRE_POWER,							0x00},//消防电源
	{NT_DEV_CONTR_POWER,						0x1a},//联动电源
};


//设备类型转换
UINT8 NT8003_Change_DeviceType(UINT8 nt_device)
{
	UINT8 i,device_num,ts_type;

	device_num = sizeof(C_DEV_CHANGE_LIST_NT8003)/sizeof(NT_DEV_TYPE);
	for(i=0;i<device_num;i++)
	{
		if(C_DEV_CHANGE_LIST_NT8003[i].nt_dev_type == nt_device)
		{
			ts_type = C_DEV_CHANGE_LIST_NT8003[i].ts_dev_type;
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

void Save_Component_Info_NT8003(UINT8 *buf)
{
	UINT8 uc[3];
	UINT8 dev_type;
	RECORD_STRUCT record;

	memset(&record.Sta, 0xFF, sizeof(record));

	dev_type = NT8003_Change_DeviceType(*(buf+FRAME_DEV_TYPE_OFFSET_NT8003));
	record.EventType.word = Change_Event_Type_NT8003(*(buf+FRAME_EV_OFFSET_NT8003),&record.Type);
	if(record.EventType.word != INT_TYPE_NULL) 
	{
		//record.Type = Get_EventTypeGroup(record.EventType.word);		

		uc[0] = *(buf+FRAME_CONTR_NUM_OFFSET_NT8003);  //机号
		uc[1] = *(buf+FRAME_LOOP_NUM_OFFSET_NT8003);  //回路 
		uc[2] = *(buf+FRAME_LA_OFFSET_NT8003);//地址号
		Hex_To_BasicUC(&record.Uc[1],&uc[0],3);
		MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);
		record.Uc[0] = dev_type;

		Record_Controler_If_Event(&record);
	}
}


void Save_Other_Info_NT8003(UINT8 *buf)  
{
	RECORD_STRUCT record;
		

	memset(&record.Sta, 0xFF, sizeof(record));
	
	if(*(buf+FRAME_EV_OFFSET_NT8003) == NT_EV_OTHER_FAULT)//故障类事件
	{
		if(*(buf+FRAME_EV_OTHER_TYPE_OFFSET_NT8003) == NT_EV_TRANSMIT)//传输设备
		{
			record.EventType.word = INT_TYPE_REG_DEV_OFFLINE;
			memcpy(&(record.Time),(buf+FRAME_EV_TIME_OFFSET_NT8003),6);//传输设备掉线故障时间从帧中提取
			MemoryCpyInvert(&record.Time[0],&record.Time[0],6);
		}
		else if(*(buf+FRAME_EV_OTHER_TYPE_OFFSET_NT8003) == NT_EV_MAINPOWER_VOLTAGE_LOW)//主电欠压
		{
			record.EventType.word = INT_TYPE_INPUT_POWER_VOLTAGE_L;
		}
		else if(*(buf+FRAME_EV_OTHER_TYPE_OFFSET_NT8003) == NT_EV_MAINPOWER)//主电
		{
			record.EventType.word = INT_TYPE_INPUT_POWER_OPEN_CIRCUIT;
		}
		else if(*(buf+FRAME_EV_OTHER_TYPE_OFFSET_NT8003) == NT_EV_BACKUP_POWER)//备电
		{
			record.EventType.word = INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT;
		}
	}
	else if(*(buf+FRAME_EV_OFFSET_NT8003) == NT_EV_OTHER_FAULT_ELIMINALE)//故障恢复类事件
	{
		if(*(buf+FRAME_EV_OTHER_TYPE_OFFSET_NT8003) == NT_EV_TRANSMIT)//传输设备
		{
			record.EventType.word = INT_TYPE_REG_DEV_ONLINE;
		}
		else if(*(buf+FRAME_EV_OTHER_TYPE_OFFSET_NT8003) == NT_EV_MAINPOWER)//主电
		{
			record.EventType.word = INT_TYPE_MAINPOW_RECOVER;
		}
		else if(*(buf+FRAME_EV_OTHER_TYPE_OFFSET_NT8003) == NT_EV_BACKUP_POWER)//备电
		{
			record.EventType.word = INT_TYPE_BACKUP_POWER_RECOVER;
		}
	}
	
	record.Type = Get_EventTypeGroup(record.EventType.word);

	MemoryCpyInvert(record.Uc,System.ctr_addr,LEN_REG_UC);
	if(*(buf+FRAME_EV_OTHER_TYPE_OFFSET_NT8003) == NT_EV_TRANSMIT)
	{
		record.Uc[0] = 0xA4;
	}
	
	Record_Controler_If_Event(&record);
}


//以下为接口程序基本配置函数
//===============================================================================
//修改日期: 2017-07
//函数作用: 快速转换事件
//参数            : src_event控制器传递的事件 ;record_type转换后记录大类;
//返回值      : 为详细事件类型
//修改人员: 刘芳
//===============================================================================

UINT16 Change_Event_Type_NT8003(UINT8 src_event,UINT8 *record_type)
{
	UINT16 dest_event = 0xffff;
	UINT16 k,eventNum;

	eventNum = sizeof(C_EVENT_CHANGE_LIST_NT8003)/sizeof(CHANGE_EVENT_TYPE);
	for(k=0; k < eventNum; k++)
	{
		if(C_EVENT_CHANGE_LIST_NT8003[k].SrcEventType == src_event)
		{
			dest_event = C_EVENT_CHANGE_LIST_NT8003[k].DestEventType;
			break;
		}
	}
	if(k == eventNum)
	{ 
		dest_event = INT_TYPE_NULL;
	}
	else
	{
		(*record_type) = C_EVENT_CHANGE_LIST_NT8003[k].RecordType;//Get_EventTypeGroup(dest_event);
	}
	return dest_event;
}


void Send_Answer_Frame_NT8003(CONTR_IF *buf)
{
	buf->T.Buf[buf->T.Tail] = FRAME_ANSWER_NT8003;
	buf->T.Tail += 1;
}


UINT8 Analyse_NT8003(CONTR_IF *buf)
{
	UINT16 i,data_loc,fr_head2_loc,head_end_loc,fr_len_loc,fr_end_loc,cs_initial_value,xor_initial_value=0; 
	UINT8 count,data_len,frame_total_len,xor_offset_loc,cs_offset_loc,ope_flag=0;

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
				if(buf->R.Buf[i] == FRAME_HEAD1_NT8003)
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
			//两字节帧头，确认收完
			head_end_loc = buf->R.Clev + FRAME_HEAD_LEN_NT8003;
			if(head_end_loc >= CONTR_BUF_LEN)
			{
				head_end_loc -= CONTR_BUF_LEN;
			}

			if(head_end_loc > buf->R.Clev)
			{
				if((buf->R.Head > buf->R.Clev) && (buf->R.Head < head_end_loc)) break;
			}
			else
			{
				if((buf->R.Head > buf->R.Clev) || (buf->R.Head < head_end_loc)) break;
			}

			fr_head2_loc = buf->R.Clev+1;
			if(fr_head2_loc >= CONTR_BUF_LEN)
			{
				fr_head2_loc -= CONTR_BUF_LEN;
			}
			if(buf->R.Buf[fr_head2_loc] == FRAME_HEAD2_NT8003)
			{
				buf->AnalyseSta = FRAME_LEN;
				Contr.ComFault.Count = 0;
			}
			else
			{
				buf->R.Clev++;
				break;
			}
						
			

		case FRAME_LEN:
			//计算帧长度所在位置
			fr_len_loc = buf->R.Clev+FRAME_LEN_OFFSET_NT8003;
			if(fr_len_loc >= CONTR_BUF_LEN)
			{
				fr_len_loc -= CONTR_BUF_LEN;
			}
			//确认收到帧长度
			if(fr_len_loc > buf->R.Clev)
			{
				if((buf->R.Head > buf->R.Clev) && (buf->R.Head <= fr_len_loc)) break;
			}
			else
			{
				if((buf->R.Head > buf->R.Clev) || (buf->R.Head <= fr_len_loc)) break;
			}

			//提取帧长度//可加入其它限制
			data_len = buf->R.Buf[fr_len_loc];
			
			//判断该帧是否有转圈
			fr_end_loc = (buf->R.Clev+data_len+FRAME_EXCEPT_DATA_LEN_NT8003);
			if(fr_end_loc >= CONTR_BUF_LEN)
			{
				fr_end_loc -= CONTR_BUF_LEN;
			}

			frame_total_len = data_len + FRAME_EXCEPT_DATA_LEN_NT8003;
			
			buf->R.FrameLen = frame_total_len;
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
			cs_initial_value = FRAME_CS_INIT_VALUE_NT8003;
			xor_initial_value = FRAME_XOR_INIT_VALUE_NT8003;
			cs_offset_loc = FRAME_CS_OFFSET_NT8003;
			xor_offset_loc = FRAME_XOR_OFFSET_NT8003;
			frame_total_len = buf->R.FrameLen;
			data_loc = buf->R.Clev;

			for(count=0; count<frame_total_len; count++)
			{
				buf->DAT_Return[count] = buf->R.Buf[data_loc];
				if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈

				if((count >= FRAME_CS_XOR_START_OFFSET_NT8003) && (count < frame_total_len))
				{
					cs_initial_value += buf->DAT_Return[count];
					xor_initial_value ^= buf->DAT_Return[count];
				}
			}
			if(((UINT8)cs_initial_value == buf->DAT_Return[cs_offset_loc])
				&& ((UINT8)xor_initial_value == buf->DAT_Return[xor_offset_loc]))
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



void Handle_NT8003_ZJ(CONTR_IF *buf)
{
	UINT8 ev_type;//命令类型

	ev_type = buf->DAT_Return[FRAME_EV_OFFSET_NT8003];
	switch(ev_type)
	{
		case NT_EV_FIRE:
		case NT_EV_START:
		case NT_EV_FEEDBACK:
		case NT_EV_MONITOR:
		case NT_EV_COMPONENT_FAULT:
		case NT_EV_MODULE_FAULT:
		case NT_EV_MODULE_STOP:
		case NT_EV_FEEDBACK_CANCLE:
		case NT_EV_MONITOR_CANCLE:
		case NT_EV_FAULT_ELIMINALE:
			Save_Component_Info_NT8003(&buf->DAT_Return[0]);
			break;

		case NT_EV_RESET:
			Save_Controller_Self_Info(RECORD_RESET,INT_TYPE_RESET_SUCESS);			
			break;
		
		case NT_EV_OTHER_FAULT:
		case NT_EV_OTHER_FAULT_ELIMINALE:
			Save_Other_Info_NT8003(&buf->DAT_Return[0]);
			break;

		case NT_EV_POLL://在外面统一应答
			break;
		
		default:
			break;

	}
	//该帧总长度=数据包中长度+包头+校验+结束符
	buf->R.Clev = buf->R.FrameEndLoc;
	buf->R.FrameLen = 0;

	Monitor_Controller_Fault_Eliminate();
}


//===============================================================================
//函数描述: 依爱控制器通讯处理
//输入参数: 无
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================

void Com_Task_NT8003_ZJ(CONTR_IF *buf)
{
	if(Analyse_NT8003(buf))
	{
		Handle_NT8003_ZJ(buf);
		Send_Answer_Frame_NT8003(buf);
		On_Led_Contor();
	}
}

