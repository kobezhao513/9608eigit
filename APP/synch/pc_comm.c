/**********************************************************************/
//Filename: comm_analyse.c
//Function: 
//      Date:  04-17-12
//   Author:   
/**********************************************************************/
#include "pc_comm.h"
#include "global.h"
#include "uart.h"
#include "xycnbus.h"
#include "record_status.h"
#include "ex_flash.h"
#include "basic.h"
#include "bus_basic_cmd.h"
#include "record_manage.h"
#include "synch.h" 
#include "sys_init.h" 
#include "rtc.h"
#include "controller.h"
//#include "delay.h"
//#include "Faraway_Com.h"
#include "ts.h"
#include "led.h"

CTR_COMM_STA CtrCommSta;
STRUCT_DEV_COMMAND_PROCESS DevCommPro;

STRUCT_FARAWAY_UPDATA Faraway_Updata;


AUTH_DATA Auth;

extern UINT8 time_string[7];


void Dead_Line_1S(void)
{
	if(Auth.Deadline)
	{
		Auth.Deadline--;
		if(Auth.Deadline == 0)
		{
			Auth.PasswordLevel= 0x00;
		}
	}
}

/**********************************************************************/
UINT8 Operate_Level_Check_Comm(UINT8 level)
{
		Auth.Deadline = C_DEADLINE_TIME_10S;
		return(TRUE);
}

uint16_t VCP_DataTx (uint8_t* Buf, uint32_t Len);
//计算crc，启动发送，发送长度为TotalLen
void Uart_Slaves_Init_Send(UART_BUFFER  *uart)
{
	UINT16 i,CS_loc;
	UINT16 CS = 0xFFFF;
	

	if(Contr_Mode == COM_232)
	{
		
		memcpy(&RS232.T.Buf,(*uart).Send.Buff,(*uart).Send.TotalLen); 
		
		CS_loc = (*uart).Send.TotalLen;
		RS232.T.Tail = (*uart).Send.TotalLen + 2;
		//计算CRC
		for(i=0;i<CS_loc;i++)
		{
			CS = Caculate_CRC(CS,RS232.T.Buf[i]);
		}
		
		RS232.T.Buf[CS_loc] = (UINT8)(CS&0x00FF);
		RS232.T.Buf[CS_loc+1] = (UINT8)((CS&0xFF00)>>8);
		
	}
	else if(Contr_Mode ==COM_485 )
	{
		memcpy(&RS485.T.Buf,(*uart).Send.Buff,(*uart).Send.TotalLen); 
		
		CS_loc = (*uart).Send.TotalLen;
		RS485.T.Tail = (*uart).Send.TotalLen + 2;
		//计算CRC
		for(i=0;i<CS_loc;i++)
		{
			CS = Caculate_CRC(CS,RS485.T.Buf[i]);
		}
		
		RS485.T.Buf[CS_loc] = (UINT8)(CS&0x00FF);
		RS485.T.Buf[CS_loc+1] = (UINT8)((CS&0xFF00)>>8);
		
	}
}


/**********************************************************************/
//Prototype: void Dev_Reg_Mode_1BYTE(UINT8 di0)
//Function:  1字节工作模式 寄存器地址计算
//       IN:  串口缓存
//     OUT:  无
//     Date:  13.7.29
//  Author:   
/**********************************************************************/
void Dev_Reg_Mode_1BYTE(UART_BUFFER  *uart)
{
    DevCommPro.mode = ((*uart).Rec.Buff[DevCommPro.diLoc] & DI_EXTENSION)>>1;
    
    if(DevCommPro.mode == C_BIT_MODE)
	{
    	DevCommPro.addr = 0x1F&(*uart).Rec.Buff[DevCommPro.diLoc];
	}
	else
	{
		DevCommPro.addr = 0x3F&(*uart).Rec.Buff[DevCommPro.diLoc];
	}
    DevCommPro.dataLoc = DevCommPro.diLoc +1; 
}

/**********************************************************************/
//Prototype: void Dev_Reg_Mode_2BYTE(UART_BUFFER  *uart)
//Function:  2字节工作模式 寄存器地址计算
//       IN:  串口缓存
//     OUT:  无
//     Date:  13.7.29
/**********************************************************************/
void Dev_Reg_Mode_2BYTE(UART_BUFFER  *uart)
{
	DevCommPro.mode = ((*uart).Rec.Buff[DevCommPro.diLoc+1]&DI_EXTEN_MODE_MASK);
    if(DevCommPro.mode == C_BIT_MODE)
	{
    	DevCommPro.addr = 0x1F&(*uart).Rec.Buff[DevCommPro.diLoc+1];
		DevCommPro.addr <<= 5;
		DevCommPro.datalen = 0;
	}
	else
	{
		DevCommPro.addr = 0x3F&(*uart).Rec.Buff[DevCommPro.diLoc+1];
		DevCommPro.addr <<= 6;
		DevCommPro.datalen = 1;
	}
	
	DevCommPro.addr |= (0x3F & (*uart).Rec.Buff[DevCommPro.diLoc]);
	DevCommPro.dataLoc = DevCommPro.diLoc +2; 
}

/**********************************************************************/
//Prototype: void Dev_Reg_Mode_NBYTE(UART_BUFFER  *uart)
//Function:  多字节工作模式 寄存器地址计算
//        IN:  
//     OUT: 
//     Date:  03-26-13
//  Author:   
/**********************************************************************/
void Dev_Reg_Mode_NBYTE(UART_BUFFER  *uart)
{
	uint32 addr;
	UINT8 dilen;
	dilen = 0;
	DevCommPro.mode = ((*uart).Rec.Buff[DevCommPro.diLoc+1]&DI_EXTEN_MODE_MASK);
	if(DevCommPro.mode == C_WRITE_MUl_BYTE_MODE)
	{
		DevCommPro.addr = 0x1F&(*uart).Rec.Buff[DevCommPro.diLoc+1];
		DevCommPro.addr <<= 6;
		DevCommPro.addr |= 0x3F & (*uart).Rec.Buff[DevCommPro.diLoc];
		DevCommPro.datalen = (*uart).Rec.Buff[DevCommPro.diLoc+2];
		DevCommPro.dataLoc = DevCommPro.diLoc +3;
	}
	else if(DevCommPro.mode == C_WRITE_MORE_BYTE_MODE)
	{
		
		DevCommPro.addr = 0x3F&(*uart).Rec.Buff[DevCommPro.diLoc];		//DI计算
		dilen = 1;
		addr = 0x0F&(*uart).Rec.Buff[DevCommPro.diLoc+dilen];
		addr <<= 6;
		DevCommPro.addr += addr;
		
		if((*uart).Rec.Buff[DevCommPro.diLoc+dilen]&BIT4)				//有扩展
		{	  
			dilen = 2;
			addr = 0x7f & (*uart).Rec.Buff[DevCommPro.diLoc+dilen];
			addr <<= 10;
			DevCommPro.addr += addr;
		
			while((*uart).Rec.Buff[DevCommPro.diLoc+dilen] & BIT7)		//有扩展
			{
				dilen++;
				addr = 0x7f & (*uart).Rec.Buff[DevCommPro.diLoc+dilen];
				addr <<= (10+(dilen-2)*7);
				DevCommPro.addr += addr;  
				if(dilen > 4)
				{
					DevCommPro.mode = C_NO_SURPORT_MODE;	  			//no support
					break;
				}
			}
		}
		
		dilen++;
		if((*uart).Rec.Buff[DevCommPro.diLoc + dilen] & BIT7) 			//数据长度扩展
		{  
			DevCommPro.datalen = (*uart).Rec.Buff[DevCommPro.diLoc + dilen];
			dilen++;
			addr = (*uart).Rec.Buff[DevCommPro.diLoc + dilen];
			addr <<= 7;
			DevCommPro.datalen += addr;

		}
		else                                                            //不扩展
		{
			DevCommPro.datalen = (*uart).Rec.Buff[DevCommPro.diLoc + dilen];
		}
        DevCommPro.dataLoc = DevCommPro.diLoc + dilen + 1; //包含一个或二个字节数据长度
	}
}
	
		
/**********************************************************************/
//Prototype: void Dev_Reg_Mode_NBLOCK(UINT8 di0)
//Function:  多块操作
//		IN:  
//	   OUT: 
//	  Date:  03-26-13
//	Author:   
/**********************************************************************/
void Dev_Reg_Mode_NBLOCK(UART_BUFFER  *uart)
{
	UCHAR8 dilen;
	UINT32 addr;
	DevCommPro.mode = ((*uart).Rec.Buff[DevCommPro.diLoc+1]&DI_EXTEN_MODE_MASK);
	DevCommPro.addr = 0x3F&(*uart).Rec.Buff[DevCommPro.diLoc];		//DI计算
	dilen = 1;
	addr = 0x0F&(*uart).Rec.Buff[DevCommPro.diLoc+dilen];
	addr <<= 6;
	DevCommPro.addr += addr;
	
	if((*uart).Rec.Buff[DevCommPro.diLoc+dilen]&BIT4)				//有扩展
	{	  
		dilen = 2;
		addr = 0x7f & (*uart).Rec.Buff[DevCommPro.diLoc+dilen];
		addr <<= 10;
		DevCommPro.addr += addr;
	
		while((*uart).Rec.Buff[DevCommPro.diLoc+dilen] & BIT7)		//有扩展
		{
			dilen++;
			addr = 0x7f & (*uart).Rec.Buff[DevCommPro.diLoc+dilen];
			addr <<= (10+(dilen-2)*7);
			DevCommPro.addr += addr;  
			if(dilen > 4)
			{
				DevCommPro.mode = C_NO_SURPORT_MODE;	  			//no support
				break;
			}
		}
	}

	dilen++;
	DevCommPro.block_count = Uchar_To_Uint16((*uart).Rec.Buff[DevCommPro.diLoc+dilen+1],(*uart).Rec.Buff[DevCommPro.diLoc+dilen]);
	dilen++;
	dilen++;
	DevCommPro.valid_len = Uchar_To_Uint16((*uart).Rec.Buff[DevCommPro.diLoc+dilen+1],(*uart).Rec.Buff[DevCommPro.diLoc+dilen]);
	dilen++;
	dilen++;
	DevCommPro.unit_len = Uchar_To_Uint16((*uart).Rec.Buff[DevCommPro.diLoc+dilen+1],(*uart).Rec.Buff[DevCommPro.diLoc+dilen]);
	dilen++;
	dilen++;
	DevCommPro.dataLoc = DevCommPro.diLoc+dilen;
}

