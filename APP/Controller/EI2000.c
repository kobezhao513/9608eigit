 
#include "EI2000.h"
#include "controller.h"
#include "xycnbus.h"
#include "basic.h"	
#include "record_manage.h"
#include "record_status.h"
#include "sys_init.h"
#include "led.h"
//控制器事件转换表
CHANGE_EVENT_TYPE const C_EVENT_CHANGE_LIST_EI2000[]=
{
//部件类报警
	{EI_EV_TYPE_ALARM ,                          RECORD_FIRE,          INT_TYPE_ALARM},
	{EI_EV_TYPE_SMOKE_ALARM ,                    RECORD_FIRE,          INT_TYPE_SMOKE_ALARM},
	{EI_EV_TYPE_TEMPERATURE_ALARM ,              RECORD_FIRE,          INT_TYPE_TEMPERATURE_ALARM},
	{EI_EV_TYPE_THEFT_ALARM ,                    RECORD_FIRE,          INT_TYPE_MONITOR_THEFT_ALARM},
	{EI_EV_TYPE_PRE_ALARM ,                      RECORD_EARLYFIRE,     INT_TYPE_PRE_FIRE_ALARM},
	{EI_EV_TYPE_DETECTOR_FAULT,                  RECORD_FAULT,         INT_TYPE_REG_DEV_OFFLINE},
	{EI_EV_TYPE_AGE_FAULT,                       RECORD_FAULT,         INT_TYPE_PARAMETER_CHANGE},
	{EI_EV_TYPE_SHORT_ISULATE_FAULT,             RECORD_FAULT,         INT_TYPE_BUS_SHORT_CIRCUIT},
	{EI_EV_TYPE_NO_FAULT,                        RECORD_FAULT,         INT_TYPE_DEV_FAULT},
		
	{EI_EV_TYPE_ACTION,                          RECORD_ANSWER,        INT_TYPE_ACTION_FEEDBACK},
	{EI_EV_TYPE_ISOLATOR,                        RECORD_SHIELD,        INT_TYPE_REQ_SHIELD},
	{EI_EV_TYPE_START,                           RECORD_OTHERS,        INT_TYPE_START_SUCESS},
	{EI_EV_TYPE_LEAKAGE,                         RECORD_MONITER,       INT_TYPE_MONITOR_GAS_LEAKAGE_ALARM},

	{EI_EV_TYPE_INPUT_ALARM,                     RECORD_FIRE,          INT_TYPE_MANUAL_ALARM},
	{EI_EV_TYPE_CONTROL_DELAY,                   RECORD_DELAY,         INT_TYPE_MANUAL_DELAY_START},
	{EI_EV_TYPE_SMOKE_SENSE_FAULT,               RECORD_FAULT,         INT_TYPE_SMOKE_SENSOR_FAU},
	{EI_EV_TYPE_TEMPERATURE_FAULT,               RECORD_FAULT,         INT_TYPE_TEMPE_SENSOR_FAU},	
	{EI_EV_TYPE_ADDR_REPEAT_FAULT,               RECORD_FAULT,         INT_TYPE_INF_FAU},
	
	{EI_EV_TYPE_24V_OPEN_FAULT, 	             RECORD_FAULT,         INT_TYPE_24V_OPEN_FAV},
	{EI_EV_TYPE_INPUT_OR_OUTPUT_OVERSTEP_FAULT,  RECORD_FAULT, 	       INT_TYPE_DEV_FAULT},
	{EI_EV_TYPE_SIGNAL_OPEN_FAULT,               RECORD_FAULT,         INT_TYPE_BUS_OPEN_CIRCUIT},
    {EI_EV_TYPE_MONITOR_ALARM,                   RECORD_MONITER,       INT_TYPE_MONITOR_ALARM},
    {EI_EV_TYPE_MUL_START,                       RECORD_START,         INT_TYPE_REQ_START},
//回路故障
	{EI_EV_TYPE_BUS_LOOP_SHORT_FAULT,            RECORD_FAULT,         INT_TYPE_BUS_SHORT_CIRCUIT},
	{EI_EV_TYPE_BUS_LOOP_OPEN_FAULT,             RECORD_FAULT,         INT_TYPE_BUS_OPEN_CIRCUIT},

//其他故障
	{EI_EV_MULTI_PANEL_OUTPUT_OPEN_FAULT,		 RECORD_FAULT,	       INT_TYPE_OUTPUT1_OFF_FAU},
	{EI_EV_MULTI_PANEL_OUTPUT_SHORT_FAULT,		 RECORD_FAULT,		   INT_TYPE_OUTPUT1_SHORT_FAU},
	{EI_EV_24VPOWER_OUTPUT_SHORT_FAULT,		     RECORD_FAULT,		   INT_TYPE_OUTPUT_POWER_SHORT_CIRCUIT},
	{EI_EV_LCD_PANEL_FAULT,				  	     RECORD_FAULT,         INT_TYPE_HOST_KEY_FAU},

	{EI_EV_LINKAGE_MAIN_PANEL_FAULT,  	         RECORD_FAULT, 		   INT_TYPE_HOST_KEY_FAU},
	{EI_EV_DIRRECT_PANEL_COM_FAULT,   	         RECORD_FAULT, 		   INT_TYPE_NODEANS_FAU},
	{EI_EV_LINKAGE_PANEL_COM_FAULT,	  	         RECORD_FAULT, 		   INT_TYPE_NODEANS_FAU},
    {EI_EV_MAIN_POWER_FAULT,		  	         RECORD_FAULT, 		   INT_TYPE_INPUT_POWER_OPEN_CIRCUIT},
	{EI_EV_BACKUP_POWET_FAULT,		  	         RECORD_FAULT, 		   INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT},

	{EI_EV_AREA_ALARM,					         RECORD_FIRE,          INT_TYPE_FIRE_ALARM},
	{EI_EV_MUL_START_SHORT_FAULT,				 RECORD_FAULT, 		   INT_TYPE_OUTPUT1_SHORT_FAU},
	{EI_EV_MUL_START_OPEN_FAULT,				 RECORD_FAULT, 		   INT_TYPE_OUTPUT1_OFF_FAU},
	{EI_EV_MUL_STOP_SHORT_FAULT,				 RECORD_FAULT, 		   INT_TYPE_OUTPUT1_SHORT_FAU},
	{EI_EV_MUL_STOP_OPEN_FAULT,					 RECORD_FAULT, 	       INT_TYPE_OUTPUT1_OFF_FAU},

	{EI_EV_SG_COM_FAULT, 						 RECORD_FAULT, 		   INT_TYPE_NODEANS_FAU},
	{EI_EV_BUS_BOARD_FAULT,					     RECORD_FAULT, 	       INT_TYPE_NODEANS_FAU},
	{EI_EV_SG_START,					         RECORD_OTHERS,        INT_TYPE_START_SUCESS},
	{EI_EV_SG_SHORT_FAULT,					     RECORD_FAULT,         INT_TYPE_OUTPUT1_SHORT_FAU},
	{EI_EV_SG_OPEN_FAULT,					     RECORD_FAULT,         INT_TYPE_OUTPUT1_OFF_FAU},
};


