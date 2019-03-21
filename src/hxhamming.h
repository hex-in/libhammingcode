/*****************************************************************************************************************
 * Copyright (c) 2018-2019, Hexin (Dalian) Technology Co. Ltd All rights reserved.
 * Author  : Heyn
 * Version : V0.2
 * 
 * LICENSING TERMS:
 * -----------------
 * 
 * HISTORIC VERSION:
 * -----------------
 *          New Create at   2019/03/07 V0.2   [Heyn] Initialization.
 * 
*****************************************************************************************************************/

#ifndef __HXHAMMING_H__
#define __HXHAMMING_H__


#define HEXIN_PRINTF_DEBUG									1
#define HEXIN_HAMMING_PADDING                       		0x00    // 0x00 or 0x01
#define HEXIN_PADDING_ALIGN_BITS							4
#define HEXIN_MAX_BUFFER_SIZE								128

/***************************************************************************************
 *                     				Macro Definitions
 ***************************************************************************************/

#if defined( _WIN32 )
    #define LIBHANMING_API      __declspec(dllexport)
    #ifdef __cplusplus
        #define EXTERN_C        extern "C"
    #else
        #define EXTERN_C        extern
    #endif
#else
    #define LIBHANMING_API
    #define EXTERN_C            extern
#endif

/***************************************************************************************
 *                     				Function declaration
 ***************************************************************************************/

unsigned int hexinHammingEncode( const unsigned char *code, /* Raw data      */
									   unsigned int   size, /* Raw data size */
									   unsigned char *pbuf  /* Hanming code  */ );

		 int hexinHammingDecode( const unsigned char *code,  /* Hamming code.  	*/
									   unsigned int   size,  /* Hamming code size. */
									   unsigned char *pbuf,  /* Raw data  		*/
									   unsigned int  *length /* Raw data size	*/ );

unsigned int bitsTobytes( unsigned char *bits,
						  unsigned int  size,
						  unsigned char *bytes,
						  unsigned char compressbits );

unsigned int bytesTobits( unsigned char *bytes,
						  unsigned int  size,
						  unsigned char *bits,
						  unsigned char decompressbits );

#endif /* __HXHAMMING_H__ */
