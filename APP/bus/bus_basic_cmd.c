/*
 * bus_dn_basic_cmd.c
 *
 *  Created on: 2012-7-20
 *      Author: Administrator
 */


#include "bus_basic_cmd.h"

#include "xycnbus.h"
#include "basic.h"
//===============================================================================
//函数描述: 命令重发处理
//输入参数: UART_BUFFER *buff:缓存指针
//输出参数: 无
//修改日期: 2014-07
//修改人员: 魏立彬
//===============================================================================
void Comm_Repeat_Send(UART_BUFFER *buff)												
{
	buff->Send.ActualLen = 0;
	buff->FrameTimer     = 0;
	buff->RunSta         = RUN_STA_TXD;
	buff->Send.Crc.word  = 0xFFFF; 
	memset(buff->Rec.Buff,0x00,UART_BUFFER_LEN);
}
//===============================================================================
//函数描述: 命令重发监控
//输入参数: UART_BUFFER *buff:缓存指针
//输出参数: 无
//修改日期: 2014-07
//修改人员: 魏立彬
//===============================================================================
UINT8 Comm_Repeat_Check(UART_BUFFER *buff)
{
	if(buff->Send.RepeatNum)
	{
		buff->Send.RepeatNum--;
		return 1;
	}
	return 0;
}

//=============================================================================
//功能描述: 命令应答帧检验
//输入参数: 接口缓存
//输出参数: 1:正确 0:错误
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
UINT8 Comm_Judge_Answer(UART_BUFFER *uart)
{
	UINT8 flag,addrloc;
	
	flag = BUS_ANS_TYPE_ERR;

	if(uart->Rec.Status == UART_REC_OVER)
	{
		//if(uart->Send.Delay != PDC_COM_BROD_DELAYTIME)
		//{ 
			if((uart->Send.Buff[TC_LOC_CTR]&(CTR_ADDR_MODE|CTR_RE_OR_WR|CTR_EXPAND_CTR))         				
				== (uart->Rec.Buff[TC_LOC_CTR]&(CTR_ADDR_MODE|CTR_RE_OR_WR|CTR_EXPAND_CTR)))      //发出的与应答的一致	 
			{
				if((uart->Rec.Buff[TC_LOC_CTR]&(CTR_ANSWER_STA|CTR_TANS_DIR)) == CTR_ANSWER_STA)	//正常应答
				{
					if(uart->Send.Buff[TC_LOC_CTR]&CTR_MMADR)
					{
						addrloc = 3;
					}
					else
					{
						addrloc = 4;
					}
					switch(uart->Rec.Buff[TC_LOC_CTR]&CTR_ADDR_MODE)
					{
						case CTR_ADDR_MODE_ID:
							if(memcmp(&uart->Rec.Buff[3],&uart->Send.Buff[addrloc],LEN_REG_ID) == 0)
							{
								flag = BUS_ANS_TYPE_OK;
							}
							break;		
							
						case CTR_ADDR_MODE_LA:
							if(memcmp(&uart->Rec.Buff[3],&uart->Send.Buff[addrloc],LEN_REG_LA) == 0)
							{
								flag = BUS_ANS_TYPE_OK;
							}
							break;
						
						case CTR_ADDR_MODE_UC:
							if(memcmp(&uart->Rec.Buff[3],&uart->Send.Buff[addrloc],LEN_REG_UC) == 0)
							{
								flag = BUS_ANS_TYPE_OK;
							}
							break;
							
						default:
							break;
					}
				}
				else
				{
					flag = BUS_ANS_TYPE_ABNORMAL;
				}
			}
		//}
		//else
		//{
			//flag = BUS_ANS_TYPE_SPECIAL;
		//}
	}
	return(flag);								
}
//=============================================================================
//功能描述: 准备向一级总线发送发数据，送缓存内数据
//输入参数: 控制字，发送字节长度len
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_Prepare_Send(UART_BUFFER *uart,UINT8 len)
{   
	uart->Send.Buff[0] = FRAME_HEADER;					//加帧头
	uart->Send.Buff[1] = len;
	len++;
	uart->Send.TotalLen = len;
	uart->Send.ActualLen  = 0;
	uart->Rec.Status = UART_REC_HEARD;
	uart->Send.RepeatNum = UART_REPEAT_TIMES;
	uart->RunSta = RUN_STA_TXD;
	uart->FrameTimer = 0;
	uart->Send.Status = UART_SEND_DATA;

	memset(uart->Rec.Buff,0x00,UART_BUFFER_LEN);

	
}

