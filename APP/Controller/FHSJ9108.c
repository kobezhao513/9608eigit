 
#include "fhsj9108.h"
#include "controller.h"
#include "xycnbus.h"
#include "basic.h"	
#include "record_manage.h"
#include "record_status.h"
#include "sys_init.h"
#include "led.h"
//事件类型转换表
CHANGE_EVENT_TYPE const FHSJ9108PRT_EVENT_CHANGE_LIST[]=
{
	{FHSJ9108_PRT_FIRE,					RECORD_FIRE,		INT_TYPE_ALARM},//火警
	{FHSJ9108_PRT_FAULT,				RECORD_FAULT,		INT_TYPE_DEV_FAULT},//故障
	{FHSJ9108_PRT_FAULT_ELIMINATE,		RECORD_ELIMINATE,	INT_TYPE_DEV_RECOVER_NOMAL},//故障消除
	{FHSJ9108_PRT_START,				RECORD_START,		INT_TYPE_REQ_START},//请求
	{FHSJ9108_PRT_STOP,					RECORD_STOP,		INT_TYPE_REQ_STOP},//请求消除
};

//设备类型转换表


//以下为特殊程序:不同接口程序存储有区别

void Save_Component_Info_Fhsj9108_Prt(UINT8 *buf,UINT16 ev_type)
{
	UINT8 a,b,c,i,j,k,m,n;
	UINT8 uc_loc;
	UINT8 uc[3];
	RECORD_STRUCT record;

	memset(&record.Sta, 0xFF, sizeof(record));
			
	record.EventType.word = ev_type;
	if(record.EventType.word != INT_TYPE_NULL) 
	{
		record.Type = Get_EventTypeGroup(record.EventType.word);

		uc_loc = FRAME_UC_LOC_FHSJ9108_PRT;

		a = *(buf+uc_loc) - 0x30;
		b = *(buf+uc_loc+1) - 0x30;
		c = *(buf+uc_loc+2) - 0x30;
		uc[0] = a*100 + b*10 + c;     //机号

		m = *(buf+uc_loc+3) - 0x30;
		n = *(buf+uc_loc+4) - 0x30;
		uc[1] = m*10 + n; 				//回路
		 
		i = *(buf+uc_loc+5) - 0x30;
		j = *(buf+uc_loc+6) - 0x30;
		k = *(buf+uc_loc+7) - 0x30;
		uc[2] = i*100 + j*10 + k;		//LA
		
		Hex_To_BasicUC(&record.Uc[1],&uc[0],3);
		MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);

		Record_Controler_If_Event(&record);
	}
}


