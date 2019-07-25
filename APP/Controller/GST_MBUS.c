#include "gst_mbus.h"
#include "gst500.h"
#include "controller.h"
#include "gst200.h"
#include "xycnbus.h"
#include "record_manage.h"
#include "record_status.h"
#include "basic.h"
#include "sys_init.h"
#include "ts.h"
#include "led.h"
UINT16 Selsct_Sta; //查询位置
UINT8  Selsct_Powsta;

UINT8  Selsct_Sta_Save[1201];




void GST_Modbus_Event_Sum(uint16*event_num)
{
	UINT16 i;

	if(Contr.Type == GST500_SF_ZJ)
	{
		for(i=(Selsct_Start_GST5000/2);i<=(Selsct_Total_GST5000/2);i++)
		{
			if(Selsct_Sta_Save[i]!=0x00)
			{
				if((Selsct_Sta_Save[i]&0xF0)>>4 == 01)
					event_num[0] ++;
				else if((Selsct_Sta_Save[i]&0xF0)>>4  == 02)
					event_num[2] ++;
				else if((Selsct_Sta_Save[i]&0xF0)>>4  == 07)
					event_num[3] ++;
				else if((Selsct_Sta_Save[i]&0xF0)>>4  == 03)
					event_num[4] ++;
				
				if((Selsct_Sta_Save[i]&0x0F) == 01)
					event_num[0] ++;
				else if((Selsct_Sta_Save[i]&0x0F) == 02)
					event_num[2] ++;
				else if((Selsct_Sta_Save[i]&0x0F) == 07)
					event_num[3] ++;
				else if((Selsct_Sta_Save[i]&0x0F) == 03)
					event_num[4] ++;
			}
			
		}
	}

	else if(Contr.Type == GST200_SF_ZJ)
	{
		for(i=(Selsct_Start_GST200/2);i<=(Selsct_Total_GST200/2);i++)
		{
			if(Selsct_Sta_Save[i]!=0x00)
			{
				if((Selsct_Sta_Save[i]&0xF0)>>4 == 01)
					event_num[0] ++;
				else if((Selsct_Sta_Save[i]&0xF0)>>4  == 02)
					event_num[2] ++;
				else if((Selsct_Sta_Save[i]&0xF0)>>4  == 07)
					event_num[3] ++;
				else if((Selsct_Sta_Save[i]&0xF0)>>4  == 03)
					event_num[4] ++;
				
				if((Selsct_Sta_Save[i]&0x0F) == 01)
					event_num[0] ++;
				else if((Selsct_Sta_Save[i]&0x0F) == 02)
					event_num[2] ++;
				else if((Selsct_Sta_Save[i]&0x0F) == 07)
					event_num[3] ++;
				else if((Selsct_Sta_Save[i]&0x0F) == 03)
					event_num[4] ++;
			}
		}
	}
}

UINT16 GST_Modbus_Event_Sta(uint16 start_loc,uint16 sum,uint8*event_buff)
{
	UINT16 event_loc = 0,end_loc,i,event_num =0,loop_addr,event_sta_loc;
	UINT8 j,start_sta,end_sta;
	UINT8 event_ctr_type[6]={1,2,7,3,5};
	UINT8 event_type[6]={0,2,3,4,5};
	start_sta = start_loc%2;
	end_sta = (start_loc+sum)%2;
	end_loc = (start_loc+sum)/2;
	if(end_sta)
	{
		end_loc +=1;
	}
	for(j=0;j<5;j++)
	{
		event_sta_loc = event_loc  ;
		event_loc = event_sta_loc +3;
		for(i=(start_loc/2);i<end_loc;i++)
		{
			if(Selsct_Sta_Save[i]!=0x00)
			{
				if((start_sta)&&(i==(start_loc/2)))//起始奇数，从后半个字节算起
				{
					
				}
				else
				{
					if((Selsct_Sta_Save[i]&0xF0)>>4 == event_ctr_type[j])
					{
					
						loop_addr = i*2;
						event_buff[event_loc]=(UINT8)(loop_addr/240);
						event_loc ++ ;
						event_buff[event_loc]=(UINT8)(loop_addr%240);
						event_loc ++ ;
						
						event_num ++;
					}
				}
				if((end_sta)&&(i == (end_loc-1)))//结束位置为前半个字节
				{
					
				}
				else
				{
					if((Selsct_Sta_Save[i]&0x0F) == event_ctr_type[j])
					{
						loop_addr = i*2 + 1;
						event_buff[event_loc]=(UINT8)(loop_addr/240);
						event_loc ++ ;
						event_buff[event_loc]=(UINT8)(loop_addr%240);
						event_loc ++ ;
						
						event_num ++;
					}
				}
			}
			
		}
		if(event_num != 0)
		{
			event_buff[event_sta_loc]= event_type[j];
			event_buff[event_sta_loc+2]=(UINT8)((event_num&0xFF00)>>8);
			event_buff[event_sta_loc+1]=(UINT8)(event_num&0x00FF);
			event_num = 0;
		}
		else
		{
			event_loc = event_sta_loc ;
		}

	}
	return event_loc;
}