//=============================================================================================
//函    数： void Bus_Check_Comm_Need_ANS(uchar tdla)
//功    能： 计算命令是否需要应答
//参    数： 无
//返 回 值:  无
//*==============================================================================================*/
void Bus_Check_Comm_Need_ANS(UART_BUFFER  *comm_buff, UINT8 la)
{	
	if(la == BRO_ADDR_LA)									//广播事件
	{
		(*comm_buff).AnsFlag =  FALSE;
	}
	else 
	{
		//if(la<MAX_LOOP_NUM)
		{
			(*comm_buff).AnsFlag =  TRUE;
		}
	}
}


//=============================================================================
//功能描述: 特殊命令
//输入参数: 缓存指针 命令 长度 数据缓存
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_Spec_Comm( UART_BUFFER *uart,UINT8 bufpc,UINT8 comm,UINT8 len,UINT8 *buf)
{
	UINT8 k;
	
	uart->Send.Buff[bufpc] = comm;
	for(k = 0; k < len; k++)
	{
		bufpc++;
		uart->Send.Buff[bufpc] = *buf;
		buf++;
	}
	Bus_Prepare_Send(uart,bufpc);	
}
//=============================================================================
//功能描述: 位写操作命令
//输入参数: 缓存地址 地址 数据
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_Wr_Bit_Reg(UART_BUFFER *uart,UINT8 bufpc,UINT16 addr,UINT8 data)
{	
	if(addr < BIT_ADDR_RANGE_1BYTE)
	{
		uart->Send.Buff[bufpc]  = (UINT8)addr;
		uart->Send.Buff[bufpc] += DI_1BYTE_BIT_COMM;
		if(data)
		{
			uart->Send.Buff[bufpc] += 0x20;
		}
	}
	else if(addr < BIT_ADDR_RANGE_2BYTE)
	{		
		uart->Send.Buff[bufpc]  = (UINT8)(addr&DI_EXTEN_ADDR_MASK);
		uart->Send.Buff[bufpc] += DI_EXTENSION;	
		addr >>= 6;
		bufpc++;
		uart->Send.Buff[bufpc]  = addr&0x000F;
		uart->Send.Buff[bufpc] += DI_2BYTE_BIT_COMM;
		if(data)
		{
			uart->Send.Buff[bufpc] += 0x10;
		}
	}	
	Bus_Prepare_Send(uart,bufpc);					
}
//=============================================================================
//功能描述: 写字节操作命令
//输入参数: 缓存地址 地址 数据
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_Wr_Byte_Reg(UART_BUFFER *uart,UINT8 bufpc,UINT16 addr,UINT8 data)
{		
	if(addr < BYTE_ADDR_RANGE_1BYTE)
	{
		uart->Send.Buff[bufpc]  = addr;
		uart->Send.Buff[bufpc] += DI_1BYTE_BYTE_COMM;
		bufpc++;
		uart->Send.Buff[bufpc]  = data;
		bufpc++;
		uart->Send.Buff[bufpc]  = DATA_BYTE_MASK_ALL;
	}
	else if(addr < BYTE_ADDR_RANGE_2BYTE)
	{	uart->Send.Buff[bufpc]  = (UINT8)(addr&DI_EXTEN_ADDR_MASK);
		uart->Send.Buff[bufpc] += DI_EXTENSION;
		addr >>= 6;
		bufpc++;
		uart->Send.Buff[bufpc]  = (UINT8)(addr&0x001F);
		uart->Send.Buff[bufpc] += DI_2BYTE_BYTE_COMM;
		bufpc++;
		uart->Send.Buff[bufpc]  = data;
		bufpc++;
		uart->Send.Buff[bufpc]  = DATA_BYTE_MASK_ALL;
	}						
	Bus_Prepare_Send(uart,bufpc);					
}
//=============================================================================
//功能描述: 写连续多字节寄存器操作
//输入参数: 缓存地址 地址 长度 数据缓存
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_Wr_NByte_Reg(UART_BUFFER *uart,UINT8 bufpc,UINT32 addr,UINT8 len,UINT8 *buf)
{
	UINT8 k;
	
	if(addr < NBYTE_ADDR_RANGE_2BYTE)
	{
		uart->Send.Buff[bufpc]  = (UINT8)(addr&DI_EXTEN_ADDR_MASK);	//Di0
		uart->Send.Buff[bufpc] += DI_EXTENSION;
		addr>>=6;
		bufpc++;
		uart->Send.Buff[bufpc]  = (UINT8)(addr&0x001F);				//Di1
		uart->Send.Buff[bufpc] += DI_2BYTE_NBYTE_COMM;
		bufpc++;
		uart->Send.Buff[bufpc]  = len;									//Di2
		
		for(k = 0; k < len; k++)
		{	
			bufpc++;
			uart->Send.Buff[bufpc] = *buf;
			buf++;
		}
	}
	else if(addr < NBYTE_ADDR_RANGE_3BYTE)
	{	
		uart->Send.Buff[bufpc]  = (UINT8)(addr&DI_EXTEN_ADDR_MASK);	//Di0
		uart->Send.Buff[bufpc] += DI_EXTENSION;
		
		addr>>=6;
		bufpc++;
		uart->Send.Buff[bufpc]  = (UINT8)(addr&0x000F);				//Di1
		uart->Send.Buff[bufpc] += DI_3BYTE_NBYTE_COMM;

		addr>>=4;
		bufpc++;
		uart->Send.Buff[bufpc]  = (UINT8)(addr&0x007F);				//Di2
		bufpc++;
		uart->Send.Buff[bufpc]  = len;									//Di3
	
		for(k = 0; k < len; k++)
		{	
			bufpc++;
			uart->Send.Buff[bufpc] = *buf;
			buf++;
		}
	}		
	else if(addr < NBYTE_ADDR_RANGE_4BYTE)
	{	
		uart->Send.Buff[bufpc]  = (UINT8)(addr&DI_EXTEN_ADDR_MASK);	//Di0
		uart->Send.Buff[bufpc] += DI_EXTENSION;
		bufpc++;
		addr>>=6;
		uart->Send.Buff[bufpc]  = (UINT8)(addr&0x000F);				//Di1
		uart->Send.Buff[bufpc] += DI_3BYTE_NBYTE_COMM;
		bufpc++;
		addr>>=4;
		uart->Send.Buff[bufpc]  = (UINT8)(addr&0x007F);				//Di2
		uart->Send.Buff[bufpc] += 0x80;
		bufpc++;
		addr>>=7;
		uart->Send.Buff[bufpc] = (UINT8)(addr&0x007F);					//Di2
		bufpc++;
		uart->Send.Buff[bufpc] = len;									//Di3
		
		for(k = 0; k < len; k++)
		{
			bufpc++;
			uart->Send.Buff[bufpc] = *buf;
			buf++;
		}
	}		
	else
	{
	}
	Bus_Prepare_Send(uart,bufpc);			
}