/**********************************************************************/
//Prototype: void Dev_Reg_Mode(void)
//Function:  根据DI计算出寄存器访问模式、寄存器地址
//        IN:  串口缓存
//     OUT:   无
//     Date:  13.7.29
/**********************************************************************/
void Dev_Reg_Mode(UART_BUFFER  *uart)
{
    UINT8 di1;										

	if((*uart).Rec.Buff[DevCommPro.diLoc] < DI_EXTENSION)       //di0
    { 
    	Dev_Reg_Mode_1BYTE(uart);
    }
    else                 										//di0 di1
    {
    	di1 = (*uart).Rec.Buff[DevCommPro.diLoc+1] & DI_EXTEN_MODE_MASK;
    	if(di1 < C_WRITE_MUl_BYTE_MODE)							//
    	{
			Dev_Reg_Mode_2BYTE(uart);
		}
		else if(di1 < C_WRITE_MUl_BLOCK_MODE)
		{
			Dev_Reg_Mode_NBYTE(uart);
		}
		else
		{
			Dev_Reg_Mode_NBLOCK(uart);
		}
    }
	
    if(B_READ_WRITE)
    { 
        DevCommPro.mode |= WRITE_REG_FLAG;
    }
}
/**********************************************************************/
//Prototype: UINT8 Mmadr_Anslyse(UART_BUFFER  *uart) 
//Function:  多级地址处理
//      IN:  
//     OUT: 
//    Date:  04-18-13
//  Author:   
/**********************************************************************/
/*UINT8 Frame_Mmadr_Anslyse(UART_BUFFER  *uart) 
{
	UINT8 mmadr;
	UINT8 havetrans;
	UINT8 tansover;
	
	tansover = FALSE;
	(*uart).Rec.Buff[DevCommPro.lenexpand+S_MMADR]--;
	mmadr = (*uart).Rec.Buff[DevCommPro.lenexpand+S_MMADR];
	havetrans = mmadr&MMADR_TO_TRANS_MASK;
	havetrans >>= 4;
	if(havetrans == (mmadr&MMADR_HAVE_TRANS_MASK))
	{
		tansover = TRUE;
	}
	return(tansover);
}*/

/**********************************************************************/
//Prototype: UINT8 Frame_Len_Anslyse(UART_BUFFER  *uart) 
//Function:  帧地址计算
//        IN:  串口缓存
//      OUT:  TRUE:双字节 FALSE:单字节
//     Date:  13.7.29
/**********************************************************************/
UINT8 Frame_Len_Anslyse(UART_BUFFER  *uart) 
{
	UINT8 len2byte;
	UINT8 len0;
	UINT8 len1;

        if((*uart).Rec.Buff[S_LEN]&LEN_EXPAND_BIT)				//扩展
        {
            len2byte = TRUE;
            len0 = (((*uart).Rec.Buff[S_LEN]) & (~LEN_EXPAND_BIT));
            len1 = (*uart).Rec.Buff[S_LEN+1];
            
            DevCommPro.FrameLen = len1;
            DevCommPro.FrameLen <<= 7;
            DevCommPro.FrameLen += len0;
        }
        else
        {	
            len2byte = FALSE;
            len0 = ((*uart).Rec.Buff[S_LEN]);
            DevCommPro.FrameLen = (len0&(~LEN_EXPAND_BIT));
        }

	return(len2byte);
}


/**********************************************************************/
//Prototype: UINT8 Dev_Frame_Analyse(void)
// Function: 算出DI的缓存位置，目前暂支持广播和LA 的方式
//         IN:  串口缓存
//      OUT:  TRUE:地址匹配FALSE:不匹配、不处理
//     Date:  13.7.29
//  Author:   
/**********************************************************************/
UINT8 Dev_Frame_Analyse(UART_BUFFER  *uart) 
{
    UINT8 addr_fit;
	UINT8 comp;
	UINT8 str[64];
    static UINT8 ser=0;

	addr_fit = FALSE;           
	memset(&DevCommPro,0x00,sizeof(DevCommPro));
	
	DevCommPro.lenexpand = Frame_Len_Anslyse(uart);
	if(DevCommPro.lenexpand)
	{
		DevCommPro.ctr.byte = (*uart).Rec.Buff[S_CTR+1];
	}
	else
	{
		DevCommPro.ctr.byte = (*uart).Rec.Buff[S_CTR];
	}

    switch(DevCommPro.ctr.byte & 0xEF)  //zjc20140820
    {
        case CTR_BRO_WR:
		case CTR_BRO_RE:
			DevCommPro.wildcardFlag = TRUE;
            DevCommPro.diLoc = DevCommPro.lenexpand+DI_LOC_BRO;
           	Dev_Reg_Mode(uart);    
			addr_fit = TRUE;   
        break;
		
		case CTR_MMADR_BRO_WR:	 
		case CTR_MMADR_BRO_RE:
			DevCommPro.diLoc = DevCommPro.lenexpand+DI_LOC_MMADR_BRO;
			DevCommPro.wildcardFlag = TRUE;
			addr_fit = TRUE; 
			break;
			
		case CTR_LA_WR: 
        case CTR_LA_RE:              						//LA  len ctr la SEI DI DATA
            DevCommPro.diLoc = DevCommPro.lenexpand+DI_LOC_TDLA;	
            if ((*uart).Rec.Buff[(DevCommPro.diLoc-2)] == BRO_ADDR_LA) 
            {
                DevCommPro.wildcardFlag = TRUE;
                Dev_Reg_Mode(uart);
				addr_fit = TRUE;   
            }
            else //if ((*uart).Rec.Buff[(DevCommPro.diLoc-2)] == System.la) 
            {
            	if(System.trans_equip_connect)
        		{
        			if ((*uart).Rec.Buff[(DevCommPro.diLoc-2)] == System.la)
    				{
					    Dev_Reg_Mode(uart);
						addr_fit = TRUE;
    				}
					else
					{
						addr_fit = FALSE;
					}
        		}
				else
				{
					Dev_Reg_Mode(uart);
					addr_fit = TRUE;
				}
            }
			/*else  if((*uart).Rec.Buff[DevCommPro.diLoc] == DI_POLL_INT) 
			{
			    Dev_Reg_Mode(uart);
				addr_fit = TRUE;   
			}*/
             
        break;

		case CTR_MMADR_LA_WR:
		case CTR_MMADR_LA_RE:
			DevCommPro.diLoc = DevCommPro.lenexpand+DI_LOC_TDLA_LA;
			if ((*uart).Rec.Buff[(DevCommPro.lenexpand+ADDR_LOC_MMA_LA)] == BRO_ADDR_LA) 
            {
                DevCommPro.wildcardFlag = TRUE;
				addr_fit = TRUE;   
            }
            else if ((*uart).Rec.Buff[(DevCommPro.lenexpand+ADDR_LOC_MMA_LA)] == System.la) 
            {
				addr_fit = TRUE;   
            }
			else  if((*uart).Rec.Buff[DevCommPro.diLoc] == DI_POLL_INT) 
			{
				addr_fit = TRUE;   
			}
			if(addr_fit)
			{ 
				//LA_Frame_Relay(uart);
				Dev_Reg_Mode(uart);
			}
			break;

		case   CTR_UC_WR:	 
        case   CTR_UC_RE:             						 	//7f len ctr uc SEI DI DATA 
       		memcpy(str,System.uc,LEN_REG_UC);
            MemoryCpyInvert(str ,str,LEN_REG_UC);
        	comp = addr_compare(0,LEN_REG_UC,str,&(*uart).Rec.Buff[3+DevCommPro.lenexpand]);
			if(comp == COMPARE_SUCESS_NO_WILDCARD)
            {
				DevCommPro.diLoc = DevCommPro.lenexpand+DI_LOC_TDUC ;
				Dev_Reg_Mode(uart);
				addr_fit = TRUE;  
                break;
            }   
			else if(comp == COMPARE_SUCESS_WILDCARD)
			{
				DevCommPro.wildcardFlag = TRUE;
				DevCommPro.diLoc = DevCommPro.lenexpand+DI_LOC_TDUC ;
				Dev_Reg_Mode(uart);
				addr_fit = TRUE;  
                break;
			}


			str[0] = 0x7D;
        	comp = addr_compare(0,LEN_REG_UC,str,&(*uart).Rec.Buff[3+DevCommPro.lenexpand]);
			if(comp == COMPARE_SUCESS_WILDCARD)
            {
                //DevCommPro.linkFlag = 5;
                DevCommPro.wildcardFlag = TRUE;
				DevCommPro.diLoc = DevCommPro.lenexpand+DI_LOC_TDUC ;
				Dev_Reg_Mode(uart);
				addr_fit = TRUE;  
            }
			break;
		case   CTR_MMADR_UC_WR:	 
		case   CTR_MMADR_UC_RE:	  
			memcpy(str,System.uc,LEN_REG_UC);
            MemoryCpyInvert(str ,str,LEN_REG_UC);
        	comp = addr_compare(0,LEN_REG_UC,str,&(*uart).Rec.Buff[ADDR_LOC_MMA_UC+DevCommPro.lenexpand]);
			 if(comp == COMPARE_SUCESS_NO_WILDCARD)
            {
				addr_fit = TRUE;   
            }   
			else if(comp == COMPARE_SUCESS_WILDCARD)
			{
				addr_fit = TRUE;   
				DevCommPro.wildcardFlag = TRUE;
			}
			DevCommPro.diLoc = DevCommPro.lenexpand+DI_LOC_TDUC_UC;
			if(addr_fit)
			{ 
				//UC_Frame_Relay(uart);
				Dev_Reg_Mode(uart);
			}
 
		 break;
		case   CTR_ID_WR: 
        case   CTR_ID_RE:             						 //ID  len ctr id[12] SEI DI DATA

            memcpy(str,System.id,12);
            MemoryCpyInvert(str ,str,12);
			comp = addr_compare(0,12,str,&(*uart).Rec.Buff[3+DevCommPro.lenexpand]);
            if(comp == COMPARE_SUCESS_NO_WILDCARD)
            {
				DevCommPro.diLoc = DevCommPro.lenexpand+DI_LOC_TDID ;
				Dev_Reg_Mode(uart);
				addr_fit = TRUE;  
            }   
			else if(comp == COMPARE_SUCESS_WILDCARD)
			{
				DevCommPro.wildcardFlag = TRUE;
				DevCommPro.diLoc = DevCommPro.lenexpand+DI_LOC_TDID ;
				Dev_Reg_Mode(uart);
				addr_fit = TRUE;  
			}
        break;
		
		case   CTR_MMADR_ID_WR: 
        case   CTR_MMADR_ID_RE:             						 //ID  len ctr id[12] SEI DI DATA
			memcpy(str,System.id,12);
            MemoryCpyInvert(str ,str,12);
			comp = addr_compare(0,12,str,&(*uart).Rec.Buff[ADDR_LOC_MMA_ID+DevCommPro.lenexpand]);
			 if(comp == COMPARE_SUCESS_NO_WILDCARD)
            {
				addr_fit = TRUE;   
            }   
			else if(comp == COMPARE_SUCESS_WILDCARD)
			{
				addr_fit = TRUE;   
				DevCommPro.wildcardFlag = TRUE;
			}
			DevCommPro.diLoc = DevCommPro.lenexpand+DI_LOC_TDID_ID ;
			if(addr_fit)
			{ 
				//ID_Frame_Relay(uart);
			}
        break;
		
        default:
            addr_fit = FALSE;
        break;      
    }
    if(DevCommPro.diLoc)
    {
		if(ser == (*uart).Rec.Buff[DevCommPro.diLoc-1])
		{
			if(Compete_Event.send_flag == TRUE)
			{
				Module_Msg_Send_Fal();
			}
			else
			{
				addr_fit = FALSE;
			}
		}
		else
		{
			if(Compete_Event.send_flag == TRUE)
			{
				if((*uart).Rec.Buff[DevCommPro.diLoc-1] == (ser + 1))
				{
					Module_Msg_Send_Suc();
				}
				else
				{
					Module_Msg_Send_Fal();
				}
			}
			
			ser = (*uart).Rec.Buff[DevCommPro.diLoc-1];
		}
	}
    
    return(addr_fit); 
}