UINT16 Get_Ev_Type_Fhsj9108_Prt(CONTR_IF *buf)
{
	UINT8 i,j,valid_len,event_num,fhsj_type,event_start_loc,event_stop_loc,flag=0;
	signed result;
	UINT16 ts_event;
	UINT8 ev_fhsj[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

	UINT8 s[5][8]=
		{
			{0xBB,0xF0,0xBE,0xAF,0xFF,0xFF,0xFF,0xFF},//火警
			{0xB9,0xCA,0xD5,0xCF,0xFF,0xFF,0xFF,0xFF},//故障
			{0xB9,0xCA,0xD5,0xCF,0xCF,0xFB,0xB3,0xFD},//故障消除
			{0xC7,0xEB,0xC7,0xF3,0xFF,0xFF,0xFF,0xFF},//请求
			{0xC7,0xEB,0xC7,0xF3,0xCF,0xFB,0xB3,0xFD},//请求消除
		};

	for(i=ROW1_LENGTH_FHSJ9108_PRT;i<256;i++)//找事件大类起始位置
	{
		if((buf->DAT_Return[i] == FRAME_FORM_PRINT1_FHSJ9108_PRT)
			&& (buf->DAT_Return[i+1] == FRAME_FORM_PRINT2_FHSJ9108_PRT))
		{
			flag++;
			if(flag == 2)
			{
				event_start_loc = i+2;
				break;
			}
		}
	}
	for(i=event_start_loc;i<256;i++)//找事件大类结束位置
	{
		if((buf->DAT_Return[i] == FRAME_FORM_PRINT1_FHSJ9108_PRT)
			&& (buf->DAT_Return[i+1] == FRAME_FORM_PRINT3_FHSJ9108_PRT))
		{
			event_stop_loc = i;
			break;
		}
	}
	//计算事件类型有效长度
	valid_len = event_stop_loc - event_start_loc;
	
	memcpy(&ev_fhsj[0],&buf->DAT_Return[event_start_loc],valid_len);

	for(fhsj_type=0;fhsj_type<8;fhsj_type++)
	{
		result = memcmp(&s[fhsj_type][0],&ev_fhsj[0],8);

		if(result == 0) break;//匹配到事件则直接退出匹配
	}

	if(result != 0)//未匹配到事件
	{
		fhsj_type = 0xFF;
	}

	event_num = sizeof(FHSJ9108PRT_EVENT_CHANGE_LIST)/sizeof(CHANGE_EVENT_TYPE);
	for(j=0;j<event_num;j++)
	{
		if(fhsj_type == FHSJ9108PRT_EVENT_CHANGE_LIST[j].SrcEventType)
		{
			ts_event = FHSJ9108PRT_EVENT_CHANGE_LIST[j].DestEventType;
			break;
		}
	}
	if(j >= event_num)
	{
		ts_event = INT_TYPE_NULL;
	}
	return ts_event;
}


UINT8 Analyse_Fhsj9108_Prt(CONTR_IF *buf)
{
	UINT16 i,j,data_loc,fr_end_loc,row1_end_loc;
	UINT8 frame_length;
	UINT8 poll_flag = 0;
	UINT8 count,ope_flag=0;

	UINT8 poll_str[4] = {FRAME_HEAD_POLL_FHSJ9108_PRT,FRAME_DATA1_POLL_FHSJ9108_PRT,FRAME_DATA2_POLL_FHSJ9108_PRT,FRAME_TAIL_POLL_FHSJ9108_PRT};

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
				if((buf->R.Buf[i] == FRAME_HEAD_POLL_FHSJ9108_PRT)
					|| (buf->R.Buf[i] == FRAME_HEAD_DATA_FHSJ9108_PRT))
				{
					buf->R.Clev = i;
					break;
				}
				if(++i == CONTR_BUF_LEN)
				{
					i = 0;
				}
			}
			//找了一圈未找到帧头
			if(i == buf->R.Head)
			{
				buf->R.Clev = buf->R.Head;
				buf->R.FrameEndLoc = buf->R.Head;
				break;
			}

			buf->AnalyseSta = FRAME_LEN;
	

		case FRAME_LEN:
			if(buf->R.Buf[buf->R.Clev] == FRAME_HEAD_POLL_FHSJ9108_PRT)
			{
				data_loc = buf->R.Clev + FRAME_POLL_LENGTH_FHSJ9108_PRT;
				if(data_loc >= CONTR_BUF_LEN)
				{
					data_loc -= CONTR_BUF_LEN;
				}
				//先收4字节
				if(data_loc > buf->R.Clev)
				{
					if((buf->R.Head > buf->R.Clev) && (buf->R.Head < data_loc)) break;
				}
				else
				{
					if((buf->R.Head > buf->R.Clev) || (buf->R.Head < data_loc)) break;
				}

				//判断是否为巡检帧
				j = buf->R.Clev;
				for(count=0;count<FRAME_POLL_LENGTH_FHSJ9108_PRT;count++)
				{
					if(poll_str[count] == buf->R.Buf[j])
					{
						if(++j >= CONTR_BUF_LEN) j = 0;//判断是否转圈
						poll_flag++;

						if(poll_flag == FRAME_POLL_LENGTH_FHSJ9108_PRT)
						{
							buf->R.Clev = data_loc;
							buf->R.FrameEndLoc = data_loc;
							buf->AnalyseSta = FRAME_HEAD;
							ope_flag = POLL_FLAG_FHSJ9108_PRT;
							return ope_flag;
						}
					}
					else
					{
						buf->R.Clev++;
						buf->AnalyseSta = FRAME_HEAD;
						break;
					}
				}
			}
			//不是巡检帧，继续收数
			else if(buf->R.Buf[buf->R.Clev] == FRAME_HEAD_DATA_FHSJ9108_PRT)
			{
				data_loc = buf->R.Clev + ROW1_LENGTH_FHSJ9108_PRT;//第一行结束位置
				if(data_loc >= CONTR_BUF_LEN)
				{
					data_loc -= CONTR_BUF_LEN;
				}
				if(data_loc > buf->R.Clev)
				{
					if((buf->R.Head > buf->R.Clev) && (buf->R.Head < data_loc)) break;
				}
				else
				{
					if((buf->R.Head > buf->R.Clev) || (buf->R.Head < data_loc)) break;
				}

				//判断第一行是否收对
				if(data_loc == 0)
				{
					row1_end_loc = data_loc + CONTR_BUF_LEN - 1;
				}
				else
				{
					row1_end_loc = data_loc - 1;
				}

				if(buf->R.Buf[row1_end_loc] == FRAME_FLAG2_FHSJ9108_PRT)
				{
					buf->R.FrameEndLoc = data_loc;
					buf->AnalyseSta = FRAME_DATA;
				}
				else
				{
					buf->R.Clev++;
					buf->AnalyseSta = FRAME_HEAD;
					break;
				}
			}
			
		case FRAME_DATA://确认一帧接收完成
			fr_end_loc = buf->R.FrameEndLoc;
			
			while(fr_end_loc != buf->R.Head)
			{
				if(buf->R.Buf[fr_end_loc] == FRAME_FLAG2_FHSJ9108_PRT)
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
			//收数完成，计算帧长度
			if(i > buf->R.Clev)
			{
				frame_length = fr_end_loc-buf->R.Clev+1;
			}
			else
			{
				frame_length = fr_end_loc+CONTR_BUF_LEN-buf->R.Clev+1;
			}

			buf->R.FrameEndLoc = fr_end_loc;
			buf->R.FrameLen = frame_length;
			buf->AnalyseSta = FRAME_CS;

		case FRAME_CS:
			Watch_Dog();

			frame_length = buf->R.FrameLen;
			if((buf->R.Buf[buf->R.FrameEndLoc] == FRAME_FLAG2_FHSJ9108_PRT)
				&& (buf->R.Buf[buf->R.Clev] == FRAME_HEAD_DATA_FHSJ9108_PRT))
			{
				j = buf->R.Clev;
				for(count=0;count<frame_length;count++)
				{
					buf->DAT_Return[count] = buf->R.Buf[j];
					if(++j >= CONTR_BUF_LEN) j = 0;//判断是否转圈
				}
					
				buf->AnalyseSta = FRAME_HEAD;
				ope_flag = TRUE;
			}
			else
			{
				buf->R.Clev ++;
				buf->AnalyseSta = FRAME_HEAD;
				break;
			}
			break;

		default:
			buf->AnalyseSta = FRAME_HEAD;
			break;
	}
	return ope_flag;
}



