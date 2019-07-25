 
#include "QN_193k.h"
#include "controller.h"
#include "xycnbus.h"
#include "sys_init.h"	
#include "record_manage.h"
#include "record_status.h"
#include "basic.h"
#include "led.h"
//UINT16 QN_Len_Loc=0;

CHANGE_EVENT_TYPE const C_EVENT_CHANGE_LIST_QN193K[]=
{
	{QN193K_EV_CONTR_FAULT,					RECORD_FAULT, 			INT_TYPE_DEV_FAULT},//控制器故障
	{QN193K_EV_CONTR_RESET,					RECORD_RESET,			INT_TYPE_RESET_SUCESS},//控制器复位
	//{QN193K_EV_CONTR_SILENCE,				},//控制器消音
	{QN193K_EV_FIRE,						RECORD_FIRE,			INT_TYPE_ALARM},//火警
	{QN193K_EV_FIRE1,						RECORD_FIRE,			INT_TYPE_ALARM},//火警
	{QN193K_EV_FAULT,						RECORD_FAULT,			INT_TYPE_REG_DEV_OFFLINE},//故障
	{QN193K_EV_FAULT_ELIMINATE,				RECORD_ELIMINATE,		INT_TYPE_REG_DEV_ONLINE},//故障恢复
	{QN193K_EV_LOOP_FAULT,					RECORD_FAULT,         	INT_TYPE_BUS_OPEN_CIRCUIT},//回路故障
	{QN193K_EV_LOOP_FAULT_ELIMINATE,		RECORD_ELIMINATE,		INT_TYPE_DEV_RECOVER_NOMAL},//回路故障恢复
	{QN193K_EV_START_AUTO,					RECORD_OTHERS,			INT_TYPE_START_SUCESS},//自动启动
	{QN193K_EV_STOP_AUTO,					RECORD_OTHERS,			INT_TYPE_STOP_SUCESS},//自动停止
	{QN193K_EV_START_MANUAL,				RECORD_OTHERS,			INT_TYPE_START_SUCESS},//手动启动
	{QN193K_EV_STOP_MANUAL,					RECORD_OTHERS,			INT_TYPE_STOP_SUCESS},//手动停止
	{QN193K_EV_DEV_ANSWER,					RECORD_ANSWER,			INT_TYPE_DEVICE_START_FEED_SUC},//设备应答
	{QN193K_EV_DEV_ANSWER_STOP,				RECORD_CLEAR,			INT_TYPE_DEVICE_STOP_FEED_SUC},//设备应答撤销
	{QN193K_EV_SHIELD,						RECORD_OTHERS,			INT_TYPE_SHILED_SUCESS},//屏蔽
	{QN193K_EV_SHIELD_RELEASE,				RECORD_OTHERS,			INT_TYPE_RELEASE_SUCESS},//取消屏蔽
	{QN193K_EV_MULTI_START_MANUAL,			RECORD_OTHERS,			INT_TYPE_START_SUCESS},//多线手动启动
	{QN193K_EV_MULTI_STOP_MANUAL,			RECORD_OTHERS,			INT_TYPE_STOP_SUCESS},//多线手动停止
	{QN193K_EV_MULTI_ANSWER,				RECORD_ANSWER,			INT_TYPE_DEVICE_START_FEED_SUC},//多线应答
	{QN193K_EV_MULTI_ANSWER_STOP,			RECORD_CLEAR,			INT_TYPE_DEVICE_STOP_FEED_SUC},//多线应答停止
	{QN193K_EV_MULTI_START_AUTO,			RECORD_OTHERS,			INT_TYPE_START_SUCESS},//多线自动启动
	{QN193K_EV_MULTI_STOP_AUTO,				RECORD_OTHERS,			INT_TYPE_STOP_SUCESS},//多线自动停止
	{QN193K_EV_MULTI_DELAY_START_AUTO,		RECORD_OTHERS,			INT_TYPE_START_SUCESS},//多线自动启动延时
	{QN193K_EV_MULTI_DELAY_STOP_AUTO,		RECORD_FAULT,			INT_TYPE_DEVICE_START_FEED_FAIL},//多线应答缺失
	{QN193K_EV_MULTI_FAULT,					RECORD_FAULT,			INT_TYPE_REG_DEV_OFFLINE},//多线线路故障
	{QN193K_EV_MULTI_FAULT_ELIMINATE,		RECORD_ELIMINATE,		INT_TYPE_REG_DEV_ONLINE},//多线线路故障恢复
	//{QN193K_EV_SIMULATED_FIRE,				},//模拟火警
	{QN193K_EV_MONITOR,						RECORD_MONITER,			INT_TYPE_MONITOR_ALARM},//监管
	{QN193K_EV_MONITOR_RELEASE,				RECORD_FREE,			INT_TYPE_MONITOR_ALARM_FREE},//监管取消
};