/***=======================================================================
//Function:     void Change_Byte(UINT8 *data)
//Description:  
//Parameters:   UINT8 *data
//Return Value: None
//-----------------------------------------------------------------------------------------------
//Date:         2012.03
//=======================================================================***/

void Change_Byte(UINT8 *data,UART_BUFFER  *uart)
{
    UINT8 temp,flag;
    
    temp = ~(*uart).Rec.Buff[DevCommPro.dataLoc+1];
    temp &= *data;            //Save the original value
    
    flag = (*uart).Rec.Buff[DevCommPro.dataLoc]
          &(*uart).Rec.Buff[DevCommPro.dataLoc+1];
          
    *data = temp|flag ;      //Assign a new value
}

/**********************************************************************/
//Prototype: void Ans_Process_Normal(UART_BUFFER  *uart,UINT16 len,UCHAR8 *buff)
//Function:  正常应答处理
//      IN:  len：数据长度；buff：数据缓存首地址
//     OUT: 
//    Date:  05-9-13
//  Author:   
/**********************************************************************/
void Ans_Process_Normal(UART_BUFFER  *uart,UINT16 len,UINT8 *buff)
{
	UINT16 framlen;

    if(!DevCommPro.wildcardFlag)
	{
		framlen = len + DevCommPro.dataLoc;
		if(framlen < MAX_LEN_1BYTE)								//数据长度
		{
			if(DevCommPro.lenexpand == TRUE)					//收到的数据帧
			{
				memcpy((*uart).Send.Buff,&(*uart).Rec.Buff[1],DevCommPro.dataLoc);
				DevCommPro.dataLoc--;
			}
			else
			{
				memcpy((*uart).Send.Buff,&(*uart).Rec.Buff[0],DevCommPro.dataLoc);
			}
			framlen = len + DevCommPro.dataLoc-1;
			(*uart).Send.Buff[S_LEN] = framlen; 				//加帧长度
			(*uart).Send.Buff[S_CTR] &= (~CTR_TANS_DIR);
		}
		else
		{	
			if(DevCommPro.lenexpand == TRUE)					//收到的数据帧
			{
				memcpy(&((*uart).Send.Buff[0]),&(*uart).Rec.Buff[0],DevCommPro.dataLoc);
			}
			else
			{
				memcpy(&((*uart).Send.Buff[1]),&(*uart).Rec.Buff[0],DevCommPro.dataLoc);
				DevCommPro.dataLoc++;
			}
			framlen = len + DevCommPro.dataLoc-1;
			(*uart).Send.Buff[S_LEN] = framlen&0x7f; 			//加帧长度
			(*uart).Send.Buff[S_LEN] += LEN_EXPAND_BIT; 		//加帧长度
			(*uart).Send.Buff[S_LEN2] = (framlen>>7); 			//加帧长度2
			(*uart).Send.Buff[S_CTR+1] &= (~CTR_TANS_DIR);
		}
        
		(*uart).Send.Buff[S_HEAD] = FRAME_HEADER;			//加帧头
		memcpy(&((*uart).Send.Buff[DevCommPro.dataLoc]),buff,len);//数据
		framlen++;
		(*uart).Send.TotalLen = framlen;
		(*uart).Send.RepeatNum = UART_REPEAT_TIMES;
		(*uart).FrameTimer = 0;
		Uart_Slaves_Init_Send(uart);
	}
} 

/**********************************************************************/
//Prototype: void Ans_Process_Comm_Ans(UART_BUFFER  *uart,UINT16 len,UCHAR8 *buff)
//Function:  应答处理-通信应答
//      IN:  
//     OUT: 
//    Date:  05-9-13
//  Author:   
/**********************************************************************/
void Ans_Process_Comm_Ans(UART_BUFFER  *uart,UINT16 len,UCHAR8 *buff)
{
	uint16 framlen;

    if(!DevCommPro.wildcardFlag)
	{
		framlen = len + DevCommPro.dataLoc;
		if(framlen < MAX_LEN_1BYTE)								//数据长度
		{
			if(DevCommPro.lenexpand == TRUE)					//收到的数据帧
			{
				memcpy((*uart).Send.Buff,&(*uart).Rec.Buff[1],DevCommPro.dataLoc);
				DevCommPro.dataLoc--;
			}
			else
			{
				memcpy((*uart).Send.Buff,&(*uart).Rec.Buff[0],DevCommPro.dataLoc);
			}
			framlen = len + DevCommPro.dataLoc-1;
			(*uart).Send.Buff[S_LEN] = framlen; 				//加帧长度
			(*uart).Send.Buff[S_CTR] &= (~CTR_TANS_DIR);
		}
		else
		{	
			if(DevCommPro.lenexpand == TRUE)					//收到的数据帧
			{
				memcpy(&((*uart).Send.Buff[0]),&(*uart).Rec.Buff[0],DevCommPro.dataLoc);
			}
			else
			{
				memcpy(&((*uart).Send.Buff[1]),&(*uart).Rec.Buff[0],DevCommPro.dataLoc);
				DevCommPro.dataLoc++;
			}
			framlen = len + DevCommPro.dataLoc-1;
			(*uart).Send.Buff[S_LEN] = framlen&0x7f; 			//加帧长度
			(*uart).Send.Buff[S_LEN] += LEN_EXPAND_BIT; 		//加帧长度
			(*uart).Send.Buff[S_LEN2] = (framlen>>7); 			//加帧长度2
			(*uart).Send.Buff[S_CTR+1] &= (~CTR_TANS_DIR);
		}
		(*uart).Send.Buff[S_HEAD] = FRAME_HEADER;			//加帧头
		
		memcpy(&((*uart).Send.Buff[DevCommPro.dataLoc]),buff,len);//数据
		framlen++;
		(*uart).Send.TotalLen = framlen;
		(*uart).Send.RepeatNum = UART_REPEAT_TIMES;
		(*uart).FrameTimer = 0;
		Uart_Slaves_Init_Send(uart);
	}

} 
/**********************************************************************/
//Prototype: void Abnormal_Ans_Process(UINT16 answer,UART_BUFFER  *uart)
//Function:   巡检异常应答处理              
//      IN:  
//     OUT: 
//    Date:  07-02-12 
//  Author:   
/**********************************************************************/
void Abnormal_Ans_Process(UINT16 answer,UART_BUFFER  *uart)
{
	if(!DevCommPro.wildcardFlag)
	{
		DevCommPro.anslen = DevCommPro.dataLoc+1;
		(*uart).Rec.Buff[2] &=CTR_MASK_DEV_ANS_ERR;
		(*uart).Rec.Buff[DevCommPro.dataLoc+1] = (UINT8)answer;
		(*uart).Rec.Buff[DevCommPro.dataLoc]   = (UINT8)(answer>>8);
		memcpy((*uart).Send.Buff,(*uart).Rec.Buff,DevCommPro.anslen+1);
		
		Uart_Slave_Prepare_Send(uart,DevCommPro.anslen);
	}
}

