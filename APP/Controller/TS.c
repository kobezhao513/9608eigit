 
#include "ts.h"
#include "controller.h"
#include "xycnbus.h"
#include "sys_init.h"
#include "record_manage.h"
#include "record_status.h"
#include "basic.h"
#include "pc_comm.h"
#include "led.h"
#include "ex_flash.h"

UINT8 Fr_Lastser;
UINT8 Contr_Mode;

TS_FARAWAY_COM TS_Far;



extern const UINT8 auchCRCHi[];
extern const UINT8 auchCRCLo[];


UINT8 Read_Module_Info(UINT16 start_offset,UINT16 end_offset,UINT8 *buf)
{
	UINT8 copy_flag=FALSE;
	UINT16 i;
	UINT16 count=0;
	RECORD_STRUCT record;

	//memset(&Event,0x00,sizeof(Event));


	//从start位置开始找，找到end-1位置则无未传输信息
	//查找记录总条数:0到5119
    i = start_offset;

    while(i != end_offset)
    {
        Watch_Dog();
				
		if(get_record(i,&record))
		{
			copy_flag = TRUE;
			break;
		
		}
		if(i == (MAXN_FLASH_RECORD - 1))
		{
			i = 0;
		}
		else 
		{
			i++;
		}
		
		count++;
		if(count >= MAXN_FLASH_RECORD)
		{
			return EVENT_NO;
		}
	}	
	if(copy_flag)
	{
		*buf = GET_EVENT_NOMAL_LEN; //默认提取的有效字节长度
		*(buf+1) = record.EventType.byte[1];
		*(buf+2) = record.EventType.byte[0];
		
		memcpy(buf+3,record.Uc,LEN_REG_UC);
		memcpy(buf+8,record.Cc,LEN_REG_CC);
		memcpy(buf+40,&record.Time[0],6);  //yeat month data hour minuite second,共发送6字节
		*(buf+46) = record.Event[0];  //控制器LA:FF或者其他
		//*(buf+GET_EVENT_TYPE_LOC) = record.Type;
		//copy_flag = 0;
		
		return EVENT_FIND;
	}
	return EVENT_NO;
}




UINT8 Find_Module_Msg(UINT8 *Record_Buffer)
{
    UINT8 find_flag=EVENT_NO;
//	UINT16 addr;	

    //找新事件，如果已经找到，则待该事件上传后有一定结果则标志清0
	if( Compete_Event.Event_Pc!= RecordInf.SavePc)//有新信息	
	{
		find_flag = Read_Module_Info(Compete_Event.Event_Pc,RecordInf.SavePc,Record_Buffer);
		if(find_flag == EVENT_NO)
		{
			memset(&Compete_Event, 0x00, sizeof(Compete_Event));
			Compete_Event.Event_Pc= RecordInf.SavePc;
		}
	}
	return find_flag; 
}
void Module_Msg_Send_Suc(void)
{
	Compete_Event.send_flag = FALSE;
	Compete_Event.Event_Pc ++;
}
void Module_Msg_Send_Fal(void)
{
	Compete_Event.send_flag = FALSE;
}


//===============================================================================
//函数描述: 从中心提取数据，存入接收缓存
//输入参数: 接收缓存
//输出参数: 无
//修改日期: 2017-4
//修改人员: 刘芳
//===============================================================================
//收到:  7F+LEN+CTR+MMADR+UC传+UC设2+SER+Ser+DI+CRC8
//存储:  7F+LEN+CTR+UC传+SER+0x38+0x24+0x01+CTR1+UC设2+Ser4+DI+DATA+CRC9  

void TS_Get_Center_Data(UINT8 *rec_buf)
{
    UINT8 len=0;
	if(Contr.Type == TS)
	{
		len = *(rec_buf+1);

		TS_Far.R.Buf[1] = len;

		TS_Far.R.Buf[6] = 0x24;
		TS_Far.R.Buf[7] = 0x01;
		TS_Far.R.Buf[8] = *(rec_buf+2);  
   		TS_Far.R.Buf[8] |= CTR_MMADR;
		len -= 8;//去掉 :len+ctr+mmadr+uc传输设备
		memcpy(&TS_Far.R.Buf[9], rec_buf+9, len);
		
		TS_Far.R.Flag = TS_DATA_VALID;
	    //ctr+addr+ser+di需要后补	
	}
}
//===============================================================================
//函数描述: 从鼎信控制器提取数据，存入发送缓存
//输入参数: 无
//输出参数: 无
//修改日期: 2017-4
//修改人员: 刘芳
//===============================================================================
//收到:  7F+LEN+CTR+LA      +SER+0x38+0x24+0x01+CTR1+UC设2+Ser4+DI+DATA+CRC8
//存储:  7F+LEN+CTR+UC传+SER+0x38+0x24+0x01+CTR1+UC设2+Ser4+DI+DATA+CRC9  

