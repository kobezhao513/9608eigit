 
#include "tanda3004.h"
#include "controller.h"
#include "xycnbus.h"
#include "basic.h"	
#include "record_manage.h"
#include "record_status.h"
#include "sys_init.h"
#include "led.h"
//事件类型转换表
CHANGE_EVENT_TYPE const TANDA_3004PRT_EVENT_CHANGE_LIST[]=
{
	{TANDA_POWEROFF,			RECORD_POWERONOFF,		INT_TYPE_POWER_OFF},
	{TANDA_POWERON,				RECORD_POWERONOFF,		INT_TYPE_POWER_ON},
	{TANDA_FIRE,				RECORD_FIRE,			INT_TYPE_FIRE_ALARM},
	{TANDA_START,				RECORD_START,			INT_TYPE_START_SUCESS},
	{TANDA_STOP,				RECORD_STOP,			INT_TYPE_STOP_SUCESS},
	{TANDA_SHIELD,				RECORD_SHIELD,			INT_TYPE_SHILED_SUCESS},
	{TANDA_RESET,				RECORD_RESET,			INT_TYPE_RESET_SUCESS},
	{TANDA_MAINPOWER_FAULT,		RECORD_FAULT,			INT_TYPE_INPUT_POWER_OPEN_CIRCUIT},
	{TANDA_BATTERY_FAULT,		RECORD_FAULT,			INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT},
	{TANDA_DEV_OFFLINE,			RECORD_FAULT,			INT_TYPE_REG_DEV_OFFLINE},
	{TANDA_MANUAL_FORBID,		REOCRD_MODE,			INT_TYPE_MANUAL_CONTROL_FORBID},
	{TANDA_MANUAL_ALLOW,		REOCRD_MODE,			INT_TYPE_MANUAL_CONTROL_ALLOW},
	{TANDA_AUTO_ALLOW,			REOCRD_MODE,			INT_TYPE_AUTO_CONTROL_ALLOW},
	{TANDA_SHIELD_RELEASE,		RECORD_RELEASE,			INT_TYPE_RELEASE_SUCESS},
	{TANDA_OUTPUT_FAULT,		RECORD_FAULT,			INT_TYPE_OUTPUT1_OFF_FAU},
	{TANDA_DEV_ONLINE,			RECORD_ELIMINATE,		INT_TYPE_REG_DEV_ONLINE},
};

//设备类型转换表
TANDA_DEV_TYPE const TANDA_3004PRT_DEV_CHANGE_LIST[]=
{
	{TANDA_TEMPERATURE,			0x1f},
	{TANDA_SMOKE,				0x2a},
	{TANDA_BUTTON,				0x17},
	{TANDA_INOUT,				0x57},
	{TANDA_ALARM,				0x7d},
};

//以下为特殊程序:不同接口程序存储有区别

void Save_Component_Info_Tanda3004_Prt(UINT8 *buf,UINT16 ev_type,UINT8 dev_type)
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

		for(uc_loc=0;uc_loc<ROW0_MAX_LENGTH_TX3004_PRT;uc_loc++)
		{
			if(*(buf+uc_loc) == FRAME_FLAG1_TX3004_PRT)
			{
				uc_loc += 1;
				break;
			}
		}

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
		record.Uc[0] = dev_type;		//设备类型

		Record_Controler_If_Event(&record);
	}
}


