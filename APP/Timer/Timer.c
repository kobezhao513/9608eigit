

#include "timer.h"
#include "controller.h"
#include "rtc.h"
#include "led.h"


SYS_TIMER  SysTimer;
UINT16  Rt_1D0mS;	
extern LED Led;



/******************************************************************
* Function Name: Timer7_Init()
* Description  : 主定时器（定时器7）初始话程序，中断时间为1ms.
* Arguments    : None
* Return Value : None
* Call Relation: Extern
* Change Time  : 2014-11
********************************************************************/
void Timer14_Init(UINT16 arr,UINT16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = arr; 					//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到100*600为1ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 					//设置用来作为TIMx时钟频率除数的预分频值  100Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure); 			//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	/* TIM IT enable */
	TIM_ITConfig(  			//使能或者失能指定的TIM中断
	TIM14, 
	TIM_IT_Update,    	//TIM 中断源
	ENABLE  			//使能
	);
	/* Enable the TIM3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;  			//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPriority = 3;   //先占优先级0级
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  		//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  							//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM14, ENABLE);  //使能TIMx外设
						 
}

void TIM14_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM14, TIM_IT_Update);    //清除TIMx的中断待处理位:TIM 中断源 

		SysTimer.Flag1Ms = 1;
		SysTimer.Tick ++;

		SysTimer.Count1Ms++;	
		switch(SysTimer.Count1Ms & 0x07)
		{
			case 0:
				//Monitor_Gprs_Timer();

			break;

			case 1: 
				Monitor_Controller_Uart_Timer_Run(&RS232);
				Monitor_Controller_Uart_Timer_Run(&RS485);
			break;
			case 2:
				Monitor_Led();
			break;
			case 3: 
				
			break;
			case 4:
				SysTimer.Count1S++;
				if(SysTimer.Count1S > 124)	//1s
				{
					SysTimer.Count1S = 0;
					//Contr.ComFault.Count++; 				
				}				
			break;
			case 5:
				if(Contr.delay)
				{
					Contr.delay --;
				}

			break;
			case 6:

			break;
			case 7:
				//Monitor_GprsLink_Close();
			break;
			
			default:
			break;
		}		

	}
}


/*********************************************************************************
* Function Name  : void Per_Second_Event_Deal(void)
* Description    : Events that need to be processed per second.
* Arguments      : None
* Return         : None
* Call Relation  : Extern
* Change Time    : 2014-11
*********************************************************************************/
void Per_Second_Event_Deal(void)
{ 
	Rt_1D0mS++;
	
	if((Rt_1D0mS %99)!=0)
	{
		  return;
	} 
	//Per_100mS_Event_Deal();
	
	switch(Rt_1D0mS/99)              //1s
	{		
		case 1:            
			break;

		case 2:
			//Per_Minute_Event_Deal();
			break;

		case 3:				
//			Monitor_Controller_Offline();
//			Monitor_Trans_Equip_Offline();
			break;
			
		case 4:	

			break;

		case 5:	

		break;

		case 6:
		break;

		case 7:			
			Get_RTC_Time(time_string);	//获取实时时钟时间
			break;	

		case 8: 
			//Monitor_System_Run_Sta();
			//UINT16 temp_adc = 0;// volt_ref_adc = 0;
			//   
			//float Vsense = 0;
			//temp (℃)= {(V25 - VSENSE) / Avg_Slope} + 25 
			//V25=VSENSE在25℃时的数值,min=1.32V,typ=1.41V,max=1.50V
			//Avg_Slope = 温度在VSENSE曲线的平均斜率（单位为mV/℃或uV/℃）,min=4.0,typ=4.3,max=4.6 mV/℃
			
			//volt_ref_adc = T_Get_Volt();//内部参考电压采样：min=1.16V,typ=1.2V,max=1.26V
			//temp_adc = T_Get_Temp();
			//Vsense = (float)temp_adc*1.2/volt_ref_adc;//((float)temp_adc*3.3/4096);
			//Vsense = (float)temp_adc*3.24/4096;

			//temp_offset = (1.41 - Vsense) / 0.0043;//偏离25℃的值，计算耗时25us
			
			//temp_offset = (AD_TEMP_25-temp_adc)*3.24/(4096*0.0043);
			//temp_offset = (AD_TEMP_25-temp_adc)*0.184;
			//temp_offset = (AD_TEMP_25-temp_adc)*3.31/(4096*0.0043);
			//temp_offset = (AD_TEMP_25-temp_adc)*0.188;
			//temp_real = ((SINT16)temp_offset + 25);
		break;	

		case 9: 
			/*static UINT8 counter_20s = 0;
			counter_20s ++;
			if(counter_20s > 19)//20秒温补一次
			{
				 counter_20s = 0;
				 RTC_Calibra();
			}*/									
//			RTC_Save_PowerOff_Time();
		break;	

		case 10: 
					
		break;

		default:
			break;
	}
	if(Rt_1D0mS >  C_TIME_1S) 								//1S	
	{
		 Rt_1D0mS = 0;
	}
}

UINT32  get_systemtick(void)
{
    return SysTimer.Tick;//1ms
}

void Monitor_Task(void)
{ 
	if(SysTimer.Flag1Ms)
	{
        SysTimer.Flag1Ms = 0;

		switch(SysTimer.Count1Ms & 0x07)
		{
			case 1:
				Monitor_Controller_Uart(&RS232);
				Monitor_Controller_Slave_Uart(&RS232);
				break;
				
			case 2:
				//Monitor_Controller_Uart(&RS485);
				//Monitor_Controller_Slave_Uart(&RS485);
			break;
			case 3:
				if(Led.rec_light_time)
				{
					Led.rec_light_time--;
					LED_REC_ON();
				}
				else
					Off_Led_REC();
				if(Led.trs_light_time)
				{
					Led.trs_light_time--;
					LED_SEND_ON();
				}
				else
					Off_Led_SEND();
				break;
			default:
				break;
		}
   
        Per_Second_Event_Deal();
	}    
}