void Save_Component_Info_QN193k(UINT8 *buf,UINT8 source_event)  
{
	UINT8 uc[3];
	UINT8 contr_num,contr_num0,contr_num1,loop,loop0,loop1,la,la0,la1;
	RECORD_STRUCT record;
	memset(&record.Sta, 0xFF, sizeof(record));
	
	record.Sta = RECORD_STA_INIT; 
	record.Mode = RMODE_CONTR_UC; 

	record.EventType.word = Change_Event_Type_QN193k(source_event,&record.Type);

	contr_num0 = (*(buf+FRAME_CONTR_NUM_OFFSET_QN193k)) & 0x0f;//机号
	contr_num1 = (*(buf+FRAME_CONTR_NUM_OFFSET_QN193k+1)) & 0x0f;
	contr_num = (contr_num0<<4) + contr_num1;

	loop0 = (*(buf+FRAME_LOOP_NUM_OFFSET_QN193k)) & 0x0f;//回路号
	loop1 = (*(buf+FRAME_LOOP_NUM_OFFSET_QN193k+1)) & 0x0f;
	loop = (loop0<<4) + loop1;

	la0 = (*(buf+FRAME_LA_OFFSET_QN193k)) & 0x0f;//la
	la1 = (*(buf+FRAME_LA_OFFSET_QN193k+1)) & 0x0f;
	la = (la0<<4) + la1;
	
	if(record.EventType.word != INT_TYPE_NULL)
	{
		//record.Type = Get_EventTypeGroup(record.EventType.word);
		
		uc[0] = contr_num;  //机号
		uc[1] = loop;  //回路 
		uc[2] = la;  //地址号
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

//事件类型转换
UINT16 Change_Event_Type_QN193k(UINT8 src_event,UINT8 *record_type)
{
	UINT16 dest_event = 0xffff;
	UINT16 k,eventNum;

	eventNum = sizeof(C_EVENT_CHANGE_LIST_QN193K)/sizeof(CHANGE_EVENT_TYPE);
	for(k=0; k < eventNum; k++)
	{
		if(C_EVENT_CHANGE_LIST_QN193K[k].SrcEventType == src_event)
		{
			dest_event = C_EVENT_CHANGE_LIST_QN193K[k].DestEventType;
			break;
		}
	}
	if(k == eventNum)
	{ 
		dest_event = INT_TYPE_NULL;
	}
	else
	{
		(*record_type) = C_EVENT_CHANGE_LIST_QN193K[k].RecordType;//Get_EventTypeGroup(dest_event);
	}
	return dest_event;
}


//巡检帧: 82 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 83
void Send_Poll_Contr_QN193k(CONTR_IF *buf)
{
	UINT8 start;
	start = buf->T.Tail;

	buf->T.Buf[start] = 0x82;
	buf->T.Buf[start+1] = 0x30; 
	buf->T.Buf[start+2] = 0x30;
	buf->T.Buf[start+3] = 0x30;
	buf->T.Buf[start+4] = 0x30;
	buf->T.Buf[start+5] = 0x30;
	buf->T.Buf[start+6] = 0x30;
	buf->T.Buf[start+7] = 0x30;
	buf->T.Buf[start+8] = 0x30;
	buf->T.Buf[start+9] = 0x30;
	buf->T.Buf[start+10] = 0x30;
	buf->T.Buf[start+11] = 0x30;
	buf->T.Buf[start+12] = 0x30;
	buf->T.Buf[start+13] = 0x30;
	buf->T.Buf[start+14] = 0x30;
	buf->T.Buf[start+15] = 0x30;
	buf->T.Buf[start+16] = 0x30;
	buf->T.Buf[start+17] = 0x30;
	buf->T.Buf[start+18] = 0x30;
	buf->T.Buf[start+19] = 0x30;
	buf->T.Buf[start+20] = 0x30;
	//buf->T.Buf[start+21] = 0x30;
	//buf->T.Buf[start+22] = 0x30;
	//buf->T.Buf[start+23] = 0x30;
	//buf->T.Buf[start+24] = 0x30;
	buf->T.Buf[start+21] = 0x83;
	
	buf->T.Tail += 22;	
}

UINT8 Analyse_QN193k(CONTR_IF *buf)
{
	UINT16 i,data_loc,fr_end_loc,cs_initial_value=0; 
	UINT8 count,data,data0,data1,cs,cs0,cs1,cs_count,frame_total_len,cs_offset_loc,ope_flag=0;

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
				if(buf->R.Buf[i] == FRAME_HEAD_QN193K)
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

		case FRAME_LEN:
			//计算帧结尾的位置
			frame_total_len = FRAME_CSLEN_QN193K;
			fr_end_loc = buf->R.Clev+frame_total_len-1;
			if(fr_end_loc >= CONTR_BUF_LEN)
			{
				fr_end_loc -= CONTR_BUF_LEN;
			}
			buf->R.FrameEndLoc = fr_end_loc;
			buf->AnalyseSta = FRAME_DATA;
			

		case FRAME_DATA://确认一帧接收完成
			fr_end_loc = buf->R.FrameEndLoc;
			if(fr_end_loc > buf->R.Clev)//没有转圈
			{
				if((buf->R.Head > buf->R.Clev) && (buf->R.Head <= fr_end_loc)) break;
			}
			else
			{
				if((buf->R.Head > buf->R.Clev) || (buf->R.Head <= fr_end_loc)) break;
			}

			if(buf->R.Buf[fr_end_loc] == FRAME_TAIL_QN193K)
			{
				buf->AnalyseSta = FRAME_CS;
			}
			else
			{
				buf->R.Clev++;
				buf->AnalyseSta = FRAME_HEAD;
				break;
			}
			

		case FRAME_CS:
			Watch_Dog();
			//计算CRC校验
			cs_initial_value = FRAME_CS_INIT_VALUE_QN193K;
			cs_offset_loc = FRAME_CS_LOC_QN193K;
			frame_total_len = FRAME_CSLEN_QN193K;
			data_loc = buf->R.Clev;
			
			for(count=0; count<frame_total_len; count++)
			{
				buf->DAT_Return[count] = buf->R.Buf[data_loc];
				if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈
			}
			//计算校验和
			for(cs_count=FRAME_CS_START_OFFSET_QN193K;cs_count<cs_offset_loc;)
			{
				data0 = buf->R.Buf[cs_count] & 0x0f;
				data1 = buf->R.Buf[cs_count+1] &0x0f;
				data = (data0<<4) + data1;
				cs_initial_value += data;
				cs_count += 2;
				
			}

			cs0 = buf->R.Buf[cs_offset_loc] & 0x0f;
			cs1 = buf->R.Buf[cs_offset_loc+1] & 0x0f;
			cs = (cs0<<4) + cs1;
			
			cs_initial_value = (UINT8)(cs_initial_value&0x00ff);
			if(cs_initial_value == cs)
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



void Handle_QN193k_Zj(CONTR_IF *buf)
{
	UINT8 ev_type;//命令类型
	UINT8 ev_type0,ev_type1,i,j,k;

	ev_type0 = buf->DAT_Return[FRAME_EV_OFFSET_QN193k] & 0x0f;
	ev_type1 = buf->DAT_Return[FRAME_EV_OFFSET_QN193k+1] & 0x0f;
	ev_type = (ev_type0<<4) + ev_type1;

	if(ev_type == QN193K_EV_POLL_ANSWER)//巡检应答
	{
		Poll_Controller_Init(buf);
	}
	else if(ev_type == QN193K_EV_CONTR_RESET)//复位
	{
		Save_Controller_Self_Info(RECORD_RESET,INT_TYPE_RESET_SUCESS);
	}
	else if(ev_type == QN193K_EV_MAIN_POWER)//主电事件
	{
		j = buf->DAT_Return[FRAME_LOOP_NUM_OFFSET_QN193k] & 0x0f;
		k = buf->DAT_Return[FRAME_LOOP_NUM_OFFSET_QN193k+1] & 0x0f;
		i = (j<<4)+k;
		if(i == QN193K_EV_FAULT1)
		{
			Save_Controller_Self_Info(RECORD_FAULT,INT_TYPE_INPUT_POWER_OPEN_CIRCUIT);
		}
		else if(i == QN193K_EV_FAULT_ELIMINATE1)
		{
			Save_Controller_Self_Info(RECORD_ELIMINATE,INT_TYPE_MAINPOW_RECOVER);
		}
	}
	else if(ev_type == QN193K_EV_BACKUP_POWER)//备电事件
	{
		j = buf->DAT_Return[FRAME_LOOP_NUM_OFFSET_QN193k] & 0x0f;
		k = buf->DAT_Return[FRAME_LOOP_NUM_OFFSET_QN193k+1] & 0x0f;
		i = (j<<4)+k;
		if(i == QN193K_EV_FAULT1)
		{
			Save_Controller_Self_Info(RECORD_FAULT,INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT);
		}
		else if(i == QN193K_EV_FAULT_ELIMINATE1)
		{
			Save_Controller_Self_Info(RECORD_ELIMINATE,INT_TYPE_BACKUP_POWER_RECOVER);
		}
	}
	else//其他单字节事件类型事件
	{
		Save_Component_Info_QN193k(&buf->DAT_Return[0],ev_type);
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

void Com_Task_QN193k_Zj(CONTR_IF *buf)
{
	if(Analyse_QN193k(buf))
	{
		Handle_QN193k_Zj(buf);
		On_Led_Contor();
	}
	
	if(B_Poll_Ctr)
	{
		Send_Poll_Contr_QN193k(buf); 
		B_Poll_Ctr = 0;
	}
}

