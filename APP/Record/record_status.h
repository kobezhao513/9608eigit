/*
 * record.h
 *
 *  Created on: 2012-7-31
 *      Author: Administrator
 */

#ifndef RECORD_STATUS_H_
#define RECORD_STATUS_H_

//------------------------------------- include -------------------------------------

#include "global.h"
//------------------------------------- define --------------------------------------

void Record_Controler_If_Event(RECORD_STRUCT *record);
void Record_Sys_If_Event(RECORD_STRUCT *record);
void Record_Gprs_Modul_Fault(void);



#endif

