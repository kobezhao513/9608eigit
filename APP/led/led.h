

#ifndef _LED_H_
#define _LED_H_

#include "global.h"

#define	LED_ERROR_OFF()  	GPIOB->BSRR = GPIO_Pin_13
#define	LED_ERROR_ON()  	GPIOB->BRR = GPIO_Pin_13


#define	LED_SEND_OFF()  	GPIOA->BSRR = GPIO_Pin_8 	
#define	LED_SEND_ON()  	GPIOA->BRR = GPIO_Pin_8 

#define	LED_REC_OFF()  	GPIOA->BSRR = GPIO_Pin_9
#define	LED_REC_ON()  	GPIOA->BRR = GPIO_Pin_9


#define CONTOR_LIGHE_TIME	5

typedef struct
{
	UINT8 rec_light_time;
	UINT8 trs_light_time;
}LED;

//typedef struct
//{
//	UINT8 gprs_light_time;
//	UINT8 contor_light_time;
//}LED;


void Led_Init(void);
void Monitor_Led(void);  //亮灯时间计时
void Off_Led_Error(void);
void Off_Led_SEND(void);
void Off_Led_Contor(void);
void On_Led_Error(void);
void On_Led_SEND(void);
void On_Led_Contor(void);
void Off_Led_REC(void);
void On_Led_REC(void);



#endif
