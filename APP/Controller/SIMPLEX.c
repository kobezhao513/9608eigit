 
#include "simplex.h"
#include "controller.h"
#include "xycnbus.h"
#include "basic.h"	
#include "record_manage.h"
#include "record_status.h"
#include "sys_init.h"
#include "led.h"

//控制器事件转换表
CHANGE_EVENT_TYPE const C_EVENT_CHANGE_LIST_SIMPLEX[]=
{
	{SIMPLEX_EV_FIRE,					RECORD_FIRE,				INT_TYPE_FIRE_ALARM},//火警
	{SIMPLEX_EV_EARLY_ALARM,			RECORD_EARLYFIRE,			INT_TYPE_PRE_FIRE_ALARM},//预警
	{SIMPLEX_EV_EARLY_ALARM_RESET,		RECORD_RESUME,				INT_TYPE_EARLY_ALARM_RESET},//预警恢复
	{SIMPLEX_EV_FAULT,					RECORD_FAULT,				INT_TYPE_DEV_FAULT},//故障
	{SIMPLEX_EV_FAULT_ELIMINAATE,		RECORD_ELIMINATE,			INT_TYPE_DEV_RECOVER_NOMAL},//故障恢复
	{SIMPLEX_EV_MONITOR,				RECORD_MONITER,				INT_TYPE_MONITOR_ALARM},//监管
	{SIMPLEX_EV_MONITOR_RELEASE,		RECORD_FREE,				INT_TYPE_MONITOR_ALARM_FREE},//监管释放
};


//以下为特殊程序:不同接口程序存储有区别

//CRC16计算
UINT16 CRC12_Simplex(UINT16 cs,CONTR_IF *buf,UINT8 cs_loc)
{
	UINT8 i;
	UINT16 cs1,cs2;

	cs1 = 0;
	cs2 = 0;
	for(i=FRAME_CS_START_OFFSET_SIMPLEX;i<cs_loc;i++)
	{
		cs += buf->DAT_Return[i];
	}
	cs = ~cs;
	cs1 = ((cs&0x0FC0)>>6) + 0x40;//取高6位
	cs2 = (cs&0x003F) + 0x40;//取低6位
	cs = (cs1<<8) + cs2;
	return cs;
	
}


