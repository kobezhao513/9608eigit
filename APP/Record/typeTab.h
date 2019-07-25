#ifndef TYPE_TAB_H
#define	TYPE_TAB_H
#include "global.h"
#include "record_manage.h"

#include "xycnbus.h"
#include "type_define.h"
#include "dev_ico.h"



#define  OVER 0xFF


//-----------------------------------各设备对应事件类型列表---------------------//

//控制器
//UINT16 const EVENT_LIST_CONTROL[] = {INVALID};



/*-------------------------------------------事件注释列表-------------------------------------------*/

//struct t9py_index const t9py_index_mb
//S_EVENT_TYPE_INF const  C_EVENT_ANNOTATION_LIST[]=  
struct event_type const  C_EVENT_ANNOTATION_LIST[]=
{

	   //-----------------------------------------人工控制实时事件--------------------------------//									   
	   {INT_TYPE_KEY_ACTION,					   RECORD_OTHERS,      TYPE_CRT_RESERVE,   "按键动作"},
	   {INT_TYPE_MANUAL_ALARM,					   RECORD_FIRE,		   TYPE_CRT_ALARM,	   "手动报警"},
	   {INT_TYPE_SET_TIME,						   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "设置时间"},
	   {INT_TYPE_SET_PRINT_FIRE,				   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "打印火警"},
	   {INT_TYPE_SET_PRINT_FIRE_AND_FEEDBACK,	   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "打印火警和反馈"},
	   {INT_TYPE_SET_PINTT_ALL_EVENT,			   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "打印所有信息"},
	   {INT_TYPE_SET_PRINT_DISEN,				   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "禁止打印"},
	   {INT_TYPE_SET_PRINTER,					   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "打印浓度设置"},
	   {INT_TYPE_SET_LCD_BACKGROUND_LIGHT,		   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "液晶背光设置"},
	   {INT_TYPE_SET_PREALARM_ENABLE,			   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "预警功能允许"},
	   {INT_TYPE_SET_PREALARM_DISEN,			   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "预警功能禁止"},
	   {INT_TYPE_SET_PREALARM_RELATION, 		   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "预警关联设置"},
	   {INT_TYPE_SET_STARTUP_DIRECT_VIA_FIRE,	   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "火警直接启动"},
	   {INT_TYPE_SET_STARTUP_VIA_LINKAGE,		   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "联动公式启动"},
	   {INT_TYPE_SET_NETWORK_ENABLE,			   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "网络通信开启"},
	   {INT_TYPE_SET_NETWORK_DISEN, 			   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "网络通信关闭"},
	   {INT_TYPE_SET_ADD_LINKAGE,				   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "新建联动公式"},
	   {INT_TYPE_SET_EDIT_LINKAGE,				   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "修改联动公式"},
	   {INT_TYPE_SET_DELETE_LINKAGE,			   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "删除联动公式"},
	   {INT_TYPE_REQ_START, 					   RECORD_START, 	   TYPE_CRT_RESERVE,   "启动请求"},
	   {INT_TYPE_REQ_STOP,						   RECORD_STOP, 	   TYPE_CRT_RESERVE,   "停动请求"},
	   {INT_TYPE_REQ_SHIELD,					   RECORD_SHIELD,	   TYPE_CRT_RESERVE,   "屏蔽请求"},
	   {INT_TYPE_REQ_RELEASE,					   RECORD_RELEASE,	   TYPE_CRT_RESERVE,   "解除屏蔽请求"},
	   {INT_TYPE_REQ_RESET, 					   RECORD_LOCALCOMMAND,TYPE_CRT_RESERVE,   "复位请求"},
	   {INT_TYPE_PARA_MODIFY,					   RECORD_LOCALCOMMAND,TYPE_CRT_RESERVE,   "参数修改请求"},
	   {INT_TYPE_REQ_SILENCE,					   RECORD_LOCALCOMMAND,TYPE_CRT_RESERVE,   "设备消音请求"}, 	   
	   {INT_TYPE_HELP_INF,						   RECORD_LOCALCOMMAND,TYPE_CRT_RESERVE,   "帮助请求"},
	   {INT_TYPE_LAN_SET_TIME,					   RECORD_LOCALCOMMAND,TYPE_CRT_RESERVE,   "网络校时"}, 	   
	   {INT_TYPE_LAN_READ_TIME,					   RECORD_LOCALCOMMAND,TYPE_CRT_RESERVE,   "网络读取时间"},
	   //-----------------------------------------命令及应答类事件--------------------------------//
	   {INT_TYPE_IDLOG_DATA,					   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "ID登录命令数据"},
	   {INT_TYPE_LALOG_DATA,					   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "LA登录命令数据"},
	   {INT_TYPE_UCLOG_DATA,					   RECORD_SETUP,	   TYPE_CRT_RESERVE,   "UC登录命令数据"},
	   {INT_TYPE_TRANSPARENT_TRANS, 			   RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "透传信息"},
	   {INT_TYPE_LINK_FORMULA_OUT,				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "联动公式成立联动输出"},
	   {INT_TYPE_LINK_OUT,						   RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "关联的联动公式输出"},
	   {INT_TYPE_UC_FIND_LC,					   RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "二次码查询一次码"},
	   {INT_TYPE_MULTI_ADDRCOMM_RES,			   RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "本地多级地址命令"},
	   {INT_TYPE_INNER_UP_MSG, 			           RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "主控设备上报信息"},
	   {INT_TYPE_WILDCOMM_EXECUTE_RIGHT,		   RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "通配命令正确执行"},
	   //{INT_TYPE_DEVICE_SYNCH,					   RECORD_MISC,		   TYPE_CRT_RESERVE,   "设备信息同步"},
	   {INT_TYPE_EVENT_SYNCH,					   RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "事件信息同步"},
	   {INT_TYPE_MONITOR_PDC_SEND_DATA, 		   RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "监控PDC总线发码数据"},
	   {INT_TYPE_MONITOR_PDC_RECE_DATA, 		   RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "监控PDC总线收码数据"},
	   {INT_TYPE_MONITOR_IMTMPI_SEND_DATA,		   RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "监控IMT/MPI总线发码数据"},
	   {INT_TYPE_MONITOR_IMTMPI_RECE_DATA,		   RECORD_OTHERS,  	   TYPE_CRT_RESERVE,   "监控IMT/MPI总线收码数据"},
	   {INT_TYPE_MONITOR_NODE_ID,				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "监控节点ID"},
	   {INT_TYPE_MONITOR_BROCAST_SEND_DATA, 	   RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "监控广播总线发码"},
	   {INT_TYPE_MONITOR_BROCAST_RECE_DATA, 	   RECORD_OTHERS,	   TYPE_CRT_RESERVE,   "监控广播总线收码"},
	   //----------------------------------------自动报警事件-------------------------------------//
	   {INT_TYPE_FIRE_ALARM,					   RECORD_FIRE,		   TYPE_CRT_ALARM,	    "火焰报警"},
	   {INT_TYPE_TEMPERATURE_ALARM, 			   RECORD_FIRE,		   TYPE_CRT_ALARM,	    "温度报警"},
	   {INT_TYPE_SMOKE_ALARM,					   RECORD_FIRE,		   TYPE_CRT_ALARM,	    "烟雾报警"},
	   {INT_TYPE_ALARM,				        	   RECORD_FIRE, 	   TYPE_CRT_ALARM,		"火警报警"},

