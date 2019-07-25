#ifndef	SYNCH_H
#define	SYNCH_H

#include "global.h"



#define		SYNCH_CUE_Y			60


#define UDISK_EXPORT_DATA		0x04    //导出系统配置
#define UDISK_IMPORT_MT			0x00    //导入主控板程序
#define UDISK_IMPORT_PW			0x01    //导入电源板程序
#define UDISK_IMPORT_DATA		0x02    //导入系统配置
#define UDISK_IMPORT_FONTLAB    0x03    //导入字库文件
#define	MAX_FILE_PERPAGE		3
#define	START_Y_FILE_BROWSE		0
#define	DIST_Y_FILE_BROWSE		2
#define BUFFER_WR_FLASH_SIZE        (512)//必须是2的n次幂




UINT8 Udisk_Import_GetType(void);
void Udisk_SetType(UINT8 type);
void Udisk_Import_Info_Cue(void);
void Udisk_Import_Info(UINT8 type);

void Udisk_Import_Info_This(UINT8 key);
void Udisk_Export_Info(UINT8 type);

void Udisk_Import_Info_This(UINT8 key);

void Udisk_Export(void);
void Udisk_Import_Info(UINT8 type);
void config_sync_this(UINT8 key);
void Config_Sync(UINT8 type);

#endif
