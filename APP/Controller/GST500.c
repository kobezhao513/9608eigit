#include "gst500.h"
#include "controller.h"
#include "gst200.h"
#include "xycnbus.h"
#include "record_manage.h"
#include "record_status.h"
#include "basic.h"
#include "sys_init.h"
#include "led.h"
//UINT8 LAST_EVENT[6];
UINT8 LAST_CTR_STA;
//===============================================================================
//函数描述: 准备应答数据 
//输入参数: buf为控制器接口;data为应答数据
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Send_Answer_Frame_GST500(CONTR_IF *buf,UINT8 data)
{
	buf->T.Buf[buf->T.Tail] = data;
	buf->T.Tail += 1;
}
//===============================================================================
//函数描述: 存储部件信息
//输入参数: buf为输入数据
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Save_Alarm_Info_GST500(UINT8 *buf)
{
	UINT8 device_type;
	RECORD_STRUCT record;
	memset(&record.Sta, 0xFF, sizeof(record));

    if(*(buf+5)==0x01)
    {
    	if((*(buf+4)<=0x11)||((*(buf+4)<=0x90)&&(*(buf+4)>=0x80)))//火警
    	{
    		record.Type = RECORD_FIRE;
			record.EventType.word = INT_TYPE_ALARM;//火警报警
    	}
		else if(*(buf+4)==0x29)//压力开关
		{
			record.Type = RECORD_MONITER;
			record.EventType.word = INT_TYPE_MONITOR_HIGH_VOLTAGE;//过压
		}
		else if(*(buf+4)==0x30)//水流指示
		{
			record.Type = RECORD_MONITER;
			record.EventType.word = INT_TYPE_MONITOR_HIGH_CURRENT;//过流
		}
		else if(*(buf+4)==0x32)//空调机组
		{
			record.Type = RECORD_MONITER;
			record.EventType.word = INT_TYPE_MONITOR_POWER_INTERRUPT;//供电中断
		}
		else if(*(buf+4)==0x48)//防盗开关
		{
			record.Type = RECORD_MONITER;
			record.EventType.word = INT_TYPE_MONITOR_THEFT_ALARM;//盗警
		}
		else if(*(buf+4)==0x69)//可燃气体
		{
			record.Type = RECORD_MONITER;
			record.EventType.word = INT_TYPE_MONITOR_GAS_LEAKAGE_ALARM;//燃气泄漏
		}
		else//反馈
		{
			record.Type = RECORD_ANSWER;
			record.EventType.word = INT_TYPE_DEVICE_START_FEED_SUC;
		}
    }
	else if((*(buf+5)==0x02)&&(*(buf+4)>=0x12))//启动
	{
		record.Type = RECORD_START;
		record.EventType.word = INT_TYPE_REQ_START;
	}
	else if((*(buf+5)==0x03)&&(*(buf+4)>=0x12))//停动
	{
		record.Type = RECORD_STOP;
		record.EventType.word = INT_TYPE_REQ_STOP;
	}
	else if((*(buf+5)==0x00)||(*(buf+5)==0x10))//故障排除
	{
		record.Type = RECORD_ELIMINATE;
		record.EventType.word = INT_TYPE_DEV_RECOVER_NOMAL;
	}
	
	device_type =BCD2Hex(*(buf+4));
	record.Uc[LEN_REG_UC-5] = Change_UC_Type_GST200(device_type);     //设备类型
	record.Uc[LEN_REG_UC-1] = System.ctr_addr[0];  //机号
	record.Uc[LEN_REG_UC-2] = *(buf+1);  //楼层号
	record.Uc[LEN_REG_UC-3] = *(buf+2);
	record.Uc[LEN_REG_UC-4] = *(buf+3);//前一位为楼号，后三位为房间号
	
	Record_Controler_If_Event(&record);	
}
//===============================================================================
//函数描述: 存储故障信息
//输入参数: buf为输入数据
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Save_Fault_Info_GST500(UINT8 *buf)
{
	UINT8 device_type;
	RECORD_STRUCT record;
	memset(&record.Sta, 0xFF, sizeof(record));

    if((*(buf+5)==0x00 )|| (*(buf+5)==0x20))//故障
	{
		record.Type = RECORD_FAULT;
		record.EventType.word = INT_TYPE_DEV_FAULT;//设备故障
	}
	else if(*(buf+5)==0x01)//屏蔽
	{
		record.Type = RECORD_SHIELD;
		record.EventType.word = INT_TYPE_REQ_SHIELD;
	}
	else if((*(buf+5)==0x02 )||(*(buf+5)==0x10))//解除屏蔽
	{
		record.Type = RECORD_RELEASE;
		record.EventType.word = INT_TYPE_REQ_RELEASE;
	}
	else if(*(buf+5)==0x03)//延时
	{
		record.Type = RECORD_DELAY;
		record.EventType.word = INT_TYPE_MANUAL_DELAY_START;
	}
	else if(*(buf+5)==0x04)//延时取消
	{
		record.Type = RECORD_CANCEL;
		record.EventType.word = INT_TYPE_DELAY_SUC;
	}
	else if(*(buf+5)==0x05)//自检
	{
		record.Type = REOCRD_SELFCHECK;
		record.EventType.word = INT_TYPE_DEVICE_SELFCHECK;
	}
	else if(*(buf+5)==0x06)//自检结束
	{
		record.Type = REOCRD_SELFCHECK;
		record.EventType.word = INT_TYPE_SELFCHECK_OVER;
	}
	
	device_type =BCD2Hex(*(buf+4));
	record.Uc[LEN_REG_UC-5] = Change_UC_Type_GST200(device_type);     //设备类型
	record.Uc[LEN_REG_UC-1] = System.ctr_addr[0];  //机号
	record.Uc[LEN_REG_UC-2] = *(buf+1);  //楼层号
	record.Uc[LEN_REG_UC-3] = *(buf+2);
	record.Uc[LEN_REG_UC-4] = *(buf+3);//前一位为楼号，后三位为房间号
	if(device_type==98)//监控出厂时GST500控制器默认98
	{
		MemoryCpyInvert(record.Uc,System.ctr_addr,LEN_REG_UC);
	}

	Record_Controler_If_Event(&record);
	
}