/*=========================================================================================
函数：void  Bus_Prepare_Send(unsigned char len)	
功能：准备向总线发送发数据，送缓存内数据
参数：控制字，len为帧长度字节的值
返回值：无
=======================================================================================*/
void  Uart_Slave_Prepare_Send(UART_BUFFER  *uart,uchar8 len)												
{
	(*uart).Send.Buff[0] = FRAME_HEADER;					//加帧头
	(*uart).Send.Buff[1] = len;							//加帧长度
	len ++;
	(*uart).Send.TotalLen = len;
	(*uart).Send.ActualLen  = 0;
	(*uart).Rec.Status = UART_REC_HEARD;
	(*uart).Send.RepeatNum = UART_REPEAT_TIMES;
	(*uart).RunSta = RUN_STA_TXD;
	(*uart).FrameTimer = 0;
	Uart_Slaves_Init_Send(uart);
}	
/**********************************************************************/
//Prototype: void Write_Special_Comm(void)
//Function:   巡检的正常应答处理              
//      IN:  
//     OUT: 
//    Date:  07-02-12 
//  Author:   
/**********************************************************************/
void Ans_Process_Poll_Normal(UART_BUFFER  *uart)
{
    //if(uart == &USB_Dev)
    {
        if(!DevCommPro.wildcardFlag)
        {
            (*uart).Send.Buff[2] = ((CTR_LA_RE&(~CTR_TANS_DIR))|CTR_NORMAL_ANS);   //ctr 
            (*uart).Send.Buff[3] = System.la; 						//la
            (*uart).Send.Buff[4] = (*uart).Rec.Buff[DevCommPro.diLoc-1];	//ser
            (*uart).Send.Buff[5] = (*uart).Rec.Buff[DevCommPro.diLoc];		//di
            Uart_Slave_Prepare_Send(uart,5);
        }
    }
   
}
//按照传输设备上传中心的帧格式
void Send_Event_To_TransEq(UART_BUFFER  *uart,UINT8 *Record_Buffer)
{
	(*uart).Send.Buff[0] = FRAME_HEADER;					//加帧头
	(*uart).Send.Buff[1] = 6+GET_EVENT_NOMAL_LEN;							//加帧长度

	(*uart).Send.Buff[2] = 0xf6;   //ctr 
    (*uart).Send.Buff[3] = System.la; 						//la
    (*uart).Send.Buff[4] = (*uart).Rec.Buff[DevCommPro.diLoc-1];	//ser
    (*uart).Send.Buff[5] = 0x38;		//di
    memcpy(&(*uart).Send.Buff[6],&Record_Buffer[1],GET_EVENT_NOMAL_LEN);
		
	(*uart).Send.TotalLen = (*uart).Send.Buff[1]+1;
	Uart_Slaves_Init_Send(uart);
}

/*********************************************************************************
* 函数名称 : System_Update_Data_Decrpt
* 函数描述 : 解密.hex文件
* 输入参数 : 
* 输出参数 : None;
* 返 回 值 : None
* 修改日期 ：2014-10

*********************************************************************************/
void System_Update_Data_Decrpt(UINT8 *hex,UINT16 datalen,split *crc )
{
	UINT16 j;
	UINT8 data;
	
	for(j = 0;j < datalen;j ++)
	{
		data = hex[j];
		data=(data<<4)|(data>>4);  
		data=((data<<2)&0xcc)|((data>>2)&0x33);  
		data=((data<<1)&0xaa)|((data>>1)&0x55);  
		switch(j%4)
		{
			case 0:
				data ^= 0x21;//b00100001;
			break;
			case 1:
				data ^= 0x82;//0b10000010;
			break;
			case 2:
				data ^= 0x48;//0b01001000;
			break;
			default:
				data ^= 0x14;//0b00010100;
			break;
		}
		//hex[j] = data;//解密原数据
		Make_Crc((crc),data);//计算解密后的数据，只因为.hex中的文件末尾校验为有效数据的校验
		
	}
}
void Ans_Updata_Normal(UART_BUFFER  *uart,UINT8 *buff)
{
	UINT16 framlen;

	memcpy((*uart).Send.Buff,&(buff[1]),DevCommPro.dataLoc);
	DevCommPro.dataLoc--;
	framlen = DevCommPro.dataLoc-1;
	(*uart).Send.Buff[S_LEN] = framlen; 				//加帧长度
	(*uart).Send.Buff[S_CTR] &= (~CTR_TANS_DIR);
	(*uart).Send.Buff[S_HEAD] = FRAME_HEADER;			//加帧头
	framlen++;
	(*uart).Send.TotalLen = framlen;
	(*uart).Send.RepeatNum = UART_REPEAT_TIMES;
	(*uart).FrameTimer = 0;
	Uart_Slaves_Init_Send(uart);

}

/**********************************************************************/
//Prototype: void Write_Special_Comm(void)
//Function:   写特殊命令的操作，对应协议1的操作              
//      IN:  
//     OUT: 
//    Date:  07-02-12 
//  Author:   
/**********************************************************************/
void Write_Special_Comm(UART_BUFFER  *uart)
{      
	UINT32 validflag = BOOT_VALID_FALG; 
	UINT16 frame_num_total;
	UINT8 dev_type[4],ans_data_repeat[20];
	static UINT8 hard_ver[3];
	UINT8 str[3];
 	switch(DevCommPro.addr)
   	{
   		case REG_DEBUG:
			switch(uart->Rec.Buff[DevCommPro.dataLoc])
			{
				case TYPE_DEBUG_RESET:
					Ans_Process_Normal(uart,0,uart->Rec.Buff);
					CenterCmd_Rst_Flag = TRUE;
					//Soft_Reset();
				break;
				case TYPE_DEBUG_UPDATE_START://开始升级
					//Flash_Read_String(ADDR_BOOTLOADER_FLAG,(UINT8*)&temp1,4);
					//Flash_Read_String(ADDR_FLASH_PROGRAM,(UINT8*)&temp2,4);

					memcpy(dev_type,&uart->Rec.Buff[DevCommPro.dataLoc+61],4);
					if(memcmp(dev_type,"4331",4))
					{
						Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart);	
					}
					else
					{
					
						memcpy(hard_ver,&uart->Rec.Buff[DevCommPro.dataLoc+65],3);
						if((hard_ver[0]==HARD_V_BYTE2)&&(hard_ver[1]==HARD_V_BYTE1)&&(hard_ver[2]==HARD_V_BYTE0))
						{
							Ans_Process_Normal(uart,0,uart->Rec.Buff);	
							CenterCmd_Erease_Flash_Flag = TRUE;
							//擦1M升级区域

							//保证每次升级开始，静态变量都为0
							
							Faraway_Updata.rec_crc.word = 0xFFFF;
							Faraway_Updata.wr_crc.word	= 0xFFFF;
							Faraway_Updata.frame_num = 0;
							Faraway_Updata.frame_num_last = 0;
							Faraway_Updata.addr = 0;
						}
					}
					break;
				case TYPE_DEBUG_UPDATE:	//升级数据

					Faraway_Updata.frame_num   = uart->Rec.Buff[DevCommPro.dataLoc+2];
					Faraway_Updata.frame_num <<= 8;
					Faraway_Updata.frame_num  += uart->Rec.Buff[DevCommPro.dataLoc+1];
					if(Faraway_Updata.frame_num == Faraway_Updata.frame_num_last+1)
					{

						memcpy(ans_data_repeat,uart->Rec.Buff,20);
						Faraway_Updata.frame_num_last = Faraway_Updata.frame_num;


						Faraway_Updata.data_len =  uart->Rec.Buff[DevCommPro.dataLoc+8];
						Faraway_Updata.data_len <<= 8;
						Faraway_Updata.data_len  += uart->Rec.Buff[DevCommPro.dataLoc+7];
						Faraway_Updata.data_len += 9;
						memmove(uart->Rec.Buff+1, &uart->Rec.Buff[DevCommPro.dataLoc], Faraway_Updata.data_len);
						uart->Rec.Buff[0] = ':';
						//Faraway_Updata.data_buf[0] = ':';
						//memcpy(Faraway_Updata.data_buf+1, &uart->Rec.Buff[DevCommPro.dataLoc], Faraway_Updata.data_len);
						Faraway_Updata.data_len ++;
						uart->Rec.Buff[Faraway_Updata.data_len] = 0x0A;
						//Faraway_Updata.data_buf[Faraway_Updata.data_len] = 0x0A;
						Faraway_Updata.data_len ++;

						Flash_Write_String(ADDR_FLASH_PROGRAM+Faraway_Updata.addr,uart->Rec.Buff,Faraway_Updata.data_len);
						//Flash_Write_String(ADDR_FLASH_PROGRAM+Faraway_Updata.addr,Faraway_Updata.data_buf,Faraway_Updata.data_len);

						Faraway_Updata.addr += Faraway_Updata.data_len;
						Ans_Updata_Normal(uart,ans_data_repeat);
						//Ans_Process_Normal(uart,0,uart->Rec.Buff);

					}
					else
					{
						Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart); 
					}
				break;

				case TYPE_DEBUG_UPDATE_OVER://结束升级

					frame_num_total   = uart->Rec.Buff[DevCommPro.dataLoc+2];
					frame_num_total <<= 8;
					frame_num_total  += uart->Rec.Buff[DevCommPro.dataLoc+1];
	
					if(Faraway_Updata.frame_num == frame_num_total)
					{
						Faraway_Updata.data_len = 6;
						memcpy(ans_data_repeat,uart->Rec.Buff,20);
						
						memmove(uart->Rec.Buff+1, &uart->Rec.Buff[DevCommPro.dataLoc], Faraway_Updata.data_len-1);
						uart->Rec.Buff[0] = ':';
						//Faraway_Updata.data_buf[0] = ':';
						//memcpy(Faraway_Updata.data_buf+1, &uart->Rec.Buff[DevCommPro.dataLoc], Faraway_Updata.data_len-1);
						uart->Rec.Buff[Faraway_Updata.data_len] = 0x0A;
						Faraway_Updata.data_len ++;
						//Faraway_Updata.data_buf[Faraway_Updata.data_len] = 0x0A;
						Flash_Write_String(ADDR_FLASH_PROGRAM+Faraway_Updata.addr,uart->Rec.Buff,Faraway_Updata.data_len);
						//Flash_Write_String(ADDR_FLASH_PROGRAM+Faraway_Updata.addr,Faraway_Updata.data_buf,Faraway_Updata.data_len);
						Faraway_Updata.addr += Faraway_Updata.data_len;
						Flash_Write_String(ADDR_BOOTLOADER_LEN,(UINT8*)&Faraway_Updata.addr,4);

						memcpy(uart->Rec.Buff, hard_ver, 3);
						Flash_Write_String(ADDR_BOOTLOADER_HARD,uart->Rec.Buff,3);
						//memcpy(Faraway_Updata.data_buf, hard_ver, 3);
						//Flash_Write_String(ADDR_BOOTLOADER_HARD,Faraway_Updata.data_buf,3);
						//硬件版本写入片外flash暂存区的数据区，与u盘升级兼容

						memcpy(uart->Rec.Buff, ":HardVer//", 10);
						uart->Rec.Buff[10] = (hard_ver[0]>>4)+0x30;
						uart->Rec.Buff[11] = (hard_ver[0]&0x0F)+0x30;
						uart->Rec.Buff[12] = (hard_ver[1]>>4)+0x30;
						uart->Rec.Buff[13] = (hard_ver[1]&0x0F)+0x30;
						uart->Rec.Buff[14] = (hard_ver[2]>>4)+0x30;
						uart->Rec.Buff[15] = (hard_ver[2]&0x0F)+0x30;
						uart->Rec.Buff[16] = 0x0A;

						/*memcpy(Faraway_Updata.data_buf, ":HardVer//", 10);
						Faraway_Updata.data_buf[10] = (hard_ver[0]>>4)+0x30;
						Faraway_Updata.data_buf[11] = (hard_ver[0]&0x0F)+0x30;
						Faraway_Updata.data_buf[12] = (hard_ver[1]>>4)+0x30;
						Faraway_Updata.data_buf[13] = (hard_ver[1]&0x0F)+0x30;
						Faraway_Updata.data_buf[14] = (hard_ver[2]>>4)+0x30;
						Faraway_Updata.data_buf[15] = (hard_ver[2]&0x0F)+0x30;
						Faraway_Updata.data_buf[16] = 0x0A;*/
						Faraway_Updata.data_len = 17;
						Flash_Write_String(ADDR_FLASH_PROGRAM+Faraway_Updata.addr,uart->Rec.Buff,Faraway_Updata.data_len);
						//Flash_Write_String(ADDR_FLASH_PROGRAM+Faraway_Updata.addr,Faraway_Updata.data_buf,Faraway_Updata.data_len);
						Faraway_Updata.addr += Faraway_Updata.data_len;
						
						validflag = 0xAAAAAAAA;
						Flash_Write_String(ADDR_BOOTLOADER_FLAG,(UINT8*)&validflag,4);	
						Ans_Updata_Normal(uart,ans_data_repeat);

						//Ans_Process_Normal(uart,0,uart->Rec.Buff);

						CenterCmd_Rst_Flag = TRUE;
						//Soft_Reset(); //软重启

					}
					else
					Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart); 
					break;

					
				case TYPE_DEBUG_LED:
					switch((*uart).Rec.Buff[DevCommPro.dataLoc+2])
					{
					case 0x00:
						Debug.Led = 0;
						Off_Led_SEND();
						Off_Led_Error();
						Off_Led_Contor();
						str[0] = (*uart).Rec.Buff[DevCommPro.dataLoc];
						str[1] = (*uart).Rec.Buff[DevCommPro.dataLoc+1];
						str[2] = (*uart).Rec.Buff[DevCommPro.dataLoc+2];
						Ans_Process_Normal(uart,3,str);
					break;
					case 0xFF:
						Debug.Led = 1;
						On_Led_SEND();
						On_Led_Error();
						On_Led_Contor();
						str[0] = (*uart).Rec.Buff[DevCommPro.dataLoc];
						str[1] = (*uart).Rec.Buff[DevCommPro.dataLoc+1];
						str[2] = (*uart).Rec.Buff[DevCommPro.dataLoc+2];
						Ans_Process_Normal(uart,3,str);
					break;
					}
					break;
				default:	
					Ans_Process_Normal(uart,0,uart->Rec.Buff);
			}
		break;
		case REG_DEV_CTR:
			
			if(Contr_Mode == COM_232)
			{
				if(((*uart).Rec.Buff[6] == 0x24) && ((*uart).Rec.Buff[7] == 0x01))
				{
					/*if(Fr_Lastser != (*uart).Rec.Buff[4])
					{
						if(TS_Far.T.Flag == TS_DATA_IDLE)
						{*/
							TS_Get_Controller_Data(&(*uart).Rec.Buff[0]);
							Ans_Process_Normal(uart,0,uart->Rec.Buff);
							/*RS232.T.Sta = WAITING;					
						}
					}
					else
					{
						if((RS232.T.Sta == WAITING) && (TS_Far.T.Flag == TS_DATA_SEND_SUC))
						{
							TS_Far.T.Flag = TS_DATA_IDLE;
							RS232.T.Sta = SEND_IDLE;
						}
					}*/
				}
				else
				{
					//Slave_Prepare_Answer(&RS232,5);
					Ans_Process_Normal(uart,0,uart->Rec.Buff);	
					Controller_Com_Event_Save(&RS232);
				}
			}
			else if(Contr_Mode == COM_485)
			{
				if(((*uart).Rec.Buff[6] == 0x24) && ((*uart).Rec.Buff[7] == 0x01))
				{
					TS_Get_Controller_Data(&(*uart).Rec.Buff[0]);
					Ans_Process_Normal(uart,0,uart->Rec.Buff);
				}
				else
				{
					//Slave_Prepare_Answer(&RS485,5);
					Ans_Process_Normal(uart,0,uart->Rec.Buff);	
					Controller_Com_Event_Save(&RS485);
				}
			}
			
		break;
		default:                                   				//回码不支持
			Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart); 
		break;
   	}

 
}

