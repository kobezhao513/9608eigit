 
#include "tc5120.h"
#include "controller.h"
#include "xycnbus.h"
#include "basic.h"	
#include "record_manage.h"
#include "record_status.h"
#include "sys_init.h"
#include "led.h"


//以下为特殊程序:不同接口程序存储有区别

void Save_Component_Info_Tc5120_Prt(UINT8 *buf,UINT16 ev_type,UINT8 dev_type)
{
	UINT8 a,b,i,j,k;
	UINT8 uc[3];
	RECORD_STRUCT record;

	memset(&record.Sta, 0xFF, sizeof(record));
			
	record.EventType.word = ev_type;
	if(record.EventType.word != INT_TYPE_NULL) 
	{
		record.Type = Get_EventTypeGroup(record.EventType.word);		

		uc[0] = System.ctr_addr[0];     //机号

		a = *(buf+FRAME_LOOP_LOC1_TC5120_PRT) - 0x30;
		b = *(buf+FRAME_LOOP_LOC1_TC5120_PRT+1) - 0x30;
		uc[1] = a*10 + b; 				//回路
		 
		i = *(buf+FRAME_LA_LOC1_TC5120_PRT) - 0x30;
		j = *(buf+FRAME_LA_LOC1_TC5120_PRT+1) - 0x30;
		k = *(buf+FRAME_LA_LOC1_TC5120_PRT+2) - 0x30;
		uc[2] = i*100 + j*10 + k;		//LA
		
		Hex_To_BasicUC(&record.Uc[1],&uc[0],3);
		MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);
		record.Uc[0] = dev_type;		//设备类型

		Record_Controler_If_Event(&record);
	}
}


void Save_Component_Other_Info_Tc5120_Prt(UINT8 *buf,UINT16 ev_type,UINT8 dev_type)
{
	UINT8 a,b,i,j,k;
	UINT8 uc[3];
	RECORD_STRUCT record;

	memset(&record.Sta, 0xFF, sizeof(record));
			
	record.EventType.word = ev_type;
	if(record.EventType.word != INT_TYPE_NULL) 
	{
		record.Type = Get_EventTypeGroup(record.EventType.word);		

		uc[0] = System.ctr_addr[0];     //机号

		a = *(buf+FRAME_LOOP_LOC2_TC5120_PRT) - 0x30;
		b = *(buf+FRAME_LOOP_LOC2_TC5120_PRT+1) - 0x30;
		uc[1] = a*10 + b; 				//回路
		 
		i = *(buf+FRAME_LA_LOC2_TC5120_PRT) - 0x30;
		j = *(buf+FRAME_LA_LOC2_TC5120_PRT+1) - 0x30;
		k = *(buf+FRAME_LA_LOC2_TC5120_PRT+2) - 0x30;
		uc[2] = i*100 + j*10 + k;		//LA
		
		Hex_To_BasicUC(&record.Uc[1],&uc[0],3);
		MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);
		record.Uc[0] = dev_type;		//设备类型

		Record_Controler_If_Event(&record);
	}
}



//由ASKII码值确定对应的设备类型
UINT8 Get_Dev_Type_Tc5120_Prt(CONTR_IF *buf)
{
	UINT8 frame_len,dev_type;

	frame_len = buf->R.FrameLen;

	if((buf->DAT_Return[frame_len-10] == 0xCE) && (buf->DAT_Return[frame_len-9] == 0xC2)
		&& (buf->DAT_Return[frame_len-8] == 0xB8) &&(buf->DAT_Return[frame_len-7] == 0xD0))//感温
	{
		dev_type = DEV_TEMPERATURE;
	}
	else if((buf->DAT_Return[frame_len-10] == 0xD1) && (buf->DAT_Return[frame_len-9] == 0xCC)
			&& (buf->DAT_Return[frame_len-8] == 0xB8) &&(buf->DAT_Return[frame_len-7] == 0xD0))//感烟
	{
		dev_type = DEV_SMOKE;
	}
	else if((buf->DAT_Return[frame_len-10] == 0xCA) && (buf->DAT_Return[frame_len-9] == 0xD6)
			&& (buf->DAT_Return[frame_len-8] == 0xB1) &&(buf->DAT_Return[frame_len-7] == 0xA8))//手报
	{
		dev_type = DEV_BUTTON;
	}
	else if((buf->DAT_Return[frame_len-10] == 0xC9) && (buf->DAT_Return[frame_len-9] == 0xF9)
			&& (buf->DAT_Return[frame_len-8] == 0xB9) &&(buf->DAT_Return[frame_len-7] == 0xE2))//声光
	{
		dev_type = DEV_ALARM;
	}
	else if((buf->DAT_Return[frame_len-10] == 0xCA) && (buf->DAT_Return[frame_len-9] == 0xE4)
			&& (buf->DAT_Return[frame_len-8] == 0xC8) &&(buf->DAT_Return[frame_len-7] == 0xEB))//输入输出
	{
		dev_type = DEV_INOUT;
	}
	else
	{
		dev_type = DEV_UNKNOWN;
	}

	return dev_type;
}