void Handle_Fhsj9108_Prt(CONTR_IF *buf)
{
	UINT16 command_type;//命令类型

	command_type = Get_Ev_Type_Fhsj9108_Prt(buf);

	switch(command_type)
	{
		case INT_TYPE_ALARM://火警
		case INT_TYPE_DEV_FAULT://故障
		case INT_TYPE_DEV_RECOVER_NOMAL://故障恢复
		case INT_TYPE_REQ_START://启动
		case INT_TYPE_REQ_STOP://停止
			Save_Component_Info_Fhsj9108_Prt(&buf->DAT_Return[0],command_type);
			break;
		
		default:
			break;

	}
	
	Contr.ComFault.Count = 0;
	buf->R.FrameEndLoc++;
	if(buf->R.FrameEndLoc >= CONTR_BUF_LEN)
	{
		buf->R.FrameEndLoc -= CONTR_BUF_LEN;
	}
	buf->R.Clev = buf->R.FrameEndLoc;
	buf->R.FrameLen = 0;
}


//===============================================================================
//函数描述: 依爱控制器通讯处理
//输入参数: 无
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================

void Com_Task_Fhsj9108_Prt(CONTR_IF *buf)
{
	if(Analyse_Fhsj9108_Prt(buf) == TRUE)
	{
		Handle_Fhsj9108_Prt(buf);
		On_Led_Contor();
	}
	else if(Analyse_Fhsj9108_Prt(buf) == POLL_FLAG_FHSJ9108_PRT)
	{
		Contr.ComFault.Count = 0;
	}
}

