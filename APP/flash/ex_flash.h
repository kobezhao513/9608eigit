/*
 * spi_flash.h
 *
 *  Created on: 2014-11-15
 *      Author: Administrator
 */

#ifndef SPI_FLASH_H_
#define SPI_FLASH_H_


//------------------------------------- include -------------------------------------
#include "global.h"
//------------------------------------- define --------------------------------------

#define		Flash_CS()					GPIOB->BRR = GPIO_Pin_1
#define		Flash_UnCS()				GPIOB->BSRR = GPIO_Pin_1
#define		Flash_Lock()				GPIOB->BRR = GPIO_Pin_0
#define		Flash_UnLock()				GPIOB->BSRR = GPIO_Pin_0


//=======================MX25L4006寄存器地址宏定义=========================================
#define   READ        					0x03
#define   FAST_READ   					0x0b
#define   WRITE       					0x02  
#define   WREN        					0x06
#define   WRDI        					0x04
#define   RDSR        					0x05
#define   WRSR        					0x01 
#define   PE          					0x42 
#define   SE          					0x20	//0xD8 
#define   BE          					0xD8 
#define   CE          					0xC7 
#define   RDID        					0x9f 
#define   DPD         					0xB9
//Mx25L12855 ID参数
#define   MANU_ID						0xc2
#define   DEV_ID						0x20    //0x26
#define   DENSITY_ID					0x18

//--------------------------------------------------------------------------------------
#define FLASH_STA_VALID      			0xAA 
#define FLASH_STA_BLANK      			0xFF
#define FLASH_STA_UNVALID      			0x00 

#define FLASH_DATA_UNVALID      		0xFF

#define FLASH_DATA_WRITTEN_FORBID       0
#define FLASH_DATA_WRITTEN_ALLOW        1   

#define FIRST_CHIP_FLASH    0
#define SECOND_CHIP_FLASH   1

#define DATA_SIZE                       64

#define FLASH_CHIP_ERR_MASK             0X03  
#define FLASH_CHIP0_ERR_MASK            0X01   
#define FLASH_CHIP1_ERR_MASK            0X02  

//--------------------------------------------------------------------------------------
// FLASH 存储宏定义
#define	SIZE_FLASH_SECTOR				0x01000		//4096
#define	MAX_SECTOR_COUNTER				0x01000		//4096
#define SIZE_FLASH_STA_VALID                1
// 语音库文件--索引地址和语音库地址定义
#define	LEN_ASCII_LIBRARY      	  0x00001000	// 4K  				

#define	LEN_HZ_LIBRARY       	  0x0003D000    // 244K  

#define ADDR_UNICODE_START        0x00040000    //UNICODE-GBK,GBK-UNICODE
#define	LEN_UNICODE_TABLE             0x00015484   //0x00015490 // 87184
#define ADDR_GBK2UNICODE_START    0x00055490 



#define  ADDR_VIOCE_ADDR     	 0x000DC800    //大小20k为索引
#define  ADDR_VIOCE_START        (ADDR_VIOCE_ADDR+0x5000)//语音库起始地址
//#define  ADDR_VIOCE_ALARM_START			(0x004CB000 + ADDR_VIOCE_ADDR)

#define	LEN_VIOCE_STORE_SPACE     0x004CB000    //语音库总长_ADDR+索引(0x5000+4c6000)

#define	ADDR_VIOCE_ALARM_START	  (ADDR_VIOCE_ADDR+LEN_VIOCE_STORE_SPACE)
#define	ADDR_VIOCE_ALARM		  ADDR_VIOCE_ALARM_START
#define	LEN_VIOCE_ALARM		      0x00010000    //64k 

#define	ADDR_VIOCE_SKYALARM		  (ADDR_VIOCE_ALARM_START+LEN_VIOCE_ALARM)	
#define	LEN_VIOCE_SKYALARM		  0x00008000	// 32K  

