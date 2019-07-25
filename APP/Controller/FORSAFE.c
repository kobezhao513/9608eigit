 
#include "forsafe.h"
#include "controller.h"
#include "xycnbus.h"
#include "basic.h"	
#include "record_manage.h"
#include "record_status.h"
#include "sys_init.h"
#include "led.h"
//CRC表
const UINT16 crc_table_forsafe[256]=
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};



//控制器事件转换表
CHANGE_EVENT_TYPE const C_EVENT_CHANGE_LIST_FORSAFE_SF[]=
{
	{FORSAFE_EV_DEV_ONLINE,						RECORD_ELIMINATE,		INT_TYPE_REG_DEV_ONLINE},//设备上线
	{FORSAFE_EV_DEV_OFFLINE,					RECORD_FAULT,			INT_TYPE_REG_DEV_OFFLINE},//设备掉线
	{FORSAFE_EV_DEV_TYPE_FAULT,					RECORD_FAULT,			INT_TYPE_DEV_TYPE_ERROR},//设备类型错误
	{FORSAFE_EV_DEV_LA_REPEAT,					RECORD_FAULT,			INT_TYPE_LA_UNLAW_DEV_INF},//设备重码
	{FORSAFE_EV_DEV_START_FAIL,					RECORD_FAULT,			INT_TYPE_START_FAIL},//启动失败
	{FORSAFE_EV_ALARM,							RECORD_FIRE,			INT_TYPE_ALARM},//火警
	{FORSAFE_EV_START_REQUEST,					RECORD_START,			INT_TYPE_REQ_START},//启动请求
	{FORSAFE_EV_START_SUCCEED,					RECORD_OTHERS,			INT_TYPE_START_SUCESS},//启动成功
	{FORSAFE_EV_DEV_OUTPUT_FAULT,				RECORD_FAULT,			INT_TYPE_INPUT1_OFF_FAU},//模块输出故障
	{FORSAFE_EV_DEV_OUTPUT_SHORT,				RECORD_FAULT,			INT_TYPE_INPUT1_SHORT_FAU},//模块输出短路
	{FORSAFE_EV_DEV_MULTI_SHORT,				RECORD_FAULT,			INT_TYPE_OUTPUT1_SHORT_FAU},//多线短路
	{FORSAFE_EV_DEV_MULTI_CIRCUIT,				RECORD_FAULT,			INT_TYPE_OUTPUT1_OFF_FAU},//多线开路
	{FORSAFE_EV_DEV_MULTI_FAULT,				RECORD_FAULT,			INT_TYPE_OUTPUT1_OFF_FAU},//多线故障
	{FORSAFE_EV_DEV_MULTI_FAULT_ELIMINATE,		RECORD_ELIMINATE,		INT_TYPE_DEV_RECOVER_NOMAL},//多线故障恢复
	{FORSAFE_EV_DEV_SELF_CHECK_FAULT,			RECORD_FAULT, 			INT_TYPE_DEV_FAULT},//自诊断故障
	{FORSAFE_EV_DEV_POLLUTION,					RECORD_FAULT,			INT_TYPE_SMOKE_SENSOR_FAU},//污染
	{FORSAFE_EV_COMM_FAULT,						RECORD_FAULT, 			INT_TYPE_REG_DEV_OFFLINE},//通信故障
	{FORSAFE_EV_COMM_FAULT_ELIMINATE,			RECORD_ELIMINATE,		INT_TYPE_REG_DEV_ONLINE},//通信故障恢复
	{FORSAFE_EV_FLASH_FAULT,					RECORD_FAULT,			INT_TYPE_FLASH_RW_FAU},//flash故障
	{FORSAFE_EV_FLASH_FAULT_ELIMINATE,			RECORD_ELIMINATE,		INT_TYPE_DEV_RECOVER_NOMAL},//flash故障恢复
	{FORSAFE_EV_MULTI_START_REQUEST1,			RECORD_START,			INT_TYPE_REQ_START},//多线启动请求
	{FORSAFE_EV_MULTI_STOP_REQUEST_1,			RECORD_STOP,			INT_TYPE_REQ_STOP},//多线停止请求
	{FORSAFE_EV_MULTI_START_SUCCEED1,			RECORD_OTHERS,			INT_TYPE_START_SUCESS},//多线启动成功
	{FORSAFE_EV_MULTI_STOP_SUCCEED1,			RECORD_OTHERS,			INT_TYPE_STOP_SUCESS},//多线停止成功
	{FORSAFE_EV_MULTI_START_SUCCEED2,			RECORD_OTHERS,			INT_TYPE_STARTED},//多线现场启动成功
	{FORSAFE_EV_MULTI_STOP_SUCCEED2,			RECORD_OTHERS,			INT_TYPE_STOPPED},//多线现场停止成功
	{FORSAFE_EV_MULTI_START_FAIL1,				RECORD_OTHERS,			INT_TYPE_START_FAIL},//多线启动失败
	{FORSAFE_EV_MULTI_STOP_FAIL1,				RECORD_OTHERS,			INT_TYPE_STOP_FAIL},//多线停止失败
	{FORSAFE_EV_DEV_SHIELD,						RECORD_OTHERS,			INT_TYPE_SHILED_SUCESS},//屏蔽
	{FORSAFE_EV_DEV_SHIELD_RELEASE,				RECORD_OTHERS,			INT_TYPE_RELEASE_SUCESS},//屏蔽解除
	{FORSAFE_EV_LOOP_SHIELD,					RECORD_OTHERS,			INT_TYPE_SHILEDED},//回路屏蔽
	{FORSAFE_EV_LOOP_SHIELD_RELEASE,			RECORD_OTHERS,			INT_TYPE_RELEASED},//回路屏蔽解除
	{FORSAFE_EV_DEV_MANUAL_START,				RECORD_OTHERS,			INT_TYPE_START_SUCESS},//手动启动
	{FORSAFE_EV_DEV_MANUAL_STOP,				RECORD_OTHERS,			INT_TYPE_STOP_SUCESS},//手动停止
	{FORSAFE_EV_DEV_LINK_START,					RECORD_OTHERS,			INT_TYPE_START_SUCESS},//联动启动
	{FORSAFE_EV_DEV_LINK_DELAY,					RECORD_CANCEL,			INT_TYPE_DELAY_SUC},//联动延时
	{FORSAFE_EV_LOOP_SHORT,						RECORD_FAULT,			INT_TYPE_BUS_SHORT_CIRCUIT},//总线短路
	{FORSAFE_EV_24VLOOP_FAULT,					RECORD_FAULT,			INT_TYPE_24V_OPEN_FAV},//24V总线异常
	{FORSAFE_EV_24VLOOP_CURRENT_FAULT,			RECORD_FAULT,			INT_TYPE_BUS_CURRENT_H},//24V总线漏电流异常
	{FORSAFE_EV_LOOP_VOLTAGE_FAULT,				RECORD_FAULT,			INT_TYPE_BUS_VOLTAGE_L},//总线参考电压异常
	{FORSAFE_EV_5VLOOP_CURRENT_FAULT,			RECORD_FAULT,			INT_TYPE_BUS_CURRENT_H},//5V总线漏电流异常
	{FORSAFE_EV_5VLOOP_VOLTAGE_FAULT,			RECORD_FAULT,			INT_TYPE_BUS_VOLTAGE_L},//5V总线电压异常
	{FORSAFE_EV_0VLOOP_VOLTAGE_FAULT,			RECORD_FAULT,			INT_TYPE_BUS_VOLTAGE_H},//0V总线电压异常
	{FORSAFE_EV_0VLOOP_CURRENT_FAULT,			RECORD_FAULT,			INT_TYPE_BUS_CURRENT_H},//0V总线漏电流异常
	{FORSAFE_EV_LOOP_HIGHVOLTAGE_CIRCUIT,		RECORD_FAULT,			INT_TYPE_DEV_FAULT},//高压中断
	{FORSAFE_EV_LOOP_VOLTAGE_LOW,				RECORD_FAULT,			INT_TYPE_BUS_VOLTAGE_L},//总线电压低
	{FORSAFE_EV_LOOP_VOLTAGE_HIGH,				RECORD_FAULT,			INT_TYPE_BUS_VOLTAGE_H},//总线电压高
	{FORSAFE_EV_LOOP_CURRENT_FAULT,				RECORD_FAULT,			INT_TYPE_BUS_CURRENT_H},//总线电流异常
	{FORSAFE_EV_LOOP_LEAKAGE_CURRENT_FAULT,		RECORD_FAULT,			INT_TYPE_DEV_FAULT},//漏电流异常	
	{FORSAFE_EV_LOOP_GND_FAULT,					RECORD_FAULT,			INT_TYPE_BUS_SHORT_CIRCUIT},//总线负线异常
	{FORSAFE_EV_LOOP_MAIN_FAULT,				RECORD_FAULT,			INT_TYPE_BUS_SHORT_CIRCUIT},//总线正线异常
	{FORSAFE_EV_LOOP_NO_POWER,					RECORD_FAULT,			INT_TYPE_BUS_OPEN_CIRCUIT},//总线不能上电
	{FORSAFE_EV_LOOP_FAULT_ELIMENATE,			RECORD_ELIMINATE,		INT_TYPE_DEV_RECOVER_NOMAL},//总线故障恢复
	{FORSAFE_EV_EARLY_ALARM,					RECORD_EARLYFIRE,		INT_TYPE_PRE_FIRE_ALARM},//预警
	{FORSAFE_EV_DEV_FAULT,						RECORD_FAULT,			INT_TYPE_DEV_FAULT},//设备故障
	{FORSAFE_EV_MULTI_START_REQUEST2,			RECORD_START,			INT_TYPE_REQ_START},//多线启动请求
	{FORSAFE_EV_MULTI_STOP_REQUEST_2,			RECORD_STOP,			INT_TYPE_REQ_STOP},//多线停止请求
	{FORSAFE_EV_MULTI_START_SUCCEED3,			RECORD_OTHERS,			INT_TYPE_START_SUCESS},//多线启动成功
	{FORSAFE_EV_MULTI_START_FAIL2,				RECORD_OTHERS,			INT_TYPE_START_FAIL},//多线启动失败
	{FORSAFE_EV_MULTI_STOP_SUCCEED3,			RECORD_OTHERS,			INT_TYPE_STOP_SUCESS},//多线停止成功
	{FORSAFE_EV_MULTI_STOP_FAIL2,				RECORD_OTHERS,			INT_TYPE_STOP_FAIL},//多线停止失败
};