//以下为特殊程序:不同接口程序存储有区别
//===============================================================================
//函数描述: 存储部件信息
//输入参数: buf为输入数据
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Save_Component_Info_EI2000(UINT8 *buf)
{
	UINT8 i,num,slen,off_addr;
	UINT8 uc[3];
	UINT8 event_type;
	RECORD_STRUCT record;
		
	slen = 38;	
	off_addr = 0;
	num = *buf;
	buf++;	
	
	if((num == 0) || (num > 6)) return;
	//数目,回路,地址,报警类型,增加,时,分,32字节数据
	for(i=0; i<num; i++)
	{
		if(*(buf+off_addr+2))//类型非0
		{
			Watch_Dog();

			memset(&record.Sta, 0xFF, sizeof(record));
			event_type = *(buf+2+off_addr);
			record.EventType.word = Change_Event_Type_EI(event_type,&record.Type);
			if(record.EventType.word != INT_TYPE_NULL) 
			{
                if(*(buf+3+off_addr) == 0) //恢复，以下转为恢复，其他暂时不动
                {
					if(event_type == EI_EV_TYPE_DETECTOR_FAULT)  		  record.EventType.word = INT_TYPE_REG_DEV_ONLINE;
					else if(event_type == EI_EV_TYPE_AGE_FAULT)			  record.EventType.word = INT_TYPE_DEV_RECOVER_NOMAL;
					else if(event_type == EI_EV_TYPE_SHORT_ISULATE_FAULT) record.EventType.word = INT_TYPE_BUS_RECOVER;
					else if(event_type == EI_EV_TYPE_NO_FAULT)            record.EventType.word = INT_TYPE_DEV_RECOVER_NOMAL;
					else if(event_type == EI_EV_TYPE_ACTION)	          record.EventType.word = INT_TYPE_DEV_RECOVER_NOMAL;
					else if(event_type == EI_EV_TYPE_ISOLATOR)	    	  record.EventType.word = INT_TYPE_REQ_RELEASE;
					else if(event_type == EI_EV_TYPE_SMOKE_SENSE_FAULT)	  record.EventType.word = INT_TYPE_SENSOR_RECOVER;
					else if(event_type == EI_EV_TYPE_TEMPERATURE_FAULT)   record.EventType.word = INT_TYPE_SENSOR_RECOVER;
					else if(event_type == EI_EV_TYPE_ADDR_REPEAT_FAULT)   record.EventType.word = INT_TYPE_LA_UNLAW_DEV_INF;
					else if(event_type == EI_EV_TYPE_24V_OPEN_FAULT) 	  record.EventType.word = INT_TYPE_OUTPUT_POWER_RECOVER;
					else if(event_type == EI_EV_TYPE_INPUT_OR_OUTPUT_OVERSTEP_FAULT) record.EventType.word = INT_TYPE_DEV_RECOVER_NOMAL;
					else if(event_type == EI_EV_TYPE_SIGNAL_OPEN_FAULT)   record.EventType.word = INT_TYPE_BUS_RECOVER;
				}
				record.Type = Get_EventTypeGroup(record.EventType.word);

				uc[0] = CTR_NUMBER_L;     //机号
				uc[1] = *(buf+off_addr);  //回路 
				uc[2] = *(buf+1+off_addr);//地址号
				Hex_To_BasicUC(&record.Uc[1],&uc[0],3);
				MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);

				Record_Controler_If_Event(&record);
			}			
			off_addr += slen;
		}
	}
}
//===============================================================================
//函数描述: 存储回路故障信息
//输入参数: buf为输入数据
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Save_Loop_Fault_EI2000(UINT8 *buf)  
{
	UINT8 i,num,slen,off_addr;
	UINT8 uc[2];
	UINT8 event_type;
	RECORD_STRUCT record;
	
	slen = 5;
	off_addr = 0;
	num = *buf;
	buf++;
	
	if((num == 0) || (num > 48)) return;
	
	for(i=0; i<num; i++)
	{
		event_type = *(buf+off_addr);
        if((event_type == 0x10) || (event_type == 0x11) 
			||(event_type == 0x90) || (event_type == 0x91))  
        {
			Watch_Dog();
			memset(&record.Sta, 0xFF, sizeof(record));
			record.Type = RECORD_FAULT;

			if(event_type == 0x10)      record.EventType.word = INT_TYPE_BUS_SHORT_CIRCUIT;
            else if(event_type == 0x11) record.EventType.word = INT_TYPE_BUS_OPEN_CIRCUIT;
            else if((event_type == 0x90)|| (event_type == 0x91))
            {
				record.EventType.word = INT_TYPE_BUS_RECOVER; 
				record.Type = RECORD_ELIMINATE;
			}
			
			uc[0] = *(buf+1+off_addr);	//机号 
			uc[1] = *(buf+2+off_addr);//回路/板号
			Hex_To_BasicUC(&record.Uc[1],&uc[0],2);
			MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);
					
			Record_Controler_If_Event(&record);
 			
			off_addr += slen;
		}
	}
}
//===============================================================================
//函数描述: 存储其他信息
//输入参数: buf为输入数据
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Save_Other_Info_EI2000(UINT8 *buf)  
{
	UINT8 i,num,slen,off_addr;
	UINT8 uc[2];
	UINT8 event_type;
	RECORD_STRUCT record;
		
	slen = 5;
	off_addr = 0;
	num = *buf;
	buf++;
	
	if((num == 0) || (num > 48)) return;
	
	for(i=0; i<num; i++)
	{
		if(*(buf+off_addr))//类型非0
		{
			Watch_Dog();

			memset(&record.Sta, 0xFF, sizeof(record));
			event_type = *(buf+off_addr);
			//cord.EventType.word = Change_Event_Type_EI(*(buf+off_addr),&record.Type);
			
			if(event_type & 0x80)//恢复
			{
				if((event_type&0x7F) == EI_EV_MAIN_POWER_FAULT)
					 record.EventType.word = INT_TYPE_MAINPOW_RECOVER;
				else if((event_type&0x7F) == EI_EV_BACKUP_POWET_FAULT) 
                     record.EventType.word = INT_TYPE_BACKUP_POWER_RECOVER; 
				else   
				     record.EventType.word = INT_TYPE_DEV_RECOVER_NOMAL;
			}
			else     record.EventType.word = Change_Event_Type_EI(event_type,&record.Type); 

		    event_type &= 0x7F;
			if(record.EventType.word != INT_TYPE_NULL) 
			{  
				record.Type = Get_EventTypeGroup(record.EventType.word);

				if((*(buf+off_addr+1) == CTR_NUMBER_L) &&(*(buf+off_addr+2) == CTR_NUMBER_H))
				{
					MemoryCpyInvert(record.Uc,System.ctr_addr,LEN_REG_UC);
				}
				else if((event_type == EI_EV_MAIN_POWER_FAULT) 
						|| (event_type == EI_EV_BACKUP_POWET_FAULT))
				{
					MemoryCpyInvert(record.Uc,System.ctr_addr,LEN_REG_UC);
				}
				else
				{
					uc[0] = *(buf+off_addr+1);	//机号 
					uc[1] = *(buf+off_addr+2);//板/键/区
					Hex_To_BasicUC(&record.Uc[1],&uc[0],2);
					MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);
				}
				
				Record_Controler_If_Event(&record);
			}
			off_addr += slen;
		}
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

UINT16 Change_Event_Type_EI(UINT8 src_event,UINT8 *record_type)
{
	UINT16 dest_event = 0xffff;
	UINT16 k,eventNum;

	eventNum = sizeof(C_EVENT_CHANGE_LIST_EI2000)/sizeof(CHANGE_EVENT_TYPE);
	for(k=0; k < eventNum; k++)
	{
		if(C_EVENT_CHANGE_LIST_EI2000[k].SrcEventType == src_event)
		{
			dest_event = C_EVENT_CHANGE_LIST_EI2000[k].DestEventType;
			break;
		}
	}
	if(k == eventNum)
	{ 
		dest_event = INT_TYPE_NULL;
	}
	else
	{
		(*record_type) = C_EVENT_CHANGE_LIST_EI2000[k].RecordType;//Get_EventTypeGroup(dest_event);
	}
	return dest_event;
}

//===============================================================================
//函数描述: 准备应答数据 
//输入参数: buf为控制器接口;type为命令类型
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Send_Answer_Frame_EI2000(CONTR_IF *buf,UINT8 type)
{
	UINT8 start,i,cs;
	start = buf->T.Tail;  
	
	buf->T.Buf[start] = 0x68;
	buf->T.Buf[start+1] = 9;
	buf->T.Buf[start+2] = 9;
	buf->T.Buf[start+3] = 0x68;
	buf->T.Buf[start+4] = CS_NUMBER_L;
	buf->T.Buf[start+5] = CS_NUMBER_H;
	buf->T.Buf[start+6] = buf->DAT_Return[4];//CTR_NUMBER_L;
	buf->T.Buf[start+7] = buf->DAT_Return[5];//CTR_NUMBER_H;
	buf->T.Buf[start+8] = 0;
	buf->T.Buf[start+9] = EI_VERSION;
	buf->T.Buf[start+10] = EI_COMMAND_TYPE_ANSWER;
	buf->T.Buf[start+11] = 1;
	buf->T.Buf[start+12] = type;
	
	cs=0;
	for(i=4;i<13;i++)
	{
    	cs += buf->T.Buf[start+i];
	}
    //13为cs
    buf->T.Buf[start+13] = cs;
	buf->T.Buf[start+14] = 0x16;
	
	buf->T.Tail += 15;	
}
//===============================================================================
//函数描述: 准备发送巡检数据
//输入参数: buf为控制器接口
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Send_Poll_Contr_EI2000(CONTR_IF *buf)
{
	UINT8 start,i,cs;
	start = buf->T.Tail;

	buf->T.Buf[start] = 0x68;
	buf->T.Buf[start+1] = 0x09; 
	buf->T.Buf[start+2] = 0x09;
	buf->T.Buf[start+3] = 0x68;
	buf->T.Buf[start+4] = CS_NUMBER_L;
	buf->T.Buf[start+5] = CS_NUMBER_H;
	buf->T.Buf[start+6] = CTR_NUMBER_L;
	buf->T.Buf[start+7] = CTR_NUMBER_H;
	buf->T.Buf[start+8] = 0;
	buf->T.Buf[start+9] = EI_VERSION;
	buf->T.Buf[start+10] = EI_POLL_CONTR;
	buf->T.Buf[start+11] = 1;
	buf->T.Buf[start+12] = 0; //预留

	cs=0;
	for(i=4;i<13;i++)
	{
    	cs += buf->T.Buf[start+i];
	}
    //13为cs
    buf->T.Buf[start+13] = cs;
	buf->T.Buf[start+14] = 0x16;
	
	buf->T.Tail += 15;	
}
//===============================================================================
//函数描述: 接收数据分析
//输入参数: buf为控制器接口
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
UINT8 Analyse_EI2000(CONTR_IF *buf)
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
				if(buf->R.Buf[i] == FRAME_HEAD_EI2000)
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
			data_loc = buf->R.Clev+FRAME_LEN_OFFSET_EI2000;
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
			if((data_len > FRAME_MIN_LEN_EI2000) 
				&& (data_len < FRAME_MAX_LEN_EI2000))
			{
				//判断该帧是否有转圈
				fr_end_loc = (buf->R.Clev+data_len+FRAME_EXCEPT_DATA_LEN_EI2000);
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
			cs_initial_value = FRAME_CS_INIT_VALUE_EI2000;
			cs_offset_loc = buf->R.FrameLen + FRAME_HEAD_LEN_EI2000 + FRAME_LEN_BYTE_NUM_EI2000;
			frame_total_len = buf->R.FrameLen+FRAME_EXCEPT_DATA_LEN_EI2000;
			data_loc = buf->R.Clev;

			for(count=0; count<frame_total_len; count++)
			{
				buf->DAT_Return[count] = buf->R.Buf[data_loc];
				if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈

				if((count >= FRAME_CS_START_OFFSET_EI2000) && (count < cs_offset_loc))
				{
					cs_initial_value += buf->DAT_Return[count];
				}
			}
			if((UINT8)cs_initial_value == buf->DAT_Return[cs_offset_loc])
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


//===============================================================================
//函数描述: 通讯处理 :主叫方式
//输入参数: buf为控制器接口
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Handle_EI2000_ZJ(CONTR_IF *buf)
{
	UINT8 command_type;//命令类型

	command_type = buf->DAT_Return[10];
	switch(command_type)
	{
		case EI_COMMAND_TYPE_ALARM:
			Save_Component_Info_EI2000(&buf->DAT_Return[11]);
			Send_Answer_Frame_EI2000(buf,command_type); 
		break;
		
		case EI_COMMAND_TYPE_FAULT:
			Save_Loop_Fault_EI2000(&buf->DAT_Return[11]);			
			Send_Answer_Frame_EI2000(buf,command_type);
		break;

		case EI_COMMAND_TYPE_RESET:
			//EI_Save_Reset();			//存储控制器复位
			Save_Controller_Self_Info(RECORD_RESET,INT_TYPE_RESET_SUCESS);
			Send_Answer_Frame_EI2000(buf,command_type);			
		break;

		case EI_COMMAND_TYPE_POLL_RETURN:
		case EI_COMMAND_TYPE_ANSWER:

		break;
		
		case EI_COMMAND_TYPE_OTHER_ALARM:		
			Save_Other_Info_EI2000(&buf->DAT_Return[11]);
			Send_Answer_Frame_EI2000(buf,command_type);			
		break;
		
		default:
			Send_Answer_Frame_EI2000(buf,command_type);
		break;

	}
	//该帧总长度=数据包中长度+包头+校验+结束符
	buf->R.Clev = buf->R.FrameEndLoc;
	buf->R.FrameLen = 0;

	Monitor_Controller_Fault_Eliminate();
	Poll_Controller_Init(buf);
}


//===============================================================================
//函数描述: 通讯任务--主叫模式和依爱控制器通信
//输入参数: buf为控制器接口
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================

void Com_Task_EI2000_Zj(CONTR_IF *buf)
{
	if(Analyse_EI2000(buf))
	{
		Handle_EI2000_ZJ(buf);
		On_Led_Contor();
	}
	
	if(B_Poll_Ctr)
	{
		Send_Poll_Contr_EI2000(buf); 
		B_Poll_Ctr = 0;
	}
}