void GST_Modbus_Save_Event(UINT8 Selsct,UINT8 EventType)
{
    UINT8 uc[3];
	UINT16 loop_addr;
  RECORD_STRUCT record;
	//机号,回路,地址
	memset(&record.Sta, 0xFF, sizeof(record));

	switch (EventType)
   	{
	   	case 0x01:
			record.EventType.word = INT_TYPE_ALARM;		
			record.Type = RECORD_FIRE;
		break;
		case 0x02:
			record.EventType.word = INT_TYPE_DEV_FAULT; 	
			record.Type = RECORD_FAULT;
		break;
		case 0x03:
			record.EventType.word = INT_TYPE_DEVICE_START_FEED_SUC;		
			record.Type = RECORD_ANSWER;
		break;
		case 0x04:
			record.EventType.word = INT_TYPE_DEV_RECOVER_NOMAL; 	
			record.Type = RECORD_ELIMINATE;
		break;
		case 0x05:
			record.EventType.word = INT_TYPE_REQ_START;		
			record.Type = RECORD_START;
		break;
		case 0x06:
			record.EventType.word = INT_TYPE_REQ_STOP; 	
			record.Type = RECORD_STOP;
		break;
		case 0x07:
			record.EventType.word = INT_TYPE_REQ_SHIELD;		
			record.Type = RECORD_SHIELD;
		break;
		case 0x08:
			record.EventType.word = INT_TYPE_REQ_RELEASE; 	
			record.Type = RECORD_RELEASE;
		break;
		case 0x09:
			record.EventType.word = INT_TYPE_DEV_RECOVER_NOMAL; 	
			record.Type = RECORD_ELIMINATE;
		break;
		default:
			
		break;
   	}
	if(Contr.Type == GST200_SF_ZJ)
	{
		if(Selsct_Sta == 242)
		{
			loop_addr = (Selsct_Total_GST200/Selsct_Count) * Selsct_Count + Selsct/2 + 1;
			uc[0] = System.ctr_addr[0]; 	//机号
			uc[1] = (UINT8)(loop_addr/242);  //回路 
			uc[2] = (UINT8)(loop_addr%242);//地址号
			
			Hex_To_BasicUC(&record.Uc[1],&uc[0],3);
			MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);
			
		
			Record_Controler_If_Event(&record);

		}
		else if(Selsct_Sta == Selsct_Start_GST200)
		{
			if(Selsct == 1)//主电状态
			{
				if(EventType == 2)
				{	
					Save_Controller_Self_Info(RECORD_FAULT,INT_TYPE_INPUT_POWER_OPEN_CIRCUIT);
				}
				else if (EventType == 9)
				{	
					Save_Controller_Self_Info(RECORD_ELIMINATE,INT_TYPE_MAINPOW_RECOVER);
				}
				
			}
			else if(Selsct == 3)//备电状态
			{
				if(EventType == 2)
				{
          Save_Controller_Self_Info(RECORD_FAULT,INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT);
				}
				else if (EventType == 9)
				{
          Save_Controller_Self_Info(RECORD_ELIMINATE,INT_TYPE_BACKUP_POWER_RECOVER);
				}
				
			}
			else if(Selsct == 5)//总线状态
			{
				if(EventType == 02)
				{
          Save_Controller_Self_Info(RECORD_FAULT,INT_TYPE_BUS_SHORT_CIRCUIT);
				}
				
			}
		}
		else 
		{
			loop_addr = Selsct_Sta - Selsct_Count + Selsct/2 + 1;
			uc[0] = System.ctr_addr[0];     //机号
			uc[1] = (UINT8)(loop_addr/240);  //回路 
			uc[2] = (UINT8)(loop_addr%240);//地址号

			Hex_To_BasicUC(&record.Uc[1],&uc[0],3);
			MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);
					
			Record_Controler_If_Event(&record);
		}
	}
	else if(Contr.Type == GST500_SF_ZJ)
	{
		if(Selsct_Sta == 4998)
		{
			loop_addr = (Selsct_Total_GST5000/Selsct_Count-1) * Selsct_Count + Selsct/2 + 1;

			uc[0] = System.ctr_addr[0];     //机号
			uc[1] = (UINT8)(loop_addr/240);  //回路 
			uc[2] = (UINT8)(loop_addr%240);//地址号

			Hex_To_BasicUC(&record.Uc[1],&uc[0],3);
			MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);
						
			Record_Controler_If_Event(&record);
			
		}
		else if(Selsct_Sta == Selsct_Start_GST5000)
		{
			if(Selsct == 1)//主电状态
			{
				if(EventType == 2)
				{				
					Save_Controller_Self_Info(RECORD_FAULT,INT_TYPE_INPUT_POWER_OPEN_CIRCUIT);
				}
				else if (EventType == 9)
				{				
					Save_Controller_Self_Info(RECORD_ELIMINATE,INT_TYPE_MAINPOW_RECOVER);
				}
				
			}
			else if(Selsct == 3)//备电状态
			{
				if(EventType == 2)
				{
          Save_Controller_Self_Info(RECORD_FAULT,INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT);
				}
				else if (EventType == 9)
				{
					Save_Controller_Self_Info(RECORD_ELIMINATE,INT_TYPE_BACKUP_POWER_RECOVER);
				}
				
			}
		}
		else 
		{
			loop_addr = Selsct_Sta - Selsct_Count + Selsct/2 + 1;
			
			uc[0] = System.ctr_addr[0];     //机号
			uc[1] = (UINT8)(loop_addr/240);  //回路 
			uc[2] = (UINT8)(loop_addr%240);//地址号

			Hex_To_BasicUC(&record.Uc[1],&uc[0],3);
			MemoryCpyInvert(&record.Uc[1],&record.Uc[1],LEN_REG_UC-1);
					
			Record_Controler_If_Event(&record);
		}
	}	
}

