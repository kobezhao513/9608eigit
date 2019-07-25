#include "main.h"
#include "sys_init.h"
#include "Parallel_Port.h"
#include "controller.h"
#include "timer.h"
#include "EI_Print_COMM.h"

const UINT32 app_flag __attribute__((at(ADDR_MCU_PROGROM_END-4096)))   = 0xAAAAAAAA;
const UINT32 hard_ver __attribute__((at(ADDR_MCU_PROGROM_END-4096+4)))   = HARD_V_BYTE0*256*256+HARD_V_BYTE1*256+HARD_V_BYTE2;


int main(void)
{ 
	DI();
	System_Initialize();
	EI(); 

	for(;;)
	{   
		Watch_Dog();
		Monitor_Task();
		//Parallel_Task();
		//Monitor_Task();
		Data_Deal_Task();
		Controller_Com();  		//控制器通信232 或485
	}
}
