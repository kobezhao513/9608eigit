#include <string.h>

#include "synch.h"
#include "sys_init.h"

#include "sys_init.h"
#include "xycnbus.H" 
#include "ex_flash.h"

#include "bus_basic_cmd.h"
#include "record_status.h"
#include "delay.h"




//===============================U盘导入/导出===============================================

UINT8 Type_udisk     = 0xFF;


#define UDISK_EXPORT_START_Y		0
#define UDISK_EXPORT_DIST		    2
#define MAX_ITEM_UDISK_EXPORT		4


extern UINT8			USB_Enum_Done;
extern void (* this_func)(UINT8 key);



/*********************************************************************************
* 函数名称 : Udisk_SetType
* 函数描述 : 设置操作类型，用于复位操作类型；
* 输入参数 : 菜单入口序号;
* 输出参数 : None;
* 返 回 值 : None;
* 修改日期 ：2015-02
*********************************************************************************/
void Udisk_SetType(UINT8 type)
{
	Type_udisk = type;
}
/*********************************************************************************
* 函数名称 : Udisk_Import_GetType
* 函数描述 : 获取当前操作的类型,即对应的菜单项，查表后获取Flash.h中的类型；
* 输入参数 : None；
* 输出参数 : None;
* 返 回 值 : 菜单入口序号
* 修改日期 ：2015-02
*********************************************************************************/
UINT8 Udisk_Import_GetType(void)
{
	return Type_udisk;
}


void Usb_Keybord_Creat(void)
{
    /*UINT8 const * const udisk_cue[] = {"USB键盘启动","Init UDisk,Please wait."};

	LCD_Show_String_Mediacy(48,udisk_cue[System.language]);
	Usb_Mode_Set(USB_KEYBOARD);
	Menu_NoKeep();
*/
}