/**********************************************************************/
//Prototype: void Read_Special_Comm(void)
//Function:   读特殊命令的操作，对应协议1的操作              
//      IN:  
//     OUT: 
//    Date:  04-4-13 
//  Author:   
/**********************************************************************/
void Read_Special_Comm(UART_BUFFER  *uart)
{
  	UINT8 str[12],info_type; 
	UINT8 Record_Buffer[150]={0},Find_Msg_Sta;
	
  	switch(DevCommPro.addr)
   	{
        	case DI_ID_LOG:
        	{
	                DevCommPro.anslen = DevCommPro.dataLoc+12;
	                DevCommPro.wildcardFlag = FALSE;
	                memcpy(str, System.id,12);
	                MemoryCpyInvert(str ,str,12);
	                Ans_Process_Normal(uart,12,str);
        	}
        	break;
        
		case DI_POLL_INT:
			DevCommPro.wildcardFlag = FALSE;
			if(System.trans_equip_connect)
			{
			
				info_type = Find_Info_Upload();
				if(info_type == SELF_EVENT)
				{   
					if(Contr_Mode == COM_232)
					{
						Slave_Info_Send(&RS232,info_type);
					}
					else if(Contr_Mode == COM_485 )
					{
						Slave_Info_Send(&RS485,info_type);
					}
				}
				else
				{
					Find_Msg_Sta = Find_Module_Msg(Record_Buffer);
					if(Find_Msg_Sta == EVENT_NO)
					{
						Ans_Process_Poll_Normal(uart); //正常巡检应答帧
					}
					else if(Find_Msg_Sta == EVENT_FIND)
					{
						Send_Event_To_TransEq(uart,Record_Buffer);
						Compete_Event.send_flag  = TRUE;
					}

				}
			
			}
			else
			{
				if(DevCommPro.FrameLen == 0x06)
				{
					info_type = Find_Info_Upload();
					if(info_type)
					{   
						if(Contr_Mode == COM_232)
						{
							Slave_Info_Send(&RS232,info_type);
						}
						else if(Contr_Mode == COM_485 )
						{
							Slave_Info_Send(&RS485,info_type);
						}
					}
					else
					{
						Ans_Process_Poll_Normal(uart); //正常巡检应答帧
					}
				}
			}
	 	break;

		default:                                   				//回码不支持
			Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart); 
		break;
   	}
}

/***=======================================================================
//Function:     void Write_Bit_Reg(void)
//Description:  
//Parameters:   None
//Return Value: None
//-----------------------------------------------------------------------------------------------
//Date:         2012.03
//=======================================================================***/

void Write_Bit_Reg(UART_BUFFER  *uart)
{
//    UINT8 answer,data;
	UINT8 markbit;
	//UINT8 succ;
	//succ = TRUE;
	
	switch(DevCommPro.addr)
	{
		case REG_BIT_ID_LOG_FLAG:
		case REG_BIT_LA_LOG_FLAG:
		case REG_BIT_UC_LOG_FLAG:
            
            markbit = 1;
            markbit <<= (DevCommPro.addr - REG_BIT_ID_LOG_FLAG);
			if((*uart).Rec.Buff[DevCommPro.diLoc]&DI_BIT_DATA_MASK_1BYTE)
			{
				CtrCommSta.LogFlag |= markbit;
			}
			else
			{
            CtrCommSta.LogFlag &= (~markbit);
			}
            
            Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
			break;
			
		case REG_BIT_CLEAR_RUN_STA:
				Ans_Process_Normal(uart,0,(*uart).Rec.Buff);

			break;

		case REG_BIT_WP_CC:
		case REG_BIT_WP_LA:
		case REG_BIT_WP_UC:
				Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
				markbit = 1;
				markbit <<= (DevCommPro.addr - REG_BIT_WP_CC);
				if((*uart).Rec.Buff[DevCommPro.diLoc]&DI_BIT_DATA_MASK_1BYTE)
				{
					CtrCommSta.WriteEn |= markbit;
					CtrCommSta.WriteDeadLine = C_DEADLINE_TIME_10S;
				}
				else
				{
					CtrCommSta.WriteEn &= (~markbit);
				}
		
			break;
			
		case REG_BIT_CHECK_FLAG:
	
				Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
			break;
		default:
			 Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart); 
			break;
	}
	/*if( succ == FALSE)
	{
		Abnormal_Ans_Process(INT_TYPE_LIMIT_PERMISSIONS,uart);
	}*/
}

/***=======================================================================
//Function:     void Read_Bit_Reg(void)
//Description:  32 Bit operating
//Parameters:   None
//Return Value: None
//-----------------------------------------------------------------------------------------------
//Date:         2012.03
//=======================================================================***/

