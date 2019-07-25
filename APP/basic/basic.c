
#include "basic.h"
#include "xycnbus.h"
#include "ex_flash.h"
//================================================== 基础操作函数 ==================================================
//********************************************************************
UINT8 Hex2BCD(UINT8 a)   //十六进制转BCD    
{   
    UINT8 t_h,t_l;   
       
    t_h = a/10;   
    t_l = a - t_h*10;      
    a = (t_h<<4) + t_l;   
       
    return(a);   
}   
   
 //************************************************************
UINT8 BCD2Hex(UINT8 a)   //BCD---->Hex  
{   
    UINT8 t_h,t_l;   
       
	t_h = (a&0xf0)>>4;   
    t_l = a&0x0f;   
    a = t_h*10 + t_l;   
       
    return(a);   
}  
/***********************************************************************/
//Prototype: UINT16 Uchar_To_Uint16(UCHAR8 msb,UCHAR8 lsb)
//Function:   两个单字节，拼凑成双字节            
//      IN:      高低字节
//     OUT:    双字节
//    Date:  04-25-12 
//  Author:   
/**********************************************************************/

UINT16 Uchar_To_Uint16(UINT8 msb,UINT8 lsb)
{
  UINT16 data;

  data = msb;
  data <<= 8;
  data += lsb;
  return data;
}

//计算x的y次方
uint32 Power(uint32 x,uchar y)
{
	uint32 sum = 1;
	
	if(!x)	return 0;
	for(; y>0; y--)	sum *= x;
	return sum;	
}
//十六进制数转换为10进制字串
uchar Uint32_DecString(uint32 hex,uchar *s,uchar len)
{
	uchar i;
	uint32 temp;

	for(i=0; i<len; i++)
	{
		temp = Power(10,(len-i-1));
		if((hex/temp)<10)
		{
			*(s+i) = (hex/temp) + '0';
			hex =  hex % temp;
		}
		else return 0;
	}
	return 1;
}



