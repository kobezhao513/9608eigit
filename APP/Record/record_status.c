
#include "record_status.h"
#include "global.h"
#include "ex_flash.h"
#include "xycnbus.h"
#include "basic.h"
#include "record_manage.h"
#include "controller.h"


void Record_Controler_If_Event(RECORD_STRUCT *record)
{
	record->Sta  = RECORD_STA_INIT; 
	record->Mode = RMODE_CONTR_UC; 

	record->Ic.Loop = IC_LOOP_CONTROL ;
	record->Ic.Dev  = IC_DEV_CONTROL ;


	Record_Save(record);

	switch (record->Type)
    {
		case RECORD_FIRE:

		
        break;

		case RECORD_FAULT:
			
        break;
		
		case RECORD_MONITER:
				
        break;

		case RECORD_SHIELD:
							
        break;

        default:
		break;
	} 
}
void Record_Sys_If_Event(RECORD_STRUCT *record)
{
    record->Sta = RECORD_STA_INIT; 
    record->Mode = RMODE_UC; 

    record->Ic.Loop = IC_LOOP_SYS ;

	if((record->Type != RECORD_ELIMINATE)
		&&(record->Type != RECORD_FAULT))
	{
		record->Ic.Dev = IC_DEV_SYS;
	}

	Record_Save(record);
}

void Record_Gprs_Modul_Fault(void)
{

	RECORD_STRUCT record;
	memset(&record.Sta, 0xFF, sizeof(record));

	record.Type = RECORD_FAULT;
	record.EventType.word = INT_TYPE_GPRS_NACK;
	Record_Sys_If_Event(&record);
	
}



