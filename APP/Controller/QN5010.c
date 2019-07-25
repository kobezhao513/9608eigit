 
#include "QN5010.h"
#include "controller.h"
#include "xycnbus.h"
#include "sys_init.h"	
#include "record_manage.h"
#include "record_status.h"
#include "basic.h"
#include "led.h"
//UINT16 QN_Len_Loc=0;

CHANGE_EVENT_TYPE const C_EVENT_CHANGE_LIST_QN5010[]=
{
{QN_EV_TYPE_REG_DEV_ONLINE, 		   RECORD_ELIMINATE,	      INT_TYPE_REG_DEV_ONLINE},
{QN_EV_TYPE_REG_DEV_OFFLINE, 		   RECORD_FAULT,	          INT_TYPE_REG_DEV_OFFLINE},
{QN_EV_TYPE_REG_DEV_SHILED,			   RECORD_SHIELD,		      INT_TYPE_REQ_SHIELD},
{QN_EV_TYPE_REG_DEV_SHILED_RELEASE,    RECORD_RELEASE,			  INT_TYPE_REQ_RELEASE},

{QN_EV_TYPE_MAIN_POWER_FAULT,          RECORD_FAULT,			  INT_TYPE_INPUT_POWER_OPEN_CIRCUIT},
{QN_EV_TYPE_MAIN_POWER_RECOVER,        RECORD_ELIMINATE,		  INT_TYPE_MAINPOW_RECOVER},
{QN_EV_TYPE_BACKUP_POWER_FAULT, 	   RECORD_FAULT,			  INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT},
{QN_EV_TYPE_BACKUP_POWER_RECOVER,      RECORD_ELIMINATE,	      INT_TYPE_BACKUP_POWER_RECOVER},
{QN_EV_TYPE_MANNU_START,               RECORD_START,	          INT_TYPE_REQ_START},
{QN_EV_TYPE_MANNU_STOP,			       RECORD_STOP,	              INT_TYPE_REQ_STOP},
{QN_EV_TYPE_AUTO_START,                RECORD_START,	          INT_TYPE_REQ_START},
{QN_EV_TYPE_AUTO_STOP,                 RECORD_STOP,	              INT_TYPE_REQ_STOP},
{QN_EV_TYPE_START_FEEDBACK,            RECORD_ANSWER,             INT_TYPE_DEVICE_START_FEED_SUC},
{QN_EV_TYPE_START_NO_FEEDBACK, 		   RECORD_FAULT, 		      INT_TYPE_DEVICE_START_FEED_FAIL},
{QN_EV_TYPE_FEEDBACK,				   RECORD_ANSWER,			  INT_TYPE_DEVICE_START_FEED_SUC},
//加入设备故障
};





void Save_Component_Info_QN5010(UINT8 *buf)
{
    UINT8 uc[3];
	RECORD_STRUCT record;
	memset(&record.Sta, 0xFF, sizeof(record));
	//机号,回路,地址
	
	record.EventType.word = INT_TYPE_ALARM;		
	record.Type = RECORD_FIRE;		

	uc[0] = *buf;     //机号
	uc[1] = *(buf+1);  //回路 
	uc[2] = *(buf+2);//地址号
	Hex_To_BasicUC(&record.Uc[1],&uc[0],3);
	MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);
	
	Record_Controler_If_Event(&record);	
}