	   {INT_TYPE_PRE_FIRE_ALARM,				   RECORD_EARLYFIRE,   TYPE_CRT_EARLY_ALARM,"火警预警"},
	   {INT_TYPE_PRE_TEMPERATURE_ALARM, 		   RECORD_EARLYFIRE,   TYPE_CRT_EARLY_ALARM,"温警预警"},
	   {INT_TYPE_PRE_SMOKE_ALARM,				   RECORD_EARLYFIRE,   TYPE_CRT_EARLY_ALARM,"烟警预警"},
	   {INT_TYPE_PRE_ALARM,			        	   RECORD_EARLYFIRE,   TYPE_CRT_EARLY_ALARM,"火警预警"},
		   	   
	   {INT_TYPE_MONITOR_FIRE_ALARM, 		   	   RECORD_MONITER,	   TYPE_CRT_MONITER,    "火焰报警"},
	   {INT_TYPE_MONITOR_TEMPERATURE_ALARM, 	   RECORD_MONITER,	   TYPE_CRT_MONITER,    "温度报警"},
	   {INT_TYPE_MONITOR_SMOKE_ALARM,			   RECORD_MONITER,	   TYPE_CRT_MONITER,	"烟雾报警"},
	   {INT_TYPE_MONITOR_CURRENT_LEAKAGE_ALARM,    RECORD_MONITER,	   TYPE_CRT_MONITER,	"漏电报警"},
	   {INT_TYPE_MONITOR_GAS_LEAKAGE_ALARM,   	   RECORD_MONITER,	   TYPE_CRT_MONITER,	"燃气泄漏"},
	   {INT_TYPE_MONITOR_THEFT_ALARM,    		   RECORD_MONITER,	   TYPE_CRT_MONITER,	"盗警"},
	   {INT_TYPE_MONITOR_TIMEOUT_ALARM,    		   RECORD_MONITER,	   TYPE_CRT_MONITER,	"超时报警"},
	   {INT_TYPE_MONITOR_ALARM,                    RECORD_MONITER,	   TYPE_CRT_MONITER,	"监管报警"},

	   {INT_TYPE_MONITOR_HIGH_VOLTAGE,    		   RECORD_MONITER,	   TYPE_CRT_MONITER,	"过压故障"},
	   {INT_TYPE_MONITOR_LOW_VOLTAGE,    		   RECORD_MONITER,	   TYPE_CRT_MONITER,	"欠压故障"},
	   {INT_TYPE_MONITOR_HIGH_CURRENT,    		   RECORD_MONITER,	   TYPE_CRT_MONITER,	"过流故障"},
	   {INT_TYPE_MONITOR_LOSE_PHASE,    		   RECORD_MONITER,	   TYPE_CRT_MONITER,	"缺相故障"},
	   {INT_TYPE_MONITOR_FAULT_PHASE,    		   RECORD_MONITER,	   TYPE_CRT_MONITER,	"错相故障"},
	   {INT_TYPE_MONITOR_POWER_INTERRUPT,    	   RECORD_MONITER,	   TYPE_CRT_MONITER,	"中断供电"},
	   //----------------------------------------状态信息事件-------------------------------------//
	   {INT_TYPE_START_SUCESS,					   RECORD_OTHERS,	   TYPE_CRT_START,	    "设备启动"},
	   {INT_TYPE_STOP_SUCESS,					   RECORD_OTHERS,	   TYPE_CRT_START,	    "设备停动"},
	   {INT_TYPE_SHILED_SUCESS, 				   RECORD_OTHERS,	   TYPE_CRT_SHIELD,     "设备屏蔽"},
	   {INT_TYPE_RELEASE_SUCESS, 				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "解除屏蔽"},
	   {INT_TYPE_PHONE_CALL, 			           RECORD_MONITER,	   TYPE_CRT_RESERVE,    "电话呼叫"},
	   {INT_TYPE_PHONE_CONNECT,			           RECORD_MONITER,	   TYPE_CRT_RESERVE,    "电话接通"},
	   {INT_TYPE_PHONE_HUNGUP,			           RECORD_FREE,	       TYPE_CRT_RESERVE,    "电话挂断"},
	   {INT_TYPE_RESET_SUCESS,					   RECORD_RESET, 	   TYPE_CRT_RESET,	    "设备复位"},
	   {INT_TYPE_SET_RARA_SUCESS,				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "参数修改"},
	   {INT_TYPE_DEVICE_SILENCE, 		   		   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "设备消音"},
	   
