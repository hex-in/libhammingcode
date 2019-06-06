/*****************************************************************************************************************
 * Copyright (c) 2018-2019, Hexin (Dalian) Technology Co. Ltd All rights reserved.
 * Author  : Heyn
 * Version : V0.4
 * 
 * LICENSING TERMS:
 * -----------------
 * 
 * HISTORIC VERSION:
 * -----------------
 *          New Create at   2019/03/21 V0.2   [Heyn] Initialization.
 * 							2019/03/23 V0.3   [Heyn] New add uintTobits\bitsTouint\hexinGrayCode.
 * 							2019/03/26 V0.4   [Heyn] Bugfix: bitsTobytes_align_right(...) 4bytes alignment.
 * 							2019/06/06 V0.5	  [Heyn] Bugfixed190606: when size=17, It's will be loop forever.
 * 
*****************************************************************************************************************/

/******************************************************************************
    ::clang build windows dll method.
    ::x86:
    clang -c -o hxhamming.o hxhamming.c -target i686-pc-windows-msvc
    clang -shared -v -o libhxhammingcode.dll hxhamming.o -target i686-pc-windows-msvc
    ::x86-64:
    clang -c -o hxhamming.o hxhamming.c -target x86_64-pc-windows-gnu
    clang -shared -v -o lbhxhammingcode.dll hxhamming.o -target x86_64-pc-windows-gnu

    clang .\hxhamming.c -o hxhamming.exe -target i686-pc-windows-msvc

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "hxhamming.h"

static unsigned int __hexinPower( unsigned int n, unsigned int m )
{
	unsigned int i = 0, sum = 1;
	for ( i=1; i<=m; i++ ) {
		sum *= n;
	}
	return sum;
}

static unsigned char __hexinHammingCalculate( const unsigned char *code,
												    unsigned int  position,
												    unsigned int  totalbits )
{
    unsigned int i = 0, j = 0, count = 0;

	i = position - 1;
	while ( i < totalbits ) {
		for ( j=i; j<i+position; j++) {
			if ( code[j] == 1 ) {
				count++;
			}
		}
		i = i + (position << 1);
	}

	return ( count%2 );
}

static unsigned int __getHammingParityBitSize( unsigned int size )
{
    unsigned int paritybit = 0;

	while ( size > __hexinPower(2, paritybit) - (paritybit + 1) ) {
		paritybit++;
#if HEXIN_PRINTF_DEBUG
		printf( ">>> [ Parity  Size ]: " );
		printf( "size=%d, parity size=%d", size, paritybit );
		printf( "\r\n" );
#endif
	}
    return paritybit;
}

static unsigned int __getHammingDataBitsSize ( unsigned int size )
{
	unsigned int databits = 0, paritybit = 0;

	// Bugfixed190606: when size=17, It's will be loop forever.
	// while ( (databits + paritybit) != size ) {
	// 	paritybit = __getHammingParityBitSize( databits++ );
	// }
	do {
		databits++;
		paritybit = __getHammingParityBitSize( databits );
	} while ( (databits + paritybit) != size );

	return databits;
}

/*
 * @brief Hamming encode.
 * @param [IN]  code : Raw data (bits).
 * 		  [IN]  size : Raw data size.
 *		  [OUT] pbuf : Hamming code.
 *
 * @retval Hamming code size.
 * @see None.
 */

