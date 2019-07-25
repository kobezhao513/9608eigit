/*
 * bus_dn_basic_cmd.h
 *
 *  Created on: 2012-7-20
 *      Author: Administrator
 */

#ifndef BUS_BASIC_CMD_H_
#define BUS_BASIC_CMD_H_
//------------------------------------- include -------------------------------------

#include "global.h"
#include "uart.h"

														                // 总线任务类型
#define	TYPE_DEV_LOG			    0x01
#define	TYPE_COMM_TC			    TYPE_DEV_LOG+1
#define	TYPE_START_COM	      TYPE_COMM_TC+1
#define	TYPE_DOWN_INF			    TYPE_START_COM+1
#define	TYPE_POLL			      	TYPE_DOWN_INF+1
#define	TYPE_ONLINE				    TYPE_POLL+1
#define TYPE_BRO_TIME			    TYPE_ONLINE+1
#define TYPE_DEBUG   			    TYPE_BRO_TIME+1
#define TYPE_SYNC_AREA_UC			TYPE_DEBUG+1
#define TYPE_POW24				    TYPE_SYNC_AREA_UC+1
#define TYPE_RELAY				    TYPE_POW24+1
#define TYPE_RESET                  TYPE_RELAY+1

void Comm_Repeat_Send(UART_BUFFER *buff);
UINT8 Comm_Repeat_Check(UART_BUFFER *buff);

UINT8 Comm_Judge_Answer(UART_BUFFER *uart);

void Bus_Check_Comm_Need_ANS(UART_BUFFER  *comm_buff, UINT8 la);

void Bus_Prepare_Send(UART_BUFFER *uart,UINT8 len);
void Bus_Spec_Comm( UART_BUFFER *uart,UINT8 bufpc,UINT8 comm,UINT8 len,UINT8 *buf);

void Bus_Check_Comm_Need_ANS(UART_BUFFER  *comm_buff, uchar tdla);
void Bus_Wr_Bit_Reg(UART_BUFFER *uart,UINT8 bufpc,UINT16 addr,UINT8 data);
void Bus_Wr_Byte_Reg(UART_BUFFER *uart,UINT8 bufpc,UINT16 addr,UINT8 data);
void Bus_Wr_NByte_Reg(UART_BUFFER *uart,UINT8 bufpc,UINT32 addr,UINT8 len,UINT8 *buf);
void Bus_Re_NByte_Reg(UART_BUFFER *uart,UINT8 bufpc,UINT32 addr,UINT8 len);

void Bus_La_Wr(UART_BUFFER *uart,UINT8 la);
void Bus_La_Wr_Spec_Comm(UART_BUFFER *uart,UINT8 la,UINT8 comm,UINT8 len,UINT8 *buf);
void Bus_La_Wr_Bit_Reg(UART_BUFFER *uart,UINT8 la,UINT16 addr,UINT8 data);
void Bus_La_Wr_Byte_Reg(UART_BUFFER *uart,UINT8 la,UINT16 addr,UINT8 data);
void Bus_La_Wr_NByte_Reg(UART_BUFFER *uart,UINT8 la,UINT32 addr,UINT8 len,UINT8 *buf);
void Bus_La_Re(UART_BUFFER *uart,UINT8 la);
void Bus_La_Re_Spec_Comm(UART_BUFFER *uart,UINT8 la,UINT8 comm,UINT8 len,UINT8 *buf);
void Bus_La_Re_NByte_Reg(UART_BUFFER *uart,UINT8 la,UINT32 addr,UINT8 len);
UINT8 Caculate_Frame_Num(UART_BUFFER  *uart);


#endif /* BUS_DN_BASIC_CMD_H_ */