void Read_Bit_Reg(UART_BUFFER  *uart)
{
//    UINT8 data,temp,adr;
	UINT8 markbit;

	switch(DevCommPro.addr)
	{
		case REG_BIT_ID_LOG_FLAG:
		case REG_BIT_LA_LOG_FLAG:
		case REG_BIT_UC_LOG_FLAG:
		case REG_BIT_CLEAR_RUN_STA:
			markbit = 1;
			markbit <<= (DevCommPro.addr - REG_BIT_ID_LOG_FLAG);
			if(CtrCommSta.LogFlag&markbit)
			{
				(*uart).Rec.Buff[DevCommPro.diLoc] |=DI_BIT_DATA_MASK_1BYTE;
			}
			else
			{
				(*uart).Rec.Buff[DevCommPro.diLoc] &= (~DI_BIT_DATA_MASK_1BYTE);
			}
			Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
			break;

		case REG_BIT_WP_CC:
		case REG_BIT_WP_LA:
		case REG_BIT_WP_UC:
			markbit = 1;
			markbit <<= (DevCommPro.addr - REG_BIT_ID_LOG_FLAG);
			if(RunSta.wpFlag.byte&markbit)
			{
				(*uart).Rec.Buff[DevCommPro.diLoc] |=DI_BIT_DATA_MASK_1BYTE;
			}
			else
			{
				(*uart).Rec.Buff[DevCommPro.diLoc] &= (~DI_BIT_DATA_MASK_1BYTE);
			}
			Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
			break;
			
		case REG_BIT_CHECK_FLAG:
			if(RunSta.self_check)
			{
				(*uart).Rec.Buff[DevCommPro.diLoc] |=DI_BIT_DATA_MASK_1BYTE;
			}
			else
			{
				(*uart).Rec.Buff[DevCommPro.diLoc] &= (~DI_BIT_DATA_MASK_1BYTE);
			}
			Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
			break;
			
		default:
			  Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart); 
			break;
	}
}


/***=======================================================================
//Function:     void Write_Byte_Reg(void)
//Description:  
//Parameters:   None
//Return Value: None
//-----------------------------------------------------------------------------------------------
//Date:         2012.03
//=======================================================================***/

void Write_Byte_Reg(UART_BUFFER  *uart)
{
	//UINT16 data16;
	UINT8 temp;
	//UINT8 succ;
	//succ = TRUE;
	
    DevCommPro.anslen = DevCommPro.dataLoc-1;
	
	switch(DevCommPro.addr)
	{
		case REG_BYTE_TRANS_EQUIP_CON:
			temp = (*uart).Rec.Buff[DevCommPro.dataLoc]; 
			if(temp<=1)
			{
				System.trans_equip_connect = (*uart).Rec.Buff[DevCommPro.dataLoc]; 
				Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
				Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
			}
			break;
		case REG_BYTE_CTR_LOOP_START:
			temp = (*uart).Rec.Buff[DevCommPro.dataLoc]; 
			if (temp <= System.ctr_loop_end)
			{
				System.ctr_loop_start= (*uart).Rec.Buff[DevCommPro.dataLoc]; 
			}
			else
			{
				System.ctr_loop_start = System.ctr_loop_end;
				System.ctr_loop_end = (*uart).Rec.Buff[DevCommPro.dataLoc];
			}
			Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
			Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
			break;
		case REG_BYTE_CTR_LOOP_END:
			temp = (*uart).Rec.Buff[DevCommPro.dataLoc]; 
			if (temp >= System.ctr_loop_start)
			{
				System.ctr_loop_end= (*uart).Rec.Buff[DevCommPro.dataLoc]; 
			}
			else
			{
				System.ctr_loop_end = System.ctr_loop_start;
				System.ctr_loop_start = (*uart).Rec.Buff[DevCommPro.dataLoc];
			}
			Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
			Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
			break;
        
        case REG_RECOVER_FACTORY:
			Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
            Recover_Factory_Set();
        
            break;
        
		case REG_BYTE_INIT:
			Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
			Change_Byte(&CtrCommSta.LogFlag,uart);
			CtrCommSta.LogDeadLine = C_DEADLINE_TIME_10S;
			break;

		case REG_BYTE_PARA_WR_EN:
			Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
			RunSta.wpFlag.byte = (*uart).Rec.Buff[DevCommPro.dataLoc];
//			Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));	
			break;	
		case CRT_TYPE:
			System.ctr_type = (*uart).Rec.Buff[DevCommPro.dataLoc]; 
			Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
			Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
			break;
		case REG_BYTE_KEY_IDENTITY:
			Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
            if (((*uart).Rec.Buff[DevCommPro.dataLoc+1]) & BIT0)		//自检
            {        
            	if(((*uart).Rec.Buff[DevCommPro.dataLoc]) & BIT0)
            	{
            		//spk_self_check();
            	}
            }

			if (((*uart).Rec.Buff[DevCommPro.dataLoc+1]) & BIT1)		//巡检控制
            {        
            	if(((*uart).Rec.Buff[DevCommPro.dataLoc]) & BIT1)
				{
					//Flash_Load_Info(SYSCONFIG,0,(UINT8*)(&System)); 
				}
				else
				{
					//System.poll_period = 0; 							//停止为临时停止
				}
            }
			
			if (((*uart).Rec.Buff[DevCommPro.dataLoc+1]) & BIT2)		//校验
            {        
            	if(((*uart).Rec.Buff[DevCommPro.dataLoc]) & BIT2)
				{
					Flash_Load_Info(SYSCONFIG,0,(UINT8*)(&System)); 
				}
				else
				{
//					System.VerifyPeriod = 0;							//停止为临时停止
				}
            }
            
			break;

			
		case REG_BYTE_CTR_ERASE_RECARD:         //一般情况下，禁止此操作，记录不容删除
			Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
		  Delete_All_Record();
			break;
		case REG_BYTE_CTR_RS232_PBS:
			temp = (*uart).Rec.Buff[DevCommPro.dataLoc];
			if(temp<5 )
			{
				Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
				Contr_232Uart_Init(temp);
				Contr_232Uart_Sta_Switch(UART_REC_INIT);
			}
			break;
		case REG_BYTE_CTR_RS485_PBS:
			temp = (*uart).Rec.Buff[DevCommPro.dataLoc];
			if(temp<5)
			{
				Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
				Contr_485Uart_Init(temp);
				Contr_485Uart_Sta_Switch(UART_REC_INIT);
			}
			else
			{
				Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart); 
			}
			break;

		case REG_BYTE_LA:
            System.la = (*uart).Rec.Buff[DevCommPro.dataLoc];
			Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
			Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
			break;
        
		default:
			Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart); 
			break;
	}

	/*if(succ == FALSE)
	{
		Abnormal_Ans_Process(INT_TYPE_LIMIT_PERMISSIONS,uart);
	}  */
}


/***=======================================================================
//Function:     void Read_Byte_Reg(void)
//Description:  
//Parameters:   None
//Return Value: None
//-----------------------------------------------------------------------------------------------
//Date:         2012.03
//=======================================================================***/

void Read_Byte_Reg(UART_BUFFER  *uart)
{
    UINT8 data=0;
	switch(DevCommPro.addr)
	{
		//case REG_BYTE_INIT:
		case REG_BYTE_KEY_IDENTITY:
		//case REG_BYTE_SHIELD:
		case REG_BYTE_PARA_WR_EN:
			(*uart).Send.Buff[DevCommPro.dataLoc] = RunSta.wpFlag.byte;
			Ans_Process_Normal(uart,1,(*uart).Rec.Buff); 
			break;

		case REG_BYTE_LA:
			Ans_Process_Normal(uart,1,&System.la);
			break;
		case CRT_TYPE:
			Ans_Process_Normal(uart,1,&System.ctr_type);
			break;

		case REG_BYTE_COM_RESULT:
			
			Ans_Process_Normal(uart,1,&Debug.Gprs);
			//Debug.Gprs = 0;
			break;

		case REG_BYTE_CTR_OPERATE_AUTH:
			data = Auth.PasswordLevel;
			Ans_Process_Normal(uart,1,(UINT8*)(&(data)));
			break;



		default:
			Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart); 
			break;
	}
}



/***=======================================================================
//Function:     void Write_Mul_Byte_Reg(void)
//Description:  
//Parameters:   None
//Return Value: None
//-----------------------------------------------------------------------------------------------
//Date:         2013.08
//=======================================================================***/

