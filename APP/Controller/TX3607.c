
//泰和安TX3607配接北京防威FW19000控制器
//串口:28800-8-N-1
#include "TX3607.h"
#include "sys_init.h"
#include "TS.h"
#include "record_status.h"
#include "xycnbus.h"
#include "record_manage.h"
#include "timer.h"
#include "led.h"

GET_EVENT_TYPE_ASCII const C_EVENT_CHANGE_LIST_TX3607[]=
{
	{"BFA",		RECORD_FAULT,		 	INT_TYPE_BACKUP_POWER_FAU},
	{"BFR",		RECORD_ELIMINATE,		INT_TYPE_BACKUP_POWER_RECOVER},
	{"BCF",	  	RECORD_FAULT,		 	INT_TYPE_NET_COMM_FAULT},
	{"BMF",		RECORD_FAULT,	 	 	INT_TYPE_OUTPUT1_OFF_FAU},
	{"DCF",		RECORD_FAULT,	 	 	INT_TYPE_REG_DEV_OFFLINE},
	{"DLL",		REOCRD_MODE,		 	INT_TYPE_AUTO_CONTROL_FORBID},
	{"DDR",   	RECORD_FAULT,	 	 	INT_TYPE_REG_DEV_OFFLINE},
	{"ERR",	  	RECORD_FAULT,		 	NULL},
	{"FIR",	  	RECORD_FIRE,	 		INT_TYPE_ALARM},
	{"FAT",	  	RECORD_FAULT,	 		INT_TYPE_DEV_FAULT},
	{"FCI",		RECORD_SHIELD,		 	INT_TYPE_REQ_SHIELD},
	{"FCF",		RECORD_FAULT,		 	INT_TYPE_DEV_FAULT},
	{"HLM",	  	REOCRD_MODE,		 	INT_TYPE_SET_MANUAL_ENABLE},
	{"ISO",		RECORD_SHIELD,	 	 	INT_TYPE_REQ_SHIELD},
	{"ISR",		RECORD_RELEASE,	 	 	INT_TYPE_REQ_RELEASE},
	{"ILF",		RECORD_FAULT,		 	INT_TYPE_INPUT1_SHORT_FAU},
	{"LBS",   	RECORD_FAULT,	 	 	INT_TYPE_BUS_SHORT_CIRCUIT},
	{"LBL",	  	RECORD_FAULT,		 	INT_TYPE_BUS_CURRENT_H},
	{"LBF",	  	RECORD_FAULT,	 		INT_TYPE_INF_FAU},
	{"LPE",	  	REOCRD_MODE,	 		INT_TYPE_AUTO_CONTROL_ALLOW},
	{"LAL",		REOCRD_MODE,		 	INT_TYPE_AUTO_CONTROL_ALLOW},
	{"MFA",		RECORD_FAULT,		 	INT_TYPE_INPUT_POWER_FAULT},
	{"MFR",	  	RECORD_ELIMINATE,		INT_TYPE_MAINPOW_RECOVER},
	{"NDR",		RECORD_FAULT,	 	 	INT_TYPE_NEW_DEV_ONLINE},
	{"OLF",		RECORD_FAULT,	 	 	INT_TYPE_OUTPUT1_OFF_FAU},
	{"PON",		RECORD_POWERONOFF,		INT_TYPE_POWER_ON},
	{"POF",   	RECORD_POWERONOFF,	 	INT_TYPE_POWER_OFF},
	{"POO",	  	RECORD_FAULT,		 	INT_TYPE_INPUT_POWER_CURRENT_H},
	{"RST",	  	RECORD_RESET,	 		INT_TYPE_RESET_SUCESS},
	{"RET",	  	RECORD_ANSWER,	 		INT_TYPE_DEVICE_START_FEED_SUC},
	{"RTS",		RECORD_CLEAR,		 	INT_TYPE_DEVICE_STOP_FEED_SUC},
	{"RTM",		RECORD_OTHERS,			NULL},
	{"RLF",	  	RECORD_FAULT,		 	INT_TYPE_INPUT1_OFF_FAU},
	{"STA",		RECORD_START,	 	 	INT_TYPE_REQ_START},
	{"STP",		RECORD_STOP,	 	 	INT_TYPE_REQ_STOP},
	{"SIS",		RECORD_SHIELD,		 	INT_TYPE_REQ_SHIELD},
	{"SFA",   	RECORD_FAULT,	 	 	INT_TYPE_DEV_FAULT},
	{"SYF",	  	RECORD_FAULT,		 	INT_TYPE_DEV_FAULT},
	{"STI",	  	RECORD_NETCOMMAND,	 	INT_TYPE_LAN_SET_TIME},
	{"TFA",	  	RECORD_FAULT,	 		INT_TYPE_DEV_FAULT},
	{"WAT",	  	RECORD_FAULT,	 		INT_TYPE_REQUEST_MAINTENANCE},
};