#define	ADDR_VIOCE_HELP			  (ADDR_VIOCE_SKYALARM+LEN_VIOCE_SKYALARM)
#define	LEN_VIOCE_HELP			  0x0000C800    // 50K  

#define	ADDR_VIOCE_GUN			  (ADDR_VIOCE_HELP+LEN_VIOCE_HELP)
#define	LEN_VIOCE_GUN			  0x00002000    // 8K  

#define	ADDR_VIOCE_DIDI			  (ADDR_VIOCE_GUN+LEN_VIOCE_GUN)
#define	LEN_VIOCE_DIDI			  0x00000800    // 2K  

#define	ADDR_VIOCE_DUDU 		  (ADDR_VIOCE_DIDI+LEN_VIOCE_DIDI)
#define	LEN_VIOCE_DUDU 		      0x00001400    // 5K  

#define	ADDR_VIOCE_KEY 			  (ADDR_VIOCE_DUDU+LEN_VIOCE_DUDU)
#define	LEN_VIOCE_KEY 			  0x00001400    // 5K  


// 字库文件--ASC码和汉字起始地址定义 
#define     ADDR_ASC_START      0x005D1000
#define     ADDR_HZ_START       0x005D3000

//字库+语音库+图库

#define FONT_FLASH_PROGRAM              0X7FF000

//--------------------------------------------------------------------------------------

#define	COPY_SIZE_FLASH					0x200000    //2M

//	系统配置4k	
#define	SYSCONFIG						0
#define	ADDR_FLASH_SYSCONFIG			0x800000
#define	SLEN_FLASH_SYSCONFIG			sizeof(CONFIG)//140
#define	ALEN_FLASH_SYSCONFIG			512
#define	MAXN_FLASH_SYSCONFIG			1	
#define TIMES_FLASH_SYSCONFIG			(4096/512)       //每个信息项可写次数

//	运行记录2032kbyte	
#define	RECORD							(SYSCONFIG + 1)
#define	ADDR_FLASH_RECORD				(ADDR_FLASH_SYSCONFIG+4096*2)//0x802000
#define	SLEN_FLASH_RECORD				sizeof(RECORD_STRUCT)//126//根据结构体RECORD_STRUCT来定
#define	ALEN_FLASH_RECORD				128
#define	MAXN_FLASH_RECORD				16256//5120
#define TIMES_FLASH_RECORD			    1

//sector=4k可写32条记录
#define COUNT_RECORD_PER_SECTOR			(SIZE_FLASH_SECTOR / ALEN_FLASH_RECORD)			//每扇区记录数量
//占用2032k/4=508sector
#define MAX_RECORD_SECTOR				(MAXN_FLASH_RECORD / COUNT_RECORD_PER_SECTOR)	//存储记录的扇区数量

//	Flash存取记录结束地址
#define	CHECK							(RECORD+1)
#define	ADDR_FLASH_CHECK				(ADDR_FLASH_SYSCONFIG+0x1000)//4K

//#define	CHECK							(RECORD+1)
//#define	ADDR_FLASH_CHECK				(ADDR_FLASH_RECORD+0x1FC000)//0x1FC000=16256*128


//程序更新
#define PROGRAM							(CHECK+1)
#define	ADDR_FLASH_PROGRAM				0xF00000
#define	SLEN_FLASH_PROGRAM				(1024*1024)
#define	ALEN_FLASH_PROGRAM				(1024*1024) //1024K
#define	MAXN_FLASH_PROGRAM				2	
#define TIMES_FLASH_PROGRAM				1


//===========外部FLASH数据存储区临时备份区=================
#define SIZE_SECTOR                   0x1000

#define	ADDR_BOOTLOADER_START			 0xF00000//0xF80000		//距离结尾0x1000000还有1Mbytes  //程序升级区