	   {INT_TYPE_STARTED,				   		   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "启动已成功"},
	   {INT_TYPE_STOPPED,				   		   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "停动已成功"},
	   {INT_TYPE_SHILEDED,				   		   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "屏蔽已成功"},
	   {INT_TYPE_RELEASED,				   		   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "解除屏蔽已成功"},
	   {INT_TYPE_UPDATA_SUCC,				   	   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "升级成功"},

	   {INT_TYPE_START_FAIL,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	   	   "启动失败"},
	   {INT_TYPE_STOP_FAIL, 					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	   	   "停动失败"},
	   {INT_TYPE_SHILED_FAIL,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,		   "屏蔽失败"},
	   {INT_TYPE_RESUME_FAIL,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	   	   "解除屏蔽失败"},
	   {INT_TYPE_PHONE_CALL_FAIL,				   RECORD_FAULT, 	   TYPE_CRT_SHIELD_RESUME, "电话呼叫失败"},
	   {INT_TYPE_PHONE_CONNECT_FAIL,			   RECORD_FAULT, 	   TYPE_CRT_SHIELD_RESUME, "电话接通失败"},
	   {INT_TYPE_PHONE_HUNGUP_FAIL, 			   RECORD_FAULT, 	   TYPE_CRT_SHIELD_RESUME, "电话挂断失败"},
	   {INT_TYPE_RESET_FAIL,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	   	   "复位失败"},
	   {INT_TYPE_SET_RARA_FAIL, 				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	  	   "参数修改失败"},
	   {INT_TYPE_SILENCE_FAIL,			   		   RECORD_FAULT, 	   TYPE_CRT_FAULT,	   	   "设备消音失败"},
	   {INT_TYPE_UPDATA_FAIL,				   	   RECORD_FAULT,	   TYPE_CRT_RESERVE,   	   "升级失败"},

	   {INT_TYPE_MANUAL_CONTROL_ALLOW,			   REOCRD_MODE,	   	   TYPE_CRT_FAULT,	   	"手动控制允许"},
	   {INT_TYPE_AUTO_CONTROL_ALLOW,			   REOCRD_MODE,	   	   TYPE_CRT_FAULT,	    "自动控制允许"},
	   {INT_TYPE_SPRAY_ALLOW,					   REOCRD_MODE,	   	   TYPE_CRT_FAULT,	    "喷洒允许"},
	   {INT_TYPE_PARTIAL_AUTO_CONTROL_ALLOW,	   REOCRD_MODE,	   	   TYPE_CRT_FAULT,	    "部分自动允许"},
	   
	   {INT_TYPE_MANUAL_CONTROL_FORBID, 		   REOCRD_MODE,	   	   TYPE_CRT_FAULT,	    "手动控制禁止"},
	   {INT_TYPE_AUTO_CONTROL_FORBID,			   REOCRD_MODE,	   	   TYPE_CRT_FAULT,	    "自动控制禁止"},
	   {INT_TYPE_SPRAY_FORBID,					   REOCRD_MODE,	   	   TYPE_CRT_FAULT,	    "喷洒禁止"},
	   
	   {INT_TYPE_CHANNEL_SETUP, 				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "通道建立"},
	   {INT_TYPE_STS_PHONE_SETUP,				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "通话建立"},
	   {INT_TYPE_LINK_SETUP,					   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "链路建立"},
	   
	   {INT_TYPE_EMERGENCY_BRODCAST_AUTO,		   REOCRD_MODE,	   	   TYPE_CRT_RESERVE,    "自动应急广播"},
	   {INT_TYPE_EMERGENCY_BRODCAST_MANUAL, 	   REOCRD_MODE,	   	   TYPE_CRT_RESERVE,    "手动应急广播"},
	   {INT_TYPE_EMERGENCYT_OVER_AUTO,			   REOCRD_MODE, 	   TYPE_CRT_RESERVE,    "自动应急结束"},
	   {INT_TYPE_EMERGENCYT_OVER_MANUAL,		   REOCRD_MODE, 	   TYPE_CRT_RESERVE,    "手动应急结束"},
	   
	   {INT_TYPE_ON_SCENCE_COMMANDER, 			   REOCRD_MODE,	   	   TYPE_CRT_RESERVE,    "现场指挥"},
	   {INT_TYPE_ON_SCENCE_COMMANDER_OVER,		   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "指挥结束"},
	   
	   {INT_TYPE_EXSOUND_BEGIN, 				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "外音播放"},
	   {INT_TYPE_EXSOUND_OVER,					   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "外音结束"},
	   {INT_TYPE_EXSOUND_PAUSE, 				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "外音暂停"},
	   {INT_TYPE_EXSOUND_RECOVER,				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "外音恢复"},
	   