UINT8 GST_Modbus_Analyse(CONTR_IF *buf)
{
	UINT16 i,data_loc,fr_end_loc,cs_initial_value=0; 
	UINT8 count,data_len,frame_total_len,cs_offset_loc,ope_flag=0;
	
	if(buf->R.Clev == buf->R.Head) return 0;

	if(buf->R.Clev >= CONTR_BUF_LEN)  buf->R.Clev=0;
	i = buf->R.Clev;
	
	switch(buf->AnalyseSta)
	{
		case FRAME_HEAD:
			Watch_Dog();
			while(i != buf->R.Head)
			{
				if(buf->R.Buf[i] == CTR_NUMBER_L) 
				{
					buf->R.Clev = i;
					break;
				}
				if(++i >= CONTR_BUF_LEN) i=0;
			}
			if(i == buf->R.Head ) 
			{
				buf->R.Clev = buf->R.Head;
				buf->R.FrameEndLoc = buf->R.Head; 
				break;
			}
			Contr.ComFault.Count = 0;
			buf->AnalyseSta = FRAME_LEN;
			
		case FRAME_LEN:
			//计算帧长度所在位置
			data_loc = buf->R.Clev+FRAME_LEN_OFFSET_MBUS;
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
			if((data_len > FRAME_MIN_LEN_MBUS) 
				&& (data_len < FRAME_MAX_LEN_MBUS))
			{
				//判断该帧是否有转圈
				fr_end_loc = (buf->R.Clev+data_len+FRAME_EXCEPT_DATA_LEN_MBUS);
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
			cs_initial_value = FRAME_CS_INIT_VALUE_MBUS;
			cs_offset_loc = buf->R.FrameLen+FRAME_HEAD_LEN_MBUS +FRAME_CTR_LEN_MBUS +FRAME_LEN_BYTE_NUM_MBUS;
			frame_total_len = buf->R.FrameLen+FRAME_EXCEPT_DATA_LEN_MBUS;
			data_loc = buf->R.Clev;

			for(count=0; count<frame_total_len; count++)
			{
				buf->DAT_Return[count] = buf->R.Buf[data_loc];
				if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈

				if(count < cs_offset_loc)
				{
					cs_initial_value = Caculate_CRC(cs_initial_value,buf->DAT_Return[count]);
					//cs_initial_value += buf->DAT_Return[count];
				}
			}
			if((buf->DAT_Return[cs_offset_loc] == (UINT8)(cs_initial_value&0x00ff))
				&&(buf->DAT_Return[cs_offset_loc+1] == (UINT8)(cs_initial_value>>8)))
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
void GST_Modbus_Handle(CONTR_IF *buf)
{
	UINT8 i;
	UINT16 loop_addr=0;
	memmove(&buf->DAT_Return[0],&buf->DAT_Return[3],buf->R.FrameLen);
	for (i=1;i<buf->R.FrameLen;)
	{
		if(Contr.Type == GST200_SF_ZJ)
		{
			if(Selsct_Sta == 242)//余数状态处理
			{
				if(Selsct_Total_GST200%Selsct_Count==0)
				{
					loop_addr = (Selsct_Total_GST200/Selsct_Count-1) * Selsct_Count + i/2 + 1;
				}
				else
				{
					loop_addr = (Selsct_Total_GST200/Selsct_Count) * Selsct_Count + i/2 + 1;
				}
			}
			else if(Selsct_Sta == Selsct_Start_GST200)//主备电状态
			{
				loop_addr = 242 + i/2 + 2;
			}
			else 
			{
				loop_addr = Selsct_Sta - Selsct_Count + i/2 + 1;
			}
		}
		else if(Contr.Type == GST500_SF_ZJ)
		{
			if(Selsct_Sta == 4998)
			{
				if(Selsct_Total_GST5000%Selsct_Count ==0)
				{
					loop_addr = (Selsct_Total_GST5000/Selsct_Count-1) * Selsct_Count + i/2 + 1;
				}
				else
				{
					loop_addr = (Selsct_Total_GST5000/Selsct_Count) * Selsct_Count + i/2 + 1;
				}
			}
			else if(Selsct_Sta == Selsct_Start_GST5000)//主备电状态
			{
				loop_addr = 2400 + i/2 + 2;
			}
			else 
			{
				loop_addr = Selsct_Sta - Selsct_Count + i/2 + 1;
			}
		}
		
		if(loop_addr%2)//前4位放奇数节点状态，后4位放偶数节点状态
		{
			if(buf->DAT_Return[i]!= (Selsct_Sta_Save[loop_addr/2]&0x0F))
			{
				if( (buf->DAT_Return[i]>0) && (buf->DAT_Return[i]<10))
			   	{
					GST_Modbus_Save_Event(i,buf->DAT_Return[i]);
			   	}
			}
			Selsct_Sta_Save[loop_addr/2] = buf->DAT_Return[i]+(Selsct_Sta_Save[loop_addr/2]&0xf0);//存放节点状态
		}
		else
		{
			if(buf->DAT_Return[i]!= (Selsct_Sta_Save[loop_addr/2]>>4))
			{
				if( (buf->DAT_Return[i]>0) && (buf->DAT_Return[i]<10))
				{
					GST_Modbus_Save_Event(i,buf->DAT_Return[i]);
				}
			}
			Selsct_Sta_Save[loop_addr/2] = (buf->DAT_Return[i]<<4)+(Selsct_Sta_Save[loop_addr/2]&0x0f);
		}
		i+=2;

	}
	buf->R.Clev = buf->R.FrameEndLoc;
	buf->R.FrameLen = 0;
	Contr.ComFault.Count = 0;
	//Poll_Controller_Init(buf);
	
}

void GST_Modbus_Send_Poll(CONTR_IF *buf)
{
		UINT8 start,i;
		UINT16 cs;
		start = buf->T.Tail;

		if(Contr.Type == GST200_SF_ZJ)//200点与5000点主备电寄存器不一致所以分开写
		{
			buf->T.Buf[start] = CTR_NUMBER_L;//控制器机号
			buf->T.Buf[start+1] = 0x03;//命令码
			buf->T.Buf[start+2] = (UINT8)((Selsct_Sta & 0xFF00)>>8);
			buf->T.Buf[start+3] = (UINT8)(Selsct_Sta & 0x00FF);//起始位置
			buf->T.Buf[start+4] = 0;
			if(Selsct_Powsta)//查询主备电状态
			{
				buf->T.Buf[start+5] = 3;
				Selsct_Powsta = 0;
				Selsct_Sta = Selsct_Start_GST200;
			}
			else
			{
				if(Selsct_Sta>Selsct_Total_GST200-Selsct_Count)
				{
					buf->T.Buf[start+5] = (UINT8)(Selsct_Total_GST200 - Selsct_Sta);//余数处理	
				}
				else
				{
					buf->T.Buf[start+5] = Selsct_Count;	//长度最大100
				}
				Selsct_Sta += Selsct_Count;
				if(Selsct_Sta >= Selsct_Total_GST200)
				{
					Selsct_Powsta = 1;
					Selsct_Sta = 242;//242、243、244 主电、备电、总线状态
				}
			}
			cs=0XFFFF;
			for(i=0;i<6;i++)
			{
				cs = Caculate_CRC(cs,buf->T.Buf[start+i]);//CRC校验
			}
			buf->T.Buf[start+6] = (UINT8)(cs&0x00FF);
			buf->T.Buf[start+7] = (UINT8)((cs&0xFF00)>>8);
			
			buf->T.Tail += 8;
		}
		else if(Contr.Type == GST500_SF_ZJ)
		{
			buf->T.Buf[start] = CTR_NUMBER_L;
			buf->T.Buf[start+1] = 0x03;
			buf->T.Buf[start+2] = (UINT8)((Selsct_Sta & 0xFF00)>>8);
			buf->T.Buf[start+3] = (UINT8)(Selsct_Sta & 0x00FF);
			buf->T.Buf[start+4] = 0;
			if(Selsct_Powsta)
			{
				buf->T.Buf[start+5] = 2;
				Selsct_Powsta = 0;
				Selsct_Sta = Selsct_Start_GST5000;
			}
			else
			{
				if(Selsct_Sta>Selsct_Total_GST5000-Selsct_Count)
				{
					buf->T.Buf[start+5] = (UINT8)(Selsct_Total_GST5000 - Selsct_Sta);	
				}
				else
				{
					buf->T.Buf[start+5] = Selsct_Count;	
				}
				Selsct_Sta += Selsct_Count;
				if(Selsct_Sta >= Selsct_Total_GST5000)
				{
					Selsct_Powsta = 1;
					Selsct_Sta = 4998;//4998、4999主电、备电状态
				}
			}
			cs=0xFFFF;
			for(i=0;i<6;i++)
			{
				cs = Caculate_CRC(cs,buf->T.Buf[start+i]);
			}
			buf->T.Buf[start+6] = (UINT8)(cs&0x00FF);
			buf->T.Buf[start+7] = (UINT8)((cs&0xFF00)>>8);
			
			buf->T.Tail += 8;
		}

}
void Com_Task_GST_Modbus(CONTR_IF *buf)
{
	if(GST_Modbus_Analyse(buf))
	{
		GST_Modbus_Handle(buf);
		Monitor_Controller_Fault_Eliminate();
		On_Led_Contor();
	}
	
	if(B_Poll_Ctr)
	{
		GST_Modbus_Send_Poll(buf); 
		B_Poll_Ctr = 0;
	}
}