void Write_Mul_Byte_Reg(UART_BUFFER  *uart)
{
	UINT8 i,digit_num=0;
    UINT8 succ;
	succ = TRUE;

    DevCommPro.dataLoc = DevCommPro.diLoc + 3;
       
    switch (DevCommPro.addr)
    {		
        case REG_RTC_SS:
			if(DevCommPro.datalen != 7)
          	{
             	succ = FALSE;
          	}
          	else
          	{   
				memcpy(time_string,&(*uart).Rec.Buff[DevCommPro.dataLoc],7);
			 	Set_RTC_Time(time_string);
				Ans_Process_Normal(uart,0,&(*uart).Rec.Buff[DevCommPro.dataLoc]);
				GET_RTC_TIME();
          	}
            break;

        case REG_BYTE_ID:
            if ((DevCommPro.datalen != 12) 
				||(B_FRAME_ADDRESSING != C_CTR_BROADCAST_ADR)
                ||((*uart).Rec.Buff[DevCommPro.dataLoc+11] != FIRE_EXTINGUISHER_SYSTEM)
                ||((*uart).Rec.Buff[DevCommPro.dataLoc+10] != DEVICE_TYPE)
                ||((*uart).Rec.Buff[DevCommPro.dataLoc+9] != TYPE_LEVEL)
                )
            {
               succ = FALSE;
            } 
            else 
            {
                for(i=0;i<12;i++)
                {
                    if(System.id[i]!=0x00)
                    {
                        break;
                    }
                }             
                if(i>=12)
                {
                    memcpy(System.id,&(*uart).Rec.Buff[DevCommPro.dataLoc],12);
                    RunSta.wpFlag.byte &= 0xFE;
                    MemoryCpyInvert(System.id ,System.id,12);
                    Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
                    Ans_Process_Normal(uart,0,(*uart).Rec.Buff);
                }
				else
				{
					succ = FALSE;
				}

            }
        	break;
      
        case REG_BYTE_UC:
            if((DevCommPro.datalen != LEN_REG_UC)
            ||((*uart).Rec.Buff[DevCommPro.dataLoc] != DEVICE_TYPE_OF_UC_CODE))
            {
                succ = FALSE;
            }
            else 
            {
				memcpy(System.uc,&(*uart).Rec.Buff[DevCommPro.dataLoc],LEN_REG_UC);
				MemoryCpyInvert(System.uc ,System.uc,LEN_REG_UC);
				Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
				Ans_Process_Normal(uart,0,&(*uart).Rec.Buff[DevCommPro.dataLoc]);
            }
        	break;
		case CTR_ADDR:
			if((DevCommPro.datalen != LEN_REG_UC)
            ||((*uart).Rec.Buff[DevCommPro.dataLoc] != DEV_TYPE_CONTROL_UC))
            {
                succ = FALSE;
            }
            else 
            {
				memcpy(System.ctr_addr,&(*uart).Rec.Buff[DevCommPro.dataLoc],LEN_REG_UC);
				MemoryCpyInvert(System.ctr_addr ,System.ctr_addr,LEN_REG_UC);
				Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
				Ans_Process_Normal(uart,0,&(*uart).Rec.Buff[DevCommPro.dataLoc]);
            }
			break;

		case REG_BYTE_CC:
            if ((DevCommPro.datalen != 32))
            {
                succ = FALSE;
            }
            else
            {
				memcpy(System.cc,&(*uart).Rec.Buff[DevCommPro.dataLoc],32);
				MemoryCpyInvert(System.cc ,System.cc,32);
				if(System.cc[31]>0xA0)
				{
					for(i=0;i<32;i++)
					{
						if(System.cc[i]<0xA0)
						digit_num ++;
					}
					if(digit_num%2==1)
					{
						System.cc[31]=0x00;
					}
				}
				Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
				Ans_Process_Normal(uart,0,&(*uart).Rec.Buff[DevCommPro.dataLoc]);
            }
            break;
			
        case REG_BYTE_PROTOCOL_VER_REG:
            if ((DevCommPro.datalen != 3))
            {
                succ = FALSE;
            }
            else
            {
				memcpy(System.product_ver,&(*uart).Rec.Buff[DevCommPro.dataLoc],3);
				Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
				Ans_Process_Normal(uart,0,&(*uart).Rec.Buff[DevCommPro.dataLoc]);
            }
            break;
        case REG_BYTE_PROJECT_NAME:
          	if((DevCommPro.datalen <= LEN_PROJECT_NAME))
          	{
				memset(System.project_name,0x00,32);
				memcpy(System.project_name,&(*uart).Rec.Buff[DevCommPro.dataLoc],DevCommPro.datalen);
				MemoryCpyInvert(System.project_name ,System.project_name,32);
				if(System.project_name[31]>0xA0)//最后一个缓存为半个汉字处理
				{
				    for(i=0;i<32;i++)
					{
						if(System.project_name[i]<0xA0)
						digit_num ++;
					}
					if(digit_num%2==1)
					{
						System.project_name[31]=0x00;
					}
				}
				Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
				Ans_Process_Normal(uart,0,&(*uart).Rec.Buff[DevCommPro.dataLoc]);
          	}
          	else
          	{    
				succ = FALSE;
          	}
        break;
		
	 	case REG_BYTE_PROJECT_ADDR:
          	if((DevCommPro.datalen <= LEN_PROJECT_ADDR))
          	{
				memset(System.project_address,0x00,32);
				memcpy(System.project_address,&(*uart).Rec.Buff[DevCommPro.dataLoc],DevCommPro.datalen);
				MemoryCpyInvert(System.project_address ,System.project_address,32);
				if(System.project_address[31]>0xA0)
				{
					for(i=0;i<32;i++)
					{
						if(System.project_address[i]<0xA0)
						digit_num ++;
					}
					if(digit_num%2==1)
					{
						System.project_address[31]=0x00;
					}
				}

				Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
				Ans_Process_Normal(uart,0,&(*uart).Rec.Buff[DevCommPro.dataLoc]);
          	}
          	else
          	{    
				succ = FALSE;
          	}
        	break;

		case REG_BYTE_PROJECT_CONTACT:
          	if((DevCommPro.datalen <= LEN_PROJECT_CONTACT))
          	{
				memset(System.project_linkman,0x00,16);
				memcpy(System.project_linkman,&(*uart).Rec.Buff[DevCommPro.dataLoc],DevCommPro.datalen);
				MemoryCpyInvert(System.project_linkman ,System.project_linkman,16);
				Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
				Ans_Process_Normal(uart,0,&(*uart).Rec.Buff[DevCommPro.dataLoc]);
          	}
          	else
          	{    
				succ = FALSE;
          	}
        	break;
		
		case REG_BYTE_PROJECT_TEL:
          	if((DevCommPro.datalen <= LEN_PROJECT_TEL))
          	{
				memset(System.project_phone,0x00,16);
				memcpy(System.project_phone,&(*uart).Rec.Buff[DevCommPro.dataLoc],DevCommPro.datalen);
				MemoryCpyInvert(System.project_phone ,System.project_phone,16);
				Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
				Ans_Process_Normal(uart,0,&(*uart).Rec.Buff[DevCommPro.dataLoc]);
          	}
          	else
          	{    
				succ = FALSE;
          	}
        	break;
		
		case REG_BYTE_PROJECT_EMAIL:
          	if((DevCommPro.datalen <= LEN_PROJECT_EMAIL))
          	{
				memset(System.project_email,0x00,32);
				memcpy(System.project_email,&(*uart).Rec.Buff[DevCommPro.dataLoc],DevCommPro.datalen);
				MemoryCpyInvert(System.project_email ,System.project_email,32);
				Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
				Ans_Process_Normal(uart,0,&(*uart).Rec.Buff[DevCommPro.dataLoc]);
          	}
          	else
          	{    
				succ = FALSE;
          	}
       	 	break;



		case REG_BYTE_CTR_INFO:
		if(DevCommPro.datalen == 0X32)
		{
			if(System.trans_equip_connect)
    		{
				MemoryCpyInvert(&System.trans_addr[0],&(*uart).Rec.Buff[DevCommPro.dataLoc+12],LEN_REG_UC);
				B_Upload_Self_Info = 1;
			}
			else
			{
				MemoryCpyInvert(&System.ctr_addr[0],&(*uart).Rec.Buff[DevCommPro.dataLoc+12],LEN_REG_UC);
				Ans_Process_Normal(uart,0,&(*uart).Rec.Buff[DevCommPro.dataLoc]);
			}
			Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));
		}
		else
		{	 
			succ = FALSE;
		}			 
		break;

		
        default:
            Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart);
        break; 
    }
	//写回路和节点信息
	if(succ == FALSE)
	{
		Abnormal_Ans_Process(INT_TYPE_LIMIT_CONDITION_FAU,uart);
	}  
}



/***=======================================================================
//Function:     void Read_Mul_Byte_Reg_Para_Inf(void)
//Description:  
//Parameters:   None
//Return Value: None
//--------------------------------------------------------------------------
//Date:         2013.04
//=======================================================================***/
void Read_Mul_Byte_Reg_Para_Inf(UART_BUFFER  *uart)
{
	UINT8 regoffset;									//寄存器偏置
	UINT8 buff[64];
	
	memset(buff,0x00,sizeof(buff));

	
	regoffset= (DevCommPro.addr - REG_BYTE_CTR_VERIFY_PERIOD);	
	Ans_Process_Normal(uart,DevCommPro.datalen,&buff[regoffset]);
}