void TS_Get_Controller_Data(UINT8 *send_buf)
{
	//send_buf:  7F+LEN8+CTR1+0x01+SER6+0x38+0x24+0x01+CTR+UC设2+Ser4+DI+DATA+CRC8
  //重新组合为:7F+LEN9+CTR+UC传+Ser7+0x38+0x24+0x01+CTR+UC设2+Ser4+DI+DATA+CRC9

	UINT16 len = 0;
	UINT8  len_exp = 0;
	
	len =  DevCommPro.FrameLen + 4;
	TS_Far.T.Buf[0] = FRAME_HEAD_TS;
	if(len&0x80)
	{	
		len_exp = 1;
		TS_Far.T.Buf[1] = (uint8)(len&0x7F) + 0X80;
		TS_Far.T.Buf[1+len_exp] = (uint8)(len>>7);
	}
	else
	{
		TS_Far.T.Buf[1] = len;
	}
	TS_Far.T.Buf[2 + len_exp] = 0xB4;//0xf4
    MemoryCpyInvert(&TS_Far.T.Buf[3+len_exp],System.uc,LEN_REG_UC);

	memcpy(&TS_Far.T.Buf[9 + len_exp], &send_buf[DevCommPro.diLoc], DevCommPro.FrameLen-4);

	TS_Far.T.Flag = TS_DATA_VALID;
	//需替换SER
}
/*********************************************************************************
* 函数名称 :  Controller_Com_Event_Save
* 函数描述 :  控制器事件记录
* 输入参数 : 
* 输出参数 :  无
* 返 回 值     :  无
* 修改日期 :  2016-01-25
*********************************************************************************/
void Controller_Com_Event_Save(CONTR_IF * buf)
{
    UINT8 i;
    RECORD_STRUCT record;
	memset(&record.Sta, 0xFF, sizeof(record));

	record.EventType.byte[1] = buf->DAT_Return[6];//从数据域起始位开始存
	record.EventType.byte[0] = buf->DAT_Return[7];
    
	record.Type = Get_EventTypeGroup(record.EventType.word);//RECORD_OTHERS;

	for(i=0;i<LEN_REG_UC;++i) 
	{
		record.Uc[i] = buf->DAT_Return[8+i];
	}
	
	for(i=0;i<LEN_REG_CC;++i) 
	{
		record.Cc[i] = buf->DAT_Return[8+LEN_REG_UC+i];
	}
	if(buf->DAT_Return[1] > 49)//控制器la位置
	{
		record.Event[0] = buf->DAT_Return[8+LEN_REG_UC+LEN_REG_CC+5];//存储控制器LA:bus协议格式+uc+cc+time
	}
	if(buf->DAT_Return[1] > 99)
	{
		if((record.EventType.word == INT_TYPE_TEMPERATURE_ALARM)
			|| (record.EventType.word == INT_TYPE_SMOKE_ALARM)
			|| (record.EventType.word == INT_TYPE_ALARM_ERR))
		
		{
			memcpy(&record.Event[1],&buf->DAT_Return[8+LEN_REG_UC+LEN_REG_CC+5+1],50);	//50个字节数据
		}
	}			
	
	Record_Controler_If_Event(&record);

	if(record.Type == RECORD_RESET)
	{
		B_Upload_Self_Info = 1;
	}

}

UINT8 Find_Info_Upload(void)
{
	 UINT8 info_type=0;
	if(B_Upload_Self_Info)
	{
		info_type = SELF_EVENT;
		B_Upload_Self_Info = 0;
	}
	else if(B_Reset_Ctr)
	{
		info_type = RESET_EVENT;
		B_Reset_Ctr = 0;
	}
	else if(B_Silence_Ctr)
	{
		info_type = SILENCE_EVENT;
		B_Silence_Ctr = 0;
	}
	else if(TS_Far.R.Flag)
	{
		TS_Far.R.Flag = TS_DATA_IDLE;
		info_type = COMMAND_EVENT;
	}
	return info_type;
}