// 单字节16进制数转换为字符串(每个字节转换2个字串)
void Hex2_String(uchar *s,uchar *hex,uchar len)
{
	uchar i,ch[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	for(i=0;i<len;i++)
	{
		*(s+i*2)   = ch[(*(hex+i)/16)];
		*(s+i*2+1) = ch[(*(hex+i)%16)];
	}	
}

//***************************************************************
void String_2_Hex(UINT8 *hex,UINT8 *s,UINT8 len)//
{
	UINT8 i;

	for(i=0;i<len;i++)
	{		
		if(*(s+i)>='0' && *(s+i)<='9')	
		{
			if(i%2==0)	
			{
				*(hex+i/2) &= 0x0f;
				*(hex+i/2) |= ((*(s+i)-'0')<<4);
			}
			else
			{
				*(hex+i/2) &= 0xf0;
				*(hex+i/2) |= (*(s+i)-'0');
			}
		}
		else if(*(s+i)>='A' && *(s+i)<='F')
		{
			if(i%2==0)	
			{
				*(hex+i/2) &= 0x0f;
				*(hex+i/2) |= ((*(s+i)-'A'+10)<<4);
			}
			else
			{
				*(hex+i/2) &= 0xf0;
				*(hex+i/2) |= (*(s+i)-'A'+10);
			}
		}
		else if(*(s+i)>='a' && *(s+i)<='f')
		{
			if(i%2==0)	
			{
				*(hex+i/2) &= 0x0f;
				*(hex+i/2) |= ((*(s+i)-'a'+10)<<4);
			}
			else
			{
				*(hex+i/2) &= 0xf0;
				*(hex+i/2) |= (*(s+i)-'a'+10);
			}
		}
		else break;
	}	
}
//字串转换为16进制数(每2个字串转换为1个字节,不符合转换标准的退出转换)
UINT8 String_Hex2(uchar *hex,uchar *s,uchar len)
{
	uchar i;

	for(i=0;i<len;i++)
	{		
		if(*(s+i)>='0' && *(s+i)<='9')	
		{
			if(i%2==0)	
			{
				*(hex+i/2) &= 0x0f;
				*(hex+i/2) |= ((*(s+i)-'0')<<4);
			}
			else
			{
				*(hex+i/2) &= 0xf0;
				*(hex+i/2) |= (*(s+i)-'0');
			}
		}
		else if(*(s+i)>='A' && *(s+i)<='F')
		{
			if(i%2==0)	
			{
				*(hex+i/2) &= 0x0f;
				*(hex+i/2) |= ((*(s+i)-'A'+10)<<4);
			}
			else
			{
				*(hex+i/2) &= 0xf0;
				*(hex+i/2) |= (*(s+i)-'A'+10);
			}
		}
		else if(*(s+i)>='a' && *(s+i)<='f')
		{
			if(i%2==0)	
			{
				*(hex+i/2) &= 0x0f;
				*(hex+i/2) |= ((*(s+i)-'a'+10)<<4);
			}
			else
			{
				*(hex+i/2) &= 0xf0;
				*(hex+i/2) |= (*(s+i)-'a'+10);
			}
		}
		else break;				// 不符合HEX标准字串，退出
	}	
	return((i+1)/2);
}

//16进制数转换为用户编码
void Hex2_UserCode(uchar *uc,uchar *hex,uchar len)
{
	uchar i,ch[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','#','*'};
	for(i=0;i<len;i++)
	{
		if(i<LEN_REG_UC-1)
		{
			*(uc+i*2)   = ch[(*(hex+i)/16)];
			*(uc+i*2+1) = ch[(*(hex+i)%16)];
		}
		else
		{ 
			Hex2_String(uc+i*2,hex+i,1);
		}
	}	
}
//用户编码转换为16进制数
uchar UserCode_Hex2(uchar *hex,uchar *uc,uchar len)
{
	uchar i;
	for(i=0; i<len; i++)
	{
		if(i>=(len-2))
		{
			String_Hex2(hex+i/2,uc+i,2);
			i+=2;
		}
		else if(*(uc+i)>='0' && *(uc+i)<='9')	
		{
			if(i%2==0)	*(hex+i/2) = (*(uc+i)-'0')<<4;
			else		*(hex+i/2) |= (*(uc+i)-'0');
		}
		else if(*(uc+i)=='*' && i<(len-2))
		{
			if(i%2==0)	*(hex+i/2) = 0xF0;
			else		*(hex+i/2) |= 0x0F;	
		}
		else if(*(uc+i)=='#' && i<(len-2))
		{
			if(i%2==0)	*(hex+i/2) = 0xE0;
			else		*(hex+i/2) |= 0x0E;	
		}
		else	return 0;
	}
	return 1;
}


//zjc根据ID查询设备类型
uchar DeviceId_Type(uchar *id)
{
	uint16 type = 0x00;

	type = ((uint16)id[1]>>4&0xf)*100+((uint16)id[1]&0xf)*10+((uint16)id[2]>>4&0xf);

	if(type < 0xff)	return ((uchar)type);
	else			return 0xff;
}



// 计算字符串数组的有效串个数(使用查找最后一个字串的方法)
uchar String_Array_Len(uchar const * const str_array[],uchar const *str_last)
{
	uchar i,len = strlen((char *)str_last);
	for(i=0; i<0xff; i++)
	{
		if(memcmp(str_array[i],str_last,len)==0)
			break;
	}
	return i;
}
//-------disorder the data in buf
void MemoryCpyInvert(UINT8 *to,UINT8 *from,UINT8 num)
{
	UINT8 i,str[32];

	if(!num || num > 32)	return;
	(void)memcpy(str,from,num);
	for(i=0; i<num; i++)
	{
		*(to+i) = str[num-1-i];
	}
}

// 回路存储地址转换为通信地址
uchar addr_comm(uchar addr_save)
{
	return addr_save;
}

//=================================================================
//函    数：uchar Check_Add_WildCard(uchar* add,uchar len)
//功    能：检测地址域是否具有统配
//参    数： 地址指针 以及长度
//返 回 值:  0:无 1:有
//-----------------------------------------------------------------
//更新日期：  2013.1       
//*===============================================================*/

uchar Check_Add_WildCard(uchar len,uchar* add)
{
	uchar i;
	uchar wildcard;
	
	wildcard = FALSE;
	for(i = 0; i < len; i++)
	{
		if ((*add&0xf0)==0xF0)					
		{
			wildcard = TRUE;
			break;
		}

		if((*add&0x0F)==0x0F)					
		{
			wildcard = TRUE;
			break;
		}
		add++;
	}
	return(wildcard);
}



UINT8 addr_LA_compare(UINT8 *addr_local,UINT8 *addr_comm)
{
	UINT8 p_buff,check_sta=COMPARE_SUCESS_NO_WILDCARD;
	for(p_buff=0; p_buff < 1; p_buff++)
	{
		if( addr_local[p_buff] !=addr_comm[p_buff])
		{
			if(addr_comm[p_buff]==0xFF)
			{
				check_sta=COMPARE_SUCESS_WILDCARD;
			}
			else
			{
				check_sta=COMPARE_FAILURE;
			}
		}	
	}
  return(check_sta);
}
/*============================================
		X??XX2?XXX·???XXààDí

===============================================*/

UINT8 addr_UC_compare(UINT8 *addr_local,UINT8 *addr_comm)
{
	UINT8 p_buff,check_sta=COMPARE_SUCESS_NO_WILDCARD;
//	UINT8 check_result1,check_result2;
	for(p_buff=0; p_buff < LEN_REG_UC; p_buff++) 
	{
		if(p_buff==0)
		{
			 if(addr_comm[p_buff]!=addr_local[p_buff])
			 {
				if(addr_comm[p_buff]!=0xFF)
				{
					check_sta=COMPARE_FAILURE;
					break;
				}
				else
				{
					check_sta=COMPARE_SUCESS_WILDCARD;
				}
			 }
		}
		else
		{
			 if ((addr_comm[p_buff]^addr_local[p_buff])&0xf0)						
			 {
				 if ((addr_comm[p_buff]&0xf0) != 0xf0) //*			 		 
				 {
						 check_sta=COMPARE_FAILURE;
						 break;
				 }
				 else
				 {
					check_sta=COMPARE_SUCESS_WILDCARD;
				 }
			 }
			 if ((addr_comm[p_buff]^addr_local[p_buff])&0x0f)						
			 {
				 if ((addr_comm[p_buff]&0x0f) != 0x0f) //*			 		 
				 {
						 check_sta=COMPARE_FAILURE;
						 break;
				 }
				 else
				 {
					check_sta=COMPARE_SUCESS_WILDCARD;
				 }
			 }
		}
	}
	return(check_sta);
}
UINT8 addr_ID_compare(UINT8 *addr_local,UINT8 *addr_comm)// all UINT32 138us,UINT16 UINT32 81.2us,now 30us
{
	UINT8  p_buff,check_sta=COMPARE_SUCESS_NO_WILDCARD;
	UINT8  check_result1,check_result2,check_result3;
	for(p_buff=0; p_buff < LEN_REG_ID; p_buff++) 
	{

 		if(p_buff==3)
		{
			check_result1=COMPARE_SUCESS_NO_WILDCARD;
			if(addr_comm[p_buff]!=addr_local[p_buff])
			{
				if(addr_comm[p_buff]!=0xFF)
				{
					check_sta=COMPARE_FAILURE;
					break;
				}
				else
				{
					check_result1=COMPARE_SUCESS_WILDCARD;
				}
			}
		}
		else if(p_buff==4)
		{	
			check_result2=COMPARE_SUCESS_NO_WILDCARD;
			if (((addr_comm[p_buff])^(addr_local[p_buff]))&0x0F)
			{
				if((addr_comm[p_buff]&0x0F)!=0x0F)
				{
					check_sta=COMPARE_FAILURE;
					break;
				}
				else
				{
					check_result2=COMPARE_SUCESS_WILDCARD;
				}
			}
			if(check_result1==check_result2)
			{
				if(check_sta==COMPARE_SUCESS_NO_WILDCARD)
				{
					check_sta=check_result1;
				}
			}
			else
			{
				check_sta=COMPARE_FAILURE;
				break;	
			}
			check_result1=COMPARE_SUCESS_NO_WILDCARD;
			if (((addr_comm[p_buff])^(addr_local[p_buff]))&0xF0)
			{
				if((addr_comm[p_buff]&0xF0)!=0xF0)
				{
					check_sta=COMPARE_FAILURE;
					break;
				}
				else
				{
					check_result1=COMPARE_SUCESS_WILDCARD;
				}
			}
		}
		else if(p_buff==5)
		{
			check_result2=COMPARE_SUCESS_NO_WILDCARD;
			if (((addr_comm[p_buff])^(addr_local[p_buff]))&0x0F)
			{
				if((addr_comm[p_buff]&0x0F)!=0x0F)
				{
					check_sta=COMPARE_FAILURE;
					break;
				}
				else
				{
					check_result2=COMPARE_SUCESS_WILDCARD;
				}
			}
			if(check_result1==check_result2)
			{
				if(check_sta==COMPARE_SUCESS_NO_WILDCARD)
				{
					check_sta=check_result1;
				}
			}
			else
			{
				check_sta=COMPARE_FAILURE;
				break;	
			}
			check_result1=COMPARE_SUCESS_NO_WILDCARD;
			if (((addr_comm[p_buff])^(addr_local[p_buff]))&0xF0)
			{
				if((addr_comm[p_buff]&0xF0)!=0xF0)
				{
					check_sta=COMPARE_FAILURE;
					break;
				}
				else
				{
					check_result1=COMPARE_SUCESS_WILDCARD;
				}
			}
			
		}
		else if(p_buff==6)
		{
			check_result2=COMPARE_SUCESS_NO_WILDCARD;
			if(addr_comm[p_buff]!=addr_local[p_buff])
			{
				if(addr_comm[p_buff]!=0xFF)
				{
					check_sta=COMPARE_FAILURE;
					break;
				}
				else
				{
					check_result2=COMPARE_SUCESS_WILDCARD;
				}
			}
		}
		else if(p_buff==7)
		{
			check_result3=COMPARE_SUCESS_NO_WILDCARD;
			if (((addr_comm[p_buff])^(addr_local[p_buff]))&0x0F)
			{
				if((addr_comm[p_buff]&0x0F)!=0x0F)
				{
					check_sta=COMPARE_FAILURE;
					break;
				}
				else
				{
					check_result3=COMPARE_SUCESS_WILDCARD;
				}
			}
			if((check_result1==check_result2)&&(check_result2==check_result3))
			{
				if(check_sta==COMPARE_SUCESS_NO_WILDCARD)
				{
					check_sta=check_result1;
				}
			}
			else
			{
				check_sta=COMPARE_FAILURE;
				break;	
			}
			check_result1=COMPARE_SUCESS_NO_WILDCARD;
			if (((addr_comm[p_buff])^(addr_local[p_buff]))&0xF0)
			{
				if((addr_comm[p_buff]&0xF0)!=0xF0)
				{
					check_sta=COMPARE_FAILURE;
					break;
				}
				else
				{
					check_result1=COMPARE_SUCESS_WILDCARD;
				}
			}
		}
		else if(p_buff==8)
		{
			check_result2=COMPARE_SUCESS_NO_WILDCARD;
			if(addr_comm[p_buff]!=addr_local[p_buff])
			{
				if(addr_comm[p_buff]!=0xFF)
				{
					check_sta=COMPARE_FAILURE;
					break;
				}
				else
				{
					check_result2=COMPARE_SUCESS_WILDCARD;
				}
			}
			if(check_result1==check_result2)
			{
				if(check_sta==COMPARE_SUCESS_NO_WILDCARD)
				{
					check_sta=check_result1;
				}
			}
			else
			{
				check_sta=COMPARE_FAILURE;
				break;	
			}
		}
		else 
		{
			if((addr_comm[p_buff]!=addr_local[p_buff])
				||(addr_comm[p_buff+1]!=addr_local[p_buff+1])
				||(addr_comm[p_buff+2]!=addr_local[p_buff+2]))
			{
				if((addr_comm[p_buff]!=0xFF)
					||(addr_comm[p_buff+1]!=0xFF)
					||(addr_comm[p_buff+2]!=0xFF))
				{
					check_sta=COMPARE_FAILURE;
					break;
				}
				else
				{
					check_sta=COMPARE_SUCESS_WILDCARD;
				}
			}
			p_buff+=2;
		}
	}
	return(check_sta);
}
//======================================================================
 //   addr_mmadr_flag:ctr_mmadr

//======================================================================

UINT8 addr_compare(UINT8 addr_mmadr_flag ,UINT8 len,UINT8 *addr_local,UINT8 *addr_comm)
{
	UINT8 check_sta;
	if(len==LEN_REG_LA)
	{
		check_sta=addr_LA_compare(addr_local,addr_comm);
	}
	else if(len==LEN_REG_UC)
	{
		check_sta = addr_UC_compare(addr_local,addr_comm);
		if(check_sta==COMPARE_FAILURE)
        {
            if(addr_mmadr_flag)
            {
            	check_sta=COMPARE_FAILURE_LOCAL_UC;
            }
        }
	}
	else if(len==LEN_REG_ID)
	{
		check_sta = addr_ID_compare(addr_local,addr_comm);
	}
	else if(len==0)
	{
	 	check_sta = COMPARE_SUCESS_WILDCARD;	
	}
	else
	{
		check_sta=COMPARE_FAILURE;
	}
	return(check_sta);
}



/*void Random_Data_Cal(uchar *str)
{
	uchar i;
	for(i = 0; i < 8; i++)
	{
		*(str+i) = Bus.Send.Buff[i];
	} 
	Auth.Random[0] = *(str) << 0x02;                             //A
	Auth.Random[1] = (*(str)|*(str+1)) + 0x10;                  	//B
	Auth.Random[2] = *(str+1) + *(str+5);                     	//C
	Auth.Random[3] = (*(str+2) + *(str+3)) + 0x0A;             	//D
	Auth.Random[4] = (*(str+3) & *(str+4))+ Auth.Random[1];			//E
	Auth.Random[5] = *(str+4) ^ *(str+5);                       	//F
	Auth.Random[6] = *(str+6) ^ *(str+5);                       	//G
	Auth.Random[7] = *(str+7) ^ *(str+2);                       //H
}*/





