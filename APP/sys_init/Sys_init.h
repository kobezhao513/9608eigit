#ifndef __SYS_INIT_H__
#define __SYS_INIT_H__

#include "global.h"



#define	ADDR_MCU_PROGROM_START			0x8004000									//MCU程序起始地址
#define	ADDR_MCU_PROGROM_END			0x8020000//(0x8080000-MCU_DATA_FLASH_LEN)				//MCU程序结束地址

//软件版本号18.02.07
#define SOFT_V_BYTE0	0x19
#define SOFT_V_BYTE1	0x06
#define SOFT_V_BYTE2	0x018


//硬件版本号V1.00.00
#define HARD_V_BYTE0	0x01
#define HARD_V_BYTE1	0x00
#define HARD_V_BYTE2	0x01


/*=====================================================*/
void Power_485_Init(void);

void McuCoreConfig(void);
void Init_Config(void);
void Watch_Dog(void);
void Mcu_Hard_Init(void);
void Mcu_Data_Init(void);
void Recover_Factory_Set(void);
void System_Initialize(void);

void Soft_Reset(void);

#endif