UINT16 Caculate_CRC(UINT16 CS,UINT8 data)
{
	UINT8 uIndex,CS0,CS1;

	CS0 = CS & 0x00FF;
	CS1 = (CS & 0xFF00) >> 8;
	CS  = 0x0000;

	uIndex = CS0 ^ (data);//计算CRC
	CS0 = CS1 ^ auchCRCHi[uIndex];
	CS1 = auchCRCLo[uIndex];

	CS = ((CS | CS1) << 8) + CS0;

	return CS;
}

void Slave_Prepare_Answer(CONTR_IF * buf,UINT8 len)
{
	UINT8 i,CS_loc;
	UINT16 CS = 0xFFFF;
	
	//memcpy(&buf->T.Buf,&buf->DAT_Return,6);
	memcpy(&buf->T.Buf,&Contr_Pro.Rec.Buff,6);

	buf->T.Buf[1] = len;
	buf->T.Buf[2] &= (~CTR_TANS_DIR);
	buf->T.Tail = len + 3;
	CS_loc = len + 1;

	//计算CRC
	for(i=0;i<CS_loc;i++)
	{
		CS = Caculate_CRC(CS,buf->T.Buf[i]);
	}

	buf->T.Buf[CS_loc] = (UINT8)(CS&0x00FF);
	buf->T.Buf[CS_loc+1] = (UINT8)((CS&0xFF00)>>8);

	
}

void Slave_Info_Send(CONTR_IF * buf,UINT8 info_type)
{
	UINT8 len = 0;
	len = 0x06;//数据域起始位置

	if(info_type == SELF_EVENT)
	{
		buf->T.Buf[len] = 0xE3;
		buf->T.Buf[len+1] = 0x0D;

	    len += 2;
		memcpy(&buf->T.Buf[len],System.id,LEN_REG_ID); 
		MemoryCpyInvert(&buf->T.Buf[len] ,&buf->T.Buf[len],LEN_REG_ID);	

		len += LEN_REG_ID;
		memcpy(&buf->T.Buf[len],System.uc,LEN_REG_UC);	
		MemoryCpyInvert(&buf->T.Buf[len] ,&buf->T.Buf[len],LEN_REG_UC);

		len += LEN_REG_UC;
		memcpy(&buf->T.Buf[len],System.cc,LEN_REG_CC);	
		MemoryCpyInvert(&buf->T.Buf[len] ,&buf->T.Buf[len],LEN_REG_CC); 
		len += LEN_REG_CC;
		
		memcpy(&buf->T.Buf[len],&System.la,LEN_REG_LA);	
		len += LEN_REG_LA;

		Slave_Prepare_Answer(buf,len-1);
	}

	else if(info_type == RESET_EVENT)
	{
		buf->T.Buf[len]   = 0x24;//上报
		buf->T.Buf[len+1] = 0x01;
		buf->T.Buf[len+2] = 0xF4;//ctr
		memset(&buf->T.Buf[len+3],0xff,LEN_REG_UC); //addr
		buf->T.Buf[len+3+LEN_REG_UC] = buf->DAT_Return[4];//ser
		buf->T.Buf[len+4+LEN_REG_UC] = 0x63;//di
		len += 4+LEN_REG_UC;
		Slave_Prepare_Answer(buf,len);
	}

	else if(info_type == SILENCE_EVENT)
	{
		buf->T.Buf[len]   = 0x24;//上报
		buf->T.Buf[len+1] = 0x01;
		buf->T.Buf[len+2] = 0xF4;//ctr
		memset(&buf->T.Buf[len+3],0xff,LEN_REG_UC); //addr
		buf->T.Buf[len+3+LEN_REG_UC] = buf->DAT_Return[4];//ser
		buf->T.Buf[len+4+LEN_REG_UC] = 0x7c;//di
		len += 4+LEN_REG_UC;
		Slave_Prepare_Answer(buf,len);
	}
	else if(info_type == COMMAND_EVENT)
	{
        memcpy(&buf->T.Buf[1],&TS_Far.R.Buf[1],TS_Far.R.Buf[1]);
		len = buf->T.Buf[1];

		Slave_Prepare_Answer(buf,len);

	}
	else if(info_type == CMD_SEND_SUCC)
	{
        buf->T.Buf[len]   = 0xD0;//收到确认
		buf->T.Buf[len+1] = 0x03;
		len += 2;
		Slave_Prepare_Answer(buf,len);
	}
}



