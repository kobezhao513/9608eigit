
#include "record_manage.h"
#include "record_status.h"
#include "ex_flash.h"
#include "xycnbus.h"
#include "rtc.h"
#include "basic.h"
#include "type_define.h"
#include "typeTab.h"
#include "sys_init.h"
//RECORD_STRUCT Record;
RECORD_INF RecordInf;


extern const S_EVENT_TYPE_INF   C_EVENT_ANNOTATION_LIST[];
//extern void (* this_func)(INT8U key);			//菜单回调函数指针定义

//====================================================================================

/*********************************************************************************
* 函数名称 : Record_Get_Event
* 函数描述 : 获取事件类型码为"event_code"的事件描述，放到str中。表中有该类型返回1，无则返回0
* 输入参数 : event_code：事件类型码，
* 输出参数 : str：事件描述
* 返 回 值 : 1：获取成功，0：获取失败
* 修改日期 ：2013-06
*********************************************************************************/
/*UINT8 Record_Get_Event(UINT16 const event_code,UINT8 * str)
{
	UINT16 i,event_counter;
	
	event_counter = sizeof(C_EVENT_ANNOTATION_LIST)/sizeof(S_EVENT_TYPE_INF);
	for(i=0; i < event_counter; i++)
	{
		if(event_code == C_EVENT_ANNOTATION_LIST[i].EventType)
		{
			strcpy((INT8 *)str,(INT8 *)(C_EVENT_ANNOTATION_LIST[i].Event_Annotaiton));
			return strlen((INT8 *)C_EVENT_ANNOTATION_LIST[i].Event_Annotaiton) ;
		}
	}
	return 0;
}*/

/*********************************************************************************
* 函数名称 : Delete_All_Record
* 函数描述 : 删除所有记录
* 输入参数 : 无
* 输出参数 : 无
* 返 回 值     : 无
* 修改日期 ：2016-01
*********************************************************************************/

void Delete_All_Record(void)
{
	uint16 i,sec; 



    //TTS_Data_Init();
	//擦除外部flash的记录存储区域	
	sec = MAXN_FLASH_RECORD*ALEN_FLASH_RECORD/4096;
	for(i=0;i<sec;i++)
	{
        Watch_Dog();
		Flash_Erase_3Addr(ADDR_FLASH_RECORD+i*4096);
	}
	//记录信息数量清0，清消息显示
	RecordInf.StartPc = 0;
	RecordInf.SavePc  = 0;
	//RecordInf.Total = 0;
	RecordInf.ttsRecordPc = RecordInf.SavePc;
	
}



//**************************************************************************************************
//Function Name  : get_record
//Description    : 从FLASH中读取一条记录，
//Arguments      : offset：记录在FLASH中的偏移量，起始地址为RECORD；
//                 record_buffer  ：读取到的记录结构体；
//Return         : 读取成功返回1，失败返回0
//**************************************************************************************************
UINT8 get_record(UINT16 offset,RECORD_STRUCT * record_buffer)
{
	//memcpy(record_buffer,&Record[offset],sizeof(Record[0]));
	if(Flash_Load_Info(RECORD,offset,(uchar *)record_buffer))//record_buffer))
	{
		return 1;
	}
	return 0;
}