UINT16 Get_Ev_Type_Tanda3004_Prt(CONTR_IF *buf)
{
	UINT8 i,j,valid_len,event_num,tanda_type;
	signed result;
	UINT16 ts_event;
	UINT8 ev_tanda[12] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

	UINT8 s[16][12]=
		{
			{0xB9,0xD8,0xBB,0xFA,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},//关机
			{0xBF,0xAA,0xBB,0xFA,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},//开机
			{0xBB,0xF0,0xBE,0xAF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},//火警
			{0xC6,0xF4,0xB6,0xAF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},//启动
			{0xCD,0xA3,0xD6,0xB9,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},//停止
			{0xC6,0xC1,0xB1,0xCE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},//屏蔽
			{0xCF,0xB5,0xCD,0xB3,0xB8,0xB4,0xCE,0xBB,0xFF,0xFF,0xFF,0xFF},//系统复位
			{0xD6,0xF7,0xB5,0xE7,0xB9,0xCA,0xD5,0xCF,0xFF,0xFF,0xFF,0xFF},//主电故障
			{0xB1,0xB8,0xB5,0xE7,0xB9,0xCA,0xD5,0xCF,0xFF,0xFF,0xFF,0xFF},//备电故障
			{0xCD,0xA8,0xD0,0xC5,0xB9,0xCA,0xD5,0xCF,0xFF,0xFF,0xFF,0xFF},//通信故障
			{0xBD,0xFB,0xD6,0xB9,0xC1,0xAA,0xB6,0xAF,0xFF,0xFF,0xFF,0xFF},//禁止联动
			{0xCA,0xD6,0xB6,0xAF,0xC1,0xAA,0xB6,0xAF,0xFF,0xFF,0xFF,0xFF},//手动联动
			{0xD7,0xD4,0xB6,0xAF,0xC1,0xAA,0xB6,0xAF,0xFF,0xFF,0xFF,0xFF},//自动联动
			{0xBD,0xE2,0xB3,0xFD,0xC6,0xC1,0xB1,0xCE,0xFF,0xFF,0xFF,0xFF},//解除屏蔽
			{0xCA,0xE4,0xB3,0xF6,0xCF,0xDF,0xB9,0xCA,0xD5,0xCF,0xFF,0xFF},//输出线故障
			{0xCD,0xA8,0xD0,0xC5,0xB9,0xCA,0xD5,0xCF,0xBB,0xD6,0xB8,0xB4},//通信故障恢复
		};

	for(i=0;i<ROW0_MAX_LENGTH_TX3004_PRT;i++)
	{
		if(buf->DAT_Return[i] == FRAME_FLAG1_TX3004_PRT)
		{
			valid_len = i;//事件类型的长度
			break;
		}
	}
	memcpy(&ev_tanda[0],&buf->DAT_Return[0],valid_len);

	for(tanda_type=0;tanda_type<16;tanda_type++)
	{
		result = memcmp(&s[tanda_type][0],&ev_tanda[0],12);

		if(result == 0) break;//匹配到事件则直接退出匹配
	}

	if(result != 0)//未匹配到事件
	{
		tanda_type = 0xFF;
	}

	event_num = sizeof(TANDA_3004PRT_EVENT_CHANGE_LIST)/sizeof(CHANGE_EVENT_TYPE);
	for(j=0;j<event_num;j++)
	{
		if(tanda_type == TANDA_3004PRT_EVENT_CHANGE_LIST[j].SrcEventType)
		{
			ts_event = TANDA_3004PRT_EVENT_CHANGE_LIST[j].DestEventType;
			break;
		}
	}
	if(j >= event_num)
	{
		ts_event = INT_TYPE_NULL;
	}
	return ts_event;
}


