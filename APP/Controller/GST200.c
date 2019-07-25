#include "gst200.h"
#include "controller.h"
#include "record_manage.h"
#include "record_status.h"
#include "basic.h"
#include "xycnbus.h"
#include "sys_init.h"
#include "led.h"
GST Gst;

//事件类型转换表
CHANGE_EVENT_TYPE const C_CTR_EVENT_CHANGE_LIST_GST200[]=
{
	{GST200_FIRE ,             RECORD_FIRE,          INT_TYPE_ALARM},
	{GST200_ANSWER ,           RECORD_ANSWER,        INT_TYPE_DEVICE_START_FEED_SUC},
	{GST200_RESUME ,           RECORD_RESUME,        INT_TYPE_EARLY_ALARM_RESET},
	{GST200_START ,            RECORD_START,         INT_TYPE_REQ_START},
	{GST200_STOP ,             RECORD_STOP,          INT_TYPE_REQ_STOP},
	{GST200_FAULT,             RECORD_FAULT,         INT_TYPE_DEV_FAULT},
	{GST200_SHIELD,            RECORD_SHIELD,        INT_TYPE_REQ_SHIELD},
	{GST200_RELEASE,           RECORD_RELEASE,       INT_TYPE_REQ_RELEASE},
	{GST200_DELAY,             RECORD_DELAY,         INT_TYPE_MANUAL_DELAY_START},
	{GST200_CANCEL,            RECORD_CANCEL,        INT_TYPE_MANUAL_DELAY_CANCEL},
	{GST200_ELIMINATE,         RECORD_ELIMINATE,     INT_TYPE_DEV_RECOVER_NOMAL},

};
//设备类型转换表
Gst_Device_Type const C_CTR_UC_CHANGE_LIST_GST[]=
{
	{GST_GRAT_TEMPER_MEASURE ,             0XFF},
	{GST_SPOT_HEAT ,                       0X1F},
	{GST_SPOT_SMOKE ,                      0X2A},
	{GST_ALARM_INTERFACE ,                 0XFF},
	{GST_COMPOSITE_FLAME ,                 0X3F},
	{GST_BEAM_SMOKE,                       0X2B},
	{GST_ULTRAVIOLET_FLAME,                0X3D},
	{GST_LINEAR_HEAT,                      0X22},
	{GST_SUCTION_SMOKE,                    0X2C},
	{GST_COMPOSITE_FIRE,                   0X32},
	{GST_MANUAL_POINT ,                    0X17},
	{GST_FIRE_BROADCAST ,                  0X93},
	{GST_ACOUSTIC ,                        0X7D},
	{GST_FIRE_TELEPHONE ,                  0X90},
	{GST_FIRE_HYDRANT ,                    0X18},
	{GST_FIRE_HYDRANT_PUMP,                0X5B},
	{GST_SPRAY_PUMP,                       0X5F},
	{GST_STABILIZED_PUMP,                  0X64},
	{GST_SMOKE_VENTILATOR,                 0X6E},
	{GST_PRESSURE_FAN,                     0XC3},
	{GST_NEW_FAN ,		               	   0XC2},
	{GST_FRD ,					           0XB0},
	{GST_SMOKE_EXHAUST_DAMPER ,			   0X71},
	{GST_BLOW_VALVE ,			           0X72},
	{GST_SOLENOID_VALVE ,			       0XB6},
	{GST_ROLLING_DOOR_MID,				   0XB3},
	{GST_ROLLING_DOOR_DOW, 		           0XB4},
	{GST_FIREPROOF_DOOR,				   0X76},
	{GST_PRESS_SWITCH, 			           0X63},
	{GST_WATER_FLOW_INDICATOR,			   0X60},
	{GST_ELEVATOR ,                        0X77},
	{GST_AIR_CONDITIONING_UNIT ,           0X96},
	{GST_DIESEL_GENERATOR ,                0X97},
	{GST_LIGHT_DISTRIBUTION ,              0X98},
	{GST_POWER_DISTRIBUTION ,              0X99},
	{GST_WATER_ELECTROMAGNETIC,            0XB9},
	{GST_GAS_START,                        0XFF},
	{GST_GAS_STOP,                         0XFF},
	{GST_SLAVE,                            0X01},
	{GST_FIRE_DISPLAY_PANEL,               0X16},
	{GST_SLUICE_VALVE ,			           0XFF},
	{GST_DRY_POWDER ,					   0XFF},
	{GST_FOAM_PUMP ,				       0X69},
	{GST_FIRE_CONTROL_POWER ,			   0X1A},
	{GST_EMERGENCY_LIGHTING ,			   0XB5},
	{GST_GUIDANCE_INSTRUCTIONS,	           0X7C},
	{GST_SPRAYING_INSTRUCTIONS, 		   0X7B},
	{GST_SECURITY_MODULE,				   0X7A},
	{GST_SIGNAL_VALVE, 			           0X61},
	{GST_SMOKE_CONTROL_VALVE,			   0X71},
	{GST_WATER_PUMP ,			           0XBF},
	{GST_FLOOR_LAMP ,					   0XFF},
	{GST_EQUIPMENT_STOP ,				   0X57},
	{GST_PUMP_FAILURE ,			           0XFF},
	{GST_EMERGENCY_START_BUTTON ,	       0X8E},
	{GST_EMERGENCY_STOP_BUTTON,			   0X8E},
	{GST_RAIN_PUMP, 		               0XC0},
	{GST_UPPER_COMPUTER,				   0XFF},
	{GST_LOOP, 			                   0XA3},
	{GST_AIR_COMPRESSOR,			       0XBC},
	{GST_LINKAGE_POWER ,			       0X1A},
	{GST_TELEPHONE_JACK ,				   0X92},
	{GST_PART_EQUIPMENT ,				   0XFF},
	{GST_DRENCH_VALVE ,			           0XBA},
	{GST_HEAT_RODS ,			           0XFF},
	{GST_FAULT_OUTPUT,				       0X9B},
	{GST_MANUAL_ALLOW, 		               0XFF},
	{GST_AUTO_ALLOW,				       0XFF},
	{GST_COMBUSTIBLE_GAS, 			       0X0A},
	{GST_STANDBY_INDICATOR,			       0XFF},
	{GST_DOOR_LAMP ,			           0XFF},
	{GST_BACKUP_WORK ,					   0XFF},
	{GST_EQUIPMENT_TROUBLE ,			   0XFF},
	{GST_EMERGENCY_CALLING ,			   0XFF},
	{GST_CLOCK_POWER ,		               0XA2},
	{GST_SOUND_LIGHT_ALARM,				   0X9A},
	{GST_ALARM_TRANSMISSION, 		       0XFF},
	{GST_LOOP_SWITCH,				       0XFF},
	{GST_UNDEFINED, 		        	   0XFF},
	{GST_DELAY_OUTPUT,			           0XFF},
	{GST_FIRE_HYDRANT_2 ,			       0X8C},
	{GST_CABLE_HEAT ,					   0X22},
	{GST_BREATHE_SMOKE ,				   0X2C},
	{GST_BREATHE_FIRE ,			           0XFF},
	{GST_BREATHE_WARNING ,			       0XFF},
	{GST_DETECTOR_DIRTY,			       0XFF},
	{GST_MULTI_WIRE,			           0X8D},
	{GST_SIMULATR_HEAT,			           0XFF},
	{GST_ELECTRIC_ALARM,			       0XFF},
	{GST_BUS,			                   0XA3},
	
};