/*********************************************************************************
* 函数名称 : Record_Save_Local
* 函数描述 : 本地信息存储，循环存储
* 输入参数 : RecordType：存储的记录类型
* 输入参数 : None
* 返回值   : None
* 修改日期 ：2013-09
*********************************************************************************/
//记录存储+显示消息
void Record_Save(RECORD_STRUCT *record)
{
    UINT32 addr;

    if(record->Type > RECORD_OTHERS)	//类型超界
    {
        return;
    }
	
	record->TranFlag = RECORD_TRAN_INIT;

    //时间添加:放入传输设备的时间
    if(record->EventType.word == INT_TYPE_POWER_OFF)		// record time
    {
        RTC_Get_PowerOff_Time(time_string);
        memcpy(record->Time,time_string,6);
    }
    else
    {
        GET_RTC_TIME();
        memcpy((char *)record->Time,(const char *)time_string,3);
        memcpy((char *)record->Time+3,(const char *)time_string+4,3);
    }

    if(record->Ic.Loop == IC_LOOP_SYS)
    {
		MemoryCpyInvert(record->Id,System.id,LEN_REG_ID);
		MemoryCpyInvert(record->Uc,System.uc,LEN_REG_UC);
		MemoryCpyInvert(record->Cc,System.cc,LEN_REG_CC);
    }

    //存储于FLASH中,确保下一个扇区总是空白；
       //当前存储位置为每扇区第一条记录位置，则查找下一扇区非空则擦除
	   //循环存储，第一遍存储完毕,则更新第一条记录的位置

	if((RecordInf.SavePc % COUNT_RECORD_PER_SECTOR) == 0)//32 //存储到每扇区第一条记录位置时，擦除下一扇区，保证有一个正扇区的空白
    {
       if(RecordInf.SavePc == (MAXN_FLASH_RECORD - COUNT_RECORD_PER_SECTOR))//最后一扇区首条
       {
            addr = ADDR_FLASH_RECORD;
       }
       else            
       {
            addr = ADDR_FLASH_RECORD + (RecordInf.SavePc + COUNT_RECORD_PER_SECTOR)* ALEN_FLASH_RECORD;	
       }
       if(Flash_Blank_Check(addr,SIZE_FLASH_SECTOR) == 0)	//非空白扇区，即非第一遍写，则起始位置为下下个扇区
       {
    	     Flash_Erase_3Addr(addr);
    	     RecordInf.StartPc = (RecordInf.StartPc + COUNT_RECORD_PER_SECTOR) % MAXN_FLASH_RECORD;
       }
    }
	
    Flash_Save_Info(RECORD,RecordInf.SavePc,(uint8*)(record));

	//添加消息	
	//Message_Add(RecordInf.SavePc,&Record);	
	
	/*if(Record.Type == RECORD_ELIMINATE)//控制器解除屏蔽以及故障恢复均不处理，保持至确认
	{
		if(Record.Mode == RMODE_UC)   //设备自身故障消除
		{
			Message_Delete_Fault(RECORD_FAULT,Record.Ic,NULL,MSG_DEL_MODE_IC);
		}
     	else                          //控制器故障消除
     	{
			Message_Delete_Fault(RECORD_FAULT,NULL,Record.Uc,MSG_DEL_MODE_UC);
	 	}
	}*/

    //存储修改为循环存储 2013-09-24
    if(++RecordInf.SavePc > MAXN_FLASH_RECORD - 1)	//边际处理
    {
       RecordInf.SavePc = 0;//SavePc的范围：0-(MAXN_FLASH_RECORD-1)
    }
    //RecordInf.Total++;//end 存储修改	
}
/*********************************************************************************
* 函数名称 : Init_Record
* 函数描述 : 初始化设备记录变量，获取最早信息的位置 ，首先从备份寄存器读取，
			 如果失败遍历查找（由于擦除是整扇区擦除的，存在空白记录，故只能遍历）
			 查找方法：寻找第一个空白记录的位置，查找下扇区该位置是否为空白，如果空白则是
					   第一次记录，反之则是进入循环存储。
* 输入参数 : type 0上电初始化；0x01复位后初始化
* 输出参数 : None
* 返回值   : None
* 修改日期 ：2016-01
*********************************************************************************/
void Init_Record(uchar type)
{
	UINT16 i,blank_sector_sn,serch_sector_sn;	
    UINT8 temp1[SLEN_FLASH_RECORD],temp2[SLEN_FLASH_RECORD]={0};
    UINT32 addr;
	
	if(type)	return;			// 软件复位不重读记录信息
	/*RecordInf.Total = 0;
	for(i=0; i<MAXN_FLASH_RECORD; i++)
	{
        Watch_Dog();

		Flash_Read_3String(ADDR_FLASH_RECORD+i*ALEN_FLASH_RECORD,buffer,1);
		if(buffer[0]==0xaa)
		{
			RecordInf.Total ++;//= (i+1)%MAXN_FLASH_RECORD;
			//break;
		}
	}*/
 	RecordInf.SavePc  = 0;
    RecordInf.StartPc = 0;
    for(i = 1; i < MAX_RECORD_SECTOR; i++)
    {
    	Watch_Dog();
    	if(Flash_Blank_Check(ADDR_FLASH_RECORD + i * SIZE_FLASH_SECTOR, SIZE_FLASH_STA_VALID))	//查到空白扇区
    		break;
    }
    if(i >= MAX_SECTOR_COUNTER)			//出错：无空白扇区（错误所致）,理论上第一扇区是最老数据
    {
    	blank_sector_sn = 0;
    	Flash_Erase_3Addr(ADDR_FLASH_RECORD);
    	RecordInf.SavePc = 0;
    }
    else
    {
    	//获取写起始地址
    	blank_sector_sn = i;
    	serch_sector_sn = blank_sector_sn - 1;
    	memset(temp1,0xff,SLEN_FLASH_RECORD);
    	for(i=0; i < SIZE_FLASH_SECTOR / ALEN_FLASH_RECORD; i++)
    	{
    		Watch_Dog();
    		addr = ADDR_FLASH_RECORD + serch_sector_sn * SIZE_FLASH_SECTOR + i * ALEN_FLASH_RECORD;
    		Flash_Read_3String(addr,temp2,SLEN_FLASH_RECORD);
    		if(memcmp(temp1,temp2,SLEN_FLASH_RECORD) == 0)	//取到空白帧
    		{
    			RecordInf.SavePc  = serch_sector_sn * (SIZE_FLASH_SECTOR / ALEN_FLASH_RECORD) + i;
    			break;
    		}
    	}
    	if(i >= SIZE_FLASH_SECTOR / ALEN_FLASH_RECORD)	
    	{
    		RecordInf.SavePc  = (serch_sector_sn + 1)* (SIZE_FLASH_SECTOR / ALEN_FLASH_RECORD);	//擦扇区在写函数中
    	}
    }
    //获取记录起始地址，用于记录浏览：只取第一帧数据，如果空则是第一遍写
    serch_sector_sn = (blank_sector_sn == (MAX_SECTOR_COUNTER - 1)) ? 0:(blank_sector_sn + 1);
    memset(temp1,0xff,SLEN_FLASH_RECORD);

    addr = ADDR_FLASH_RECORD + serch_sector_sn * SIZE_FLASH_SECTOR;
    Flash_Read_3String(addr,temp2,SLEN_FLASH_RECORD);
    if(memcmp(temp1,temp2,SLEN_FLASH_RECORD) == 0)	//空白记录，第一遍存储
    {
    	RecordInf.StartPc  = 0;
    }
    else											//非空白记录，第n遍存储
    {
    	RecordInf.StartPc  = serch_sector_sn * (SIZE_FLASH_SECTOR / ALEN_FLASH_RECORD);	//擦扇区在写函数中
    }
    //相关数据写入备份寄存器
   
    RecordInf.ttsRecordPc = RecordInf.SavePc;
}

