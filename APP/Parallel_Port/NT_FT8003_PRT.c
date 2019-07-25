

#include "NT_FT8003_PRT.h"
#include "controller.h"
#include "sys_init.h"
#include "xycnbus.h"
#include "record_manage.h"


GET_EVENT_TYPE_ASCII const C_EVENT_CHANGE_LIST_FT8003_PRT[]=
{
	{"火警",		RECORD_FIRE,		 INT_TYPE_ALARM},
	{"故障",		RECORD_FAULT,		 INT_TYPE_DEV_FAULT},
	{"监管",	    RECORD_MONITER,		 NULL},
	{"屏蔽",		RECORD_SHIELD,	 	 INT_TYPE_REQ_SHIELD},
	{"请求",		RECORD_OTHERS,	 	 NULL},
	{"启动",		RECORD_START,		 INT_TYPE_REQ_START},
	{"反馈",   		RECORD_ANSWER,	 	 INT_TYPE_DEVICE_START_FEED_SUC},
	{"复位",	    RECORD_RESET,		 INT_TYPE_RESET_SUCESS},
	{"开机",	    RECORD_POWERONOFF,	 INT_TYPE_POWER_ON},
};
GET_Device_Type_ASCII const C_CTR_UC_CHANGE_LIST_FT8003_PRT[]=
{
	{"差定温探测器",		0X1F},
	{"光电感烟探测器",		0X2A},
	{"光温复合",			0X34},
	{"点型红外",			0X3E},
	{"点型紫外",			0X3D},
	{"可燃气体",			0X0A},
	{"红外光束",			0X2B},
	{"感温电缆",			0XFF},
	{"报警输入模块",		0X55},
	{"手报",				0X17},
	{"信号输入",			0XFF},
	{"脉冲输出模块",		0X56},
	{"电平输出模块",		0X56},
	{"广播",				0X95},
	{"火灾声光警报器",		0X7D},
	{"消火栓泵",			0X5B},
	{"喷淋泵",				0X5F},
	{"稳压泵",				0X64},
	{"水幕泵",				0XBF},
	{"雨淋泵",				0XC0},
	{"泡沫泵",				0X69},
	{"排烟机",				0X6E},
	{"送风机",				0XC3},
	{"新风机",				0XC2},
	{"空压机",				0XBC},
	{"防火阀",				0XAF},
	{"排烟阀",				0XB2},
	{"送风阀",				0XC3},
	{"电磁阀",				0XB6},
	{"防排烟阀",			0X71},
	{"水幕电磁",			0XB9},
	{"电梯",				0X77},
	{"空调机组",			0X96},
	{"柴油发电",			0X97},
	{"照明配电",			0X98},
	{"动力配电",			0X99},
	{"层号灯",				0XCD},
	{"紧急照明",			0XB5},
	{"疏导指示",			0X7C},
	{"卷帘门中",			0XB3},
	{"卷帘门下",			0XB4},
	{"防火门",				0X67},
	{"光警报器",			0X7D},
	{"声警报器",			0X7D},
	{"喷洒模块",			0XA6},
	{"消火栓按钮",			0X18},
	{"监管输入模块",		0X55},
	{"压力开关",			0X63},
	{"水流指示",			0X60},
	{"高位水箱",			0X5C},
	{"信号蝶阀",			0X61},
	{"电源",				0X1A},
};


void Handle_FT8003_PRT(CONTR_IF *buf)
{
	
}

UINT8 Analyse_FT8003_PRT(CONTR_IF *buf)
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
				if((buf->R.Buf[head1_loc] == FRAME_HEAD_FT8003_PRT) && (buf->R.Buf[i] == FRAME_HEAD2_FT8003_PRT))//事件应答
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
				if((buf->R.Buf[end1_loc] == FRAME_TAIL_FT8003_PRT) && (buf->R.Buf[i] == FRAME_TAIL2_FT8003_PRT))//事件应答
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
			if((frame_total_len > FRAME_MIN_LEN_FT8003_PRT)&&(frame_total_len < FRAME_MAX_LEN_FT8003_PRT))
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



void Com_Task_FT8003_PRT(CONTR_IF *buf)
{
	if(Analyse_FT8003_PRT(buf))
	{
	    Handle_FT8003_PRT(buf);
	}
}


