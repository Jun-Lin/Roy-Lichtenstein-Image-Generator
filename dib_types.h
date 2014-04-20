/*==========================================================================================
# dib_types.h: converting between bitmaps
#
# Date :10th February  2013 # converting dib/BMP to support 64-bit
#
# Author   :Anargyros Sarafopoulos
===========================================================================================*/
#ifndef __DIB_TYPES_H__
#define __DIB_TYPES_H__


/*! \brief Unsigned global type definitions 
 */
typedef unsigned char  _UBYTE_;
typedef unsigned char  UBYTE;   /**< stores one byte */
typedef unsigned short UWORD;   /**< stores two bytes */
typedef unsigned int   UDWORD;  /**< stores four bytes */
typedef unsigned long  UDDWORD; /**< stores eight bytes */

/*! \brief Signed global type definitions 
 */
typedef char _SBYTE_;
typedef char  SBYTE;   /**< stores one byte */
typedef short SWORD;   /**< stores two bytes */
typedef int   SDWORD;  /**< stores four bytes */
typedef long  SDDWORD; /**< stores eight bytes */

#endif /* __DIB_TYPES_H__ */