//===============================================================================
//函数描述: 存储其他信息
//输入参数: buf为输入数据
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
void Save_Other_Info_GST500(UINT8 *buf)
{
	if(LAST_CTR_STA != (*(buf+1)))
	{
		if(((*(buf+1)>>7)==0x01)&&((*(buf+1)>>7)!=(LAST_CTR_STA>>7)))//自动允许
		{
			Save_Controller_Self_Info(REOCRD_MODE,INT_TYPE_AUTO_CONTROL_ALLOW);
		}
		if(((*(buf+1)>>7)==0x00)&&((*(buf+1)>>7)!=(LAST_CTR_STA>>7)))//自动禁止
		{
			Save_Controller_Self_Info(REOCRD_MODE,INT_TYPE_AUTO_CONTROL_FORBID);
		}
		if((((*(buf+1)&0x40)>>6)==0x01)&&(((*(buf+1)&0x40)>>6)!=((LAST_CTR_STA&0x40)>>6)))//手动允许
		{
			Save_Controller_Self_Info(REOCRD_MODE,INT_TYPE_MANUAL_CONTROL_ALLOW);
		}
		if((((*(buf+1)&0x40)>>6)==0x00)&&(((*(buf+1)&0x40)>>6)!=((LAST_CTR_STA&0x40)>>6)))//手动禁止
		{
			Save_Controller_Self_Info(REOCRD_MODE,INT_TYPE_MANUAL_CONTROL_FORBID);
		}
		if((((*(buf+1)&0x20)>>5)==0x01)&&(((*(buf+1)&0x20)>>5)!=((LAST_CTR_STA&0x20)>>5)))//喷洒允许
		{
			Save_Controller_Self_Info(REOCRD_MODE,INT_TYPE_SPRAY_ALLOW);
		}
		if((((*(buf+1)&0x20)>>5)==0x00)&&(((*(buf+1)&0x20)>>5)!=((LAST_CTR_STA&0x20)>>5)))//喷洒禁止
		{
			Save_Controller_Self_Info(REOCRD_MODE,INT_TYPE_SPRAY_FORBID);
		}

		
		if((*(buf+1)&0x0f)==0x01)//主电故障
		{
			Save_Controller_Self_Info(RECORD_FAULT,INT_TYPE_INPUT_POWER_OPEN_CIRCUIT);
		}
		if((*(buf+1)&0x0f)==0x02)//主电恢复
		{
			Save_Controller_Self_Info(RECORD_ELIMINATE,INT_TYPE_MAINPOW_RECOVER);
		}
		if((*(buf+1)&0x0f)==0x03)//备电故障
		{
			Save_Controller_Self_Info(RECORD_FAULT,INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT);
		}
		if((*(buf+1)&0x0f)==0x04)//备电恢复
		{
			Save_Controller_Self_Info(RECORD_ELIMINATE,INT_TYPE_BACKUP_POWER_RECOVER);
		}
		LAST_CTR_STA = (*(buf+1));
	}
}
//===============================================================================
//函数描述: 接收数据分析--主叫模式
//输入参数: buf为控制器接口
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
UINT8 Analyse_GST500_Zj(CONTR_IF *buf)
{
	UINT16 i,data_loc=0,fr_start_loc=0,fr_end_loc=0; 
	UINT8 data_len=0,ope_flag=0,cs=0,fr_end_flag=0,fr_start_flag=0;
	
	if(buf->R.Clev == buf->R.Head) 
	{
		if(Gst.gst500_timeout > 25)//100MS超时监控
		{
			Gst.gst500_poll_process = SEND_OVER;
		}
		return ope_flag;
	}
	if(buf->R.Clev >= CONTR_BUF_LEN)  buf->R.Clev=0;
	i = buf->R.Clev;

	if(Gst.gst500_poll_process == REC_CTR_ADDR)//接收控制器地址
	{
		if(buf->R.Head == 0)//主叫模式只处理最新节拍
		{
			buf->R.Clev = CONTR_BUF_LEN;
		}
		else
		{
			buf->R.Clev = buf->R.Head-1;
		}

		if(buf->R.Buf[buf->R.Clev] == CTR_NUMBER_L) 
		{
			ope_flag = POLL_ANS;
			buf->R.Clev = buf->R.Head;
		}
		else if(Gst.gst500_timeout  > 25)//接收超时100ms
		{
		    Gst.gst500_poll_process = SEND_OVER;
			buf->R.Clev = buf->R.Head;
		}
		
	}
	else if(Gst.gst500_poll_process == REC_EVENT)//接收事件帧
	{
		while(i != buf->R.Head)
		{
			if(!fr_start_flag)
			{
				if((buf->R.Buf[i] == 0xdd) || (buf->R.Buf[i] == 0xee))//查找帧头
				{
					fr_start_loc = i; 
					fr_start_flag = 1;
				}
			}
			if(buf->R.Buf[i] == FRAME_TAIL_GST500) //查找帧尾
			{
				fr_end_loc = i;
				fr_end_flag = 1;
				break;
			}
			if(++i >= CONTR_BUF_LEN) i=0;
		}

	    if(fr_start_flag)
		{
			if(fr_end_flag)//找到帧头和帧尾
			{
				if(fr_end_loc > fr_start_loc)
				     data_len = fr_end_loc - fr_start_loc - 2;
				else
					 data_len = CONTR_BUF_LEN + fr_end_loc - fr_start_loc - 2;
				if( data_len == 5)//报警或故障帧长度
				{
					buf->DAT_Return[0] = buf->R.Buf[fr_start_loc];//校验无帧头
					data_loc = fr_start_loc+FRAME_HEAD_LEN_GST500;
					if( data_loc >= CONTR_BUF_LEN)
					{
						data_loc -= CONTR_BUF_LEN;
					}
					for(i=0;i<data_len;i++)
					{
						cs += buf->R.Buf[data_loc]; 	
						buf->DAT_Return[i+1] = buf->R.Buf[data_loc];
						data_loc++;
						if( data_loc >= CONTR_BUF_LEN)
						{
							data_loc -= CONTR_BUF_LEN;
						}
					}
					if(cs == buf->R.Buf[data_loc])//校验
					{
						ope_flag = EVENT_ANS;
						buf->R.FrameEndLoc = fr_end_loc;
						buf->R.FrameLen = 6;
						buf->R.Clev = buf->R.Head;
					}
					else
					{
						buf->R.Clev = fr_end_loc;
						Gst.gst500_poll_process = SEND_OVER;
						buf->R.Clev = buf->R.Head;
					}
				}
				else//长度收错
				{
					buf->R.Clev = fr_end_loc;
					Gst.gst500_poll_process = SEND_OVER;
				}
			}
			else//找到帧头，没找到帧尾，继续收帧
			{
				if(Gst.gst500_timeout  > 25)//超时监控
				{
					Gst.gst500_poll_process = SEND_OVER;
					buf->R.Clev = buf->R.Head;
				}
			}
		}
		else
		{
			if(fr_end_flag)//没有帧头，但是收到帧尾
    		{
				if(fr_end_loc < 3)
				{
					buf->DAT_Return[0] = buf->R.Buf[CONTR_BUF_LEN+fr_end_loc-3];
					if(fr_end_loc < 2)
					{
						buf->DAT_Return[1] = buf->R.Buf[CONTR_BUF_LEN+fr_end_loc - 2];
						if(fr_end_loc < 1)
						{
							buf->DAT_Return[2] = buf->R.Buf[CONTR_BUF_LEN+fr_end_loc - 1];
						}
					}
					else
					{
						buf->DAT_Return[1] = buf->R.Buf[fr_end_loc - 2];
						buf->DAT_Return[2] = buf->R.Buf[fr_end_loc - 1];
					}					
				}
				else
				{
					buf->DAT_Return[0] = buf->R.Buf[fr_end_loc - 3];
					buf->DAT_Return[1] = buf->R.Buf[fr_end_loc - 2];
					buf->DAT_Return[2] = buf->R.Buf[fr_end_loc - 1];
				}

				if(buf->DAT_Return[0] == 0xcb)
				{
					ope_flag = EVENT_ANS;
					buf->R.FrameLen = 2;
					buf->R.Clev = buf->R.Head;
				}
				else if((buf->DAT_Return[2] == 0xf0)||(buf->DAT_Return[2] == 0xaa)||(buf->DAT_Return[2] == 0xab))//控制器自身消息
	    		{
	    			buf->DAT_Return[0]= buf->DAT_Return[2];
	    			ope_flag = EVENT_ANS;
					buf->R.FrameLen = 1;
					buf->R.Clev = buf->R.Head;
	    		}
				else
				{
					Gst.gst500_poll_process = SEND_REC_SUCC;//直接回应答
					buf->R.Clev  = fr_end_loc;
					buf->R.Clev = buf->R.Head;
				}
				
    		}
			else//没有帧头和帧尾，等待帧头或帧尾
			{
				if(Gst.gst500_timeout  > 25)//超时监控
				{
					Gst.gst500_poll_process = SEND_OVER;
					buf->R.Clev = buf->R.Head;
				}
			}
		}
		
	}
	return ope_flag;
}

 
//===============================================================================
//函数描述: 通讯任务--主叫模式，海湾500控制器通信
//输入参数: buf为控制器接口
//输出参数: 无
//修改日期: 2016-10
//修改人员: 杨莫然
//===============================================================================
void Com_Task_GST500_Zj(CONTR_IF *buf)
{
	switch(Gst.gst500_poll_process)
	{
		case SEND_CTR_ADDR:
			Send_Answer_Frame_GST500(buf,CTR_NUMBER_L);
			Gst.gst500_poll_process = SEND_HEAD;
		break;
		case SEND_HEAD:
			Send_Answer_Frame_GST500(buf,0x00);
			Gst.gst500_poll_process = REC_CTR_ADDR;
			Gst.gst500_timeout  = 0;
			
		case REC_CTR_ADDR:
		if(Analyse_GST500_Zj(buf)==POLL_ANS)
		{
			GST_Uart_Init_Change(SPACE);			
		    Gst.gst500_poll_process = SEND_ANS;
			On_Led_Contor();
		}
		break;	
		case SEND_ANS:
			Send_Answer_Frame_GST500(buf,0x0F);
			Gst.gst500_poll_process = REC_EVENT;
			Gst.gst500_timeout  = 0;
		break;
		case REC_EVENT:
			if(Analyse_GST500_Zj(buf)==EVENT_ANS)
			{
			    Handle_GST500_Jt(buf);
				Monitor_Controller_Fault_Eliminate();
				//Gst.gst500_poll_process = SEND_REC_SUCC;
				Send_Answer_Frame_GST500 (buf,0x1F);//直接回应答
				Gst.gst500_poll_process = SEND_OVER;
				B_Poll_Ctr = 0; 
			}
		break;
		case SEND_REC_SUCC:
			Send_Answer_Frame_GST500 (buf,0x1F);
			Gst.gst500_poll_process = SEND_OVER;
			B_Poll_Ctr = 0; 
		break;
		case SEND_OVER:
			
			if(B_Poll_Ctr)
			{
				B_Poll_Ctr =0;
			    Gst.gst500_poll_process = SEND_CTR_ADDR;
				GST_Uart_Init_Change(MARK);
			}
			else if(B_Reset_Ctr)//控制器复位
			{	
				GST_Uart_Init_Change(MARK);
				Send_Answer_Frame_GST500(buf,CTR_NUMBER_L);
			    Send_Answer_Frame_GST500(buf,0x5F);
				B_Reset_Ctr = 0;
				buf->PollTimer = 0;
			}
			else if(B_Silence_Ctr)//控制器消音
			{
				GST_Uart_Init_Change(MARK);			    
				Send_Answer_Frame_GST500(buf,CTR_NUMBER_L);
			    Send_Answer_Frame_GST500(buf,0x1B);
				B_Silence_Ctr = 0;
				buf->PollTimer = 0;
			}
		break;
	    default:
		    Gst.gst500_poll_process = SEND_CTR_ADDR;
		break;

	}
}

