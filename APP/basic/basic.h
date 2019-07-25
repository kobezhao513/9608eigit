
#ifndef BASIC_H_
#define BASIC_H_

#include "global.h"



#define COMPARE_SUCESS_WILDCARD        0xAA
#define COMPARE_SUCESS_NO_WILDCARD     0xAB
#define COMPARE_FAILURE    			   0x00
#define COMPARE_FAILURE_LOCAL_UC       0x57


#define max(x,y)  ( x>y?x:y )
#define min(x,y)  ( x<y?x:y )

UINT8 Hex2BCD(UINT8 a) ;
UINT8 BCD2Hex(UINT8 a);

UINT16 Uchar_To_Uint16(UINT8 msb,UINT8 lsb);

uint32 Power(uint32 x,uchar y);




uchar Uint32_DecString(uint32 hex,uchar *s,uchar len);

void Hex2_String(uchar *s,uchar *hex,uchar len);
void String_2_Hex(UINT8 *hex,UINT8 *s,UINT8 len);
UINT8 String_Hex2(uchar *hex,uchar *s,uchar len);

void Hex2_UserCode(uchar *uc,uchar *hex,uchar len);

uchar UserCode_Hex2(uchar *hex,uchar *uc,uchar len);




uchar DeviceId_Type(uchar *id);


//uint16 UC_Search_IC(uchar *rcode);

uchar UC_Search_Relate(uchar * rcode,uchar *relate);

uint16 UC_Search_PanelKey(uchar *rcode);

uchar String_Array_Len(uchar const * const str_array[],uchar const *str_last);

//void MemCpyInvert(uchar *from,uchar *to,uchar num);
void MemoryCpyInvert(UINT8 *to,UINT8 *from,UINT8 num);
void display_deal(uchar *p,uchar n);



uchar addr_comm(uchar addr_save);

uchar Check_Add_WildCard(uchar len,uchar* add);



UINT8 addr_LA_compare(UINT8 *addr_local,UINT8 *addr_comm);

UINT8 addr_UC_compare(UINT8 *addr_local,UINT8 *addr_comm);



UINT8 addr_ID_compare(UINT8 *addr_local,UINT8 *addr_comm);// all UINT32 138us,UINT16 UINT32 81.2us,now 30us


UINT8 addr_compare(UINT8 addr_mmadr_flag ,UINT8 len,UINT8 *addr_local,UINT8 *addr_comm);




void Random_Data_Cal(uchar *str);




#endif