unsigned int hexinHammingEncode( const unsigned char *code, /* Raw data */
									   unsigned int   size, /* Raw data size */
									   unsigned char *pbuf  /* Hamming code  */ )
{
	unsigned char *ptr = pbuf;
    unsigned int  i = 0, j = 0, k = 0;
    unsigned int  paritybit = 0, totalbits = 0, position = 0;

	if ( (NULL == code) || (0 == size) || (NULL == pbuf) ) {
		return totalbits;
	}

    paritybit = __getHammingParityBitSize( size );
    totalbits = paritybit + size;

	for ( i=0; i<totalbits; i++ ) {
		if ( i == ( __hexinPower(2, k) - 1 ) ) {
			ptr[i] = 0;
			k++;
		} else {
			ptr[i] = code[j];
			j++;
		}
	}

	for ( i=0; i<paritybit; i++ ) {
		position = __hexinPower(2, i);
		ptr[position - 1] = __hexinHammingCalculate( ptr, position, totalbits );
	}

#if HEXIN_PRINTF_DEBUG
	printf( ">>> [ Hamming Code ]: " );
	for ( i=0; i<totalbits; i++ ) {
		printf( "%d", ptr[i] );
	}
	printf("\r\n");
	printf( ">>> [ Hamming Code ]: length = %3d (bits)\r\n", totalbits );
#endif

    return totalbits;
}


/*
 * @brief Hamming checked.
 * @param [IN]  code : Hamming code.
 * 		  [IN]  size : Raw data size (Without paritybit)
 *		  [OUT] pbuf : Correction Hamming code.
 *
 * @retval Error bit. ( from left to right and index form 1 to end ) 
 * @see None.
 */

static unsigned int __hexinHammingChecked ( const unsigned char *code, /* Hamming code.  */
									     		  unsigned int   size, /* Raw data size. */
									     		  unsigned char *pbuf  /* Correction Hamming code */ )
{
	unsigned char value = 0, *ptr = pbuf;
    unsigned int  i = 0, j = 0;
    unsigned int  paritybit = 0, totalbits = 0, errorbit = 0, position = 0;

	if ( (NULL == code) || (0 == size) || (NULL == pbuf) ) {
		return totalbits;
	}

    paritybit = __getHammingParityBitSize( size );
    totalbits = paritybit + size;

#if HEXIN_PRINTF_DEBUG
	printf( ">>> [ Hamming Code ]: " );
	for ( i=0; i<totalbits; i++ ) {
		printf( "%d", code[i] );
	}
	printf("\r\n");
	printf( ">>> [ Hamming Code ]: length = %3d (bits)\r\n", totalbits );
#endif

    for ( i=0; i<paritybit; i++ ) {
		position = __hexinPower( 2, i );
		value = __hexinHammingCalculate( code, position, totalbits );
		if ( value != 0 ) {
			errorbit += position;
            for ( j=0; j<totalbits; j++ ) {
                if ( j == ( errorbit - 1 ) ) {
                    ptr[j] = ( (code[j] == 0) ? 1 : 0 );
                } else {
                    ptr[j] = code[j];
                }
            }
        }
	}

    return errorbit;
}


/*
 * @brief Hamming decode.
 * @param [IN]  code   : Hamming code.
 *		  [IN]  size   : Hamming code size.
 *        [OUT] pbuf   : Raw data.
 *		  [OUT] length : Raw data size.
 *
 * @retval Error bit. ( from left to right and index form 1 to end ).
 * @see None.
 */

int hexinHammingDecode( const unsigned char *code,  /* Hamming code.  	*/
							  unsigned int   size,  /* Hamming code size. */
							  unsigned char *pbuf,  /* Raw data  		*/
							  unsigned int  *length /* Raw data size	*/ )
{
	unsigned char *ptr = pbuf;
    unsigned int  i = 0, j = 0, k = 0;
	unsigned char buffer[HEXIN_MAX_BUFFER_SIZE] = { 0x00 };
    unsigned int  databits = 0, totalbits = size, errorbit = 0;

	if ( (NULL == code) || (0 == size) || (NULL == pbuf) || (NULL == length) ) {
		return -1;
	}
	if ( (totalbits > HEXIN_MAX_BUFFER_SIZE) ) {
		return -2;
	}

	databits = __getHammingDataBitsSize( totalbits );
	errorbit = __hexinHammingChecked( code, databits, buffer );

	for ( i=0; i<totalbits; i++ ) {
		if ( i == ( __hexinPower(2, k) - 1 ) ) {
			k++;
		} else {
			if ( errorbit == 0 ) {
				ptr[j++] = code[i];
			} else {
				ptr[j++] = buffer[i];
			}
		}
	}
    *length = j;

#if HEXIN_PRINTF_DEBUG
	printf( ">>> [   Raw   Code ]: " );
	for ( i=0; i<*length; i++ ) {
		printf( "%d", ptr[i] );
	}
	printf("\r\n");
	printf( ">>> [   Raw   Code ]: length = %3d (bits)\r\n", databits );
#endif

    return errorbit;
}