	   {INT_TYPE_SDCARD_INSERT, 				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "SD卡插入"},
	   {INT_TYPE_SDCARD_EXTRACT,				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "SD卡拔出"},
	   
	   {INT_TYPE_UDISK_INSERT,					   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "U盘插入"},
	   {INT_TYPE_UDISK_EXTRACT, 				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "U盘拔出"},
	   
	   {INT_TYPE_SOUNDRECORD_BEGIN, 			   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "录音开始"},
	   {INT_TYPE_SOUNDRECORD_OVER,				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "录音结束"},
	   
	   {INT_TYPE_SOUNDPLAY_BEGIN,				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "放音开始"}, 			   
	   {INT_TYPE_SOUNDPLAY_OVER,				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "放音结束"},
	   {INT_TYPE_SOUNDPLAY_PAUSE,				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "放音暂停"},
	   {INT_TYPE_SOUNDPLAY_RECOVER, 			   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "放音恢复"},
	   {INT_TYPE_AUDIO_LENGTH, 			   		   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "放音长度"},
   
   
	   
	   {INT_TYPE_CHANNEL_BREAK, 				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "通道断开"}, 			   
	   {INT_TYPE_STS_PHONE_HUNGUP,				   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "通话结束"},
	   {INT_TYPE_LINK_BREAK,					   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "链路断开"},
	   
	   {INT_TYPE_BACKUP_POWER_MAINTENANCE,		   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "备电维护"},
	   
	   {INT_TYPE_MAINPOW_ON,					   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "主电工作"},
	   {INT_TYPE_BACKPOW_ON,					   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "备电工作"},
	   
	   {INT_TYPE_SOFTWARE_UPGRADE,				   REOCRD_MODE,		   TYPE_CRT_RESERVE,	"程序升级"},
	   {INT_TYPE_DEVICE_ADDRESS_MODIFY, 		   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "设备地址修改"},
	   {INT_TYPE_SYSTEM_MODE_MODIFY,			   REOCRD_MODE,		   TYPE_CRT_RESERVE,    "系统模式更改"},
	   
	   {INT_TYPE_POWER_ON,						   RECORD_POWERONOFF,  TYPE_CRT_FAULT,	    "设备开机"},
	   {INT_TYPE_POWER_OFF, 					   RECORD_POWERONOFF,  TYPE_CRT_FAULT,	    "设备关机"},
   
	   {INT_TYPE_SELFCHECK_CANCEL,			   	   REOCRD_SELFCHECK,   TYPE_CRT_RESERVE,   "设备自检取消"},
	   {INT_TYPE_SELFCHECK_OVER,			   	   REOCRD_SELFCHECK,   TYPE_CRT_RESERVE,   "设备自检完成"},
	   {INT_TYPE_DEVICE_SELFCHECK ,		   		   REOCRD_SELFCHECK,   TYPE_CRT_RESERVE,   "设备自检"},
   
	   {INT_TYPE_EARLY_ALARM_RESET, 			   RECORD_RESUME,	   TYPE_CRT_RESERVE,    "预警恢复"},
	   {INT_TYPE_ALARM_FROM_EARLYALM,			   RECORD_RESUME,	   TYPE_CRT_RESERVE,    "预警变为火警"},
	   {INT_TYPE_CACHE_OUT_RESET,				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "通道恢复空闲"},
	   
	   {INT_TYPE_NET_ON_LINE,					   RECORD_OTHERS,	   TYPE_CRT_RESERVE,	"在岗"},
	   {INT_TYPE_NET_OFF_LINE,					   RECORD_OTHERS,	   TYPE_CRT_RESERVE,	"漏岗"},  
	   {INT_TYPE_NET_ON_LINE_AGAIN,				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,	"复岗"},  

	   //----------------------------------------反馈信息事件-------------------------------------//
	   {INT_TYPE_DEVICE_START_FEED_SUC,		   	   RECORD_ANSWER,	   TYPE_CRT_ANSWER,     "启动反馈成功"},
	   {INT_TYPE_DEVICE_START_FEED_FAIL, 	   	   RECORD_FAULT, 	   TYPE_CRT_ANSWER,     "启动反馈失败"},
	   
	   {INT_TYPE_DEVICE_STOP_FEED_SUC,		   	   RECORD_CLEAR, 	   TYPE_CRT_ANSWER,     "停止反馈成功"},
	   {INT_TYPE_DEVICE_STOP_FEED_FAIL,		   	   RECORD_FAULT, 	   TYPE_CRT_ANSWER,     "停止反馈失败"},
	   
	   {INT_TYPE_SYSC_DATA_SUC, 				   RECORD_ANSWER,	   TYPE_CRT_DELAY,	    "信息同步成功"},
	   {INT_TYPE_SYSC_DATA_FAIL,				   RECORD_FAULT, 	   TYPE_CRT_DELAY,	    "信息同步失败"},
	   
	   {INT_TYPE_COMPARE_OVER,					   RECORD_OTHERS,	   TYPE_CRT_DELAY,	    "信息核对成功"},
	   {INT_TYPE_COMPARE_FAIL,					   RECORD_FAULT, 	   TYPE_CRT_DELAY,	    "信息核对失败"},
	   
