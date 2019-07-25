#include "Parallel_Port.h"
#include "NT_FT8003_PRT.h"
#include "Led.h"
#include "EI_Print_COMM.h"




CONTR Parallel;

void Parallel_Port_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	//并行数据接收IO口初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_15;//PA15为busy脚，需要用时需配置为输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	//并行数据接收IO口初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
		//并行数据接收IO口初始化
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_Init(GPIOC, &GPIO_InitStructure);


}

/*=====================================================*/
//函数名称:void EXTI15_Init(void)
//函数功能:初始化外部中断
//参		数:void
//返	回	 值:void
//更改记录:2018年1月31日谢韬
/*=====================================================*/
void Parallel_Port_EXTI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;//pc15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);//使能SYSCFG时钟
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource15);//中断线连到PC15
	
	//配置外部中断线
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿打印机采集有效数据
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	//配置中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}



void Parallel_Port_Init(void)
{
	Parallel_Port_IO_Init();
	Parallel_Port_EXTI_Init();
}

/*=====================================================*/
//函数名称:UINT8 Read_IO_Voltage (void)
//函数功能:读取并行接口电平
//参		数:void
//返	回	 值:data
//更改记录:
/*=====================================================*/
UINT8 Read_IO_Voltage (void)
{
	BIT_CHAR data;
	
	//读取IO口电平
	data.b.b7 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
	data.b.b6 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
	data.b.b5 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);
	data.b.b4 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6);
	data.b.b3 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
	data.b.b2 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8);
	data.b.b1 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9);
	data.b.b0 = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13);
	return data.byte;
}

/*=====================================================*/
//函数名称:UINT8 Get_Parallel_Data (void)
//函数功能:取得并行接口数据
//参		数:void
//返	回	 值:data
//更改记录:
/*=====================================================*/
UINT8 Get_Parallel_Data (void)
{
	UINT8 data = 0,data1,data2,data3;
		
	data1 = Read_IO_Voltage();
	data2 = Read_IO_Voltage();
	data3 = Read_IO_Voltage();
	
	if((data1 == data2) || (data1 == data3))
	{
		data = data1;
	}
	else if(data2 == data3)
	{
		data = data2;
	}
	
	return data;
}

/*=====================================================*/
//函数名称:void EXTI4_15_IRQHandler(void)
//函数功能:外部中断事件处理
//参		数:void
//返	回	 值:void
//更改记录:
/*=====================================================*/

//节约RAM，与232共用结构体
void EXTI4_15_IRQHandler(void)
{
	UINT8  para_daraRec;

	para_daraRec=Get_Parallel_Data();
	Parall_Uart_Rec_Data(para_daraRec);
	On_Led_REC();
	/*RS232.R.Buf[RS232.R.Head] = Get_Parallel_Data();
	//
	RS485.T.Buf[RS485.T.Tail] = RS232.R.Buf[RS232.R.Head];
	RS485.T.Tail ++;
	if(RS485.T.Tail >= CONTR_BUF_LEN)
		RS485.T.Tail = 0;
	RS232.T.Buf[RS232.T.Tail] = RS232.R.Buf[RS232.R.Head];
	RS232.T.Tail ++;
	if(RS232.T.Tail >= CONTR_BUF_LEN)
		RS232.T.Tail = 0;
	
	On_Led_REC();
	//
	RS232.R.Head++;
	if(RS232.R.Head >= CONTR_BUF_LEN)
	{
			RS232.R.Head = 0;
	}*/
	EXTI_ClearITPendingBit(EXTI_Line15);//清空中断标志位，防止持续进入中断
}
void Parallel_Data_Init(void)
{
	Parallel.Type = System.parallel_type;
}
void Parallel_Task(void)
{
//	switch(Parallel.Type)
//	{
//		case FT8003:
//			Com_Task_FT8003_PRT(&RS232);
//		break;
//		default:
//			
//		break;
//	}
}