//协议上说 16位CRC占6字节，暂时按照每个字节转换成BCD码然后转换成ASCII码
UINT16 Get_TX3607_CRC(UINT8 *buf)
{
		UINT8 CS0=0,CS1=0;
		UINT16 CS = 0;
		CS0 = (buf[0]-'0')*100+(buf[1]-'0')*10+(buf[2]-'0');
		CS1 = (buf[3]-'0')*100+(buf[4]-'0')*10+(buf[5]-'0');
		CS = (CS1<<8)|CS0;
	return CS;
}



void Save_Component_Info_TX3607(UINT8 *buf)
{
		UINT8 k,eventNum;
		UINT32 time;
		RECORD_STRUCT record;
		memset(&record.Sta, 0xFF, sizeof(record));

		Watch_Dog();
		eventNum = sizeof(C_EVENT_CHANGE_LIST_TX3607)/sizeof(GET_EVENT_TYPE_ASCII);
		for(k=0; k < eventNum; k++)
		{
			if( strstr((const char*)&buf[19],(const char*)C_EVENT_CHANGE_LIST_TX3607[k].Event_Annotaiton) != NULL)
			{
				record.Type = C_EVENT_CHANGE_LIST_TX3607[k].RecordType;
				record.EventType.word = C_EVENT_CHANGE_LIST_TX3607[k].EventType;
				break;
			}
		}
		if(k != eventNum)
		{
			switch(record.EventType.word)
			{
				case INT_TYPE_BACKUP_POWER_FAU:
				case INT_TYPE_BACKUP_POWER_RECOVER:
				case INT_TYPE_NET_COMM_FAULT:
				case INT_TYPE_BUS_OPEN_CIRCUIT:
				case INT_TYPE_AUTO_CONTROL_FORBID:
				case INT_TYPE_SET_MANUAL_ENABLE:
				case INT_TYPE_INPUT1_SHORT_FAU:
				case INT_TYPE_BUS_SHORT_CIRCUIT:
				case INT_TYPE_BUS_CURRENT_H:
				case INT_TYPE_INF_FAU:
				case INT_TYPE_INPUT_POWER_FAULT:
				case INT_TYPE_MAINPOW_RECOVER:
				case INT_TYPE_OUTPUT1_SHORT_FAU:
				case INT_TYPE_POWER_ON:
				case INT_TYPE_POWER_OFF:
				case INT_TYPE_INPUT_POWER_CURRENT_H:
				case INT_TYPE_RESET_SUCESS:
				case INT_TYPE_LAN_SET_TIME:
					MemoryCpyInvert(record.Uc,System.ctr_addr,LEN_REG_UC);
				break;
				default:
					record.Uc[LEN_REG_UC-5] = 0xff; 	//设备类型
					record.Uc[LEN_REG_UC-4] = ((*(buf+17)- '0' )<<4) + (*(buf+18)- '0' );
					record.Uc[LEN_REG_UC-3] = ((*(buf+15)- '0' )<<4) + (*(buf+16)- '0' );
					record.Uc[LEN_REG_UC-2] = ((*(buf+13)- '0' )<<4) + (*(buf+14)- '0' );
					record.Uc[LEN_REG_UC-1] = CTR_NUMBER_L;
				break;
			}
			if(buf[19]=='R')
			{
				if(record.EventType.word == INT_TYPE_DEV_FAULT)
				{
					record.EventType.word = INT_TYPE_DEV_RECOVER_NOMAL;
					record.Type = RECORD_ELIMINATE;
				}
			}
			if((Last_Event_Type !=record.EventType.word)
				&&(memcmp(Last_Uc,record.Uc,LEN_REG_UC)))
			{
				Last_Event_Type =record.EventType.word;
				memcpy(Last_Uc,record.Uc,LEN_REG_UC);
				Last_Event_Time = get_systemtick();
				Record_Controler_If_Event(&record);
			}
			else
			{
				time = labs(get_systemtick()- Last_Event_Time);
				if(time > FRAME_REPEAT_TIME_TX3607)//5S后收到同样事件认为是有效记录
				{
					Last_Event_Time = get_systemtick();
					Record_Controler_If_Event(&record);
				}
			}
		}

}

