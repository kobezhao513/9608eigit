
#include "sys_init.h" 
#include "timer.h" 
#include "main.h" 
#include "Parallel_Port.h"
#include "ex_flash.h"
#include "led.h"
#include "record_manage.h"
#include "controller.h"
#include "core_cm0.h"
#include "core_cmfunc.h"


CONFIG System;//系统配置，不易变
RUNSTA RunSta;


const UINT8 SOFTWARE_VERSION[3] = {SOFT_V_BYTE0,SOFT_V_BYTE1,SOFT_V_BYTE2};
const UINT8 HARDWARE_VERSION[3] = {HARD_V_BYTE0,HARD_V_BYTE1,HARD_V_BYTE2};

/*=====================================================*/
//函数名称:void Init_WATCHDOG(void)
//函数功能:初始化看门狗
//参		数:void
//返	回	 值:void
//更改记录:
/*=====================================================*/
void Init_WATCHDOG(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_SetReload(0x0FFF);
	IWDG_ReloadCounter();
	IWDG_Enable();
}
/*=====================================================*/
//函数名称:void DOG(void)
//函数功能:清看门狗
//参		数:void
//返	回	 值:void
//更改记录:2017年2月21日李伟民
/*=====================================================*/
void Watch_Dog(void)
{
#ifdef WATCH_DOG_ENABLE
	IWDG_ReloadCounter();
#endif
}

/*********************************************************************************
* 函数名称 : str_get_len
* 函数描述 : 软件复位:从SYSRESETREQ被置为有效，到复位发生器执行复位命令往往会有一个延时。
			在此延时期间，处理器仍然可以响应中断请求。但我们的本意往往是要让此次执行到此为止，
			不要再做任何其它事情了。所以，最好在发出复位请求前，先把FAULTMASK置位。
* 输入参数 : None；
* 输出参数 : None
* 返 回 值 : None
* 修改日期 ：2014-06
*********************************************************************************/
void Soft_Reset(void)
{
   // Delay_ms(40);
	//__set_FAULTMASK(1);  	//关闭所有中端
	NVIC_SystemReset();		//复位
}


/*=====================================================*/
//函数名称:void Ppwer_485_Init(void)
//函数功能:初始化PWM
//占空比50%设置死区时间10%      到占空比4:6互补PWM波形
//参		数:void
//返	回	 值:void
//更改记录:2018年1月31日谢韬
/*=====================================================*/
void Power_485_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /*GPIOB clocks enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* TIM15 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_1);								//PB14复用为TIM15_CH1
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_3);								//PB15复用为TIM15_CH1N
	
	TIM_TimeBaseStructure.TIM_Prescaler = 0;									//预分频，实际分频系数为所设的值+1，此处不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;					//TIM向上计数模式
  TIM_TimeBaseStructure.TIM_Period = (SystemCoreClock / 100000) - 1;			//设置在下一个更新事件装入活动的自动重装载寄存器周期的值,100k
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;									//设置时钟分割
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;								//设置周期计数值,用于自动更新占空比
	TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);
	
	/* Channel 1, 2 and 3 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;								//选择定时器输出模式为PWM模式2，在向上计数时，当TIM15_CNT<TIM15_CRR1时，通道1为无效电平，否则为有效电平
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;					//比较输出使能
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;				//互补输出比较状态使能
  //TIM_OCInitStructure.TIM_Pulse = (uint16_t) ((uint32_t) 45 * (TIM_TimeBaseStructure.TIM_Period - 1)/ 100);//占空比设置
  TIM_OCInitStructure.TIM_Pulse = (uint16_t) ((uint32_t) 5 * (TIM_TimeBaseStructure.TIM_Period - 1)/ 10);//占空比设置

  //TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;					//输出极性和互补极性的有效电平为高
  //TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  //TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM15, &TIM_OCInitStructure);
  //TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
  TIM_OC2Init(TIM15, &TIM_OCInitStructure);
  //TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
  TIM_OC3Init(TIM15, &TIM_OCInitStructure);

  /* Automatic Output enable, Break, dead time and lock configuration*/
	//死区设置
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
  TIM_BDTRInitStructure.TIM_DeadTime = 48;															//调整死区大小0-ff
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;

  TIM_BDTRConfig(TIM15, &TIM_BDTRInitStructure);

  /* TIM1 counter enable */
  TIM_Cmd(TIM15, ENABLE);

  /* Main Output Enable */
  TIM_CtrlPWMOutputs(TIM15, ENABLE);//TIM15_OC通道输出PWM
}

