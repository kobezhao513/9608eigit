#include "controller.h"
#include "xycnbus.h"
#include "record_manage.h"
#include "record_status.h"
#include "uart.h"
#include "led.h"
#include "ex_flash.h"
#include "sys_init.h"
#include "EI_Print_COMM.h"



//#include "EI2000.h"
//#include "gst200.h"
//#include "qn5010.h"
//#include "gst500.h"
#include "ts.h"
//#include "ld128en.h"
//#include "gst_mbus.h"
//#include "tc5120.h"
//#include "gt603.h"
//#include "nt8003.h"
//#include "sj9102ba.h"
//#include "tanda3004.h"
//#include "forsafe.h"
//#include "fhsj9108.h"
//#include "simplex.h"
//#include "qn_193k.h"
//#include "LD128EN_PRT.h"
//#include "TX3607.h"
//#include "n3030_old.h"
//#include "PTW3300.h"

//BIT_CHAR Ctr_Task;

UINT8 Last_Uc[LEN_REG_UC];
UINT16 Last_Event_Type;
UINT32 Last_Event_Time;
UINT16 len = 0;

UINT8 CenterCmd_Erease_Flash_Flag = FALSE;
UINT8 CenterCmd_Rst_Flag = FALSE;


DEBUG Debug;
CONTR Contr;
CONTR_IF RS232;
CONTR_IF RS485;
COMPETE_TO_CONTR Compete_Event;


//===============================================================================
//修改日期: 2016-09
//修改人员: 刘芳
//===============================================================================
UINT16 HexToBCD(UINT8 hex)   //十六进制转BCD    
{   
    UINT16 rslt=0;
	UINT8 temp;
	rslt = ((hex/100)<<8);
	
	temp = (hex%100);
	rslt += (((temp/10)<<4)+(temp%10)); 
       
    return(rslt);   
} 

void Hex_To_BasicUC(UINT8 *uc,UINT8 *hex,UINT8 len)
{
    UINT8 temp;
	UINT16 i;
	*uc  = Hex2BCD(*hex);

	i = HexToBCD(*(hex+1));
	*(uc+1) = (UINT8)(i>>4);

 	temp = (UINT8)((i&0x0f)<<4);
	if(len == 2)
	{
		temp |= 0x0f;	
		*(uc+2) = temp;	
	}
	else if(len == 3)
	{
		i = HexToBCD(*(hex+2));
		*(uc+3) = (UINT8)(i&0xff);
		temp |= (UINT8)(i>>8);
		*(uc+2) = temp;	
	} 
}
void Controller_Uart_Init(void)
{
	//串口主模式初始化	
	Contr_485Uart_Init(4);
	//Contr_485Uart_Sta_Switch(UART_REC_INIT);	
	Contr_485Uart_Sta_Switch(UART_SEND_INIT);
	
	Contr_232Uart_Init(4);	
	Contr_232Uart_Sta_Switch(UART_SEND_INIT);


}

void Poll_Controller_Init(CONTR_IF *buf)
{
	buf->PollTimer = 0;

	buf->T.RepFlag = 0;
	buf->T.RepNum = 0;
	buf->T.RepCounter = 0;
}

