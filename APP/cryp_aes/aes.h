

#ifndef __AES_H
#define __AES_H


#include "global.h"



#define KEY_COUNT 3   
#if KEY_COUNT == 1   
#define KEYBITS 128 //!< Use AES128. 
#elif KEY_COUNT == 2    
#define KEYBITS 192 //!< Use AES196. 
#elif KEY_COUNT == 3    
#define KEYBITS 256 //!< Use AES256. 
#else    
#define KEYBITS 256 //!< Use AES256. 
#endif

#if KEYBITS == 128  
#define ROUNDS 10 //!< Number of rounds.  
#define KEYLENGTH 16 //!< Key length in number of bytes.
#elif KEYBITS == 192   
#define ROUNDS 12 //!< Number of rounds.    
#define KEYLENGTH 24 //!< // Key length in number of bytes.
#elif KEYBITS == 256   
#define ROUNDS 14 //!< Number of rounds.   
#define KEYLENGTH 32 //!< Key length in number of bytes. 
#else    
#define ROUNDS 14 //!< Number of rounds.   
#define KEYLENGTH 32 //!< Key length in number of bytes. 
#endif



#define BPOLY     0x1b    //!< Lower 8 bits of (x^8+x^4+x^3+x+1), ie. (x^4+x^3+x+1).
#define BLOCKSIZE 16 //!< Block size in number of bytes. 

#define EXPANDED_KEY_SIZE (BLOCKSIZE * (ROUNDS+1)) //!< 176, 208 or 240 bytes. 

#define KEY_EXPAND_SIZE 240

#define CIPHERTEXT_LEN_MAX    1078  //1080-2



void SendFrame_Encrypt(UINT8 *data,UINT16 *length);
UINT8 RecFrame_Decrypt(UINT8 *data,UINT16 start_loc);


#endif