void Save_Other_Info_QN5010(UINT8 *buf)  
{
	UINT8 uc[3];
    UINT8 source_event;
	RECORD_STRUCT record;
	memset(&record.Sta, 0xFF, sizeof(record));
	
	record.Sta = RECORD_STA_INIT; 
	record.Mode = RMODE_CONTR_UC; 

	source_event = *(buf);

	record.EventType.word = Change_Event_Type_QN5010(source_event,&record.Type);	
	if(record.EventType.word != INT_TYPE_NULL)
	{
		//record.Type = Get_EventTypeGroup(record.EventType.word);
		
		if((source_event >= QN_EV_TYPE_MAIN_POWER_FAULT) 
		  && (source_event <= QN_EV_TYPE_BACKUP_POWER_RECOVER))
		{
			MemoryCpyInvert(record.Uc,System.ctr_addr,LEN_REG_UC);
		}
		else
		{
			uc[0] = *(buf+1);  //机号
			uc[1] = *(buf+2);  //回路 
			uc[2] = *(buf+3);  //地址号
			Hex_To_BasicUC(&record.Uc[1],&uc[0],3);
			MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);
		}
			
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

//事件类型转换
UINT16 Change_Event_Type_QN5010(UINT8 src_event,UINT8 *record_type)
{
	UINT16 dest_event = 0xffff;
	UINT16 k,eventNum;

	eventNum = sizeof(C_EVENT_CHANGE_LIST_QN5010)/sizeof(CHANGE_EVENT_TYPE);
	for(k=0; k < eventNum; k++)
	{
		if(C_EVENT_CHANGE_LIST_QN5010[k].SrcEventType == src_event)
		{
			dest_event = C_EVENT_CHANGE_LIST_QN5010[k].DestEventType;
			break;
		}
	}
	if(k == eventNum)
	{ 
		dest_event = INT_TYPE_NULL;
	}
	else
	{
		(*record_type) = C_EVENT_CHANGE_LIST_QN5010[k].RecordType;//Get_EventTypeGroup(dest_event);
	}
	return dest_event;
}



void Send_Poll_Contr_QN5010(CONTR_IF *buf)
{
	UINT8 start;
	start = buf->T.Tail;

	buf->T.Buf[start] = 0xFF;
	buf->T.Buf[start+1] = 0xFF; 
	buf->T.Buf[start+2] = 0xFF;
	buf->T.Buf[start+3] = 0x09;
	buf->T.Buf[start+4] = 0x00;
	buf->T.Buf[start+5] = 0x00;
	buf->T.Buf[start+6] = 0x01;
	buf->T.Buf[start+7] = 0x6E;
	buf->T.Buf[start+8] = 0x01;
	buf->T.Buf[start+9] = 0x0A;
	buf->T.Buf[start+10] = 0x00;
	buf->T.Buf[start+11] = 0x00;
	
	buf->T.Tail += 12;	
}

//帧格式: FF FF FF ctr 00 00 6E 01 len data()  cs0 cs1
UINT8 Analyse_QN5010(CONTR_IF *buf)
{
	UINT16 i,data_loc,fr_end_loc,cs_initial_value=0; 
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
				if(buf->R.Buf[i] == FRAME_HEAD_QN5010)
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
			data_loc = buf->R.Clev+FRAME_LEN_OFFSET_QN5010;
			if(data_loc >= CONTR_BUF_LEN)
			{
				data_loc -= CONTR_BUF_LEN;
			}
			//确认收到帧长度
			if(data_loc > buf->R.Clev)
			{
				if((buf->R.Head > buf->R.Clev) && (buf->R.Head <= data_loc)) break ;
			}
			else
			{
				if((buf->R.Head > buf->R.Clev) || (buf->R.Head <= data_loc)) break ;
			}
			//提取帧长度//可加入其它限制					
			data_len = buf->R.Buf[data_loc];

			data_loc = buf->R.Clev+3;
			if(data_loc >= CONTR_BUF_LEN) data_loc -= CONTR_BUF_LEN;
			if(buf->R.Buf[data_loc] != 0xFF)
			{
				if((data_len > FRAME_MIN_LEN_QN5010) 
					&& (data_len < FRAME_MAX_LEN_QN5010))
				{
					
					//判断该帧是否有转圈
					fr_end_loc = (buf->R.Clev+data_len+FRAME_EXCEPT_DATA_LEN_QN5010);
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
			}
			else
			{
				buf->R.Clev ++;
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
			cs_initial_value = FRAME_CS_INIT_VALUE_QN5010;
			cs_offset_loc = buf->R.FrameLen+FRAME_LEN_OFFSET_QN5010+FRAME_LEN_BYTE_NUM_QN5010;
			frame_total_len = buf->R.FrameLen+FRAME_EXCEPT_DATA_LEN_QN5010;
			data_loc = buf->R.Clev;

			for(count=0; count<frame_total_len; count++)
			{
				buf->DAT_Return[count] = buf->R.Buf[data_loc];
				if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈

				if((count >= FRAME_CS_START_OFFSET_QN5010) && (count < cs_offset_loc))
				{
					cs_initial_value += buf->DAT_Return[count];
				}
			}
			if((buf->DAT_Return[cs_offset_loc+1] == (UINT8)(cs_initial_value&0x00ff))
				&&(buf->DAT_Return[cs_offset_loc] == (UINT8)(cs_initial_value>>8)))
			{
				ope_flag = 0x55;
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



void Handle_QN5010_Zj(CONTR_IF *buf)
{
	UINT8 command_type;//命令类型

	Watch_Dog();

	command_type = buf->DAT_Return[3];
	if(buf->R.FrameLen >= 0x0A) 
	{
		if(command_type == QN_COMMAND_TYPE_ALARM_EV)
		{
			Save_Component_Info_QN5010(&buf->DAT_Return[10]);
		}
		else if((command_type == QN_COMMAND_TYPE_REG_DEV_OTHER_EV)//故障,故障恢复,屏蔽,解除屏蔽
			   || (command_type == QN_COMMAND_TYPE_START_OR_STOP))//启停
		{
			Save_Other_Info_QN5010(&buf->DAT_Return[9]); 
		}
		else
		{
			Save_Other_Info_QN5010(&buf->DAT_Return[9]); 
		}		
	}
	else
	{
		if((command_type == QN_COMMAND_TYPE_RESET)
		  &&(buf->R.FrameLen == 0x01))
		{
			//QN_Save_Reset();
			Save_Controller_Self_Info(RECORD_RESET,INT_TYPE_RESET_SUCESS);
		}
		else if((command_type == QN_COMMAND_TYPE_REG_DEV_OTHER_EV)
		  &&(buf->R.FrameLen == 0x07) && (buf->DAT_Return[9] == 0x04))
		{
			//QN_Save_PowerOn();
			Save_Controller_Self_Info(RECORD_POWERONOFF,INT_TYPE_POWER_ON);
		}
		else if(command_type == QN_COMMAND_TYPE_POLL_RETURN)
		{
			Poll_Controller_Init(buf);

		}
		else
		{
			Poll_Controller_Init(buf);

		}
	}
	
	buf->R.Clev = buf->R.FrameEndLoc;
	buf->R.FrameLen = 0;

	Monitor_Controller_Fault_Eliminate();
	Poll_Controller_Init(buf);
}


//===============================================================================
//函数描述: 青鸟控制器通讯处理
//输入参数: 无
//输出参数: 无
//修改日期: 2016-10-29
//修改人员: 刘芳
//===============================================================================

void Com_Task_QN5010_Zj(CONTR_IF *buf)
{
	if(Analyse_QN5010(buf))
	{
		Handle_QN5010_Zj(buf);
		On_Led_Contor();
	}
	
	if(B_Poll_Ctr)
	{
		Send_Poll_Contr_QN5010(buf); 
		B_Poll_Ctr = 0;
	}
}

void Com_Task_QN5010_Jt(CONTR_IF *buf)
{
	if(Analyse_QN5010(buf))
	{
		Handle_QN5010_Zj(buf);
		On_Led_Contor();
	}
}