UINT8 Analyse_TX3607(CONTR_IF *buf)
{
	UINT16 i,data_loc,fr_end_loc,cs_initial_value=0,cs_frame,frame_tail_loc; 
	UINT8 count,frame_total_len,ope_flag=0;

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
				if(buf->R.Buf[i] == FRAME_HEAD_TX3607)//事件应答
				{
					buf->R.Clev = i; 
					break;
				}
				if(++i == CONTR_BUF_LEN) i=0;
			}
			if(i == buf->R.Head )  //找了一圈，未找到帧头    
			{
				buf->R.Clev = buf->R.Head;
        		buf->R.FrameEndLoc = buf->R.Head;
				break;
			}

			buf->AnalyseSta = FRAME_LEN;
			
		case FRAME_LEN:
				//判断该帧是否有转圈
				buf->R.FrameLen = FRAME_LEN_TX3607;
				fr_end_loc = (buf->R.Clev+FRAME_LEN_TX3607);
				if(fr_end_loc >= CONTR_BUF_LEN)
				{
					fr_end_loc -= CONTR_BUF_LEN;
				}
				buf->R.FrameEndLoc = fr_end_loc;
				buf->AnalyseSta = FRAME_DATA;

			
		case FRAME_DATA: //确认是否收完一帧数据
			//使用该帧结束位置判断收完一帧，而不是逐个字节累加
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
			
		case FRAME_CS:   //校验
			
			Watch_Dog();
			if(buf->R.FrameEndLoc>0)
			{
				frame_tail_loc = buf->R.FrameEndLoc-1;
			}
			else
			{
				frame_tail_loc = CONTR_BUF_LEN-2;
			}
			//计算CRC校验
			if(buf->R.Buf[frame_tail_loc] == FRAME_TAIL_TX3607)
			{
				cs_initial_value = FRAME_CS_INIT_VALUE_TX3607;
				//cs_offset_loc = buf->R.FrameLen-FRAME_CS_LEN_TX3607-FRAME_TAIL_LEN_TX3607;
				frame_total_len = buf->R.FrameLen;
				data_loc = buf->R.Clev;
				
				for(count=0; count<frame_total_len; count++)
				{
					buf->DAT_Return[count] = buf->R.Buf[data_loc];
					if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈

					if((count < FRAME_CS_START_OFFSET_TX3607)&&(count > 0))
					{
						cs_initial_value = Caculate_CRC(cs_initial_value,buf->DAT_Return[count]);
					}
				}
				cs_frame = Get_TX3607_CRC(&buf->DAT_Return[FRAME_CS_START_OFFSET_TX3607]);
				
				if(cs_initial_value == cs_frame)
				{				
					ope_flag = TRUE;
				}
				else
				{
					buf->R.Clev++;
					memset(buf->DAT_Return,0x00,sizeof(buf->DAT_Return));
				}
				buf->AnalyseSta = FRAME_HEAD;
		}
		else
		{
			buf->R.Clev++;
			buf->AnalyseSta = FRAME_HEAD;
			buf->R.FrameEndLoc = 0;
		}
		break;
			
		default:
			buf->AnalyseSta = FRAME_HEAD;
			break;	
	}
	return ope_flag;
}

void Handle_TX3607_Jt(CONTR_IF *buf)
{
	
	Save_Component_Info_TX3607(buf->DAT_Return);

	buf->R.Clev = buf->R.FrameEndLoc;
	buf->R.FrameLen = 0;	
}



void Com_Task_TX3607_Jt(CONTR_IF *buf)
{
	if(Analyse_TX3607(buf))
	{
	    Handle_TX3607_Jt(buf);
		On_Led_Contor();
	}
}