//升级应用程序 时使用
//VALID_FLAG、LEN0、LEN1、LEN2、LEN3、CRC0、CRC1、HARD0、HARD1、HARD2、SOFT0、SOTF1、SOTF2
#define	ADDR_BOOTLOADER_LEN                 (0x1000000-SIZE_SECTOR)
#define	ADDR_BOOTLOADER_CRC				    (ADDR_BOOTLOADER_LEN+4)				//待升级程序CRC
#define	ADDR_BOOTLOADER_FLAG                (ADDR_BOOTLOADER_CRC+2)             //地址：主控板应用程序升级标志

#define	ADDR_BOOTLOADER_HARD			    (ADDR_BOOTLOADER_FLAG+4)			    //程序升级硬件版本	
#define	ADDR_BOOTLOADER_SOFT			    (ADDR_BOOTLOADER_HARD+3)			    //程序升级硬件版本	
#define	ADDR_BOOTLOADER_UC                  (ADDR_BOOTLOADER_SOFT+3)

//升级电源板程序时使用		    //程序升级硬件版本								//主控板应用程序升级标志
#define POW_PROGROM_VALID_FLAG			0x55										//电源应用程序升级标志
#define ADDR_POW_BOOT_FLAG            (ADDR_BOOTLOADER_FLAG)
#define POWER_LA                       1
//升级非本机类文件时使用		    //
#define ADDR_UPDATA_FILE_NAME_LEN             (ADDR_BOOTLOADER_SOFT+3)
#define ADDR_UPDATA_FILE_NAME                 (ADDR_UPDATA_FILE_NAME_LEN+1)

//====================================================================================== end
typedef struct
{
    UINT8 Cs ;                                                    //FLASH 片选(0:主FLASH 1:从FLASH) 
    UINT8 Nums;                                                   //有效FLASH 芯片数量
    UINT8 Err;
}FLASH_STA;

extern FLASH_STA Flash;

extern UINT8 	buffer[SIZE_SECTOR];


typedef enum    
{
	ICO_TYPE_SYS_QUERY  = 0,  //系统查询
	ICO_TYPE_SYS_SET,	      //系统设置
	ICO_TYPE_SYS_DEBUG,	      //系统调试
	ICO_TYPE_USER_SET,	      //用户登录
	
	ICO_TYPE_UNLOCK,	      //解锁
	ICO_TYPE_LOCK,		      //锁屏
	ICO_TYPE_GPRS_UNCONNECT,  //GPRS未连接
	ICO_TYPE_GPRS_CONNECT1,
	ICO_TYPE_GPRS_CONNECT2,
	ICO_TYPE_GPRS_CONNECT3,
	ICO_TYPE_GPRS_CONNECT4,	  //GPRS连接
    ICO_TYPE_GPRS_MESSAGE,     //GPRS短信
	ICO_TYPE_TCP_UNCONNECT,	  //TCP未连接
	ICO_TYPE_TCP_CONNECT,     //TCP连接
	ICO_TYPE_LOGO,	          //公司logo
	ICO_TYPE_TCP_gongzuozhengchang,	  //系统工作正常

	ICO_TYPE_MAX

}E_ICO_TYPE;

//------------------------------------- function ------------------------------------
void Flash_Init(void);
void Flash_Sector_Erase(UINT32 addr);         //Erase a sector (4K byte)
void Flash_Save_Info(UINT8 type,UINT16 offset,UINT8 *p);
UINT8 Flash_Load_Info(UINT8 type,UINT16 offset,UINT8 *p);
UINT8 Flash_SelfCheck(void);
void Flash_Read_Id(UINT8 *id);
void Flash_Erase_3Addr(UINT32 data_address);
void Flash_Read_3String(UINT32 addr, UINT8 * rd_buf, UINT16 len);
UINT8 Flash_Blank_Check(UINT32 addr,UINT16 len);
void Flash_Read_String(UINT32 data_address,UINT8 *data,UINT16 data_length);
void Flash_Write_String(UINT32 data_address,UINT8 *data,UINT16 data_length);
#endif /* SPI_FLASH_H_ */


