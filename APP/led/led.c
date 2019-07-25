
#include "controller.h"
#include "led.h"


LED Led;


/*=====================================================*/
//函数名称:void Init_PORT(void)
//函数功能:初始化IO口
//参		数:void
//返	回	 值:void
//更改记录:2018年1月30日谢韬
/*=====================================================*/
void  Led_Init(void)
{
	GPIO_InitTypeDef      GPIO_InitStructure;  	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	//灯IO口初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//提前置1，避免灯在上电时先闪一下
	GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_9);
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}


void Monitor_Led(void)  //亮灯时间计时
{
//	if(Led.gprs_light_time>0)
//	{
//		Led.gprs_light_time--;
//		if(Led.gprs_light_time==0)
//		Off_Led_SEND();
//	}
//	if(Led.contor_light_time>0)
//	{
//		Led.contor_light_time--;
//		if(Led.contor_light_time==0)
//		Off_Led_Contor();
//	}
}




void Off_Led_Error(void)
{
	if(!Debug.Led)
	{
		if((!Contr.ComFault.Flag)&&(!Compete_Event.ComFault.Flag))

		{
			LED_ERROR_OFF();
		}
	}
}

void On_Led_Error(void)
{
	LED_ERROR_ON();
}

void Off_Led_SEND(void)
{
	LED_SEND_OFF();
}

void On_Led_SEND(void)
{
	Led.trs_light_time = 40;//LED_SEND_ON();
}
void Off_Led_REC(void)
{
	LED_REC_OFF();
}

void On_Led_REC(void)
{
	Led.rec_light_time = 40;//LED_REC_ON();
}
void Off_Led_Contor()
{}
void On_Led_Contor()
{}