void Controller_Data_Init(void)
{
	memset(&RS232,0x00,sizeof(RS232));
	memset(&RS485,0x00,sizeof(RS485));
	Parallel_Init();

//	Contr.ComFault.Flag = 0;
//	Contr.ComFault.Count = 0;	
//	Contr.Task.byte = 0;
////	Gst.init_mode = MARK;
//	
//	Compete_Event.Event_Pc = RecordInf.SavePc;
//	if(System.trans_equip_connect)
//	{
//		B_Upload_Self_Info = 1;
//	}

//    //需加隐形菜单，设置协议类型，放入系统配置
//	Contr.Type = System.ctr_type;

//	switch(Contr.Type)
//	{
//		case GST200_CRT_ZJ:
//		case GST500_CRT_ZJ:
//		case QN5010_CRT_ZJ:
//			RS232.PollEnFlag = 1;
//			B_Poll_Ctr = 0;
//		break;
//		case EI2000_CRT_ZJ:
//		case LD128EN_CRT_485_ZJ:
//		case LD128EN_CRT_232_ZJ:
//		case QN_193K_SF_ZJ:
//			RS232.PollEnFlag = 1;
//			B_Poll_Ctr = 1;
//		break;
//		case GST200_SF_ZJ:
//			RS232.PollEnFlag = 1;
//			B_Poll_Ctr = 0;
//			Selsct_Sta = Selsct_Start_GST200;
//		break;
//		case GST500_SF_ZJ:
//			RS232.PollEnFlag = 1;
//			B_Poll_Ctr = 0;
//			Selsct_Sta = Selsct_Start_GST5000;
//		break;
//		case TS:
//			memset(&TS_Far,0x00,sizeof(TS_Far));
//			B_Upload_Self_Info = 1;
//		break;
//		default:
//		break;
//	}
//	if(Contr.Type == EI2000_CRT_ZJ)
//	{
//		Contr.ComFault.Time = EI_CONTR_OFFLINE_TIMER;
//	}
//	else
//	{
//		Contr.ComFault.Time = CONTR_OFFLINE_TIMER;
//	}	
}
void Save_Trans_Equip_Info(UINT8 record_type,UINT16 detailed_type)
{
	RECORD_STRUCT record;

	memset(&record.Sta, 0xFF, sizeof(record));

	record.EventType.word = detailed_type;
	record.Type = record_type;
	MemoryCpyInvert(record.Uc,System.trans_addr,LEN_REG_UC);
	sprintf((char *)record.Cc, "模块 LA:%d",System.la);
	MemoryCpyInvert(record.Cc,record.Cc,LEN_REG_CC);

	Record_Controler_If_Event(&record);

}

void  Monitor_Trans_Equip_Offline(void)
{
	if(System.trans_equip_connect)
	{
		Compete_Event.ComFault.Count++;
		if(Compete_Event.ComFault.Flag == 0)
		{
			if(Compete_Event.ComFault.Count >= 60)
			{
				On_Led_Error();
				Save_Trans_Equip_Info(RECORD_FAULT,INT_TYPE_REG_DEV_OFFLINE);
				
				Compete_Event.ComFault.Flag = 1; 
				Compete_Event.ComFault.Count = 0;

			} 
		}
	}

}
void Monitor_Trans_Equip_Fault_Eliminate(void)
{
	if(System.trans_equip_connect)
	{
		Compete_Event.ComFault.Count = 0;
		if(Compete_Event.ComFault.Flag)
		{
			Save_Trans_Equip_Info(RECORD_ELIMINATE,INT_TYPE_REG_DEV_ONLINE);
			Compete_Event.ComFault.Flag = 0;
			Off_Led_Error();
		}
	}
}



//void  Monitor_Controller_Offline(void)
//{
//	if((Contr.Type == TC5120_PRT_JT) || (Contr.Type == GT603_SF_ZJ)
//	|| (Contr.Type == TANDA3004_PRT) || (Contr.Type == QN_193K_SF_ZJ)
//	||(Contr.Type == LD128EN_PRT)||(Contr.Type == TX3607_FW19000_485)
//	||(Contr.Type == TX3607_FW19000_232)||(Contr.Type == PTW3300_JT)
//	||(Contr.Type == N3030_OLD)) return;
//	if(Contr.ComFault.Flag == 0)
//	{
//		if(Contr.ComFault.Count >= Contr.ComFault.Time)
//		{
////			Off_Led_Contor();
////			On_Led_Error();
//			Save_Controller_Self_Info(RECORD_FAULT,INT_TYPE_REG_DEV_OFFLINE);
//	
//			Controller_Data_Init();//????????,??????????
//			Controller_Uart_Init();	
//			
//			Contr.ComFault.Flag = 1; 
//			Contr.ComFault.Count = 0;