/*
 * @brief Padding the Hamming code to pbuf.
 * @param [IN]  bits  : Hamming code.
 * 		  [IN]  bsize : Hamming code length.
 *		  [OUT] pbuf  : Filled Hamming code.
 *		  [IN]  bsize : pbuf length.
 *
 * @retval New Hamming code length.
 * @Note None.
 */

static unsigned int __hexinHammingPadding( unsigned char *bits, unsigned int bsize,
								  		   unsigned char *pbuf, unsigned int psize )
{
	unsigned int i = 0;
	unsigned char *ptr  = pbuf;
	unsigned int offset = ( bsize % HEXIN_PADDING_ALIGN_BITS == 0 ? \
						  0 : ( HEXIN_PADDING_ALIGN_BITS - bsize%HEXIN_PADDING_ALIGN_BITS ) );

	if ( (NULL == bits) || (0 == bsize) || (NULL == pbuf) || (0 == psize) || (psize < (bsize + offset)) ) {
		return 0;
	}

	for ( i=0; i<offset; i++ ) {
		*ptr++ = HEXIN_HAMMING_PADDING;
	}

	for ( i=0; i<bsize; i++ ) {
		*ptr++ = bits[i];
	}

	return ( bsize + offset );
}

/*
 * @brief Bits to bytes (Alignment for right.).
 * @param [IN]  bits  : Array of bits.
 * 		  [IN]  size  : Array of bits size
 *		  [OUT] bytes : Compressed (compressbits) bits to bytes.
 *		  [IN]  compressbits : 2 <= compressbits <= 8
 *
 * @retval Array of bytes size.
 * @Note The size must be greater than a multiple of 8 of the actual size.
 */

unsigned int bitsTobytes_align_right( unsigned char *bits,
									  unsigned int  size,
									  unsigned char *bytes,
									  unsigned char compressbits )
{
    int  i = 0, j = 0;
	unsigned int  length = 0;
	unsigned char *ptr = bytes;

	if ( (NULL == bits) || (0 == size) || (NULL == bytes) ) {
		return length;
	}

    length = ( size%compressbits == 0 ? size/compressbits : size/compressbits + 1 );
	ptr    = ( bytes + length - 1);
	for ( i=(size - 1); i>=compressbits; i = (i - compressbits) ) {		// Bugfix: No.20190326
		*ptr = 0x00;
		for ( j=0; j<compressbits; j++ ) {
			*ptr |= bits[i-j] << j;
		}
		ptr--;
	}

	*ptr = 0x00;
	for ( j=0; j<compressbits; j++, i-- ) {
		if ( i < 0 ) {
			*ptr |= HEXIN_HAMMING_PADDING << j;
		} else {
			*ptr |= bits[i] << j;
		}
	}
	ptr--;

#if HEXIN_PRINTF_DEBUG
	printf( ">>> [ CompressByte ]: " );
	for ( i=0; i<length; i++ ) {
		printf( "%02X ", bytes[i] );
	}
	printf("\r\n");
	printf( ">>> [ CompressByte ]: length = %3d (bytes)\r\n", length );
#endif

	return length;
}


/*
 * @brief Bits to bytes (Alignment for left.).
 * @param [IN]  bits  : Array of bits.
 * 		  [IN]  size  : Array of bits size
 *		  [OUT] bytes : Compressed (compressbits) bits to bytes.
 *		  [IN]  compressbits : 2 <= compressbits <= 8
 *
 * @retval Array of bytes size.
 * @Note The size must be greater than a multiple of 8 of the actual size.
 */