	   {INT_TYPE_MANUAL_DELAY_START,			   RECORD_DELAY, 	   TYPE_CRT_DELAY,	    "延时开始"},
	   {INT_TYPE_MANUAL_DELAY_CANCEL,			   RECORD_CANCEL,	   TYPE_CRT_DELAY,	    "延时取消"},
	   {INT_TYPE_DELAY_SUC, 					   RECORD_CANCEL,	   TYPE_CRT_DELAY,	    "延时成功"},
	   
	   {INT_TYPE_SWITCH_INPUT_ON,				   RECORD_MONITER,	   TYPE_CRT_DELAY,	    "开输入开关量"},
	   {INT_TYPE_SWITCH_INPUT_OFF,				   RECORD_MONITER,	   TYPE_CRT_DELAY,	    "关输入开关量"},
	   {INT_TYPE_ANALOG_INPUT_VOLTAGE,			   RECORD_MONITER,	   TYPE_CRT_DELAY,	    "电压模拟量输入"},
	   {INT_TYPE_ANALOG_INPUT_CURRENT,			   RECORD_MONITER,	   TYPE_CRT_DELAY,	    "电流模拟量输入"},
	   
	   {INT_TYPE_PARA1_CHECK_SUC,				   RECORD_ANSWER,	   TYPE_CRT_RESERVE,    "参数1校准成功"},
 	   {INT_TYPE_PARA1_CHECK_FAIL,				   RECORD_ANSWER,	   TYPE_CRT_RESERVE,    "参数1校准失败"},
 	   {INT_TYPE_PARA2_CHECK_SUC,				   RECORD_ANSWER,	   TYPE_CRT_RESERVE,    "参数2校准成功"},
	   {INT_TYPE_PARA2_CHECK_FAIL,				   RECORD_ANSWER,	   TYPE_CRT_RESERVE,    "参数2校准失败"},
	   {INT_TYPE_PARA3_CHECK_SUC,				   RECORD_ANSWER,	   TYPE_CRT_RESERVE,    "参数3校准成功"},
	   {INT_TYPE_PARA3_CHECK_FAIL,				   RECORD_ANSWER,	   TYPE_CRT_RESERVE,    "参数3校准失败"},
	   {INT_TYPE_PARA4_CHECK_SUC,				   RECORD_ANSWER,	   TYPE_CRT_RESERVE,    "参数4校准成功"},
	   {INT_TYPE_PARA4_CHECK_FAIL,				   RECORD_ANSWER,	   TYPE_CRT_RESERVE,    "参数4校准失败"},
	   {INT_TYPE_PARA5_CHECK_SUC,				   RECORD_ANSWER,	   TYPE_CRT_RESERVE,    "参数5校准成功"},
	   {INT_TYPE_PARA5_CHECK_FAIL,				   RECORD_ANSWER,	   TYPE_CRT_RESERVE,    "参数5校准失败"},
	   //----------------------------------------监控类事件-------------------------------------//
	   {INT_TYPE_FIRE_STATE,					   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "火焰强度"},
	   {INT_TYPE_TEMPE_STATE,					   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "温度"},
	   {INT_TYPE_SMOKE_STATE,					   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "烟浓度"},
	   {INT_TYPE_GAS_STATE, 					   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "燃气浓度"},
	   {INT_TYPE_THEFT_STATE,					   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "盗警"},
	   {INT_TYPE_HUMIDITY_STATE,				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "湿度"},
	   {INT_TYPE_PRESSURE_STATE,				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "压力"},
	   {INT_TYPE_CURRENT_STATE, 				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "电流"},
	   {INT_TYPE_VOLTAGE_STATE, 				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "电压"},
	   {INT_TYPE_CURRENT_A_STATE, 				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "A相电流"},
	   {INT_TYPE_CURRENT_B_STATE, 				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "B相电流"},
	   {INT_TYPE_CURRENT_C_STATE, 				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "C相电流"},
	   {INT_TYPE_VOLTAGE_A_STATE, 				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "A相电压"},
	   {INT_TYPE_VOLTAGE_B_STATE, 				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "B相电压"},
	   {INT_TYPE_VOLTAGE_C_STATE, 				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "C相电压"},
	   {INT_TYPE_VOLTAGE_CURRENT_STATE, 		   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "电压电流"},
	   {INT_TYPE_COMMUNICATION_SUCCESSRATE, 	   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "通讯成功率"},
	   {INT_TYPE_AMPLIFIER_INFORM,				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "功放信息"},
	   
	   //----------------------------------------故障类事件-------------------------------------//
	   {INT_TYPE_EEPROM_RW_FAU, 				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "EEPROM读写故障"},
	   {INT_TYPE_RAM_RW_FAU,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "RAM读写故障"},
	   {INT_TYPE_FLASH_RW_FAU,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "FLASH读写故障"},
	   {INT_TYPE_UDISK_R_FAU,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "U盘读错误"},
	   {INT_TYPE_UDISK_W_FAU,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "U盘写错误"},
   
	   
	   {INT_TYPE_EEPROM_ERASURE_FAU,			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "EEPROM擦除故障"},
	   {INT_TYPE_FLASH_ERASURE_FAU, 			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "FLASH擦除故障"},
	   {INT_TYPE_GSM_REG_FAIL,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "GSM NET注册失败"},
	   {INT_TYPE_GPRS_REG_FAIL, 				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "GPRS NET注册失败"},
	   {INT_TYPE_SIM_LOST,						   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "SIM卡缺失"},
	   {INT_TYPE_GPRS_NACK, 					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "GPRS模块通信无响应"},
	   {INT_TYPE_PRINTER_NO_PAPER,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "缺纸"},
	   {INT_TYPE_PRINTER_PAPER_JAM, 			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "卡纸"},
	   {INT_TYPE_PRINTER_OVER_TEMPE,			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "机头超温"},	   
	   {INT_TYPE_KEY_FAU,						   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "按键故障"},
	   {INT_TYPE_HOST_KEY_FAU,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "主机键盘故障"},
	   {INT_TYPE_SPEAKER_FAU,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "扬声器故障"},
	   {INT_TYPE_AMPLIFIER_FAU, 				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "功放故障"},
	   {INT_TYPE_MIC_FAU,						   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "MIC故障"},
	   {INT_TYPE_BLACKBOX_FAU,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "数据存储单元故障"},
	   {IND_TYPE_RTC_CLOCK_FAU, 				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "RTC时钟故障"},
	   {IND_TYPE_ETHERNET_CONTR_FAU, 		       RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "以太网控制器故障"},