//以下为特殊程序:不同接口程序存储有区别

//CRC16计算
UINT16 CRC16_Forsafe(UINT16 cs,UINT8 data)
{
	cs = crc_table_forsafe[(cs>>8^data)&0xff]^cs<<8;
	return cs;
}


//存事件记录
void Save_Component_Info_Forsafe_SF(UINT8 *buf)
{
	UINT8 uc[3];
	RECORD_STRUCT record;
		
	
	
	memset(&record.Sta, 0xFF, sizeof(record));
			
	record.EventType.word = Change_Event_Type_Forsafe_SF(*(buf+FRAME_EV_TYPE_LOC_FORSAFE),&record.Type);
	if(record.EventType.word != INT_TYPE_NULL) 
	{
		uc[0] = *(buf+FRAME_EV_CONTR_NUM_LOC_FORSAFE);//机号
		uc[1] = *(buf+FRAME_EV_LOOP_NUM_LOC_FORSAFE);//回路 
		uc[2] = *(buf+FRAME_EV_LA_LOC_FORSAFE);//地址号
		Hex_To_BasicUC(&record.Uc[1],&uc[0],3);
		MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);

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

UINT16 Change_Event_Type_Forsafe_SF(UINT8 src_event,UINT8 *record_type)
{
	UINT16 dest_event = 0xffff;
	UINT16 k,eventNum;

	eventNum = sizeof(C_EVENT_CHANGE_LIST_FORSAFE_SF)/sizeof(CHANGE_EVENT_TYPE);
	for(k=0; k < eventNum; k++)
	{
		if(C_EVENT_CHANGE_LIST_FORSAFE_SF[k].SrcEventType == src_event)
		{
			dest_event = C_EVENT_CHANGE_LIST_FORSAFE_SF[k].DestEventType;
			break;
		}
	}
	if(k == eventNum)
	{ 
		dest_event = INT_TYPE_NULL;
	}
	else
	{
		(*record_type) = C_EVENT_CHANGE_LIST_FORSAFE_SF[k].RecordType;//Get_EventTypeGroup(dest_event);
	}
	return dest_event;
}


//巡检应答组帧
void Send_Answer_Frame_Forsafe_SF(CONTR_IF *buf,UINT8 type)
{
	UINT8 start,i;
	UINT16 cs;
	start = buf->T.Tail;  
	
	buf->T.Buf[start] = FRAME_HEAD_FORSAFE;
	buf->T.Buf[start+1] = System.la;
	buf->T.Buf[start+2] = 0x03;
	buf->T.Buf[start+3] = type;
	buf->T.Buf[start+4] = 0x00;
	buf->T.Buf[start+5] = 0x00;
	
	cs=0;
	for(i=1;i<6;i++)
	{
    	cs = CRC16_Forsafe(cs,buf->T.Buf[start+i]);
	}
    
    buf->T.Buf[start+6] = (UINT8)((cs&0xFF00)>>8);
	buf->T.Buf[start+7] = (UINT8)(cs&0x00FF);
	
	buf->T.Tail += 8;	
}


//帧分析
UINT8 Analyse_Forsafe_SF(CONTR_IF *buf)
{
	UINT16 i,data_loc,len_loc,fr_end_loc,cs_initial_value=0; 
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
				if(buf->R.Buf[i] == FRAME_HEAD_FORSAFE)
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
			//Contr.ComFault.Count = 0;

		case FRAME_LEN:
			//计算帧长度所在位置
			len_loc = buf->R.Clev+FRAME_LEN_OFFSET_FORSAFE;
			if(len_loc >= CONTR_BUF_LEN)
			{
				len_loc -= CONTR_BUF_LEN;
			}
			//确认收到帧长度
			if(len_loc > buf->R.Clev)
			{
				if((buf->R.Head > buf->R.Clev) && (buf->R.Head <= len_loc)) break ;
			}
			else
			{
				if((buf->R.Head > buf->R.Clev) || (buf->R.Head <= len_loc)) break ;
			}

			//提取帧长度//可加入其它限制
			data_len = buf->R.Buf[len_loc];
			if( data_len <= FRAME_DATA_MAX_LEN_FORSAFE)
			{
				//判断该帧是否有转圈
				fr_end_loc = (buf->R.Clev+data_len+FRAME_EXCEPT_DATA_LEN_FORSAFE);
				if(fr_end_loc >= CONTR_BUF_LEN)
				{
					fr_end_loc -= CONTR_BUF_LEN;
				}

				buf->R.FrameLen = data_len;
				buf->R.FrameEndLoc = fr_end_loc;
				buf->AnalyseSta = FRAME_DATA;
			}
			else
			{
				buf->R.Clev++;
				buf->AnalyseSta = FRAME_HEAD;
				break;
			}

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
			cs_initial_value = FRAME_CS_INIT_VALUE_FORSAFE;
			cs_offset_loc = buf->R.FrameLen + FRAME_HEAD_LEN_FORSAFE + FRAME_LA_LEN_FORSAFE + FRAME_LEN_BYTE_LEN_FORSAFE;
			frame_total_len = buf->R.FrameLen+FRAME_EXCEPT_DATA_LEN_FORSAFE;
			data_loc = buf->R.Clev;

			for(count=0; count<frame_total_len; count++)
			{
				buf->DAT_Return[count] = buf->R.Buf[data_loc];
				if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈

				if((count >= FRAME_CS_START_OFFSET_FORSAFE) && (count < cs_offset_loc))
				{
					//计算校验
					cs_initial_value = CRC16_Forsafe(cs_initial_value,buf->DAT_Return[count]);
				}
			}
			if((buf->DAT_Return[cs_offset_loc] == (UINT8)((cs_initial_value&0xFF00)>>8))
				&& (buf->DAT_Return[cs_offset_loc+1] == (UINT8)(cs_initial_value&0x00FF)))
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



//事件处理
void Handle_Forsafe_SF_ZJ(CONTR_IF *buf)
{
	UINT8 command_type;//命令类型
	UINT8 command_loc;//命令位置
	UINT8 ev_source;//事件来源
	UINT8 ev_source_loc;//事件来源位置
	UINT8 ev_type;//事件类型
	UINT8 ev_type_loc;//事件类型位置

	command_loc = FORSAFE_COMMAND_LOC;
	command_type = buf->DAT_Return[command_loc];
	switch(command_type)
	{
		case FORSAFE_COMMAND_POLL:
			Send_Answer_Frame_Forsafe_SF(buf,command_type); 
			break;
		
		case FORSAFE_COMMAND_RESET://该两类命令不响应
		case FORSAFE_COMMAND_TIME:	
			break;

		case FORSAFE_COMMAND_EVENT:

			ev_source_loc = FRAME_EV_SOURCE_LOC_FORSAFE;
			ev_source = buf->DAT_Return[ev_source_loc];
			ev_type_loc = FRAME_EV_TYPE_LOC_FORSAFE;
			ev_type = buf->DAT_Return[ev_type_loc];
			switch(ev_source)
			{
				case FORSAFE_EV_SOURCE_DETECTOR:
				case FORSAFE_EV_SOURCE_MODULE:
				case FORSAFE_EV_SOURCE_CRT:
				case FORSAFE_EV_SOURCE_PANEL:
				case FORSAFE_EV_SOURCE_MULTI:
				case FORSAFE_EV_SOURCE_NET_MODULE:
					Save_Component_Info_Forsafe_SF(&buf->DAT_Return[0]);
					break;

				case FORSAFE_EV_SOURCE_LOOP:
					Save_Component_Info_Forsafe_SF(&buf->DAT_Return[0]);
					break;

				case FORSAFE_EV_SOURCE_SYS_BOARD:
					Save_Component_Info_Forsafe_SF(&buf->DAT_Return[0]);
					break;

				case FORSAFE_EV_SOURCE_MAIN_POWER:
					if(ev_type == FORSAFE_EV_POWER_FAULT)
					{
						Save_Controller_Self_Info(RECORD_FAULT,INT_TYPE_INPUT_POWER_OPEN_CIRCUIT);
					}
					else if(ev_type == FORSAFE_EV_POWER_FAULT_ELIMINATE)
					{
						Save_Controller_Self_Info(RECORD_ELIMINATE,INT_TYPE_MAINPOW_RECOVER);
					}
					break;

				case FORSAFE_EV_SOURCE_BATTERY:
					if(ev_type == FORSAFE_EV_POWER_FAULT)
					{
						Save_Controller_Self_Info(RECORD_FAULT,INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT);
					}
					else if(ev_type == FORSAFE_EV_POWER_FAULT_ELIMINATE)
					{
						Save_Controller_Self_Info(RECORD_ELIMINATE,INT_TYPE_BACKUP_POWER_RECOVER);
					}
					break;

				case FORSAFE_EV_SOURCE_CHARGE:
					//Save_Controller_Self_Info(RECORD_FAULT,INT_TYPE_BACKUP_POWER_SHORT_CIRCUIT);//暂定备电短路，需讨论
					break;

				case FORSAFE_EV_SOURCE_CONTR:
					if(ev_type == FORSAFE_EV_CONTR_POWERON)
					{
						Save_Controller_Self_Info(RECORD_POWERONOFF,INT_TYPE_POWER_ON);
					}
					else if(ev_type == FORSAFE_EV_CONTR_RESET)
					{
						Save_Controller_Self_Info(RECORD_RESET,INT_TYPE_RESET_SUCESS);
					}
					else if(ev_type == FORSAFE_EV_CONTR_AUTO)
					{
						Save_Controller_Self_Info(REOCRD_MODE,INT_TYPE_AUTO_CONTROL_ALLOW);
					}
					else if(ev_type == FORSAFE_EV_CONTR_ALARM)
					{
						Save_Controller_Self_Info(RECORD_FIRE,INT_TYPE_MANUAL_ALARM);//无火警确认事件，暂定手动火警
					}
					else if(ev_type == FORSAFE_EV_CONTR_EARLY_ALARM)
					{
						Save_Controller_Self_Info(RECORD_EARLYFIRE,INT_TYPE_PRE_FIRE_ALARM);
					}
					else if(ev_type == FORSAFE_EV_CONTR_EARLY_RESET)
					{
						Save_Controller_Self_Info(RECORD_RESUME,INT_TYPE_EARLY_ALARM_RESET);
					}
					break;
					

				default:
					break;
			}
			
			break;
		
		default:
			break;

	}
	
	buf->R.Clev = buf->R.FrameEndLoc;
	buf->R.FrameLen = 0;

	Monitor_Controller_Fault_Eliminate();
}


//===============================================================================
//函数描述:赋安控制器通讯处理
//输入参数: 无
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================

void Com_Task_Forsafe_SF_Zj(CONTR_IF *buf)
{
	if(Analyse_Forsafe_SF(buf))
	{
		Handle_Forsafe_SF_ZJ(buf);
		On_Led_Contor();
	}
}