//设备类型转换
//===============================================================================
//函数描述: //设备类型转换
//输入参数: sevent_type为需要转换的设备类型
//输出参数: 转换为鼎信的设备类型
//修改日期: 2016-10
//修改人员: 
//===============================================================================
UINT16 Change_UC_Type_GST200(UINT8 sevent_type)
{
	UINT8 dest_type = 0xff;
	UINT8 k,eventNum;

	eventNum = sizeof(C_CTR_UC_CHANGE_LIST_GST)/sizeof(Gst_Device_Type);
	for(k=0; k < eventNum; k++)
	{
		if(C_CTR_UC_CHANGE_LIST_GST[k].src_type == sevent_type)
		{
			dest_type = C_CTR_UC_CHANGE_LIST_GST[k].dest_type;
			break;
		}
	}
	return dest_type;
}
void GST_Uart_Init_Change(UINT8 type)
{
	if(Gst.init_mode != type)
	{
		Gst.init_mode = type;
		Contr_232Uart_Init(4);
		RS232_Uart_Init(UART_REC_INIT);
	}

}
//===============================================================================
//函数描述: 存储部件信息
//输入参数: buf为输入数据
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Save_Component_Info_GST200(UINT8 *buf)
{
	UINT8 i;
	RECORD_STRUCT record;
	memset(&record.Sta, 0xFF, sizeof(record));

	record.EventType.word = Change_Event_Type_GST200(*(buf),&record.Type);
	if(record.EventType.word != INT_TYPE_NULL)

	{
	    if((*buf) == GST200_FIRE)
	    {
			if((*(buf+4)==GST_PRESS_SWITCH)||(*(buf+4)==GST_WATER_FLOW_INDICATOR)||(*(buf+4)==GST_AIR_CONDITIONING_UNIT)||
				(*(buf+4)==GST_SECURITY_MODULE)||(*(buf+4)==GST_COMBUSTIBLE_GAS))
				//防盗开关（48）、水流指示（30）、压力开关（29）、空调（32）、可燃气体（69） 监管信息
			{
				record.Type = RECORD_MONITER;
				record.EventType.word= INT_TYPE_MONITOR_ALARM;
			}
		}	
		record.Uc[LEN_REG_UC-5] = Change_UC_Type_GST200(*(buf+4));     //设备类型
		record.Uc[LEN_REG_UC-1] = System.ctr_addr[0];  //机号
		record.Uc[LEN_REG_UC-2] = *(buf+1);  //楼层号
		record.Uc[LEN_REG_UC-3] = *(buf+2);
		record.Uc[LEN_REG_UC-4] = *(buf+3);//前一位为楼号，后三位为房间号
		if((record.EventType.word == INT_TYPE_FIRE_ALARM)&&record.Uc[LEN_REG_UC-5] == 0X17)
			record.EventType.word =INT_TYPE_MANUAL_ALARM;
		else if((record.EventType.word == INT_TYPE_FIRE_ALARM)&&record.Uc[LEN_REG_UC-5] == 0X1F)
			record.EventType.word =INT_TYPE_TEMPERATURE_ALARM;	
		else if((record.EventType.word == INT_TYPE_FIRE_ALARM)&&record.Uc[LEN_REG_UC-5] == 0X2A)
		    record.EventType.word =INT_TYPE_SMOKE_ALARM;	


		if(*(buf+4)==0x90)
		{
			MemoryCpyInvert(record.Uc,System.ctr_addr,LEN_REG_UC);
			record.EventType.word =INT_TYPE_BUS_SHORT_CIRCUIT;
			record.Type = RECORD_FAULT;
		}
		for(i=0;i<14;++i) 
		{
			if(*(buf+8+i)==0x00)
			{
				if(*(buf+8+i+1)!=0x00)//字符
					record.Cc[LEN_REG_CC-i-1] =0x03+0XA0;//地址号
				else
					break;
			}
			else
			{
				record.Cc[LEN_REG_CC-i-1] = BCD2Hex(*(buf+8+i))+0XA0;//地址号
			}
		}
		Record_Controler_If_Event(&record);
	}
}
//===============================================================================
//函数描述: 存储其他信息
//输入参数: buf为输入数据
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Save_Other_Info_GST200(UINT8 *buf)
{
	UINT8 i,valid_flag;
	RECORD_STRUCT record;
	memset(&record.Sta, 0xFF, sizeof(record));
	valid_flag = 0;

	if(*(buf+4)==0x82)//主电断路
	{
		record.Type = RECORD_FAULT;
		record.EventType.word =INT_TYPE_INPUT_POWER_OPEN_CIRCUIT;
		valid_flag = 1;
	}
	else if(*(buf+4)==0x84)//备电断路
	{
		record.Type = RECORD_FAULT;
		record.EventType.word =INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT;
		valid_flag = 1;
	}
	else if(*(buf+4)==0x83)//主电恢复
	{
		record.Type = RECORD_ELIMINATE;
		record.EventType.word =INT_TYPE_MAINPOW_RECOVER;
		valid_flag = 1;
	}
	else if(*(buf+4)==0x85)//备电恢复
	{
		record.Type = RECORD_ELIMINATE;
		record.EventType.word =INT_TYPE_BACKUP_POWER_RECOVER;
		valid_flag = 1;
	}
	if(valid_flag)
	{
		for(i=0;i<5;++i) 
		{
			record.Uc[LEN_REG_UC-i-1] = System.ctr_addr[i];  //机号
		}
		for(i=0;i<14;++i) 
		{
			if(*(buf+8+i)==0x00)
			{
				if(*(buf+8+i+1)!=0x00)//字符
					record.Cc[LEN_REG_CC-i-1] =0x03+0XA0;//地址号
				else
					break;
			}
			else
			{
				record.Cc[LEN_REG_CC-i-1] = BCD2Hex(*(buf+8+i))+0XA0;//地址号
			}
		}
		
		Record_Controler_If_Event(&record);
	}
}
//===============================================================================
//函数描述: 准备应答数据 
//输入参数: buf为控制器接口;type为命令类型
//输出参数: 无
//修改日期: 2016-10
//修改人员: 杨莫然
//===============================================================================
void Send_Answer_Frame_GST200(CONTR_IF *buf,UINT8 type)
{
	buf->T.Buf[buf->T.Tail] = type + (CTR_NUMBER_L&0x0F);
	buf->T.Tail += 1;
}