void Handle_GST500_Jt(CONTR_IF *buf)
{
	//if(memcmp(&buf->DAT_Return[0],LAST_EVENT,buf->R.FrameLen))
	//{
		if(buf->DAT_Return[0]==0xdd)//报警
		{
			Save_Alarm_Info_GST500(&buf->DAT_Return[0]);	
		}
		else if(buf->DAT_Return[0]==0xee)//故障
		{
			Save_Fault_Info_GST500(&buf->DAT_Return[0]);
		}
		else if(buf->DAT_Return[0]==0xcb)//控制器自身
		{
			Save_Other_Info_GST500(&buf->DAT_Return[0]);
		}
		else if(buf->DAT_Return[0]==0xaa)//复位
		{
			Save_Controller_Self_Info(RECORD_RESET,INT_TYPE_RESET_SUCESS);
		}
		else if(buf->DAT_Return[0]==0xab)//消音
		{
			Save_Controller_Self_Info(RECORD_OTHERS,INT_TYPE_DEVICE_SILENCE);
		}
		//memcpy(LAST_EVENT,&buf->DAT_Return[0],buf->R.FrameLen);
		//buf->R.Clev = buf->R.FrameEndLoc;
	//}
	if(Contr.Type == GST500_CRT_JT)
	{
		buf->R.Clev = buf->R.FrameEndLoc;
	}
	Contr.ComFault.Count = 0;
	buf->R.FrameLen = 0;
	Poll_Controller_Init(buf);
}
//===============================================================================
//函数描述: 接收数据分析--监听模式
//输入参数: buf为控制器接口
//输出参数: 无
//修改日期: 2016-10
//修改人员: 
//===============================================================================
UINT8 Analyse_GST500_Jt(CONTR_IF *buf)
{
	UINT16 i,data_loc,fr_end_loc; 
	UINT8 count,data_len,frame_total_len,cs_offset_loc,ope_flag=0,cs_initial_value=0;

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
				if((buf->R.Buf[i] == 0xdd) || (buf->R.Buf[i] == 0xee)||(buf->R.Buf[i] == 0xcb)||(buf->R.Buf[i] == 0xaa)||(buf->R.Buf[i] == 0xf0)||(buf->R.Buf[i] == 0xab))//事件应答
				{
					buf->R.Clev = i; 
					break;
				}
				if(++i >= CONTR_BUF_LEN) i=0;
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
			if(buf->R.Buf[i] == 0xf0)
			{
				data_len = 0;
			}

			else if((buf->R.Buf[i] == 0xdd) || (buf->R.Buf[i] == 0xee))
			{
				data_len = 5;
			}
			else if(buf->R.Buf[i] == 0xcb)
			{
				data_len = 1;
			}
			else if(buf->R.Buf[i] == 0xaa)
			{
				data_len = 0;
			}
			else if(buf->R.Buf[i] == 0xab)
			{
				data_len = 0;
			}
			//加入其它帧
			
			//提取帧长度//可加入其它限制
			if( data_len < FRAME_MAX_LEN_GST500)
			{
				//判断该帧是否有转圈
				if(data_len == 0)//无校验
				{
					fr_end_loc = (buf->R.Clev+data_len+FRAME_EXCEPT_DATA_LEN2_GST500);
				}
				else
				{
					fr_end_loc = (buf->R.Clev+data_len+FRAME_EXCEPT_DATA_LEN_GST500);
				}
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
			if(fr_end_loc > buf->R.Clev)
			{
				if((buf->R.Head > buf->R.Clev) && (buf->R.Head < fr_end_loc)) break ;
			}
			else
			{
				if((buf->R.Head > buf->R.Clev) || (buf->R.Head < fr_end_loc)) break ;
			}
			buf->AnalyseSta = FRAME_CS;
			
		case FRAME_CS:   //校验
			Watch_Dog();
			//计算CRC校验
			if(buf->R.FrameLen==0)//复位、消音、巡检帧处理
			{

				data_loc = buf->R.Clev;
				buf->DAT_Return[0]= buf->R.Buf[data_loc];
				if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈
				if(buf->R.Buf[data_loc] == FRAME_TAIL_GST500)
				{ 
					if(data_loc == 0)
					{
						data_loc = CONTR_BUF_LEN;
					}
					else
						data_loc -- ;
					if((buf->R.Buf[data_loc] == 0xf0)||(buf->R.Buf[data_loc] == 0xaa)||(buf->R.Buf[data_loc] == 0xab))//控制器自身消息
		    		{
		    			buf->DAT_Return[0]= buf->R.Buf[data_loc];
						buf->R.FrameLen = 1;
						ope_flag = 0x55;
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
			}
			else//带CS事件帧处理
			{
				cs_initial_value = FRAME_CS_INIT_VALUE_GST500;
				cs_offset_loc = buf->R.FrameLen+FRAME_HEAD_LEN_GST500;
				frame_total_len = buf->R.FrameLen+FRAME_HEAD_LEN_GST500;
				data_loc = buf->R.Clev;
				if(buf->R.Buf[data_loc]==0xcb)
				{
					if(buf->R.Buf[data_loc+FRAME_EXCEPT_DATA_LEN_GST500]!=FRAME_TAIL_GST500)
					{
						buf->R.Clev++;
						buf->AnalyseSta = FRAME_HEAD;
						break;
					}
				}
				
				if( data_loc >= CONTR_BUF_LEN)
				{
					data_loc -= CONTR_BUF_LEN;
				}
				for(count=0; count<frame_total_len; count++)
				{
					buf->DAT_Return[count] = buf->R.Buf[data_loc];
					if(++data_loc >= CONTR_BUF_LEN) data_loc = 0;//判断是否有转圈
			
					if((count >= FRAME_CS_START_OFFSET_GST500) && (count < cs_offset_loc))
					{
						cs_initial_value += buf->DAT_Return[count];
					}
				}
				if(buf->R.Buf[data_loc] == cs_initial_value)
				{
					ope_flag = 0x55;
				}
				else
				{
					buf->R.Clev++;
					memset(buf->DAT_Return,0x00,sizeof(buf->DAT_Return));
				}
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
//函数描述: 通讯任务--监听模式，海湾500控制器通信
//输入参数: buf为控制器接口
//输出参数: 无
//修改日期: 2016-10
//修改人员: 杨莫然
//===============================================================================
void Com_Task_GST500_Jt(CONTR_IF *buf)
{

	if(Analyse_GST500_Jt(buf))
	{
		Handle_GST500_Jt(buf);
		Monitor_Controller_Fault_Eliminate();
		On_Led_Contor();
	}

}