UINT8 Get_Dev_Type_Tanda3004_Prt(CONTR_IF *buf)
{
	UINT8 i,j,valid_len,dev_num,tanda_type,dev_type_loc;
	signed result;
	UINT8 ts_dev;
	UINT8 dev_tanda[18] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

	UINT8 s[5][18] = 
		{
			{0xB5,0xE3,0xD0,0xCD,0xB8,0xD0,0xCE,0xC2,0xBB,0xF0,0xD4,0xD6,0xCC,0xBD,0xB2,0xE2,0xC6,0xF7},//点型感温火灾探测器
			{0xB9,0xE2,0xB5,0xE7,0xB8,0xD0,0xD1,0xCC,0xBB,0xF0,0xD4,0xD6,0xCC,0xBD,0xB2,0xE2,0xC6,0xF7},//光电感烟火灾探测器
			{0xCA,0xD6,0xB6,0xAF,0xBB,0xF0,0xD4,0xD6,0xB1,0xA8,0xBE,0xAF,0xB0,0xB4,0xC5,0xA5,0xFF,0xFF},//手动火灾报警按钮
			{0xCA,0xE4,0xC8,0xEB,0xCA,0xE4,0xB3,0xF6,0xC4,0xA3,0xBF,0xE9,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},//输入输出模块
			{0xC9,0xF9,0xB9,0xE2,0xBE,0xAF,0xB1,0xA8,0xC6,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},//声光警报器
		};

	for(i=0;i<ROW0_MAX_LENGTH_TX3004_PRT;i++)
	{
		if(buf->DAT_Return[i] == FRAME_FLAG4_TX3004_PRT)
		{
			dev_type_loc = i+1;
			break;
		}
	}
	for(i=0;i<ROW0_MAX_LENGTH_TX3004_PRT;i++)
	{
		if(buf->DAT_Return[i] == FRAME_FLAG2_TX3004_PRT)
		{
			valid_len = i-dev_type_loc;//设备类型长度
			break;
		}
	}

	memcpy(&dev_tanda[0],&buf->DAT_Return[dev_type_loc],valid_len);

	for(tanda_type=0;tanda_type<5;tanda_type++)
	{
		result = memcmp(&s[tanda_type][0],&dev_tanda[0],18);

		if(result == 0) break;//匹配到设备则直接退出匹配
	}

	if(result != 0)//未匹配到设备
	{
		tanda_type = 0xFF;
	}

	dev_num = sizeof(TANDA_3004PRT_DEV_CHANGE_LIST)/sizeof(TANDA_DEV_TYPE);
	for(j=0;j<dev_num;j++)
	{
		if(tanda_type == TANDA_3004PRT_DEV_CHANGE_LIST[j].tanda_type)
		{
			ts_dev = TANDA_3004PRT_DEV_CHANGE_LIST[j].ts_type;
			break;
		}
	}
	if(j >= dev_num)
	{
		ts_dev = 0x00;
	}
	return ts_dev;
}


