

#include "LD128EN_PRT.h"
#include "record_status.h"
#include "sys_init.h"
#include "xycnbus.h"
#include "record_manage.h"
#include "led.h"

GET_EVENT_TYPE_ASCII const C_EVENT_CHANGE_LIST_LD128EN_PRT[]=
{
	{"火警",		RECORD_FIRE,		 INT_TYPE_ALARM},
	{"故障",		RECORD_FAULT,		 INT_TYPE_DEV_FAULT},
	{"监管",	    RECORD_MONITER,		 INT_TYPE_MONITOR_ALARM},
	{"屏蔽",		RECORD_SHIELD,	 	 INT_TYPE_REQ_SHIELD},
	{"请求",		RECORD_OTHERS,	 	 NULL},
	{"启动",		RECORD_START,		 INT_TYPE_REQ_START},
	{"反馈",   		RECORD_ANSWER,	 	 INT_TYPE_DEVICE_START_FEED_SUC},
	{"复位",	    RECORD_RESET,		 INT_TYPE_RESET_SUCESS},
	{"开机",	    RECORD_POWERONOFF,	 INT_TYPE_POWER_ON},
};
LD_PRT_Device_Type const C_CTR_UC_CHANGE_LIST_LD128EN_PRT[]=
{
	{"感烟探测器",		0X2A},
	{"感温探测器",		0X1F},
	{"手动报警按钮",	0X17},
	{"可燃气体",		0X0A},
	{"红外",			0X3E},
	{"组联模块",		0XFF},
	{"防爆接口",   		0XFF},
	{"消火栓",	    	0X18},
	{"压力开关",	    0X63},
	{"水流指示器",		0X60},
	{"防火阀",			0XB2},
	{"感温电缆",	    0XFF},
	{"信号阀",			0X61},
	{"接口",			0XFF},
	{"烟温复合",		0X33},
	{"其他",   			0XFF},
	{"剩余电流",	    0X11},
	{"温度",	    	0XFF},
	{"相电流",   		0X14},
	{"电源",			0X1A},
	{"消防泵",			0X5B},
	{"喷淋泵",			0X5F},
	{"气体灭火",		0XB8},
	{"泡沫灭火",		0XB8},
	{"干粉灭火",		0XB8},
	{"卷帘门",   		0X75},
	{"防火门",	    	0X67},
	{"空调新风",	    0X68},
	{"防排烟",			0X71},
	{"电梯",			0X77},
	{"消防广播",	    0X95},
	{"应急照明",		0XB5},
	{"疏散指示",		0X7C},
	{"声光警报",		0X7D},
	{"漏电",   			0XFF},
	{"喷雾泵",	    	0X5F},
	{"排烟口",	    	0X73},
	{"正压送风",   		0X6F},
	{"正压排烟",		0XC5},
	{"补风机",			0X70},
	{"新风机",			0XC2},
	{"排风机",			0XC1},
	{"预作用阀",		0XB7},
	{"末端放气",		0XFF},
	{"排烟风机",   		0X6E},
	{"防火阀",	    	0XAF},
};

UINT8 Change_UC_Type_LE128EN_PRT(CONTR_IF *buf)
{
	UINT8 dest_type = 0xff;
	UINT8 k,eventNum;

	eventNum = sizeof(C_CTR_UC_CHANGE_LIST_LD128EN_PRT)/sizeof(LD_PRT_Device_Type);
	for(k=0; k < eventNum; k++)
	{
		if(strstr((const char*)&buf->DAT_Return[0],(const char*)C_CTR_UC_CHANGE_LIST_LD128EN_PRT[k].src_type) != NULL)
		{
			dest_type = C_CTR_UC_CHANGE_LIST_LD128EN_PRT[k].dest_type;
			break;
		}
	}
	return dest_type;
}