//=============================================================================
//功能描述: 连续读多字节寄存器操作
//输入参数: 缓存地址 地址 长度
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_Re_NByte_Reg(UART_BUFFER *uart,UINT8 bufpc,UINT32 addr,UINT8 len)
{
	if(addr < NBYTE_ADDR_RANGE_2BYTE)
	{
		uart->Send.Buff[bufpc]  = (UINT8)(addr&DI_EXTEN_ADDR_MASK);	//Di0
		uart->Send.Buff[bufpc] += DI_EXTENSION;
		addr>>=6;
		bufpc++;
		uart->Send.Buff[bufpc]  = (UINT8)(addr&0x001F);				//Di1
		uart->Send.Buff[bufpc] += 0x60;
		bufpc++;
		uart->Send.Buff[bufpc]  = len;									//Di2
	}
	else if(addr < NBYTE_ADDR_RANGE_3BYTE)
	{	
		uart->Send.Buff[bufpc]  = (UINT8)(addr&DI_EXTEN_ADDR_MASK);	//Di0
		uart->Send.Buff[bufpc] += DI_EXTENSION;
		
		addr>>=6;
		bufpc++;
		uart->Send.Buff[bufpc]  = (UINT8)(addr&0x000F);				//Di1
		uart->Send.Buff[bufpc] += 0x90;

		addr>>=4;
		bufpc++;
		uart->Send.Buff[bufpc]  = (UINT8)(addr&0x007F);				//Di2
		bufpc++;
		uart->Send.Buff[bufpc]  = len;									//Di3
	}		
	else if(addr < NBYTE_ADDR_RANGE_4BYTE)
	{	
		uart->Send.Buff[bufpc]  = (UINT8)(addr&DI_EXTEN_ADDR_MASK);	//Di0
		uart->Send.Buff[bufpc] += DI_EXTENSION;
		bufpc++;
		addr>>=6;
		uart->Send.Buff[bufpc]  = (UINT8)(addr&0x000F);				//Di1
		uart->Send.Buff[bufpc] += 0x90;
		bufpc++;
		addr>>=4;
		uart->Send.Buff[bufpc]  = (UINT8)(addr&0x007F);				//Di2
		uart->Send.Buff[bufpc] += 0x80;
		bufpc++;
		addr>>=7;
		uart->Send.Buff[bufpc]  = (UINT8)(addr&0x007F);				//Di2
		bufpc++;
		uart->Send.Buff[bufpc]  = len;									//Di3
	}		
	else
	{
	}
	Bus_Prepare_Send(uart,bufpc);			
}