//存事件记录
void Save_Component_Info_Simplex(CONTR_IF *buf,UINT8 ev_simplex)
{
	UINT8 uc[3];
	RECORD_STRUCT record;
	
	memset(&record.Sta, 0xFF, sizeof(record));
			
	record.EventType.word = Change_Event_Type_Simplex(ev_simplex,&record.Type);
	if(record.EventType.word != INT_TYPE_NULL) 
	{
		uc[0] = buf->DAT_Return[buf->R.FrameLen - 11];//卡号
		uc[1] = buf->DAT_Return[buf->R.FrameLen - 9];//点号
		uc[2] = buf->DAT_Return[buf->R.FrameLen - 7];//子节点号
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

UINT16 Change_Event_Type_Simplex(UINT8 src_event,UINT8 *record_type)
{
	UINT16 dest_event = 0xffff;
	UINT16 k,eventNum;

	eventNum = sizeof(C_EVENT_CHANGE_LIST_SIMPLEX)/sizeof(CHANGE_EVENT_TYPE);
	for(k=0; k < eventNum; k++)
	{
		if(C_EVENT_CHANGE_LIST_SIMPLEX[k].SrcEventType == src_event)
		{
			dest_event = C_EVENT_CHANGE_LIST_SIMPLEX[k].DestEventType;
			break;
		}
	}
	if(k == eventNum)
	{ 
		dest_event = INT_TYPE_NULL;
	}
	else
	{
		(*record_type) = C_EVENT_CHANGE_LIST_SIMPLEX[k].RecordType;//Get_EventTypeGroup(dest_event);
	}
	return dest_event;
}


//巡检应答组帧
void Send_Answer_Frame_Simplex(CONTR_IF *buf)
{
	UINT8 start;
	UINT16 cs;
	start = buf->T.Tail;  
	
	buf->T.Buf[start] = FRAME_HEAD_SIMPLEX;
	buf->T.Buf[start+1] = buf->DAT_Return[FRAME_SEQ_LOC_SIMPLEX];
	buf->T.Buf[start+2] = FRAME_COMMAND_ANSWER_SIMPLEX;
	buf->T.Buf[start+3] = FRAME_TAIL_SIMPLEX;
	
	cs=0;
    cs = CRC12_Simplex(cs,buf,4);
    
    buf->T.Buf[start+4] = (UINT8)((cs&0xFF00)>>8);
	buf->T.Buf[start+5] = (UINT8)(cs&0x00FF);
	
	buf->T.Tail += 6;	
}

void Send_ACK_Simplex(CONTR_IF *buf)
{
	UINT8 start;

	start = buf->T.Tail;
	
	buf->T.Buf[start] = FRAME_COMMAND_ACK_SIMPLEX;
	buf->T.Tail += 1;
}


//帧分析
UINT8 Analyse_Simplex(CONTR_IF *buf)
{
	UINT16 i,data_loc,fr_end_loc,cs_initial_value=0;
	UINT8 count,fr_len,frame_total_len,cs_offset_loc,ope_flag=0;

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
				if(buf->R.Buf[i] == FRAME_HEAD_SIMPLEX)
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
			fr_end_loc = buf->R.Clev;
			
			while(fr_end_loc != buf->R.Head)
			{
				if(buf->R.Buf[fr_end_loc] == FRAME_TAIL_SIMPLEX)
				{
					break;
				}
				if(++fr_end_loc == CONTR_BUF_LEN)
				{
					fr_end_loc = 0;
				}
			}
			//如果没有找到标志，退出等待接收
			if(fr_end_loc == buf->R.Head) break;
			//数据部分接收完成，计算帧总长
			if(i > buf->R.Clev)
			{
				fr_len = fr_end_loc-buf->R.Clev+1;
			}
			else
			{
				fr_len = fr_end_loc+CONTR_BUF_LEN-buf->R.Clev+1;
			}
			frame_total_len = fr_len + FRAME_CS_LEN_SIMPLEX;
			fr_end_loc = buf->R.Clev + frame_total_len + 1;
			if(fr_end_loc >= CONTR_BUF_LEN)
			{
				fr_end_loc -= CONTR_BUF_LEN;
			}

			buf->R.FrameLen = fr_len;
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
			cs_initial_value = FRAME_CS_INIT_VALUE_SIMPLEX;
			cs_offset_loc = buf->R.FrameLen;
			frame_total_len = buf->R.FrameLen+FRAME_CS_LEN_SIMPLEX;
			data_loc = buf->R.Clev;

			for(count=0; count<frame_total_len; count++)
			{
				buf->DAT_Return[count] = buf->R.Buf[data_loc];
				if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈	
			}
			//计算校验
			cs_initial_value = CRC12_Simplex(cs_initial_value,buf,cs_offset_loc);
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
void Handle_Simplex_Zj(CONTR_IF *buf)
{
	UINT8 command_type;//命令类型
	UINT8 command_loc;//命令位置
	UINT8 ack_flag_loc;//应答标志位置
	UINT8 ev_simplex;

	command_loc = FRAME_COMMAND_LOC_SIMPLEX;
	command_type = buf->DAT_Return[command_loc];
	switch(command_type)
	{
		case FRAME_COMMAND_POLL_SIMPLEX:
			if(Contr.delay == 0)
			{
				Send_ACK_Simplex(buf);
				Contr.delay = 25;//ACK与巡检应答帧做200ms延时
			}
			//此处做巡检应答
			if(Contr.delay > 0) return;//先发ACK,再发巡检应答，中间延时200ms
			Send_Answer_Frame_Simplex(buf);
			break;
		
		case FRAME_COMMAND_START_SIMPLEX:
			if(buf->DAT_Return[FRAME_COMMAND_LOC_SIMPLEX+1] == 0x43)//开机，C的ASKII值，暂定
			{
				Save_Controller_Self_Info(RECORD_POWERONOFF,INT_TYPE_POWER_ON);
			}
			else if(buf->DAT_Return[FRAME_COMMAND_LOC_SIMPLEX+1] == 0x57)//复位，W的ASKII值，暂定
			{
				Save_Controller_Self_Info(RECORD_RESET,INT_TYPE_RESET_SUCESS);
			}
			Send_ACK_Simplex(buf);
			break;

		case FRAME_COMMAND_EVENT_SIMPLEX:
			ack_flag_loc = buf->R.FrameLen - 3;
			if(buf->DAT_Return[ack_flag_loc] == FRAME_NEED_ACK_SIMPLEX)
			{
				//组合出事件类型
				ev_simplex = buf->DAT_Return[ack_flag_loc-2] + ((buf->DAT_Return[ack_flag_loc-1])<<4);
				
				switch(ev_simplex)
				{
					case SIMPLEX_EV_FIRE:
					case SIMPLEX_EV_EARLY_ALARM:
					case SIMPLEX_EV_EARLY_ALARM_RESET:
					case SIMPLEX_EV_FAULT:
					case SIMPLEX_EV_FAULT_ELIMINAATE:
					case SIMPLEX_EV_MONITOR:
					case SIMPLEX_EV_MONITOR_RELEASE:
						Save_Component_Info_Simplex(buf,ev_simplex);
						break;

					default:
						break;
				}
				Send_ACK_Simplex(buf);
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

void Com_Task_Simplex_Zj(CONTR_IF *buf)
{
	if(Analyse_Simplex(buf))
	{
		Handle_Simplex_Zj(buf);
		On_Led_Contor();
	}
}