	   {INT_TYPE_SMOKE_SENSOR_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "烟雾传感器故障"},
	   {INT_TYPE_TEMPE_SENSOR_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "温度传感器故障"},
	   {INT_TYPE_UV_SENSOR_FAU, 				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "紫外传感器故障"},
	   {INT_TYPE_INFRARED_SENSOR_FAU,			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "红外传感器故障"},
	   {INT_TYPE_PYROELE_SENSOR_FAU,			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "热释电传感器故障"},
	   {INT_TYPE_HUMIDITY_SENSOR_FAU,			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "湿度传感器故障"},
	   {INT_TYPE_CURRENT_SENSOR_FAU,			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "电流传感器故障"},
	   {INT_TYPE_FLAMEGAS_SENSOR_FAU,			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "可燃气体传感器故障"},
	   {INT_TYPE_SWITCH_SENSOR_FAU,			   	   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "开关传感器故障"},
	   {INT_TYPE_DOOR_FAU,			   			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "门故障"},
	   
	   {INT_TYPE_REQUEST_MAINTENANCE,			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "请求维保"},
	   {INT_TYPE_PASSWORD_AGING,			       RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "密码过期"},  
	   {INT_TYPE_INPUT_POWER_SHORT_CIRCUIT, 	   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输入电源短路"},
	   {INT_TYPE_INPUT_POWER_OPEN_CIRCUIT,		   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输入电源断路"},
	   {INT_TYPE_INPUT_POWER_VOLTAGE_H, 		   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输入电源过压"},
	   {INT_TYPE_INPUT_POWER_VOLTAGE_L, 		   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输入电源欠压"},
	   {INT_TYPE_INPUT_POWER_CURRENT_H, 		   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输入电源过流"},
	   {INT_TYPE_INPUT_POWER_FAULT,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "主电故障"},
	   
	   {INT_TYPE_OUTPUT_POWER_SHORT_CIRCUIT,	   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "电源输出短路"},
	   {INT_TYPE_OUTPUT_POWER_OPEN_CIRCUIT, 	   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "电源输出断路"},
	   {INT_TYPE_OUTPUT_POWER_VOLTAGE_H,		   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "电源输出过压"},
	   {INT_TYPE_OUTPUT_POWER_VOLTAGE_L,		   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "电源输出欠压"},
	   {INT_TYPE_OUTPUT_POWER_CURRENT_H,		   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "电源输出过流"},
	   {INT_TYPE_MODULE_POWER_VOLTAGE_H,		   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "过压"},
	   {INT_TYPE_MODULE_POWER_VOLTAGE_L,		   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "欠压"},
	   
	   {INT_TYPE_BACKUP_POWER_SHORT_CIRCUIT,	   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "备电短路"},
	   {INT_TYPE_BACKUP_POWER_OPEN_CIRCUIT, 	   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "备电断路"},
	   {INT_TYPE_BACKUP_POWER_VOLTAGE_H,		   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "备电过压"},
	   {INT_TYPE_BACKUP_POWER_VOLTAGE_L,		   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "备电欠压"},
	   {INT_TYPE_BACKUP_POWER_WARNING,			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "备电警示"},
	   {INT_TYPE_BACKUP_POWER_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "备电故障"},
	   
   
	   
	   {INT_TYPE_BUS_SHORT_CIRCUIT, 			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "总线短路"},
	   {INT_TYPE_BUS_OPEN_CIRCUIT,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "总线断路"},
	   {INT_TYPE_BUS_VOLTAGE_H, 				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "总线过压"},
	   {INT_TYPE_BUS_VOLTAGE_L, 				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "总线欠压"},
	   {INT_TYPE_BUS_CURRENT_H, 				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "总线过流"},
	   {INT_TYPE_BLC_SHORT_H,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "载波总线短路"},
	   {INT_TYPE_BLC_OPEN_H,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "载波总线断路"},
	   
	   {INT_TYPE_INPUT1_SHORT_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输入1短路"},
	   {INT_TYPE_INPUT1_OFF_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输入1断路"},
	   {INT_TYPE_INPUT2_SHORT_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输入2短路"},
	   {INT_TYPE_INPUT2_OFF_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输入2断路"},
	   {INT_TYPE_INPUT3_SHORT_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输入3短路"},
	   {INT_TYPE_INPUT3_OFF_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输入3断路"},
	   {INT_TYPE_INPUT4_SHORT_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输入4短路"},
	   {INT_TYPE_INPUT4_OFF_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输入4断路"},
	   
	   {INT_TYPE_OUTPUT1_SHORT_FAU, 			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输出1短路"},
	   {INT_TYPE_OUTPUT1_OFF_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输出1断路"},
	   {INT_TYPE_OUTPUT2_SHORT_FAU, 			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输出2短路"},
	   {INT_TYPE_OUTUT2_OFF_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输出2断路"},
	   {INT_TYPE_OUTPUT3_SHORT_FAU, 			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输出3短路"},
	   {INT_TYPE_OUTPUT3_OFF_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输出3断路"},
	   {INT_TYPE_OUTPUT4_SHORT_FAU, 			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输出4短路"},
	   {INT_TYPE_OUTPUT4_OFF_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "输出4断路"},
	   {INT_TYPE_NET_LINE_FAULT,				   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "网络链路故障"},
	   {INT_TYPE_TCP_CONNECT_OPEN_FAU,			   RECORD_FAULT,	   TYPE_CRT_FAULT,		"TCP连接断开故障"},
	   {INT_TYPE_GPRS_CONNECT_OPEN_FAU,			   RECORD_FAULT,	   TYPE_CRT_FAULT,		"GPRS连接断开故障"},

	   {INT_TYPE_NODEANS_FAU,					   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "命令无应答"},
	   {INT_TYPE_NODECOMM_FAU,					   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "命令不支持"},
	   {INT_TYPE_ANSCHECK_FAU,					   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "应答校验错误"},
	   {INT_TYPE_LIMIT_CONDITION_FAU,			   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "条件受限"},
	   {INT_TYPE_NODE_REGISTER_FAU, 			   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "节点未注册"},
	   {INT_TYPE_NODE_OFFLINE,					   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "节点未在线"},
	   {INT_TYPE_ADDR_OVERRUN,					   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "寄存器地址超限"},
	   {INT_TYPE_NODE_SHILED_STA,				   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "设备被屏蔽"},
	   {INT_TYPE_NODE_NUM_OVERFLOW, 			   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "节点数量溢出"},
	   {INT_TYPE_BUFFER_OVERFLOW,				   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "通信缓存溢出"},
	   {INT_TYPE_DATA_LOST, 					   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "数据帧丢失"},
	   {INT_TYPE_DATA_CRC_FAU,					   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "数据帧CRC错误"},
	   {INT_TYPE_NET_COMM_FAULT,				   RECORD_FAULT,	   TYPE_CRT_FAULT,	    "网络通信故障"},
	   
   
   
	   {INT_TYPE_CODE_OVERFLOW, 				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "用户区代码超界"},
	   {INT_TYPE_CODE_ILLEGAL,					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "用户区代码不合法"},
	   {INT_TYPE_LINK_FORMULA_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "联动公式错误"},
	   {INT_TYPE_DEVICE_INF_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "设备信息错误"},
	   {INT_TYPE_INF_FAU,						   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "回路信息错误"},
	   {INT_TYPE_USER_PROGRAM_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "用户程序错误"},
	   {INT_TYPE_FLASH_90PERCENT_FAU,			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "录音内存不足"},
	   {INT_TYPE_FLASH_FULL_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "录音满"},
	   
	   {INT_TYPE_SYSC_LINK_FAU, 				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "联动公式同步错误"},
	   {INT_TYPE_SYSC_DEVICE_FAU,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "设备信息同步错误"},
	   {INT_TYPE_ALARM_ERR,                        RECORD_FAULT,       TYPE_CRT_FAULT,	    "异常报警"},
	   
	   {INT_TYPE_UNREGGISTE_UPLOAD, 			   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "未注册上报"},
	   {INT_TYPE_SHIELD_UPLOAD, 				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "已屏蔽上报"},
	   {INT_TYPE_RE_UPLOAD, 					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "重复上报"},
	   {INT_TYPE_RE_ACTION, 					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "重复动作"},
   
	   {INT_TYPE_MAINPOW_RECOVER,				   RECORD_ELIMINATE,    TYPE_CRT_RESERVE,   "主电恢复正常"},
	   {INT_TYPE_BACKUP_POWER_RECOVER,			   RECORD_ELIMINATE,    TYPE_CRT_RESERVE,   "备电恢复正常"},
	   {INT_TYPE_PRINTER_PAPER_RESUM,			   RECORD_ELIMINATE,    TYPE_CRT_RESERVE,   "上纸"},
	   {INT_TYPE_BUFFER_OVERFLOW_RECOVER,		   RECORD_ELIMINATE,    TYPE_CRT_RESERVE,   "缓存溢出恢复"},
	   {INT_TYPE_REG_DEV_ONLINE,				   RECORD_ELIMINATE,    TYPE_CRT_RESERVE,   "注册设备上线"},
	   {INT_TYPE_NET_LINE_RECOVER_NOMAL,		   RECORD_ELIMINATE,    TYPE_CRT_RESERVE,   "网络链路恢复正常"},
	   {INT_TYPE_NET_COM_RECOVER_NOMAL, 		   RECORD_ELIMINATE,    TYPE_CRT_RESERVE,   "网络通信恢复正常"},
	   {INT_TYPE_DEV_RECOVER_NOMAL, 		  	   RECORD_ELIMINATE,    TYPE_CRT_RESERVE,   "设备恢复"},
	   {INT_TYPE_TCP_CONNECT_RECOVER,		       RECORD_ELIMINATE,	TYPE_CRT_RESERVE,	"TCP连接恢复正常"},		   
	   {INT_TYPE_GPRS_CONNECT_RECOVER,		       RECORD_ELIMINATE,    TYPE_CRT_RESERVE,   "GPRS连接恢复正常"},
	

	   //----------------------------------------组态类事件-------------------------------------//
	   
	   {INT_TYPE_REG_DEV_OFFLINE,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "注册设备掉线"},
	   {INT_TYPE_NEW_DEV_ONLINE,				   RECORD_FAULT, 	   TYPE_CRT_RESERVE,    "新增设备上线"},
	   {INT_TYPE_DEV_REPLACE,					   RECORD_FAULT, 	   TYPE_CRT_RESERVE,    "替换设备"},	   
	   {INT_TYPE_REPEAT_ID_DEV, 				   RECORD_FAULT, 	   TYPE_CRT_RESERVE,    "存在ID重号设备"},
	   {INT_TYPE_REPEAT_UC_DEV, 				   RECORD_FAULT, 	   TYPE_CRT_RESERVE,    "存在UC重号设备"},
	   {INT_TYPE_REFRESH_DELETE,				   RECORD_FAULT, 	   TYPE_CRT_RESERVE,    "删除设备"}, 
	   {INT_TYPE_DEV_RECOVER, 		   			   RECORD_FAULT,       TYPE_CRT_RESERVE,    "设备接入"},
	   {INT_TYPE_DEV_FAULT, 		   			   RECORD_FAULT,       TYPE_CRT_RESERVE,    "设备故障"},
	   
	   {INT_TYPE_GET_MAIN_TOKEN,				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "索取主站令牌"},
	   {INT_TYPE_ONLINE_APPLIED,				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "请求上线"},
	   {INT_TYPE_COMMUN_SUCC_RATE,				   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "设备通信成功率"},
	   {INT_TYPE_UC_CHANGE, 					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "UC变化"},
	   {INT_TYPE_ID_CHANGE, 					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "ID变化"},
	   {INT_TYPE_CC_CHANGE, 					   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "CC变化"},
	   {INT_TYPE_PARAMETER_CHANGE,				   RECORD_FAULT, 	   TYPE_CRT_FAULT,	    "参数变化"},
	   
   
	   {INT_TYPE_DEV_INFLOG_FINISH, 			   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "设备信息登录完毕"},
	   {INT_TYPE_LOG_DEV_INF,					   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "登录的设备信息"},
	   {INT_TYPE_LA_UNLAW_DEV_INF,				   RECORD_FAULT, 	   TYPE_CRT_RESERVE,    "LA非法的设备信息"},
	   {INT_TYPE_LOGIN_OPER,					   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "系统操作员登录"},
	   {INT_TYPE_LOGIN_ADMIN,					   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "系统管理员登录"},
	   {INT_TYPE_LOGIN_SUPER,					   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "超级管理员登录"},
	   {INT_TYPE_REG_RENEW, 					   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "重新注册"},
	   {INT_TYPE_REG_REFRESH,					   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "刷新注册"},
	   {INT_TYPE_REG_AUTO,						   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "自动注册"},
	   {INT_TYPE_REG_NETWORK,					   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "网络注册"},
	   {INT_TYPE_DEFINE_AUTO,					   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "设备自动定义"},
	   {INT_TYPE_DEFINE_ORDER,					   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "设备继承定义"},
	   {INT_TYPE_DEFINE_SERIAL, 				   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "设备连续定义"},
	   {INT_TYPE_FEILD_DEV_DEFINE,				   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "现场设备定义"},
	   {INT_TYPE_BUS_PANEL_DEFINE,				   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "总线盘定义"},
	   {INT_TYPE_MULTI_PANEL_DEFINE,			   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "直控盘定义"},
	   {INT_TYPE_BROCAST_PANEL_DEFINE,			   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "广播盘定义"},
	   {INT_TYPE_CONTROLER_DEFINE,				   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "控制器定义"},
	   {INT_TYPE_PHONEDISTRI_PANEL_DEFINE,		   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "电话分配盘定义"},
	   {INT_TYPE_PRE_ALARM_DEFINE,				   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "预警定义"},
	   {INT_TYPE_LA_CHANGE,		                   RECORD_SETUP,	   TYPE_CRT_RESERVE,    "LA修改"},
	   {INT_TYPE_NODE_NORMAL,		     		   RECORD_OTHERS,	   TYPE_CRT_RESERVE,    "节点正常"},
	   
	   {INT_TYPE_NULL,							   RECORD_MISC,		   TYPE_CRT_RESERVE,    "未定义事件"},
};
//======================================================================================================================
//

UINT8 const * const ALARM_TYPE_TAB[]=
{
    "火警",           //0x00
	"监管",           //0x01          
	"启动",           //0x02
    "反馈",           //0x03
    "延时",           //0x04
    "故障",           //0x05
    "屏蔽",           //0x06 
    "预警",           //0x07
    "复位",           //0x08	
    //消除消息类

	"释放",           //0x09
	"停止",           //0x0A
	"清除",           //0x0B 
	"取消",           //0x0C	
	"排除",           //0x0D 
	"解除",           //0x0E
	"恢复",           //0x0F	
	//开关机
	"开关机",           //0x10
	"设置",           //0x11
	
	//状态
	"自检",           //0x12
	"模式",           //0x13
	
	//用户及状态	
	"本地命令",           //0x14	
	"网络命令",           //0x15	
	"其他",           //0x16
	"未知",           //0x17	
};
 

/*har const * const ACTION_TYPE_TAB[]=
{
	"停动",
	"启动",
	"翻转",
	"未知",
};*/



#endif