//		} 
//	}

//}

void Monitor_Controller_Fault_Eliminate(void)
{
//	Contr.ComFault.Count = 0;
//	if(Contr.ComFault.Flag)
//	{
//		Save_Controller_Self_Info(RECORD_ELIMINATE,INT_TYPE_REG_DEV_ONLINE);
//		Contr.ComFault.Flag = 0;
//		Off_Led_Error();
//	}
}
void Monitor_Controller_Uart(CONTR_IF *buf)
{	
//	switch(Contr.Type)
//	{
//		case GST200_CRT_ZJ:
//		case GST500_CRT_ZJ:
//			if(buf->PollTimer > GST_POLL_PERIOD300MS)
//			{
//				B_Poll_Ctr = 1; 
//				buf->PollTimer = 0;
//			}
//		break;
//		case EI2000_CRT_ZJ:
//			if(buf->PollTimer > EI_POLL_PERIOD30S)
//			{
//				B_Poll_Ctr = 1; 
//				buf->PollTimer = 0;
//			}
//		break;
//		case QN5010_CRT_ZJ:
//			if(buf->PollTimer > QN_POLL_PERIOD1S)
//			{
//				B_Poll_Ctr = 1; 
//				buf->PollTimer = 0;
//			}
//		break;
//		case LD128EN_CRT_232_ZJ:
//		case LD128EN_CRT_485_ZJ:
//			if(buf->PollTimer > LD128EN_CRT_ZJ_POLL_PERIOD1S)
//			{
//				B_Poll_Ctr = 1; 
//				buf->PollTimer = 0;
//			}
//		break;
//		case GST200_SF_ZJ:
//		case GST500_SF_ZJ:
//			if(buf->PollTimer > GST_POLL_PERIOD600MS)
//			{
//				B_Poll_Ctr = 1; 
//				buf->PollTimer = 0;
//			}
//		break;
//		case QN_193K_SF_ZJ:
//			if(buf->PollTimer > QN193K_POLL_PERIOD1S)
//			{
//				B_Poll_Ctr = 1; 
//				buf->PollTimer = 0;
//			}
//		break;
//		default:
//		break;
//	}
       if(EI_Comm.PollTimer > Send_Heart_Poll)
	{
		EI_Comm.PollTimer = 0;
		EI_Comm.Poll_Ctr = 1;
	}

}
//===============================================================================
//函数描述: 监控定时器运作
//输入参数: UART_BUFFER *buff: 缓存指针
//输出参数: 无
//修改日期: 2016-07
//修改人员: 
//===============================================================================
void Monitor_Controller_Uart_Timer_Run(CONTR_IF *buf)
{
//		if((Contr.Type == GST200_CRT_ZJ) || (Contr.Type == EI2000_CRT_ZJ)
//	    || (Contr.Type == GST500_CRT_ZJ) || (Contr.Type == QN5010_CRT_ZJ) 
//	    || (Contr.Type == LD128EN_CRT_232_ZJ) || (Contr.Type == LD128EN_CRT_485_ZJ)
//	    || (Contr.Type == GST200_SF_ZJ) || (Contr.Type == GST500_SF_ZJ)
//	    || (Contr.Type == QN_193K_SF_ZJ))
//	{
//		if(buf->PollEnFlag)
//		{
//			buf->PollTimer++;					
//		}
//	}

//	Gst.gst500_timeout  ++;
//	
//	buf->R.FrameCount++;
	if(EI_Comm.PollEnFlag==1)
	{
		EI_Comm.PollTimer++;
	}

	buf->R.ByteInterval++;
}
void Monitor_Controller_Slave_Uart(CONTR_IF *buf)
{
	if((Contr.Type == TS)||(Contr.Type == N3030_OLD)||(Contr.Type == PTW3300_JT))
	{
		if((buf->AnalyseSta  > FRAME_HEAD)&&(buf->AnalyseSta  < FRAME_CS))
		{		
			if(buf->R.ByteInterval > BUS_RECEIVE_INTERVAL)
			{
                            buf->R.ByteInterval=0;
				buf->R.Clev++;
				buf->AnalyseSta = FRAME_HEAD;
			}
		}
	}
}