UINT8 Analyse_Tanda3004_Prt(CONTR_IF *buf)
{
	UINT16 i,j,row0_end_loc,fr_end_loc;
	UINT8 row0_valid = TRUE;
	UINT8 row0_length,row1_length;
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
				if(buf->R.Buf[i] == FRAME_FLAG3_TX3004_PRT)//如果第一行中出现斜杠，则无效
				{
					row0_valid = FALSE;
				}
				if(buf->R.Buf[i] == FRAME_FLAG2_TX3004_PRT)
				{
					break;
				}
				if(++i == CONTR_BUF_LEN)
				{
					i = 0;
				}
			}
			//如果没有找到标志，退出等待接收
			if(i == buf->R.Head) break;

			if(i > buf->R.Clev)
			{
				row0_length = i-buf->R.Clev+1;
			}
			else
			{
				row0_length = i+CONTR_BUF_LEN-buf->R.Clev+1;
			}
			//首行限制
			if((row0_length > ROW0_MAX_LENGTH_TX3004_PRT) || (row0_valid == FALSE))
			{
				buf->R.Clev = i;
				buf->R.FrameEndLoc = i;
				break;
			}
			else
			{
				row0_end_loc = i;
				buf->AnalyseSta = FRAME_LEN;
			}
	

		case FRAME_LEN:
			row1_length = ROW1_LENGTH_TX3004_PRT;
			fr_end_loc = row0_end_loc + row1_length;
			if(fr_end_loc >= CONTR_BUF_LEN)
			{
				fr_end_loc -= CONTR_BUF_LEN;
			}
			buf->R.FrameEndLoc = fr_end_loc;
			buf->AnalyseSta = FRAME_DATA;
			
		case FRAME_DATA://确认一帧接收完成
			fr_end_loc = buf->R.FrameEndLoc;
			
			if(fr_end_loc > buf->R.Clev)
			{
				if((buf->R.Head > buf->R.Clev) && (buf->R.Head <= fr_end_loc)) break;
			}
			else
			{
				if((buf->R.Head > buf->R.Clev) || (buf->R.Head <= fr_end_loc)) break;
			}

		case FRAME_CS:
			Watch_Dog();

			buf->R.FrameEndLoc = fr_end_loc;
			if(buf->R.Buf[buf->R.FrameEndLoc] == FRAME_FLAG2_TX3004_PRT)
			{
				if(row0_valid == TRUE)
				{
					//帧中无长度信息，因此需计算帧长度
					if(buf->R.FrameEndLoc > buf->R.Clev)//未转圈
					{
						frame_total_len = buf->R.FrameEndLoc - buf->R.Clev+1;
					}
					else
					{
						frame_total_len = buf->R.FrameEndLoc + CONTR_BUF_LEN - buf->R.Clev+1;
					}
					buf->R.FrameLen = frame_total_len;

					j = buf->R.Clev;
					for(count=0;count<frame_total_len;count++)
					{
						buf->DAT_Return[count] = buf->R.Buf[j];
						if(++j >= CONTR_BUF_LEN) j = 0;//判断是否转圈
					}
					
					buf->AnalyseSta = FRAME_HEAD;
					ope_flag = TRUE;
				}
				else
				{
					buf->R.Clev = buf->R.Head;
					buf->R.FrameEndLoc = buf->R.Head;
					buf->AnalyseSta = FRAME_HEAD;
					break;
				}
			}
			else
			{
				buf->R.Clev = buf->R.Head;
				buf->R.FrameEndLoc = buf->R.Head;
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



void Handle_Tanda3004_Prt(CONTR_IF *buf)
{
	UINT16 command_type;//命令类型
	UINT8 dev_type;//设备类型

	command_type = Get_Ev_Type_Tanda3004_Prt(buf);
	dev_type = Get_Dev_Type_Tanda3004_Prt(buf);
	switch(command_type)
	{
		case INT_TYPE_FIRE_ALARM://火警
		case INT_TYPE_START_SUCESS://启动
		case INT_TYPE_STOP_SUCESS://停动
		case INT_TYPE_REG_DEV_OFFLINE://设备掉线
		case INT_TYPE_OUTPUT1_OFF_FAU://输出故障
		case INT_TYPE_SHILED_SUCESS://屏蔽
		case INT_TYPE_REG_DEV_ONLINE://设备上线
		case INT_TYPE_RELEASE_SUCESS://屏蔽解除
			Save_Component_Info_Tanda3004_Prt(&buf->DAT_Return[0],command_type,dev_type);
			break;

		case INT_TYPE_RESET_SUCESS:
			//存储控制器复位
			Save_Controller_Self_Info(RECORD_RESET,command_type);	
			break;

		case INT_TYPE_POWER_ON:
			Save_Controller_Self_Info(RECORD_POWERONOFF,command_type);
			break;

		case INT_TYPE_INPUT_POWER_OPEN_CIRCUIT:
		case INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT:
			Save_Controller_Self_Info(RECORD_FAULT,command_type);
			break;

		case INT_TYPE_MANUAL_CONTROL_FORBID://手动禁止
		case INT_TYPE_MANUAL_CONTROL_ALLOW://手动允许
		case INT_TYPE_AUTO_CONTROL_ALLOW://自动允许
			Save_Controller_Self_Info(REOCRD_MODE,command_type);
		
		default:
			break;

	}
	
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

void Com_Task_Tanda3004_Prt(CONTR_IF *buf)
{
	if(Analyse_Tanda3004_Prt(buf))
	{
		Handle_Tanda3004_Prt(buf);
		On_Led_Contor();
	}
}