//由ASKII码值确定对应的事件类型
UINT16 Get_Ev_Type_Tc5120_Prt(CONTR_IF *buf)
{
	UINT16 ev_type = 0xFFFF;

	if((buf->DAT_Return[1] == 0xBF) && (buf->DAT_Return[2] == 0xAA)
		&& (buf->DAT_Return[3] == 0xBB) && (buf->DAT_Return[4] == 0xFA))//开机
	{
		ev_type = INT_TYPE_POWER_ON;
	}
	else if((buf->DAT_Return[1] == 0xB8) && (buf->DAT_Return[2] == 0xB4)
			&& (buf->DAT_Return[3] == 0xCE) && (buf->DAT_Return[4] == 0xBB))//复位
	{
		ev_type = INT_TYPE_RESET_SUCESS;
	}
	else if((buf->DAT_Return[1] == 0xD6) && (buf->DAT_Return[2] == 0xF7)
			&& (buf->DAT_Return[3] == 0xB5) && (buf->DAT_Return[4] == 0xE7))//主电故障
	{
		ev_type = INT_TYPE_INPUT_POWER_OPEN_CIRCUIT;
	}
	else if((buf->DAT_Return[1] == 0xB1) && (buf->DAT_Return[2] == 0xB8)
			&& (buf->DAT_Return[3] == 0xB5) && (buf->DAT_Return[4] == 0xE7))//备电故障
	{
		ev_type = INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT;
	}
	else if((buf->DAT_Return[1] == 0xCE) && (buf->DAT_Return[2] == 0xBB) && (buf->DAT_Return[3] == 0xD6)
			&& (buf->DAT_Return[4] == 0xC3) && (buf->DAT_Return[5] == 0x3A))//火警、故障、启动等
	{
		if((buf->DAT_Return[60] == 0xBB) && (buf->DAT_Return[61] == 0xF0)
			&& (buf->DAT_Return[62] == 0xBE) && (buf->DAT_Return[63] == 0xAF))//火警
		{
			ev_type = INT_TYPE_FIRE_ALARM;
		}
		else if((buf->DAT_Return[60] == 0xB9) && (buf->DAT_Return[61] == 0xCA)
				&& (buf->DAT_Return[62] == 0xD5) && (buf->DAT_Return[63] == 0xCF))//故障
		{
			if((buf->DAT_Return[27] == 0xCD) && (buf->DAT_Return[28] == 0xA8)
				&& (buf->DAT_Return[29] == 0xD1) && (buf->DAT_Return[30] == 0xB6))//通讯故障
			{
				ev_type = INT_TYPE_REG_DEV_OFFLINE;
			}
			else if((buf->DAT_Return[27] == 0xCA) && (buf->DAT_Return[28] == 0xE4)
					&& (buf->DAT_Return[29] == 0xB3) && (buf->DAT_Return[30] == 0xF6))//输出故障
			{
				ev_type = INT_TYPE_OUTPUT1_OFF_FAU;
			}
			else if((buf->DAT_Return[27] == 0xCA) && (buf->DAT_Return[28] == 0xE4)
					&& (buf->DAT_Return[29] == 0xC8) && (buf->DAT_Return[30] == 0xEB))//输入故障
			{
				ev_type = INT_TYPE_INPUT1_OFF_FAU;
			}
			else if((buf->DAT_Return[27] == 0xC9) && (buf->DAT_Return[28] == 0xE8)
					&& (buf->DAT_Return[29] == 0xB1) && (buf->DAT_Return[30] == 0xB8))//设备重码
			{
				ev_type = INT_TYPE_LA_UNLAW_DEV_INF;
			}
		}
		else if((buf->DAT_Return[60] == 0xC6) && (buf->DAT_Return[61] == 0xF4)
				&& (buf->DAT_Return[62] == 0xB6) && (buf->DAT_Return[63] == 0xAF))//启动
		{
			ev_type = INT_TYPE_STARTED;
		}
		else if((buf->R.Buf[buf->R.Clev+60] == 0xCD) && (buf->R.Buf[buf->R.Clev+61] == 0xA3)//停止
				&& (buf->R.Buf[buf->R.Clev+62] == 0xD6) && (buf->R.Buf[buf->R.Clev+63] == 0xB9))
		{
			ev_type = INT_TYPE_STOPPED;
		}
		else if((buf->R.Buf[buf->R.Clev+60] == 0xBB) && (buf->R.Buf[buf->R.Clev+61] == 0xD8)//回答
				&& (buf->R.Buf[buf->R.Clev+62] == 0xB4) && (buf->R.Buf[buf->R.Clev+63] == 0xF0))
		{
			ev_type = INT_TYPE_DEVICE_START_FEED_SUC;
		}
	}

	return ev_type;
}