void Save_Controller_Self_Info(UINT8 record_type,UINT16 detailed_type)
{
	RECORD_STRUCT record;

	memset(&record.Sta, 0xFF, sizeof(record));

	record.EventType.word = detailed_type;
	record.Type = record_type;
	MemoryCpyInvert(record.Uc,System.ctr_addr,LEN_REG_UC);

	Record_Controler_If_Event(&record);

}
//===============================================================================
//oˉêy?èê?: 485?óê?êy?Y′|àí
//ê?è?2?êy: ?T
//ê?3?2?êy: ?T
//DT??è??ú: 2016-09
//DT??è??±: 
//===============================================================================
void RS485_Task(void)
{
	/*if(RS485.T.Clev != RS485.T.Tail)
	{
		if((RS485_USART->ISR >>6) & 1 == 1)
//				if(USART_GetITStatus(RS485_USART, USART_IT_TC) != RESET)//发送完成中断
		{
			RS485_USART->ICR |= 0x0040;
			RS485_USART->TDR = RS485.T.Buf[RS485.T.Clev];
			RS485.T.Clev ++;
			On_Led_SEND();
		} 
	}
	else
	Off_Led_SEND();*/
	
	Com_Task_TS(&RS485);
}

//===============================================================================
//函数描述: 232接收数据处理
//输入参数: 无
//输出参数: 无
//修改日期: 2016-09
//修改人员: 
//===============================================================================
void RS232_Task(void)
{
	if(RS232.T.Clev < RS232.T.Tail)
	{
		if(((RS232_USART->ISR >>6) & 1 )== 1)
//				if(USART_GetITStatus(RS485_USART, USART_IT_TC) != RESET)//发送完成中断
		{
			RS232_USART->ICR |= 0x0040;
			RS232_USART->TDR = RS232.T.Buf[RS232.T.Clev];
			RS232.T.Clev ++;
			if(RS232.T.Clev >= CONTR_BUF_LEN)
				RS232.T.Clev = 0;
			On_Led_SEND();
		} 
	}
	else
	{
		       RS232.T.Clev = 0;
			RS232.T.Tail = 0;
	}
	//else
		//Off_Led_SEND();
	
	Com_Task_TS(&RS232);
}


/******************************************************************************
**
* Function Name: void Center_Command_Task()
* Description  : Event drive control.
* Arguments    : None.
* Return Value : None.
* Call Relation: Internal.
* Change Time  : 2016-1
*******************************************************************************
**/
void Monitor_Updata(void)
{
	UINT16 k;
	if(CenterCmd_Erease_Flash_Flag == TRUE)
	{

		for(k=0; k<ALEN_FLASH_PROGRAM/SIZE_FLASH_SECTOR; k++)		//需时6.5S
		{
			Watch_Dog();
			Flash_Sector_Erase(ADDR_FLASH_PROGRAM+k*SIZE_FLASH_SECTOR);
		}
		
		CenterCmd_Erease_Flash_Flag = FALSE;
	}	 

	//if(Faraway_Updata.sta == FARAWAY_UPDATA_RST)
	if(CenterCmd_Rst_Flag == TRUE)
	{

		Soft_Reset();//是否延时回码？？？标志？？？？
		CenterCmd_Rst_Flag = FALSE;
	}
}

void Controller_Com(void)
{
	RS232_Task();
	RS485_Task();
	Monitor_Updata();
}