/***=======================================================================
//Function:     void (void)
//Description:  
//Parameters:   None
//Return Value: None
//--------------------------------------------------------------------------
//Date:         2013.03
//=======================================================================***/
void Read_Mul_Byte_Reg(UART_BUFFER  *uart)
{
  //  UINT8 len,start;
	UINT8 str[130];
	UINT8 succ;
	succ = TRUE;
	DevCommPro.anslen = DevCommPro.dataLoc+DevCommPro.datalen;
    switch (DevCommPro.addr)
    {
        case REG_BYTE_SHIELD:
            if (DevCommPro.datalen != 8)
            {
                succ = FALSE;
            } 
            else
            {
				Ans_Process_Normal(uart,DevCommPro.datalen,str);
            }
        break;

		case REG_BYTE_CTR_POLL_CYC://上传配置
			Read_Mul_Byte_Reg_Para_Inf(uart);
			break; 

    case REG_RECORD_NUM:
            if (DevCommPro.datalen != 2)
            {
                succ = FALSE;
            } 
            else
            {
                UINT16 record_num = MAXN_FLASH_RECORD;
				Ans_Process_Normal(uart,DevCommPro.datalen,(UINT8 *)&record_num);
            } 
        
            break;
        case REG_BYTE_RANDON_DATA:
            if (DevCommPro.datalen != 8)
            {
                succ = FALSE;
            } 
            else
            {
                //Random_Data_Cal(str);
				Ans_Process_Normal(uart,DevCommPro.datalen,str);
            }  
        break;
		
			
        case REG_BYTE_ID:
            if (DevCommPro.datalen != 12)
            {
                  succ = FALSE;
            } 
            else
            {
				DevCommPro.wildcardFlag = FALSE;
				memcpy(str, System.id,12);
                //memcpy(str, LoopInf[0].TdId,12);
				MemoryCpyInvert(str ,str,12);
				Ans_Process_Normal(uart,DevCommPro.datalen,str);
            }
        break;    
        case REG_BYTE_UC:
            if (DevCommPro.datalen != LEN_REG_UC)
            {
                 succ = FALSE;
            }
            else
            {
				memcpy(str,System.uc,LEN_REG_UC);
                //memcpy(str,LoopInf[0].Tduc,4);
				MemoryCpyInvert(str ,str,LEN_REG_UC);
				Ans_Process_Normal(uart,DevCommPro.datalen,str);
            }
        break;
		
		case CTR_ADDR:
			if (DevCommPro.datalen != LEN_REG_UC)
			{
				 succ = FALSE;
			}
			else
			{
				memcpy(str,System.ctr_addr,LEN_REG_UC);
				//memcpy(str,LoopInf[0].Tduc,4);
				MemoryCpyInvert(str ,str,LEN_REG_UC);
				Ans_Process_Normal(uart,DevCommPro.datalen,str);
			}
		break;

        case REG_BYTE_SOFT_VER_REG:
            if (DevCommPro.datalen != 3)
            {
                 succ = FALSE;
            } 
            else
            {
            	memcpy(str,System.software_ver,3);
				MemoryCpyInvert(str ,str,3);
                Ans_Process_Normal(uart,DevCommPro.datalen,str);
            }
        break;
      
        case REG_BYTE_HARD_VER_REG:
            if (DevCommPro.datalen != 3)
            {
                succ = FALSE;
            }
            else
            {
            	memcpy(str,System.hardware_ver,3);
				MemoryCpyInvert(str ,str,3);
				 Ans_Process_Normal(uart,DevCommPro.datalen,str);
            }
        break;
      
       case REG_BYTE_PROTOCOL_VER_REG:
            if( DevCommPro.datalen != 3)
            {
                 succ = FALSE;
            }
            else
            {
				memcpy(str,System.product_ver,3);
				//MemoryCpyInvert(str ,str,3);
				Ans_Process_Normal(uart,DevCommPro.datalen,str);
            }
        break;
      
        case REG_BYTE_CC:
            if (DevCommPro.datalen <= 32)
            {
				memcpy(str,System.cc,32);
				MemoryCpyInvert(str ,str,32);
				Ans_Process_Normal(uart,DevCommPro.datalen,str);
            }
            else
            {
                 succ = FALSE;
            }
        break;
            		
        case REG_BYTE_PROJECT_NAME:
            if(DevCommPro.datalen <= 32)
            {
				memcpy(str,System.project_name,32);
				MemoryCpyInvert(str ,str,32);
				Ans_Process_Normal(uart,DevCommPro.datalen,str);
            }
            else
            {
                 succ = FALSE;
            }
        break; 
		
		case REG_BYTE_PROJECT_ADDR:
            if(DevCommPro.datalen <= 32)
            {
				memcpy(str,System.project_address,32);
				MemoryCpyInvert(str ,str,32);
				Ans_Process_Normal(uart,DevCommPro.datalen,str);
            }
            else
            {
                 succ = FALSE;
            }
        break; 
		
		case REG_BYTE_PROJECT_CONTACT:
            if(DevCommPro.datalen <= 16)
            {
				memcpy(str,System.project_linkman,16);
				MemoryCpyInvert(str ,str,16);
				Ans_Process_Normal(uart,DevCommPro.datalen,str);
            }
            else
            {
                 succ = FALSE;
            }
        break; 
		
		case REG_BYTE_PROJECT_TEL:
            if(DevCommPro.datalen <= 16)
            {
				memcpy(str,System.project_phone,16);
				MemoryCpyInvert(str ,str,16);
				Ans_Process_Normal(uart,DevCommPro.datalen,str);
            }
            else
            {
                 succ = FALSE;
            }
        break; 
		case REG_BYTE_PROJECT_EMAIL:
            if(DevCommPro.datalen <= 32)
            {
				memcpy(str,System.project_email,32);
				MemoryCpyInvert(str ,str,32);
				Ans_Process_Normal(uart,DevCommPro.datalen,str);
            }
            else
            {
                 succ = FALSE;
            }
        break; 

        default:
			
			Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart);
        break; 
    }
	if( succ == FALSE)
	{
		Abnormal_Ans_Process(INT_TYPE_LIMIT_CONDITION_FAU,uart);
	}
}
/**********************************************************************/
//Prototype: UINT8 Check_Reg_Valid(UINT16 offset)
//Function:   判断有效性
//    Date:  04-4-13 
/**********************************************************************/

//回路：1-4
//节点：1-64
UINT8 Check_Reg_Valid(UINT16 offset)
{
    UINT8 result = TRUE;
    if((offset/256) == 0                //第0回路
    || (offset%256) == 0                //每个回路的第0个设备
    || (offset%256) > MAX_DEVICE_NUM)   //每个回路大于64的设备
    {
        result = FALSE;
    }
    return result;
}


/**********************************************************************/
//Prototype: void Write_MutBlock_Reg(void)
//Function:   写块寄存器的操作，对应协议4.6的操作，和上位
//                 机软件同步配置数据的过程
//      IN:  
//     OUT: 
//    Date:  04-25-12 
// update:  17-07-12     添加系统配置数据的下载
//  Author:   
/**********************************************************************/
void Write_MutBlock_Reg(UART_BUFFER  *uart)
{  

	 //不支持

	Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart);
}



/**********************************************************************/
//Prototype: void Read_MutBlock_Reg_Record(void)
//Function:   读记录信息
//    Date:  04-4-13 
/**********************************************************************/
void Read_MutBlock_Reg_Record(UART_BUFFER  *uart)
{ 
	UINT16 i;
	UINT32 offset;
	UINT8 str[400];
	RECORD_STRUCT record;

	//无需考虑记录的时间顺序，调试软件按顺序取完5120条自己排序
	if((DevCommPro.addr & (RECORD_LEN-1))==0)						//整128字节操作
	{
		DevCommPro.anslen = 0;
		memset(str,0x00,sizeof(str));
		for(i=0;i<DevCommPro.block_count;i++)
  		{
  			memset(&record,0x00,sizeof(record));
  			offset = (DevCommPro.addr - REG_RECORD)/RECORD_LEN;
			if(offset>MAXN_FLASH_RECORD)
			{
				str[DevCommPro.anslen++] = FLAG_DATA_BLOCK_INVAILD;
			}
			else           //提取有效信息，80个字节
			{
				if(Flash_Load_Info(RECORD,offset,&record.Sta))
				{ 
					str[DevCommPro.anslen++] = FLAG_DATA_BLOCK_VAILD;
					str[DevCommPro.anslen++] = record.Type;
					memcpy(&str[DevCommPro.anslen],record.Time,6);
					DevCommPro.anslen+=6;
					memcpy(&str[DevCommPro.anslen],record.Uc,LEN_REG_UC);
					DevCommPro.anslen+=5;
					memcpy(&str[DevCommPro.anslen],record.Cc,32);		
					DevCommPro.anslen+=32;
					memcpy(&str[DevCommPro.anslen++],&record.EventType.byte[1],1);
					memcpy(&str[DevCommPro.anslen++],&record.EventType.byte[0],1);
					memcpy(&str[DevCommPro.anslen++],&record.EventLen,1);
					memcpy(&str[DevCommPro.anslen+RELADR_RECORD_DATA], record.Event ,sizeof(record.Event));
					DevCommPro.anslen+=32;
				}
				else
				{
					str[DevCommPro.anslen++] = FLAG_DATA_BLOCK_INVAILD;
				}
			}
			DevCommPro.addr += DevCommPro.unit_len;
		}
		Ans_Process_Normal(uart,DevCommPro.anslen,str);
	}
	else
	{
		Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart);
	}
}

/**********************************************************************/
//Prototype: void Read_MutBlock_Reg(void)
//Function:   读块寄存器的操作，对应协议4.6的操作              
//         IN:  
//      OUT: 
//      Date:  04-21-12 
//   update:  27-06-12   添加CRT同步一个字节的状态信息
//   Author:   
/**********************************************************************/
void Read_MutBlock_Reg(UART_BUFFER  *uart)
{
	if((DevCommPro.addr >= REG_RECORD)
		&& (DevCommPro.addr < REG_END))      			//记录信息
	{
		Read_MutBlock_Reg_Record(uart);
	}
	else                                                //不支持
	{
		Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart);
	}
}
/**********************************************************************/
//Prototype: void Dev_Frame_Handle(void)
//Function:   根据寄存器地址和命令处理方式进行相应的
//                 处理
//      IN:  
//     OUT: 
//    Date:  04-21-12 
//  Author:   
/**********************************************************************/
void Dev_Frame_Handle_Self_Comm(UART_BUFFER  *uart)
{
	switch(DevCommPro.mode)
	{
		case C_WRITE_SPECIAL_CODE_MODE:
			 Write_Special_Comm(uart);
		break;

		case C_READ_SPECIAL_CODE_MODE:
			 Read_Special_Comm(uart);
		break;

		case C_WRITE_BIT_MODE:
			Write_Bit_Reg(uart);
		break;

		case C_READ_BIT_MODE:
			Read_Bit_Reg(uart);
		break;

		case C_WRITE_BYTE_MODE:
				Write_Byte_Reg(uart);
		break;

		case C_READ_BYTE_MODE:
			Read_Byte_Reg(uart);
		break;
		
		case C_WRITE_MORE_BYTE_MODE:
		case C_WRITE_MUl_BYTE_MODE:

				Write_Mul_Byte_Reg(uart);

		break;
		
		case C_READ_MORE_BYTE_MODE:
		case C_READ_MUl_BYTE_MODE:
			Read_Mul_Byte_Reg(uart);
		break;

		case C_WRITE_MUl_BLOCK_MODE:
			 Write_MutBlock_Reg(uart);
			 break;
			 
		case C_READ_MUl_BLOCK_MODE:
			 Read_MutBlock_Reg(uart);
			 break;
		default:
			Abnormal_Ans_Process(INT_TYPE_NODECOMM_FAU,uart);
		break;	
	}

}

/**********************************************************************/
//Prototype: void Dev_Frame_Handle(void)
// Function: 设备模式帧处理
//       IN: 串口缓存
//      OUT: 无
//     Date: 13.7.19
/**********************************************************************/
void Dev_Frame_Handle(UART_BUFFER  *uart)
{
	if((DevCommPro.MmadrFlag == TRUE)
		&&(DevCommPro.wildcardFlag != TRUE))						//透传命令
	{
		Ans_Process_Comm_Ans(uart,0,(*uart).Rec.Buff);
	}
	else
	{

		Dev_Frame_Handle_Self_Comm(uart);                           //自身操作
	}
}


