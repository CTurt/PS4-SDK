/*
             LUFA Library
     Copyright (C) Dean Camera, 2009.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2009  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, and distribute this software
  and its documentation for any purpose and without fee is hereby
  granted, provided that the above copyright notice appear in all
  copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  This file contains macros which are common to all library elements, and which may be useful in user code.
 */
 
/** @defgroup Group_Common Common Utility Headers - LUFA/Drivers/Common/Common.h
 *
 *  Common utility headers containing macros, functions, enums and types which are common to all
 *  aspects of the library.
 *
 *  @{
 */

/** @defgroup Group_Debugging Debugging Macros
 *
 *  Macros for debugging use.
 */
 
/** @defgroup Group_BitManip Endian and Bit Macros
 *
 *  Functions for swapping endianness and reversing bit orders.
 */

#ifndef __COMMON_H__
#define __COMMON_H__

/* Inline Functions: */
/** Function to reverse the individual bits in a byte - i.e. bit 7 is moved to bit 0, bit 6 to bit 1,
 *  etc.
 *
 *  \ingroup Group_BitManip
 *
 *  \param[in] Byte   Byte of data whose bits are to be reversed
 */
static inline uint8_t BitReverse(uint8_t Byte);
static inline uint8_t BitReverse(uint8_t Byte)
{
	Byte = (((Byte & 0xF0) >> 4) | ((Byte & 0x0F) << 4));
	Byte = (((Byte & 0xCC) >> 2) | ((Byte & 0x33) << 2));
	Byte = (((Byte & 0xAA) >> 1) | ((Byte & 0x55) << 1));

	return Byte;
}

/** Function to reverse the byte ordering of the individual bytes in a 16 bit number.
 *
 *  \ingroup Group_BitManip
 *
 *  \param[in] Word   Word of data whose bytes are to be swapped
 */
static inline uint16_t SwapEndian_16(uint16_t Word) ;
static inline uint16_t SwapEndian_16(uint16_t Word)
{
	return ((Word >> 8) | (Word << 8));				
}

/** Function to reverse the byte ordering of the individual bytes in a 32 bit number.
 *
 *  \ingroup Group_BitManip
 *
 *  \param[in] DWord   Double word of data whose bytes are to be swapped
 */
static inline uint32_t SwapEndian_32(uint32_t DWord) ;
static inline uint32_t SwapEndian_32(uint32_t DWord)
{
	return (((DWord & 0xFF000000) >> 24) |
	        ((DWord & 0x00FF0000) >> 8)  |
			((DWord & 0x0000FF00) << 8)  |
			((DWord & 0x000000FF) << 24));
}

/** Function to reverse the byte ordering of the individual bytes in a n byte number.
 *
 *  \ingroup Group_BitManip
 *
 *  \param[in,out] Data  Pointer to a number containing an even number of bytes to be reversed
 *  \param[in] Bytes  Length of the data in bytes
 */
static inline void SwapEndian_n(uint8_t* Data, uint8_t Bytes);
static inline void SwapEndian_n(uint8_t* Data, uint8_t Bytes)
{
	uint8_t Temp;
	
	while (Bytes)
	{
		Temp = *Data;
		*Data = *(Data + Bytes - 1);
		*(Data + Bytes) = Temp;

		Data++;
		Bytes -= 2;
	}
}

#endif