UINT8 Analyse_Info_TS(CONTR_IF * buf)
{
	UINT16 i,data_loc,fr_end_loc,cs=0,data_loc_exp,data_len,count,frame_total_len,cs_offset_loc; 
	UINT8 ope_flag=0;

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
				if(buf->R.Buf[i] == FRAME_HEAD_TS)
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
		data_loc = buf->R.Clev+FRAME_LEN_OFFSET_TS;
		if(data_loc >= CONTR_BUF_LEN)
		{
			data_loc -= CONTR_BUF_LEN;
		}
		
		if(data_loc > buf->R.Clev)//确认收到帧长度
		{
			if((buf->R.Head > buf->R.Clev) && (buf->R.Head <= data_loc)) break ;
		}
		else
		{
			if((buf->R.Head > buf->R.Clev) || (buf->R.Head <= data_loc)) break ;
		}

		if(buf->R.Buf[data_loc]& LEN_EXPAND_BIT)
		{
			buf->R.FrameLen = buf->R.Buf[data_loc] & (~LEN_EXPAND_BIT);
			buf->AnalyseSta = FRAME_LEN_EXP;

		}
		else
		{
			//提取帧长度	
			data_len = buf->R.Buf[data_loc];			
			if((data_len > FRAME_MIN_LEN_TS) 
				&& (data_len < FRAME_MAX_LEN_TS))
			{
				//判断该帧是否有转圈
				fr_end_loc = (buf->R.Clev+data_len+FRAME_EXCEPT_DATA_LEN_TS);
				if(fr_end_loc >= CONTR_BUF_LEN)
				{
					fr_end_loc -= CONTR_BUF_LEN;
				}

				buf->R.FrameLen = data_len;
				buf->R.FrameEndLoc = fr_end_loc;
				buf->AnalyseSta = FRAME_DATA;
				break;
			}
			else
			{
				buf->R.Clev++;
				buf->AnalyseSta = FRAME_HEAD;
				break;
			}
		}
		case FRAME_LEN_EXP:
			data_loc_exp = buf->R.Clev+FRAME_LEN_OFFSET_TS + 1;
			if(data_loc_exp >= CONTR_BUF_LEN)
			{
				data_loc_exp -= CONTR_BUF_LEN;
			}
			//确认收到帧长度

			if(data_loc_exp > buf->R.Clev)//确认收到帧长度
			{
				if((buf->R.Head > buf->R.Clev) && (buf->R.Head <= data_loc_exp)) break ;
			}
			else
			{
				if((buf->R.Head > buf->R.Clev) || (buf->R.Head <= data_loc_exp)) break ;
			}


			//提取帧长度	
			data_len = buf->R.FrameLen + ((UINT16)buf->R.Buf[data_loc_exp]<<7);

			if((data_len>FRAME_MAX_LEN_TS)&&(data_len< FRAME_MAX_LEN_EXP_TS))
			{
				//判断该帧是否有转圈
				fr_end_loc = (buf->R.Clev+data_len +FRAME_EXCEPT_DATA_LEN_TS);
				if(fr_end_loc >= CONTR_BUF_LEN)
				{
					fr_end_loc -= CONTR_BUF_LEN;
				}
				buf->R.FrameEndLoc = fr_end_loc;
				buf->R.FrameLen = data_len;
				buf->AnalyseSta = FRAME_DATA;
			}
			else
			{
				buf->R.Clev++;
				buf->AnalyseSta = FRAME_HEAD;
				buf->R.FrameLen = 0;
				break;
			}

		break;

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
			cs = FRAME_CS_INIT_VALUE_TS;
			cs_offset_loc = buf->R.FrameLen+FRAME_HEAD_LEN_TS;
			frame_total_len = buf->R.FrameLen+FRAME_EXCEPT_DATA_LEN_TS;
			data_loc = buf->R.Clev;

			for(count=0; count<frame_total_len; count++)
			{
				buf->DAT_Return[count] = buf->R.Buf[data_loc];
				if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈

				if(count < cs_offset_loc)
				{
					cs = Caculate_CRC(cs,buf->DAT_Return[count]);
				}
			}

			if((buf->DAT_Return[cs_offset_loc] == (UINT8)(cs&0x00FF))
				&& (buf->DAT_Return[cs_offset_loc+1] == (UINT8)((cs&0xFF00)>>8)))
			{
				ope_flag = TRUE;
				//buf->R.Clev = fr_end_loc;
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

void Handle_Info_TS(CONTR_IF * buf)
{
	UINT8 info_type;

	//巡检
	if((buf->DAT_Return[2] == 0xFE)&&(buf->DAT_Return[5] == DI_POLL_INT))
	{
		info_type = Find_Info_Upload();
		if(info_type)
		{
			Slave_Info_Send(buf,info_type);
		}
		else
		{
			Slave_Prepare_Answer(buf,5);
		}
	}
	//事件
	else if((buf->DAT_Return[2] == 0xF6)&&(buf->DAT_Return[5] == DI_EVENT_INF))
	{
		if((buf->DAT_Return[6] == 0x24) && (buf->DAT_Return[7] == 0x01))
		{
			if(Fr_Lastser != buf->DAT_Return[4])
            {
				if(TS_Far.T.Flag == TS_DATA_IDLE)
				{
					TS_Get_Controller_Data(&buf->DAT_Return[0]);
					Slave_Prepare_Answer(buf,5);
					buf->T.Sta = WAITING;					
				}
			}
			else
			{
				if((buf->T.Sta == WAITING) && (TS_Far.T.Flag == TS_DATA_SEND_SUC))
				{
					TS_Far.T.Flag = TS_DATA_IDLE;
					buf->T.Sta = SEND_IDLE;
				}
			}
		}
		else//if(Fr_Lastser != buf->DAT_Return[4])
		{
            if(Fr_Lastser != buf->DAT_Return[4])
            {
				Controller_Com_Event_Save(buf);
			}
			
			Slave_Prepare_Answer(buf,5);			
		}
	}
	//控制器信息
	else if((buf->DAT_Return[2] == 0xF6) && (buf->DAT_Return[5] == DI_CONTROL_INFO)
		&& (buf->DAT_Return[6] == 0x67) && (buf->DAT_Return[7] == 0x32))
	{
		MemoryCpyInvert(&System.ctr_addr[0],&buf->DAT_Return[20],LEN_REG_UC);
		Slave_Prepare_Answer(buf,7);
	}
	
	buf->R.Clev = buf->R.FrameEndLoc;
	Fr_Lastser = buf->DAT_Return[4];

	buf->R.FrameCount = 0;
	Monitor_Controller_Fault_Eliminate();
}
	
void Com_Task_TS(CONTR_IF * buf)
{
	if(Analyse_Info_TS(buf))
	{
	    //Handle_Info_TS(buf);
		if(buf==&RS232)
		{
			Contr_Mode = COM_232;
		}
		else if(buf==&RS485)
		{
			Contr_Mode = COM_485;
		}
		memcpy(Contr_Pro.Rec.Buff,&buf->DAT_Return,buf->R.FrameLen+3); 
		if(Dev_Frame_Analyse(&Contr_Pro))
		{   
		    if(System.trans_equip_connect)
	    	{
				On_Led_SEND();
	    	}
			else
			{
				On_Led_Contor();
			}
			Dev_Frame_Handle(&Contr_Pro);
			Monitor_Controller_Fault_Eliminate();
		}
		Monitor_Trans_Equip_Fault_Eliminate();
		buf->R.Clev = buf->R.FrameEndLoc;
		    //在应答时间段，则应答
	/*if(buf->T.Sta == WAITING)
	{
		if( buf->R.FrameCount <= 200)
		{
            if(TS_Far.T.Flag == TS_DATA_SEND_SUC)
            {
				TS_Far.T.Flag = TS_DATA_IDLE;
				buf->T.Sta = SEND_IDLE;
			}
		}
		else
		{
			TS_Far.T.Flag = TS_DATA_IDLE;
			buf->T.Sta = SEND_IDLE;
			buf->T.Clev = 0;
			buf->T.Tail = 0;
		}
	}*/
	}
}