//==================================================================================
//函	数：void McuCoreConfig(void)
//功	能：MUC 内核配置
//参	数：无
//返    回:   无
//更新日期：  2014.5
//==================================================================================
void McuCoreConfig(void)
{
	//SystemInit();//时钟48M  AHB:48M APB:48M
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource13, GPIO_AF_0);//PA13复用为SWJIO
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource14, GPIO_AF_0);//PA14复用为SWJCLK


#ifdef ADD_BOOT
/*
中断向量表的重映射:48个中断向量
参考: http://www.51hei.com/bbs/dpj-40235-1.html
*/
	memcpy((void*)0x20000000, (void*)ADDR_MCU_PROGROM_START, 48*4);
	SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);

#endif	


}

/********************************************************************************
* Function Name: void Init_Config(UINT8 type)
* Description  : System data is initialized.
* Arguments    : type-0:initializ,Otherwise return.
* Return Value : None
* Call Relation: External
* Change Time  : 2015-5
*********************************************************************************/
void Init_Config(void)
{
    UINT8 i;

    if(!Flash_Load_Info(SYSCONFIG,0,(UINT8*)(&System)))
			
    {
        memset((UINT8*)&System,0xff,sizeof(System));	
        memset(System.id,0,sizeof(System.id));     
    }

	if((memcmp(System.software_ver,SOFTWARE_VERSION,3)) 			//???????
		|| (memcmp(System.hardware_ver,HARDWARE_VERSION,3)))
	{														
		memcpy(System.software_ver,SOFTWARE_VERSION,3);
		memcpy(System.hardware_ver,HARDWARE_VERSION,3);
	}

	
	if((System.la == 0x00) || (System.la > 0xFC))
	{
		System.la = 0x01;
	}
	System.uc[4] = DEVICE_TYPE_OF_UC_CODE;

	for(i=0;i<LEN_REG_ID;i++)
	{
	  if(System.id[i] == 0xFF)
	  {
		  memset(System.id,0,LEN_REG_ID);
		  break;
	  }
	}


	if(System.ctr_type == 0xff)
	{
	  System.ctr_type = 0;
	}
	if(System.trans_equip_connect > 0x01)//配接传输设备标志
	{
	  System.trans_equip_connect = 0;
	}
	System.ctr_addr[4] = DEV_TYPE_CONTROL_UC;
	if(System.ctr_type)
	{
	  for(i=0;i<4;i++)
	  {
		  if(((0xF0&System.ctr_addr[i]) > 0x90)
			  || ((0x0F&System.ctr_addr[i]) > 0x09))
		  {
			  System.ctr_addr[i] = 0;
		  }
	  }
	}
	System.ctr_type = Parallel_Debug;
    Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));//有数据是否相同判断，正常情况下，不会重复擦写flash
}


/*=====================================================*/
//函数名称:void Mcu_Hard_Init(void)
//函数功能:设备硬件初始化
//参		数:void
//返	回	 值:void
//更改记录:2018年2月5日谢韬
/*=====================================================*/
void Mcu_Hard_Init(void)
{

  Timer14_Init(99,479); //主定时器,1ms*100=48MHz/(479+1)/1000
	Led_Init();
	Flash_Init();
	Power_485_Init();//100k,60%占空比232/485隔离电源供电
	Parallel_Port_Init();
	
	#ifdef WATCH_DOG_ENABLE
	Init_WATCHDOG();
	#endif
}
/*=====================================================*/
//函数名称:void Mcu_Hard_Init(void)
//函数功能:设备硬件初始化
//参		数:void
//返	回	 值:void
//更改记录:2018年2月5日谢韬
/*=====================================================*/
void Mcu_Data_Init(void)
{
  	Init_Config();
	Controller_Data_Init();//先读取控制器类型，根据类型对串口初始化
	Controller_Uart_Init();	
	
}

void Recover_Factory_Set(void)
{    	
    memset(&System.project_name,0xFF,32);
    memset(&System.project_address,0xFF,32);
    memset(&System.project_linkman,0xFF,16);
    memset(&System.project_phone,0xFF,16); 
    memset(&System.project_email,0xFF,32); 
    System.la=0x01;
	memset(&System.uc,0x00,3); 
	System.uc[3] = 0x01;
	System.uc[4] = 0x82;//联网模块
    memset(&System.cc,0xFF,32);



    Flash_Save_Info(SYSCONFIG,0,(UINT8*)(&System));

	//擦除外部flash的记录存储区域	
	Delete_All_Record();
}

/*=====================================================*/
//函数名称:void System_Initialize(void)
//函数功能:设备上电初始化
//参		数:void
//返	回	 值:void
//更改记录:2018年2月5日谢韬
/*=====================================================*/
void System_Initialize(void)
{
	McuCoreConfig();
	Mcu_Hard_Init();
	Mcu_Data_Init();
}