UINT8 Analyse_LE128EN_PRT(CONTR_IF *buf)
{
	UINT16 i,data_loc,head1_loc,end1_loc; 
	UINT8 count,frame_total_len,ope_flag = FALSE;

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
				if(i == 0)
				{
					head1_loc = CONTR_BUF_LEN-1;
				}
				else
				{
					head1_loc = i - 1;
				}
				if((buf->R.Buf[head1_loc] == FRAME_HEAD_LE128EN_PRT) && (buf->R.Buf[i] == FRAME_HEAD2_LE128EN_PRT))//事件应答
				{
					buf->R.Clev = head1_loc; 
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
			buf->AnalyseSta = FRAME_DATA;
			
		case FRAME_DATA: //确认是否收完一帧数据
			Watch_Dog();
			while(i != buf->R.Head)
			{
				if(i == CONTR_BUF_LEN-1)
				{
					end1_loc = CONTR_BUF_LEN-1;
				}
				else
				{
					end1_loc = i - 1;
				}
				if((buf->R.Buf[end1_loc] == FRAME_TAIL_LE128EN_PRT) && (buf->R.Buf[i] == FRAME_TAIL2_LE128EN_PRT))//事件应答
				{
					buf->R.FrameEndLoc = i; 
					break;
				}
				if(++i == CONTR_BUF_LEN) i=0;
			}
			if(i == buf->R.Head )  //找了一圈，未找到帧尾
			{
				break;
			}

			buf->AnalyseSta = FRAME_CS;
			
		case FRAME_CS:
			
			Watch_Dog();
			if(buf->R.FrameEndLoc < buf->R.Clev)
			{
				frame_total_len = CONTR_BUF_LEN + buf->R.FrameEndLoc - buf->R.Clev + 1;
			}
			else
			{
				frame_total_len = buf->R.FrameEndLoc - buf->R.Clev + 1;
			}
			
			data_loc = buf->R.Clev;
			if((frame_total_len > FRAME_MIN_LEN_LE128EN_PRT)&&(frame_total_len < FRAME_MAX_LEN_LE128EN_PRT))
			{
				for(count=0; count<frame_total_len; count++)
				{
					buf->DAT_Return[count] = buf->R.Buf[data_loc];
					if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈
				}
				ope_flag = TRUE;
			}
			buf->AnalyseSta = FRAME_HEAD;
			break;
			
		default:
			buf->AnalyseSta = FRAME_HEAD;
			break;	
	}
	return ope_flag;
}
void Handle_LE128EN_PRT(CONTR_IF *buf)
{
	UINT8 k,eventNum,i,event_loc;
	RECORD_STRUCT record;
	char *p;
	Watch_Dog();
	eventNum = sizeof(C_EVENT_CHANGE_LIST_LD128EN_PRT)/sizeof(GET_EVENT_TYPE_ASCII);
	for(k=0; k < eventNum; k++)
	{
		p = strstr((const char*)&buf->DAT_Return[0],(const char*)C_EVENT_CHANGE_LIST_LD128EN_PRT[k].Event_Annotaiton);
		if( p != NULL)
		{
			record.Type= C_EVENT_CHANGE_LIST_LD128EN_PRT[k].RecordType;
			record.EventType.word= C_EVENT_CHANGE_LIST_LD128EN_PRT[k].EventType;
			break;
		}
	}
	if(k != eventNum)
	{
		if((record.Type == RECORD_RESET)||(record.Type == RECORD_POWERONOFF))
		{
			MemoryCpyInvert(record.Uc,System.ctr_addr,LEN_REG_UC);
		}
		else if(record.Type == RECORD_FAULT)
		{
			if(strstr((const char*)&buf->DAT_Return[0],(const char*)"备电") != NULL)
			{
				MemoryCpyInvert(record.Uc,System.ctr_addr,LEN_REG_UC);
				record.EventType.word = INT_TYPE_BACKUP_POWER_FAU;
			}
			else if(strstr((const char*)&buf->DAT_Return[0],(const char*)"主电") != NULL)
			{
				MemoryCpyInvert(record.Uc,System.ctr_addr,LEN_REG_UC);
				record.EventType.word = INT_TYPE_INPUT_POWER_FAULT;
			}
			else
			{
				event_loc =(UINT8)( p - (char*)buf->DAT_Return);
				for(k=event_loc;k<FRAME_MAX_LEN_LE128EN_PRT;k++)
				{
					if(buf->DAT_Return[k] == 0x3A)//找到:位置 格式:   火警:01088
					{
						break;
					}
				}
				record.Uc[LEN_REG_UC-5] = Change_UC_Type_LE128EN_PRT(buf);
				record.Uc[LEN_REG_UC-4] = ((buf->DAT_Return[k+4]- '0' )<<4) + buf->DAT_Return[k+5]- '0' ;
				record.Uc[LEN_REG_UC-3] = ((buf->DAT_Return[k+2]- '0' )<<4) + buf->DAT_Return[k+3]- '0' ;
				record.Uc[LEN_REG_UC-2] = buf->DAT_Return[k+1]- '0';
				record.Uc[LEN_REG_UC-1] = CTR_NUMBER_L;

				for(k=0;k<FRAME_MAX_LEN_LE128EN_PRT;k++)
				{
					if((buf->DAT_Return[k] == 0x1B)&&(buf->DAT_Return[k+1] == 0x39))//找到:位置 格式:   火警:01088
					{
						for(i=0;i<LEN_REG_CC;i++)
						{
							if(buf->DAT_Return[k+2+i] != 0x0A)
							{
								record.Cc[LEN_REG_CC-i-1] = buf->DAT_Return[k+2+i];
							}
						}
					}
				}
			}
		}
		else
		{
			event_loc =(UINT8)( p - (char*)buf->DAT_Return);
			for(k=event_loc;k<FRAME_MAX_LEN_LE128EN_PRT;k++)
			{
				if(buf->DAT_Return[k] == 0x3A)//找到:位置 格式:   火警:01088
				{
					break;
				}
			}
			record.Uc[LEN_REG_UC-5] = Change_UC_Type_LE128EN_PRT(buf);
			record.Uc[LEN_REG_UC-4] = ((buf->DAT_Return[k+4]- '0' )<<4) + buf->DAT_Return[k+5]- '0' ;
			record.Uc[LEN_REG_UC-3] = ((buf->DAT_Return[k+2]- '0' )<<4) + buf->DAT_Return[k+3]- '0' ;
			record.Uc[LEN_REG_UC-2] = buf->DAT_Return[k+1]- '0';
			record.Uc[LEN_REG_UC-1] = CTR_NUMBER_L;

			for(k=0;k<FRAME_MAX_LEN_LE128EN_PRT;k++)
			{
				if((buf->DAT_Return[k] == 0x1B)&&(buf->DAT_Return[k+1] == 0x39))//找到:位置 格式:   火警:01088
				{
					for(i=0;i<LEN_REG_CC;i++)
					{
						if(buf->DAT_Return[k+2+i] != 0x0A)
						{
							record.Cc[LEN_REG_CC-i-1] = buf->DAT_Return[k+2+i];
						}
					}
				}
			}
		}
		
		Record_Controler_If_Event(&record);
	}
	
	memset(buf->DAT_Return,0x00,sizeof(buf->DAT_Return));
	buf->R.Clev = buf->R.FrameEndLoc;
	buf->R.FrameLen = 0;
}
void Com_Task_LE128EN_PRT(CONTR_IF *buf)
{
	if(Analyse_LE128EN_PRT(buf))
	{
	    Handle_LE128EN_PRT(buf);
		On_Led_Contor();
	}
}