unsigned int bitsTobytes_align_left( unsigned char *bits,
									 unsigned int  size,
									 unsigned char *bytes,
									 unsigned char compressbits )
{
    unsigned int  i = 0, j = 0;
	unsigned int  length = 0;
	unsigned char *ptr = bytes;

	if ( (NULL == bits) || (0 == size) || (NULL == bytes) ) {
		return length;
	}

    length = ( size%compressbits == 0 ? size/compressbits : size/compressbits + 1 );
	for ( i=0; i<length; i++ ) {
		*ptr = 0x00;
		for ( j=0; j<compressbits; j++ ) {
			if ( (compressbits*i)+j >= size ) {
				*ptr |= HEXIN_HAMMING_PADDING << (compressbits-1-j);
			} else {
				*ptr |= bits[(compressbits*i)+j] << (compressbits-1-j);
			}
		}		
		ptr++;
	}

#if HEXIN_PRINTF_DEBUG
	printf( ">>> [ CompressByte ]: " );
	for ( i=0; i<length; i++ ) {
		printf( "%02X ", bytes[i] );
	}
	printf("\r\n");
	printf( ">>> [ CompressByte ]: length = %3d (bytes)\r\n", length );
#endif

	return length;
}

/*
 * @brief Bytes to bits.
 * @param [IN]  bytes : Array of bytes.
 * 		  [IN]  size  : Array of bytes size
 *		  [OUT] bits  : Decompressed (decompressbits) bytes to bits.
 *		  [IN]  decompressbits : 2 <= compressbits <= 8
 *
 * @retval Array of bits size.
 * @Note None.
 * 
 */

unsigned int bytesTobits( unsigned char *bytes,	/* Array of bytes 		*/
						  unsigned int  size,	/* Array of bytes size 	*/
						  unsigned char *bits,
						  unsigned char decompressbits )
{
	unsigned char *ptr = bits;
	unsigned int i = 0, j = 0, length = size*decompressbits;
	unsigned int mark = (0x01 << (decompressbits - 1));

	if ( (NULL == bits) || (0 == size) || (NULL == bytes) ) {
		return 0;
	}

	for ( i=0; i<size; i++ ) {
		for ( j=0; j<decompressbits; j++ ) {
			*ptr++ = ((bytes[i] & (mark >> j)) > 0 ? 1 : 0);
		}
	}

#if HEXIN_PRINTF_DEBUG
	printf( ">>> [DecompressByte]: " );
	for ( i=0; i<length; i++ ) {
		printf( "%02X ", bits[i] );
	}
	printf("\r\n");
	printf( ">>> [DecompressByte]: length = %3d (bits)\r\n", length );
#endif

	return length;
}

unsigned int uintTobits( unsigned int data, unsigned int bitsize, unsigned char *bits )
{
	unsigned int i = 0, length = 0;
	unsigned char *ptr = bits;
	unsigned int imax = sizeof(unsigned int)*8;
	unsigned int mark = ( 0x01 << (imax - 1) );

	if ( (0 == bitsize) || (NULL == bits) ) {
		return 0;
	}

	for ( i=( imax - ( bitsize > imax ? imax : bitsize ) ); i<imax; i++ ) {
		*ptr++ = ( ( data & ( mark >> i ) ) > 0 ? 1 : 0 );
		length++;
	}

	return length;		
}

unsigned int bitsTouint( unsigned char *bits, unsigned int bitsize )
{
	unsigned int i = 0, number = 0;

	for ( i=0; i<bitsize; i++ ) {
		if ( bits[i] == 0x01 ) {
			number += __hexinPower( 2, bitsize-i-1 );
		}
	}

	return number;
}

unsigned int hexinGrayCode( unsigned int nbits, unsigned char *pbuf )
{
	unsigned int i = 0;

	if ( (0 == nbits) || (NULL == pbuf) || (nbits > 8) ) {
		return 0;
	}

	for ( i=0; i<( unsigned int )( 1 << nbits ); i++){
		pbuf[i] = ( i^( i>> 1) );
	}
	return i;
}