//===============================================================================
//函数描述: 准备命令数据 
//输入参数: buf为控制器接口;type为命令类型
//输出参数: 无
//修改日期: 2016-10
//修改人员: 杨莫然
//===============================================================================
void Send_Command_Frame_GST200(CONTR_IF *buf,UINT8 type)
{
	UINT8 start,i,cs;
	
	start = buf->T.Tail;
   
	buf->T.Buf[start] = 10;
	buf->T.Buf[start+1] = Gst.infor_index++;
	buf->T.Buf[start+2] = 0xAA;
	buf->T.Buf[start+3] = type;
	buf->T.Buf[start+4] = CTR_NUMBER_L;
	buf->T.Buf[start+5] = 0x55;
	buf->T.Buf[start+6] = 0x55;
	buf->T.Buf[start+7] = 0x55;
	//cs=0;
	cs=CTR_NUMBER_L;
	for(i=0;i<8;i++)
	{
    	cs += buf->T.Buf[start+i];
	}
	buf->T.Buf[start+8] = cs;

	buf->T.Tail += 9;
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
UINT16 Change_Event_Type_GST200(UINT8 src_event,UINT8 *record_type)
{
	UINT16 dest_event = 0xffff;
	UINT16 k,eventNum;

	eventNum = sizeof(C_CTR_EVENT_CHANGE_LIST_GST200)/sizeof(CHANGE_EVENT_TYPE);
	for(k=0; k < eventNum; k++)
	{
		if(C_CTR_EVENT_CHANGE_LIST_GST200[k].SrcEventType == src_event)
		{
			dest_event = C_CTR_EVENT_CHANGE_LIST_GST200[k].DestEventType;
			break;
		}
	}
	if(k == eventNum)
	{ 
		dest_event = INT_TYPE_NULL;
	}
	else
	{
		(*record_type) = C_CTR_EVENT_CHANGE_LIST_GST200[k].RecordType;
	}
	return dest_event;
}



//===============================================================================
//函数描述: 接收数据分析
//输入参数: buf为控制器接口
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
UINT8 Analyse_GST200(CONTR_IF *buf)
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
				if((buf->R.Buf[i] == FRAME_HEAD_GST200) || (buf->R.Buf[i] == ( 0x10 | FRAME_HEAD_GST200)))//事件应答
				{
					buf->R.Clev = i; 
					break;
				}
				if(buf->R.Buf[i] == ( 0x50 | CTR_NUMBER_L)) //无事件应答
				{
					Monitor_Controller_Fault_Eliminate();
					On_Led_Contor();
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
			//计算帧长度所在位置
			data_loc = buf->R.Clev+FRAME_LEN_OFFSET_GST200;
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
			data_len =	buf->R.Buf[data_loc];
			if((data_len > FRAME_MIN_LEN_GST200) 
				&& (data_len < FRAME_MAX_LEN_GST200))
			{
				//判断该帧是否有转圈
				fr_end_loc = (buf->R.Clev+data_len+FRAME_EXCEPT_DATA_LEN_GST200);
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
			//计算CRC校验
			cs_initial_value = FRAME_CS_INIT_VALUE_GST200;
			cs_offset_loc = buf->R.FrameLen-1;
			frame_total_len = buf->R.FrameLen+FRAME_EXCEPT_DATA_LEN_GST200;
			data_loc = buf->R.Clev;
			
			for(count=0; count<frame_total_len; count++)
			{
				buf->DAT_Return[count] = buf->R.Buf[data_loc];
				if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈

				if(count < cs_offset_loc)
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
//函数描述: 通讯处理 :主叫模式
//输入参数: buf为控制器接口
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Handle_GST200_Zj(CONTR_IF *buf)
{

	UINT8 command_type;//命令类型
	command_type = buf->DAT_Return[4];
	
	if(buf->DAT_Return[3]==0xAA)
	{
		if((command_type ==GST200_FIRE)||(command_type ==GST200_ANSWER)||(command_type ==GST200_RESUME)||(command_type ==GST200_START)||
			(command_type ==GST200_STOP)||(command_type ==GST200_FAULT)||(command_type ==GST200_SHIELD)||(command_type ==GST200_RELEASE)||
			(command_type ==GST200_DELAY)||(command_type ==GST200_CANCEL)||(command_type ==GST200_ELIMINATE))
		{//事件信息
			Save_Component_Info_GST200(&buf->DAT_Return[4]);			
		}
		else if(command_type == GST200_CTRSELF)//控制器自身主备电故障、恢复
		{
			Save_Other_Info_GST200(&buf->DAT_Return[4]);
		}
		else if(command_type == GST200_RESET)//复位
		{
			Save_Controller_Self_Info(RECORD_RESET,INT_TYPE_RESET_SUCESS);
		}
		Send_Answer_Frame_GST200 (buf,GST200_CONTR_ANSWER);//未识别帧直接回应答
	}
	else if(buf->DAT_Return[3]==0xB1)
	{
		Send_Answer_Frame_GST200 (buf,GST200_CONTR_ANSWER);
	}
	buf->R.Clev = buf->R.FrameEndLoc;
	buf->R.FrameLen = 0;
	Contr.ComFault.Count = 0;
	Poll_Controller_Init(buf);


}
//===============================================================================
//函数描述: 通讯任务--主叫模式，海湾200控制器通信
//输入参数: buf为控制器接口
//输出参数: 无
//修改日期: 2016-10
//修改人员: 杨莫然
//===============================================================================
void Com_Task_GST200_Zj(CONTR_IF *buf)
{
	if(Analyse_GST200(buf))
	{
	    Handle_GST200_Zj(buf);
		On_Led_Contor();
	}
	
	else if(B_Poll_Ctr)//巡检
	{
		GST_Uart_Init_Change(MARK);
		Send_Answer_Frame_GST200 (buf,GST200_CONTR_POLL);
		B_Poll_Ctr = 0;
	}
	/*else if(B_Reset_Ctr)//控制器复位
	{
		if(Gst.gst200_com ==GST200_COM_HEAD)
		{
			buf->PollTimer = 0;
			buf->T.Buf[buf->T.Tail] = CTR_NUMBER_L;
			buf->T.Tail += 1;
			Gst.delay = 10;
			Gst.gst200_com = GST200_COM;
		}
		else if(Gst.gst200_com == GST200_COM)
		{
		    if(Gst.delay>0)return;
			
			GST_Uart_Init_Change(SPACE);
			Send_Command_Frame_GST200(buf,GST200_RESET);
			B_Reset_Ctr = 0;
			
			Gst.gst200_com = GST200_COM_HEAD;
		}
	}
	else if(B_Silence_Ctr)//控制器消音
	{
		if(Gst.gst200_com ==GST200_COM_HEAD)
		{
			buf->PollTimer = 0;
			buf->T.Buf[buf->T.Tail] = CTR_NUMBER_L;
			buf->T.Tail += 1;
			Gst.delay = 10;
			Gst.gst200_com = GST200_COM;
		}
		else if(Gst.gst200_com == GST200_COM)
		{
		    if(Gst.delay>0)return;
			GST_Uart_Init_Change(SPACE);
			Send_Command_Frame_GST200(buf,GST200_SILENCE);
			B_Silence_Ctr = 0;
			Gst.gst200_com = GST200_COM_HEAD;
		}
	}*/
}
//===============================================================================
//函数描述: 通讯处理 :监听模式
//输入参数: buf为控制器接口
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Handle_GST200_Jt(CONTR_IF *buf)
{
	UINT8 command_type;//命令类型
	command_type = buf->DAT_Return[4];
	
	if(buf->DAT_Return[3]==0xAA)
	{
		if((command_type ==GST200_FIRE)||(command_type ==GST200_ANSWER)||(command_type ==GST200_RESUME)||(command_type ==GST200_START)||
			(command_type ==GST200_STOP)||(command_type ==GST200_FAULT)||(command_type ==GST200_SHIELD)||(command_type ==GST200_RELEASE)||
			(command_type ==GST200_DELAY)||(command_type ==GST200_CANCEL)||(command_type ==GST200_ELIMINATE))
		{//事件信息
			Save_Component_Info_GST200(&buf->DAT_Return[4]);			
		}
		else if(command_type == GST200_CTRSELF)//控制器自身主备电故障、恢复
		{
			Save_Other_Info_GST200(&buf->DAT_Return[4]);
		}
		else if(command_type == GST200_RESET)//复位
		{
			Save_Controller_Self_Info(RECORD_RESET,INT_TYPE_RESET_SUCESS);
		}
	}
	buf->R.Clev = buf->R.FrameEndLoc;
	buf->R.FrameLen = 0;
	Contr.ComFault.Count = 0;
	Poll_Controller_Init(buf);

}
//===============================================================================
//函数描述: 通讯任务--监听模式，海湾200控制器通信
//输入参数: buf为控制器接口
//输出参数: 无
//修改日期: 2016-10
//修改人员: 杨莫然
//===============================================================================
void Com_Task_GST200_Jt(CONTR_IF *buf)
{
	if(Analyse_GST200(buf))
	{
	    Handle_GST200_Jt(buf);
		On_Led_Contor();
	}
}