//=============================================================================================
//函	数： void Record_Power_On_Off(void)
//功	能： 故障:火灾控制器上下电
//参	数： 无
//返 回 值:  无
//-----------------------------------------------------------------------------------------------
//更新日期：  2013.1
//*==============================================================================================*/
void Record_Power_On_Off(UINT8 type)
{
	RECORD_STRUCT record;
	memset(&record.Sta, 0xFF, sizeof(record));

    if(type)    return ;

		//记录关机 
	record.Type = RECORD_POWERONOFF;
	record.EventType.word = INT_TYPE_POWER_OFF;//设备关机
	Record_Sys_If_Event(&record);
	
	//记录开机
	memset(&record.Sta, 0xFF, sizeof(record));

	record.Type = RECORD_POWERONOFF;
	record.EventType.word = INT_TYPE_POWER_ON;//设备开机
	Record_Sys_If_Event(&record);
		
}

void Record_Updata_Suc(void)
{
    RECORD_STRUCT record;
	memset(&record.Sta, 0xFF, sizeof(record));
	
    record.Type = RECORD_OTHERS;
    record.EventType.word = INT_TYPE_UPDATA_SUCC;
	
	Record_Sys_If_Event(&record);
}



UINT16 Get_Event_Type_Number(void)
{
	return(sizeof(C_EVENT_ANNOTATION_LIST)/sizeof(S_EVENT_TYPE_INF));
}

UINT8 Get_EventTypeGroup(UINT16 event_type)
{
	UINT8 group = RECORD_MISC;
	UINT16 k,eventNum;

	eventNum = Get_Event_Type_Number();
	if(event_type == INT_TYPE_NULL)
	{
		return group;
	}
	for(k=0; k < eventNum; k++)
	{
		if(C_EVENT_ANNOTATION_LIST[k].EventType == event_type)
		{
			group = C_EVENT_ANNOTATION_LIST[k].generic_type;
			break;
		}
	}
	return group;
}

/*********************************************************************************
* 函数名称 : Record_Type_Is_Sel
* 函数描述 : 判断记录类型是否选中，用于打印机、TTS、联网数据输入/出；
* 输入参数 : type：记录类型，参照record_manage.h
* 输出参数 : None
* 返 回 值 : 是否存在于设置项：0：不播放，1：播放
* 修改日期 ：2016.5.20
*********************************************************************************/
UINT8 Record_Type_Is_Sel(UINT8 type,UINT8 sel_setting)
{
	if(type > RECORD_MAX)	return 0;
	if(type <=  RECORD_EARLYFIRE)
		return 1;
	else
		return 0;
}

//=============================================================================
//函数描述: TTS记录信息格式化
//输入参数: UINT8 * str:格式化后信息存放指针
//输出参数: 设备类型和事件类型描述
//修改日期: 2016.5.20
//修改人员: 
//=============================================================================
/*UINT8 Record_TTS_Format(UINT8 * str)
{
	UINT8 dev_type,len;	
	UINT8 temp;
	len = 0;

    if(memcmp(C_DEV_TYPE_LIST[Record.Uc[0]].BriefDescribe ,"无效类型",8))
		dev_type = Record.Uc[0];
	else
		dev_type = DeviceId_Type(Record.Id); 	

	//len = strlen((INT8 *)C_DEV_TYPE_LIST[dev_type].DetailDescribe);//strlen((INT8 *)DEVICE_TYPE_TAB[dev_type]);
	//strcpy(str,(INT8 *)C_DEV_TYPE_LIST[dev_type].DetailDescribe);

	len = strlen((char *)C_DEV_TYPE_LIST[dev_type].DetailDescribe);//strlen((INT8 *)DEVICE_TYPE_TAB[dev_type]);
	strcpy((char *)str,(const char *)C_DEV_TYPE_LIST[dev_type].DetailDescribe);

	temp = Record_Get_Event(Record.EventType.word,(UINT8 *)(str + len));
	if(temp == 0)
	{
		strcpy((INT8 *)(str + len),"未定义事件");
		len += 10;
	}
	else
	{
		len += temp;
	}

	return len;
}*/