UINT8 Analyse_Tc5120_Prt(CONTR_IF *buf)
{
	UINT16 i,data_loc,head2_loc,fr_end_loc,type_loc,type_judge; 
	UINT16 type1_loc[5],type2_loc[4];
	UINT8 j,count,frame_total_len,ope_flag=0;

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
				if(buf->R.Buf[i] == FRAME_HEAD_TC5120_PRT)
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
			//计算消息类型结束位置
			type_loc = buf->R.Clev+FRAME_TYPE1_LEN_TC5120_PRT;
			if(type_loc >= CONTR_BUF_LEN)
			{
				type_loc -= CONTR_BUF_LEN;
			}
			//确认收到消息类型
			if(type_loc > buf->R.Clev)
			{
				if((buf->R.Head > buf->R.Clev) && (buf->R.Head <= type_loc)) break ;
			}
			else
			{
				if((buf->R.Head > buf->R.Clev) || (buf->R.Head <= type_loc)) break ;
			}
			//特殊处理
			head2_loc = buf->R.Clev+1;
			if(head2_loc >= CONTR_BUF_LEN)
			{
				head2_loc -= CONTR_BUF_LEN;
			}
			if(buf->R.Buf[head2_loc] == FRAME_HEAD_TC5120_PRT)//部分帧可能收到两个0D作为帧头,在此处跳过1个0D
			{
				buf->R.Clev++;
			}

			//提取帧长度//可加入其它限制
			//打印机接口，无打印长度，通过判断事件来判断帧长度

			for(j=0;j<5;j++)
			{
				type1_loc[j] = buf->R.Clev+j+1;
				if(type1_loc[j] >= CONTR_BUF_LEN)
				{
					type1_loc[j] -= CONTR_BUF_LEN;
				}
			}
			if(((buf->R.Buf[type1_loc[0]] == 0xB8) && (buf->R.Buf[type1_loc[1]] == 0xB4)//复位消息
					&& (buf->R.Buf[type1_loc[2]] == 0xCE) && (buf->R.Buf[type1_loc[3]] == 0xBB))
					|| ((buf->R.Buf[type1_loc[0]] == 0xBF) && (buf->R.Buf[type1_loc[1]] == 0xAA)//开机消息
					&& (buf->R.Buf[type1_loc[2]] == 0xBB) && (buf->R.Buf[type1_loc[3]] == 0xFA)))
			{
				fr_end_loc = buf->R.Clev + FRAME_LEN1_TC5120_PRT;
				//判断该帧是否有转圈
				if(fr_end_loc >= CONTR_BUF_LEN)
				{
					fr_end_loc -= CONTR_BUF_LEN;
				}

				buf->R.FrameEndLoc = fr_end_loc;
				buf->AnalyseSta = FRAME_DATA;
			}
			else if(((buf->R.Buf[type1_loc[0]] == 0xD6) && (buf->R.Buf[type1_loc[1]] == 0xF7)
						&& (buf->R.Buf[type1_loc[2]] == 0xB5) && (buf->R.Buf[type1_loc[3]] == 0xE7))//主电
					||((buf->R.Buf[type1_loc[0]] == 0xB1) && (buf->R.Buf[type1_loc[1]] == 0xB8)//备电
					&& (buf->R.Buf[type1_loc[2]] == 0xB5) && (buf->R.Buf[type1_loc[3]] == 0xE7)))
			{
				fr_end_loc = buf->R.Clev + FRAME_LEN2_TC5120_PRT;

				if(fr_end_loc >= CONTR_BUF_LEN)
				{
					fr_end_loc -= CONTR_BUF_LEN;
				}

				buf->R.FrameEndLoc = fr_end_loc;
				buf->AnalyseSta = FRAME_DATA;
			}
			else if((buf->R.Buf[type1_loc[0]] == 0xCE) && (buf->R.Buf[type1_loc[1]] == 0xBB)//其他类型消息，’位置:‘开头
					&& (buf->R.Buf[type1_loc[2]] == 0xD6) && (buf->R.Buf[type1_loc[3]] == 0xC3)
					&& (buf->R.Buf[type1_loc[4]] == 0x3A))
			{
				type_judge = buf->R.Clev + FRAME_TYPE2_LEN_TC5120_PRT;//火警与故障帧长不同，需分开处理
				if(type_judge >= CONTR_BUF_LEN)
				{
					type_judge -= CONTR_BUF_LEN;
				}

				if(type_judge > buf->R.Clev)//未转圈
				{
					if((buf->R.Head > buf->R.Clev) && (buf->R.Head <= type_judge)) break;
				}
				else//转圈
				{
					if((buf->R.Head > buf->R.Clev) || (buf->R.Head <= type_judge)) break;
				}

				for(j=0;j<4;j++)
				{
					type2_loc[j] = buf->R.Clev+j+60;
					if(type2_loc[j] >= CONTR_BUF_LEN)
					{
						type2_loc[j] -= CONTR_BUF_LEN;
					}
				}
				if(((buf->R.Buf[type2_loc[0]] == 0xBB) && (buf->R.Buf[type2_loc[1]] == 0xF0)
					&& (buf->R.Buf[type2_loc[2]] == 0xBE) && (buf->R.Buf[type2_loc[3]] == 0xAF))//火警
					|| ((buf->R.Buf[type2_loc[0]] == 0xC6) && (buf->R.Buf[type2_loc[1]] == 0xF4)//启动
						&& (buf->R.Buf[type2_loc[2]] == 0xB6) && (buf->R.Buf[type2_loc[3]] == 0xAF))
					|| ((buf->R.Buf[type2_loc[0]] == 0xCD) && (buf->R.Buf[type2_loc[1]] == 0xA3)//停止
						&& (buf->R.Buf[type2_loc[2]] == 0xD6) && (buf->R.Buf[type2_loc[3]] == 0xB9))
					|| ((buf->R.Buf[type2_loc[0]] == 0xBB) && (buf->R.Buf[type2_loc[1]] == 0xD8)//回答
						&& (buf->R.Buf[type2_loc[2]] == 0xB4) && (buf->R.Buf[type2_loc[3]] == 0xF0)))
				{

					fr_end_loc = buf->R.Clev + FRAME_LEN4_TC5120_PRT;

					if(fr_end_loc >= CONTR_BUF_LEN)
					{
						fr_end_loc -= CONTR_BUF_LEN;
					}

					buf->R.FrameEndLoc = fr_end_loc;
					buf->AnalyseSta = FRAME_DATA;
				}
				else if((buf->R.Buf[type2_loc[0]] == 0xB9) && (buf->R.Buf[type2_loc[1]] == 0xCA)
					&& (buf->R.Buf[type2_loc[2]] == 0xD5) && (buf->R.Buf[type2_loc[3]] == 0xCF))//故障
				{
					fr_end_loc = buf->R.Clev + FRAME_LEN3_TC5120_PRT;

					if(fr_end_loc >= CONTR_BUF_LEN)
					{
						fr_end_loc -= CONTR_BUF_LEN;
					}

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
				buf->R.Clev++;
				buf->AnalyseSta = FRAME_HEAD;
				break;
			}

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
			buf->AnalyseSta = FRAME_CS;

		case FRAME_CS:
			Watch_Dog();

			if(buf->R.Buf[buf->R.FrameEndLoc] == FRAME_TAIL_TC5120_PRT)//收正确
			{
				//帧中无长度信息，因此需计算帧长度
				if(buf->R.FrameEndLoc > buf->R.Clev)//未转圈
				{
					frame_total_len = buf->R.FrameEndLoc - buf->R.Clev + 1;
				}
				else
				{
					frame_total_len = buf->R.FrameEndLoc + CONTR_BUF_LEN - buf->R.Clev + 1;
				}
				buf->R.FrameLen = frame_total_len;

				data_loc = buf->R.Clev;
				for(count=0;count<frame_total_len;count++)
				{
					buf->DAT_Return[count] = buf->R.Buf[data_loc];
					if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否转圈
				}
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



void Handle_Tc5120_Prt_JT(CONTR_IF *buf)
{
	UINT16 command_type;//命令类型
	UINT8 dev_type;//设备类型

	command_type = Get_Ev_Type_Tc5120_Prt(buf);
	dev_type = Get_Dev_Type_Tc5120_Prt(buf);
	switch(command_type)
	{
		case INT_TYPE_FIRE_ALARM://火警
		case INT_TYPE_STARTED://启动
		case INT_TYPE_STOPPED://停动
		case INT_TYPE_DEVICE_START_FEED_SUC://反馈
			Save_Component_Info_Tc5120_Prt(&buf->DAT_Return[0],command_type,dev_type);
			break;

		case INT_TYPE_REG_DEV_OFFLINE://设备掉线
		case INT_TYPE_OUTPUT1_OFF_FAU://输出故障
		case INT_TYPE_INPUT1_OFF_FAU://输入故障
		case INT_TYPE_LA_UNLAW_DEV_INF://设备重码
			Save_Component_Other_Info_Tc5120_Prt(&buf->DAT_Return[0],command_type,dev_type);
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
		
		default:
			break;

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

void Com_Task_Tc5120_Prt_JT(CONTR_IF *buf)
{
	if(Analyse_Tc5120_Prt(buf))
	{
		Handle_Tc5120_Prt_JT(buf);
		On_Led_Contor();
	}
}