//=============================================================================
//功能描述: TDLA写操作命令
//输入参数: TDLA
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_La_Wr(UART_BUFFER *uart,UINT8 la)
{
	uart->Send.Buff[2] = CTR_LA_WR;		
	uart->Send.Buff[3] = la;
	if(uart->FrameNum == 0x00)
    {
    	uart->FrameNum = 0x01;
    }
	uart->Send.Buff[4] = uart->FrameNum;	
	uart->FrameNum++;
	uart->Send.DiLoc   = DI_LOC_TDLA;
	if(la == BRO_ADDR_LA)
	{
		//uart->Send.Delay = PDC_COM_BROD_DELAYTIME;
		uart->AnsFlag    = FALSE;
	}
	else
	{
		//uart->Send.Delay = PDC_COM_LA_DELAYTIME;
		uart->AnsFlag    = TRUE;
	}
	//uart->FrameInterval  = FRAME_INTERVAL_100MS;
}


//=============================================================================
//功能描述: TDLa写特殊操作命令
//输入参数: LA
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_La_Wr_Spec_Comm(UART_BUFFER *uart,UINT8 la,UINT8 comm,UINT8 len,UINT8 *buf)
{
	Bus_La_Wr(uart,la);
	Bus_Spec_Comm(uart,DI_LOC_TDLA,comm,len,buf);
}

//=============================================================================
//功能描述: TDLA写位操作命令
//输入参数: 回路号 地址 数据
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_La_Wr_Bit_Reg(UART_BUFFER *uart,UINT8 la,UINT16 addr,UINT8 data)
{
	Bus_La_Wr(uart,la);
	Bus_Wr_Bit_Reg(uart,DI_LOC_LA,addr,data);
}

//=============================================================================
//功能描述: 发送TDLA写寄存器操作命令
//输入参数: 回路号 地址 数据
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_La_Wr_Byte_Reg(UART_BUFFER *uart,UINT8 la,UINT16 addr,UINT8 data)
{
	Bus_La_Wr(uart,la);
	Bus_Wr_Byte_Reg(uart,DI_LOC_LA,addr,data);
}
//=============================================================================
//功能描述: 发送TDLA写N字节寄存器操作命令
//输入参数: 回路号 地址 长度 数据缓存
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_La_Wr_NByte_Reg(UART_BUFFER *uart,UINT8 la,UINT32 addr,UINT8 len,UINT8 *buf)
{
	Bus_La_Wr(uart,la);
	Bus_Wr_NByte_Reg(uart,DI_LOC_LA,addr,len,buf);
}

//=============================================================================
//功能描述: TDLA读操作命令
//输入参数: TDLA
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_La_Re(UART_BUFFER *uart,UINT8 la)
{
	uart->Send.Buff[2] = CTR_LA_RE;		
	uart->Send.Buff[3] = la;	
	if(uart->FrameNum == 0x00)
    {
    	uart->FrameNum = 0x01;
    }
	uart->Send.Buff[4] = uart->FrameNum;	
	uart->FrameNum++;
	uart->Send.DiLoc   = DI_LOC_LA;
	if(la == BRO_ADDR_LA)
	{
		//uart->Send.Delay = PDC_COM_BROD_DELAYTIME;
		uart->AnsFlag    = FALSE;
	}
	else
	{
		//uart->Send.Delay = PDC_COM_LA_DELAYTIME;
		uart->AnsFlag    = TRUE;
	}	
}
//=============================================================================
//功能描述: TDLa读特殊操作命令
//输入参数: TDLA 命令 长度 数据缓存
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_La_Re_Spec_Comm(UART_BUFFER *uart,UINT8 la,UINT8 comm,UINT8 len,UINT8 *buf)
{
	Bus_La_Re(uart,la);
	Bus_Spec_Comm(uart,DI_LOC_LA,comm,len,buf);
}

//=============================================================================
//功能描述: 发送TDLA读N字节寄存器操作命令
//输入参数: 回路号 地址 长度
//输出参数: 无
//修改日期: 2014.10.30
//修改人员: 
//=============================================================================
void Bus_La_Re_NByte_Reg(UART_BUFFER *uart,UINT8 la,UINT32 addr,UINT8 len)
{
	Bus_La_Re(uart,la);
	Bus_Re_NByte_Reg(uart,DI_LOC_LA,addr,len);
}


// 计算发送帧的序列号
UINT8 Caculate_Frame_Num(UART_BUFFER  *uart)
{
	(*uart).FrameNum ++;
	if(!(*uart).FrameNum)	
		(*uart).FrameNum ++;
	
	return (*uart).FrameNum;
}


