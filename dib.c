/*==========================================================================================
# dib.c : interface for Windows UNCOMPRESSED 24-32-bit bitmaps *.BMP(s)
# Author : Anargyros Sarafopoulos
# Date   : 30 December 1998 
#        : 17 June     2001, added 32-bit support
#        : 10 July     2008, added 8-bit indexed BMP output with dib_Make8, fixed dib_Write
===========================================================================================*/

/*
 * include files
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "dib.h"

/*
 * MACROS
 */

#define LSB_FIRST 0  /* Least significan byte at lowest address, little-endian */
#define MSB_FIRST 1  /* Most significan byte at lowest address, big-endian */

/* constants for the biCompression field */
#ifndef BI_RGB
#define BI_RGB        0ul
#endif 

#ifndef BI_RLE8
#define BI_RLE8       1ul
#endif 

#ifndef BI_RLE4
#define BI_RLE4       2ul
#endif 

#ifndef BI_BITFIELDS
#define BI_BITFIELDS  3ul
#endif 

/* 
 * This is an enumeation of the size in bytes of 
 * BMP file-header struct, the size in bytes of 
 * different versions of BMP info-header struct, 
 * followed by the size in bytes of the bitmap-
 * masks struct and pallete colour entry 
 * struct (all defined later in this file)
 */
#define SIZEOF_BITMAPFILEHEADER      14u
#define SIZEOF_BITMAPINFOHEADER      40u
#define SIZEOF_BITMAPINFOHEADER_V3   40u
#define SIZEOF_BITMAPINFOHEADER_V4   108u
#define SIZEOF_BITMAPINFOHEADER_V5   124u
#define SIZEOF_BITMASKS              16u
#define SIZEOF_RGBQUAD               4u

/* 
 * windows' BMP file-header 
 */
typedef struct bmfh 
{ /* bmfh */
   UWORD bfType;      /* File type, always 4D42h ("BM") */
   UDWORD bfSize;     /* Size of the file in bytes */
   UWORD bfReserved1; /* Always 0 */
   UWORD bfReserved2; /* Always 0 */
   UDWORD bfOffBits;  /* Starting position of image data in bytes */
}bitmapfileheader, *pbitmapfileheader;


/*
 * windows' BMP information-header or info-header 
 *
 * This struct is never actually used in this file,
 * it is inlcuded to show the layout of the BMP info-
 * header.
 * Here, we allways read the info-header from a file 
 * as an array of UBYTES and every time we need to 
 * access a data field of the information-header we 
 * use the info-header marcros which follow after 
 * this definition.
 */
typedef struct bmih 
{ /* bmih */ 
   UDWORD biSize;         /* Size of this header in bytes */ 
   SDWORD biWidth;        /* Image width in pixels */
   SDWORD biHeight;       /* Image height in pixels */ 
   UWORD  biPlanes;       /* Number of color planes, which should always be 1 */
   UWORD  biBitCount;     /* Number of bits per pixel */
   UDWORD biCompression;  /* Compression methods used */ 
   UDWORD biSizeImage;    /* Size of bitmap in bytes */
   SDWORD biXPelsPerMeter;/* Horizontal resolution in pixels per meter */ 
   SDWORD biYPelsPerMeter;/* Verrical resolution in pixels per meter */ 
   UDWORD biClrUsed;      /* Number of colours in the colour look-up table */ 
   UDWORD biClrImportant; /* Minimum number of important colours in the colour look-up table */ 
   /* Fields added for Windows v4.x follow this line */

   UDWORD redMask;       /* Mask identifying bits of red component */
   UDWORD greenMask;     /* Mask identifying bits of green component */
   UDWORD blueMask;      /* Mask identifying bits of blue component */
   UDWORD alphaMask;     /* Mask identifying bits of alpha component */
   UDWORD csType;        /* Color space type */
   SDWORD redX;          /* X coordinate of red endpoint */
   SDWORD redY;          /* Y coordinate of red endpoint */
   SDWORD redZ;          /* Z coordinate of red endpoint */
   SDWORD greenX;        /* X coordinate of green endpoint */
   SDWORD greenY;        /* Y coordinate of green endpoint */
   SDWORD greenZ;        /* Z coordinate of green endpoint */
   SDWORD blueX;         /* X coordinate of blue endpoint */
   SDWORD blueY;         /* Y coordinate of blue endpoint */
   SDWORD blueZ;         /* Z coordinate of blue endpoint */
   UDWORD gammaRed;      /* Gamma red coordinate scale value */
   UDWORD gammaGreen;    /* Gamma green coordinate scale value */
   UDWORD gammaBlue;     /* Gamma blue coordinate scale value */
   /* Fields added for Windows v5.x follow this line */

   UDWORD intent;        /* Rendering intent for bitmap */
   UDWORD profileData;   /* Offset, in bytes, to the start of the profile data */
   UDWORD profileSize;   /* Size, in bytes, of embedded profile data */
   UDWORD reserved;      /* Reserved, its value should be 0 */
}bitmapinfoheader, *pbitmapinfoheader;

/* byte offsets of data in bimapinfoheader structure */
#define _HSIZE_OFFSET     0x0000u
#define _WIDTH_OFFSET     0x0004u
#define _HEIGHT_OFFSET    0x0008u
#define _PLANES_OFFSET    0x000cu
#define _BITCOUNT_OFFSET  0x000eu
#define _COMPRESS_OFFSET  0x0010u
#define _SIZEIMAGE_OFFSET 0x0014u
#define _XPELSM_OFFSET    0x0018u
#define _YPELSM_OFFSET    0x001cu
#define _CLRUSED_OFFSET   0x0020u
#define _CLRIMPR_OFFSET   0x0024u
#define _REDMASK_OFFSET   0x0028u
#define _GREENMASK_OFFSET 0x002cu
#define _BLUEMASK_OFFSET  0x0030u
#define _ALPHAMASK_OFFSET 0x0034u
#define _COLOURSPA_OFFSET 0x0038u
#define _REDX_OFFSET      0x003cu
#define _REDY_OFFSET      0x0040u
#define _REDZ_OFFSET      0x0044u
#define _GREENX_OFFSET    0x0048u
#define _GREENY_OFFSET    0x004cu
#define _GREENZ_OFFSET    0x0050u
#define _BLUEX_OFFSET     0x0054u
#define _BLUEY_OFFSET     0x0058u
#define _BLUEZ_OFFSET     0x005cu
#define _GAMMAR_OFFSET    0x0060u
#define _GAMMAG_OFFSET    0x0064u
#define _GAMMAB_OFFSET    0x0068u
#define _INTENT_OFFSET    0x006cu
#define _PROFILED_OFFSET  0x0070u
#define _PROFILES_OFFSET  0x0074u
#define _RESERVED_OFFSET  0x0078u

/*
 *  macros to access the fields in bitmapinfoheader struct
 *  field_value = macro(pbitmapInfo)
 */
#define BI_HSIZE(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _HSIZE_OFFSET)))

#define BI_WIDTH(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _WIDTH_OFFSET)))

#define BI_HEIGHT(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _HEIGHT_OFFSET)))

#define BI_PLANES(pBI) \
(*((UWORD*)((UBYTE *)(pBI) + _PLANES_OFFSET)))

#define BI_BITCOUNT(pBI) \
(*((UWORD*)((UBYTE *)(pBI) + _BITCOUNT_OFFSET)))

#define BI_COMPRESS(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _COMPRESS_OFFSET)))

#define BI_XPELSM(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _XPELSM_OFFSET)))

#define BI_YPELSM(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _YPELSM_OFFSET)))

#define BI_CLRUSED(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _CLRUSED_OFFSET)))

#define BI_REDMASK(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _REDMASK_OFFSET)))

#define BI_GREENMASK(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _GREENMASK_OFFSET)))

#define BI_BLUEMASK(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _BLUEMASK_OFFSET)))

#define BI_ALPHAMASK(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _ALPHAMASK_OFFSET)))

#define BI_ALPHAMASK(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _ALPHAMASK_OFFSET)))

#define BI_COLOURSPA(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _COLOURSPA_OFFSET)))

#define BI_PROFILES(pBI) \
(*((UDWORD*)((UBYTE *)(pBI) + _PROFILES_OFFSET)))

/* Number of colours in the colour table when bit-count is < 16 */
#define BI_COLORS(pBI) \
((BI_CLRUSED(pBI) != 0u) ?  (BI_CLRUSED(pBI)) : \
(1u<<(BI_BITCOUNT(pBI))))

/* byte counts ? */
#define ONE_BYTES   1
#define TWO_BYTES   2
#define FOUR_BYTES  4

/* 
 * colour quad which is used to strore
 * BMP image colour palette entries
 */
typedef struct rgbq 
{ /* rgbq */
   UBYTE   rgbBlue;
   UBYTE   rgbGreen;
   UBYTE   rgbRed;
   UBYTE   rgbReserved;
}rgbquad, *prgbquad;

/*
 * bit-masks used to identify the RGB components
 * in 16-bit and 32-bit pixel-depth BMP images
 */
typedef struct bitm
{ /* bitm */
   UDWORD redMask;       /* Mask identifying bits of red component */
   UDWORD greenMask;     /* Mask identifying bits of green component */
   UDWORD blueMask;      /* Mask identifying bits of blue component */
}bitmasks, *pbitmasks;

/* 
 * stores the the data read from a BMP 
 * file, apart from the file-header data
 */
typedef struct bmi 
{ /* bmi */   
   UBYTE *bmiHeader;   /* pointer to the DIB data read from file */
   UDWORD dib_bisize;  /* offset in bytes to the actual image data */  
}bitmapinfo;

/* colour space enumeration BMP V4 */
typedef enum
{
   CALIBRATED_RGB = 0x00000000,
   sRGB = 0x73524742,               /* is an encoding of the ASCII characters "sRGB" */
   WINDOWS_COLOR_SPACE = 0x57696E20 /* is an encoding of the ASCII characters "Win " */
} colourSpace;

/* colour space enumeration BMP V5 */
typedef enum 
{
   PROFILE_LINKED = 0x4C494E4B, /* consists of the string "LINK" from the Windows character set (code page 1252) */
   PROFILE_EMBEDDED = 0x4D424544 /* consists of the string "MBED" from the Windows character set */
} logicalColourSpace;

/*
 * Static Fuction Prototypes 
 */

/* static pdib dib_Copy(pdib dest, pdib source); to do */
static pdib dib_load_8bit(pdib pDIB);
static pdib dib_load_24bit(pdib pDIB);
static pdib dib_load_32bit(pdib pDIB);

static SDWORD CheckByteOrder(void);
static UWORD SwapBytesUWORD(UWORD w);
static UDWORD SwapBytesUDWORD(UDWORD dw);
static void ConvertUWORD(UWORD *array, SDWORD length);
static void ConvertUDWORD(UDWORD *array, SDWORD length); 

/*
 * static global variables
 *
 * The dib_Make/8/24/32 routines assume by default
 * Red-Green-Blue channel ordering of input colour-
 * table data and pixel data. However, DIB files use 
 * Blue-Green-Red order to store colour channels. 
 * For that reason colour channel order is reversed 
 * when storing the data into a BMP file. This is the 
 * default behaviour of the dib_Make/8/24/32 routines.
 *
 * In the case of dib_Write, however, the data we use to
 * create BMP files come directly from DIB formated arrays.
 * Which means the colour channel order is correct and 
 * does have to be reversed when storing the data into
 * a file. To specify this and avoid the default behaviour
 * of reversing the colour-channel order, we set the 
 * 'rgb_order' global to 0, using the RGB_REVERSE macro.
 * Once we finished storing BGR data into a file we need 
 * to restore the 'rgb_order' to its default value (!=0)
 * by invoking the RGB_REVERSE() once more (look at the 
 * implementation of the dib_Write routine).
 *
 */

static SDWORD rgb_order = 1; /* 1 (or != 0) is the default value, it means 
                             colour channel data passed to dib_Make/8/24/32 
                             routines comes in RGB order. A value of zero (0) 
                             signifies the colour channel order is reversed
                             that is, colour data come in a BGR byte format. */

#define RGB_ORDER()     (rgb_order)
#define RGB_REVERSE()   ((rgb_order) = !(rgb_order))

/*
 *
 * Calculate Starting position of image data in bytes.
 * 
 * This will return the correct starting position of image data only 
 * when the BMP bitmap is packed.
 *
 * If the BMP bitmap is packed the return value of the fucntion should 
 * agree with the value of 'bfOffBits' of the 'bitmapfileheader'.
 *
 * A bitmap is packed when the bitmap array follows immediately after 
 * the 'bitmapinfoheader' and colour table entries. Packed bitmaps are 
 * referenced by a single pointer. 
 * 
 */
#define PRINT_PACKED_WARNING 1
static UDWORD dib_biSize(pdib pDIB)
{
   UDWORD dib_size = pDIB->dib_bisize;
   UBYTE *buffer   = pDIB->bmiHeader;
   /* 
    * when we have a version-3 BMP 
    */
   if (BI_HSIZE(buffer) == SIZEOF_BITMAPINFOHEADER_V3)
   {
      if (BI_BITCOUNT(buffer) <= 8u) /* 8-bit pixel depth */
      {
         UDWORD size = BI_HSIZE(buffer) + BI_COLORS(buffer) * SIZEOF_RGBQUAD;
         if (size != dib_size && PRINT_PACKED_WARNING)
         {
            fprintf(stderr, "Warning in dib_biSize(pdib): this is not a packed BMP file\n");
         }
         return dib_size;
      }
      else 
      {
         UDWORD colour_palette_size = BI_CLRUSED(buffer) * SIZEOF_RGBQUAD;
         UDWORD size;
         /* in case of 16-bit or 32-bit pixel depth */
         if (BI_BITCOUNT(buffer) == 16 || BI_BITCOUNT(buffer) == 32)
         {
            if (BI_COMPRESS(buffer) == BI_BITFIELDS) /* and bit-fields */ 
            {
               size = BI_HSIZE(buffer) + SIZEOF_BITMASKS + colour_palette_size;
               if (size != dib_size && PRINT_PACKED_WARNING)
               {
                  fprintf(stderr, "Warning in dib_biSize(pdib): this is not a packed BMP file\n");
               }
               return dib_size;
            }
         }
         size = BI_HSIZE(buffer) + colour_palette_size;
         if (size != dib_size && PRINT_PACKED_WARNING)
         {
            fprintf(stderr, "Warning in dib_biSize(pdib): this is not a packed BMP file\n");
         }
         return dib_size;
      }
   }
   else /*if (BI_HSIZE(buffer) > SIZEOF_BITMAPINFOHEADER_V3  ||
              BI_HSIZE(buffer) == SIZEOF_BITMAPINFOHEADER_V4 || 
              BI_HSIZE(buffer) >= SIZEOF_BITMAPINFOHEADER_V5) */
   {
      if (BI_BITCOUNT(buffer) <= 8u) /* 8-bit pixel depth */
      {
         UDWORD size = BI_HSIZE(buffer) + BI_COLORS(buffer) * SIZEOF_RGBQUAD;
         if (size != dib_size && PRINT_PACKED_WARNING)
         {
            fprintf(stderr, "Warning in dib_biSize(pdib): this is not a packed BMP file\n");
         }
         return dib_size;
      }
      else 
      {
         UDWORD colour_palette_size = BI_CLRUSED(buffer) * SIZEOF_RGBQUAD;
         UDWORD size = BI_HSIZE(buffer) + colour_palette_size;
         if (size != dib_size && PRINT_PACKED_WARNING)
         {
            fprintf(stderr, "Warning in dib_biSize(pdib): this is not a packed BMP file\n");
         }
         return dib_size;
      }
   }
}

/* 
 * return the offset in bytes to the BMP colour-map,
 * which, if it exists, has to be a value greater than 
 * zero; if there is no colour-map return zero
 */
static UDWORD dib_colourMap(pdib pDIB)
{
   /* UDWORD dib_size = pDIB->dib_bisize; */
   UBYTE *buffer   = pDIB->bmiHeader;
   /* 
    * when we have a version-3 BMP 
    */
   if (BI_HSIZE(buffer) == SIZEOF_BITMAPINFOHEADER_V3)
   {
      if (BI_BITCOUNT(buffer) <= 8u) /* 8-bit pixel depth */
      {
         return BI_HSIZE(buffer);
      }
      else 
      {
         if (BI_CLRUSED(buffer) == 0)
         {
            return 0;
         }

         /* in case of 16-bit or 32-bit pixel depth */
         if (BI_BITCOUNT(buffer) == 16 || BI_BITCOUNT(buffer) == 32)
         {
            if (BI_COMPRESS(buffer) == BI_BITFIELDS) /* and bit-fields */ 
            {
               return BI_HSIZE(buffer) + SIZEOF_BITMASKS;
            }
         }
         return BI_HSIZE(buffer);
      }
   }
   else /*if (BI_HSIZE(buffer) > SIZEOF_BITMAPINFOHEADER_V3) ||
              BI_HSIZE(buffer) == SIZEOF_BITMAPINFOHEADER_V4 || 
              BI_HSIZE(buffer) >= SIZEOF_BITMAPINFOHEADER_V5) */
   {
      if (BI_BITCOUNT(buffer) <= 8u) /* 8-bit pixel depth */
      {
         return BI_HSIZE(buffer);
      }
      else 
      {
         if (BI_CLRUSED(buffer) == 0)
         {
            return 0;
         }
         return BI_HSIZE(buffer);
      }
   }
}


/*
 * Public query fuctions
 */
SDWORD dib_Width (pdib bmp) {
   return BI_WIDTH(bmp->bmiHeader);
}
SDWORD dib_Height(pdib bmp) {
   return BI_HEIGHT(bmp->bmiHeader); 
}
UWORD dib_Bitcount(pdib bmp) {
   return BI_BITCOUNT(bmp->bmiHeader); 
}
UBYTE* dib_Image(pdib bmp) {
   return (UBYTE *) ((bmp->bmiHeader) + dib_biSize(bmp));
}

/*
 * DIB_Destroy :
 */
void dib_Destroy(pdib bmp)
{
   if (bmp!=NULL) 
   {
      if (bmp->bmiHeader != NULL)
      {
         /* free image data */
         free(bmp->bmiHeader); 
      }
   }
   /* free the dib structure*/
   free(bmp);
}

/*
 * dib_StorageWidth
 * calculates the width in bytes of a bmp scanline
 * bmp's scanlines end on LONG data type boundary
 */
SDWORD dib_StorageWidth(pdib pDIB)
{
   SDWORD byteWidth=0;
   switch(BI_BITCOUNT(pDIB->bmiHeader))
   {
   case 8:
      byteWidth = (BI_WIDTH(pDIB->bmiHeader) + 3) & ~3;
      break;
   case 24:
      byteWidth = ((3*BI_WIDTH(pDIB->bmiHeader)) + 3) & ~3;
      break;
   case 32:
      byteWidth = 4*BI_WIDTH(pDIB->bmiHeader);
      break;
   default:
      fprintf(stderr, "Error in dib_StorageWidth: bitcount of %d\n",
              BI_BITCOUNT(pDIB->bmiHeader));
      exit(1);
   }

   return (byteWidth);
}

/*
 * dib_Load 
 */
pdib dib_Load(const SBYTE *filename)
{
   SDWORD byteorder = CheckByteOrder();
   bitmapfileheader dib_file_header; /* DIB file header */
   UBYTE *file_buffer= NULL;         /* stores the location of DIB read from file in memory */
   UDWORD storage = 0;               /* number of bytes of the DIB data read from file */
   pdib dib_buffer = NULL;           /* pointer to bitmapinfo */

   /* open bitmap file */
   FILE *fp = fopen( filename, "rb" );
   if ( fp == NULL ) 
   {
      fprintf(stderr, "Error in dib_Load(SBYTE*):cannot open bitmap file: %s\n", filename);
      /* return ZERO */
      return NULL;
   }

   /* read bitmap file header */
   fread(&(dib_file_header.bfType),       sizeof(dib_file_header.bfType), 1, fp);
   fread(&(dib_file_header.bfSize ),      sizeof(dib_file_header.bfSize), 1, fp);
   fread(&(dib_file_header.bfReserved1),  sizeof(dib_file_header.bfReserved1), 1, fp);
   fread(&(dib_file_header.bfReserved2),  sizeof(dib_file_header.bfReserved2), 1, fp);
   fread(&(dib_file_header.bfOffBits),    sizeof(dib_file_header.bfOffBits), 1, fp);

   if (byteorder == MSB_FIRST) 
   {
      dib_file_header.bfType = SwapBytesUWORD(dib_file_header.bfType);
      dib_file_header.bfSize = SwapBytesUDWORD(dib_file_header.bfSize);
      dib_file_header.bfReserved1 = SwapBytesUWORD(dib_file_header.bfReserved1);
      dib_file_header.bfReserved2 = SwapBytesUWORD(dib_file_header.bfReserved2);
      dib_file_header.bfOffBits = SwapBytesUDWORD(dib_file_header.bfOffBits);
   }

   if (dib_file_header.bfType != 0X4D42u)  /* it has to be equal to 'BM' encoded in ascii */
   {
      fprintf(stderr, "Error in dib_Load(SBYTE*) : %s is not a BMP bitmap file\n", filename);
      /* close bitmap file */
      fclose(fp);
      /* return ZERO */
      return NULL;
   }

   storage = dib_file_header.bfSize - SIZEOF_BITMAPFILEHEADER;
   if (storage <= 0)  /* negative or zero bitmap size! */
   {
      fprintf(stderr, "Error in dib_Load(SBYTE*) : %s bitmap size is %d - bytes\n", filename, storage);
      /* close bitmap file */
      fclose(fp);
      /* return ZERO */
      return NULL;
   }

   /* allocate a buffer to hold the DIB in memory */
   file_buffer  = (UBYTE*) malloc(storage);
   if (file_buffer == NULL) 
   {
      fprintf(stderr, "Error in dib_Load(SBYTE*) :cannot load file: %s out of memory\n", filename);
      /* close bitmap file */
      fclose(fp);
      /* return ZERO */
      return NULL;
   }

   /* prepare to open and read the BMP file */
   if (!fread(file_buffer, storage, 1, fp)) 
   {
      fprintf(stderr, "Error in dib_Load(SBYTE*):cannot read bitmap file: %s\n", filename);
      /* close bitmap file */
      fclose(fp);
      /* free up alocated memory */
      free(file_buffer);
      /* return ZERO */
      return NULL;
   }

   /* Read the size INFO-header to figure out the version of the BMP */
   if (BI_HSIZE(file_buffer) != SIZEOF_BITMAPINFOHEADER_V3 && 
       BI_HSIZE(file_buffer) != SIZEOF_BITMAPINFOHEADER_V4 &&
       BI_HSIZE(file_buffer) != SIZEOF_BITMAPINFOHEADER_V5)
   {
      fprintf(stderr, "Warning in dib(Load(SBYTE*):BMP INFO-header size is %d long, which is not a standard BMP-V3/4/5\n", BI_HSIZE(file_buffer));
   }

   if (byteorder == MSB_FIRST) 
   {
      if (BI_HSIZE(file_buffer) == SIZEOF_BITMAPINFOHEADER_V4)
      {
         ConvertUDWORD(((UDWORD*) (file_buffer +  0)), 3);
         ConvertUWORD(((UWORD*) (file_buffer + 12)), 2);
         ConvertUDWORD(((UDWORD*) (file_buffer + 16)), 6+17);
      }
      else if (BI_HSIZE(file_buffer) == SIZEOF_BITMAPINFOHEADER_V5)
      {
         ConvertUDWORD(((UDWORD*) (file_buffer +  0)), 3);
         ConvertUWORD(((UWORD*) (file_buffer + 12)), 2);
         ConvertUDWORD(((UDWORD*) (file_buffer + 16)), 6+17+4);
      } 
      else /* SBI_HSIZE(file_buffer) == IZEOF_BITMAPINFOHEADER_V3 or other */
      {
         ConvertUDWORD(((UDWORD*) (file_buffer +  0)), 3);
         ConvertUWORD(((UWORD*) (file_buffer + 12)), 2);
         if (BI_HSIZE(file_buffer) == SIZEOF_BITMAPINFOHEADER_V3 && BI_COMPRESS(file_buffer)== BI_BITFIELDS) 
         {
            /* file contains red, green, and blue bit-masks and holds either a 16-bit or 32-bit image*/
            ConvertUDWORD(((UDWORD*) (file_buffer + 16)), 6+3);
         }
         else
         {
            ConvertUDWORD(((UDWORD*) (file_buffer + 16)), 6);
         }
      }
   }

   if (BI_COMPRESS(file_buffer) != BI_BITFIELDS && BI_COMPRESS(file_buffer) != BI_RGB)
   {
      fprintf(stderr, "Error in dib(Load(SBYTE*):this library does not read compressed BMP files\n");
      fprintf(stderr, "Suggestion:try to convert your BMP file to an uncompressed version using GIMP or Photoshop\n");
      /* close bitmap file */
      fclose(fp);
      /* free up alocated memory */
      free(file_buffer);
      /* return ZERO */
      return NULL;
   }
   
   if (BI_BITCOUNT(file_buffer) != 8 && BI_BITCOUNT(file_buffer) != 24 && BI_BITCOUNT(file_buffer) != 32)
   {
      fprintf(stderr, "Error in dib(Load(SBYTE*):this library does not read %d bit BMP files\n", BI_BITCOUNT(file_buffer));
      fprintf(stderr, "Suggestion:try to convert your BMP file to an 8/24/32-bit version using GIMP or Photoshop\n");
      /* close bitmap file */
      fclose(fp);
      /* free up alocated memory */
      free(file_buffer);
      /* return ZERO */
      return NULL;
   }

   if ((BI_HSIZE(file_buffer) == SIZEOF_BITMAPINFOHEADER_V4 || BI_HSIZE(file_buffer) == SIZEOF_BITMAPINFOHEADER_V5) && 
        (BI_COLOURSPA(file_buffer) != WINDOWS_COLOR_SPACE)) 
   {
      fprintf(stderr, "Warning in dib(Load(SBYTE*):this library does not support calibrated RGB or sRGB colour space BMP files\n");
      fprintf(stderr, "Suggestion:try to convert your BMP file to a RGB device-depenent colour space using GIMP or Photoshop\n");
      /* close bitmap file */
      /* fclose(fp); */
      /* free up alocated memory */
      /* free(file_buffer); */
      /* return ZERO */
      /* return NULL; */
   }

   if (BI_XPELSM(file_buffer) != BI_YPELSM(file_buffer))
   {
      fprintf(stderr, "Warning in dib(Load(SBYTE*):this library does not support BMP files with non-square pixels\n");
      fprintf(stderr, "Suggestion:try to convert your BMP file to have square-pixels using GIMP or Photoshop\n");
      /* close bitmap file */
      /*fclose( fp ); */
      /* free up alocated memory */
      /* free(file_buffer); */
      /* return ZERO */
      /* return NULL; */
   }

   /* allocate a buffer to hold the bitmapinfo in memory */
   dib_buffer  = (pdib) malloc(sizeof(bitmapinfo));
   if (dib_buffer == NULL) 
   {
      fprintf(stderr, "Error in dib_Load(SBYTE*) :cannot allocate 'pdib': out of memory\n");
      /* close bitmap file */
      fclose(fp);
      /* free up alocated memory */
      free(file_buffer);
      /* return ZERO */
      return NULL;
   }

   /* initialise the pdib */
   dib_buffer->bmiHeader = file_buffer;
   dib_buffer->dib_bisize = dib_file_header.bfOffBits - SIZEOF_BITMAPFILEHEADER;

   switch (BI_BITCOUNT(dib_buffer->bmiHeader))
   {
   case 8:
      dib_load_8bit(dib_buffer);
      break;
   case 24:
      dib_load_24bit(dib_buffer);
      break;
   case 32:
      dib_load_32bit(dib_buffer);
      break;
   default:
      break;
   }

   /* close bitmap file */
   fclose(fp);
   return dib_buffer;
}

/*
 * dib_load_8bit:
 * we convert the 8bit dib to a 24bit dib so it can be displayed 
 * in OpenGL.
 * 
 * additionally the image is loaded with the colour channels
 * for each pixel stored in order RGB; that is Red (channel) 
 * is stored first, the Green channel, followed by the Blue 
 * channel; as opposed to the 'normal' BMP convention, where 
 * 24-bit pixels are stored with Blue (channel) first, then 
 * Green, followed by the Red channel.
 */
static pdib dib_load_8bit(pdib pDIB)
{
   UBYTE *buffer = (UBYTE *) pDIB->bmiHeader;
   UDWORD storage;          /* new storage in bytes */
   UDWORD storage_width_24;  /* 24-bit storage width */
   UBYTE *new_buffer;    
   UBYTE *new_bitmap;
   UBYTE *old_bitmap;
   prgbquad lut;

   /* compute the size.of the new 24bit file */
   storage_width_24 = ((3*BI_WIDTH(buffer) + 3) & ~3);
   storage = SIZEOF_BITMAPINFOHEADER_V3 + 
             storage_width_24 * BI_HEIGHT(buffer);
   

   /* initialise pointer to the colour table */
   lut = (prgbquad ) (buffer + dib_colourMap(pDIB));

   /* Allocate a new RGB 24bit image */
   new_buffer = (UBYTE *) malloc(storage);
   if (!new_buffer) {
      fprintf(stderr, "Error in dib_load_8bit: cannot allocate bitmap data");
      exit(1);
   }

   /* byte offsets of data in bimapinfoheader structure */
   *((UDWORD*) (new_buffer + 0 ))=               SIZEOF_BITMAPINFOHEADER;
   *((SDWORD*) (new_buffer + _WIDTH_OFFSET ))=   BI_WIDTH(buffer);
   *((SDWORD*) (new_buffer + _HEIGHT_OFFSET ))=  BI_HEIGHT(buffer);
   *((UWORD*)(new_buffer + _PLANES_OFFSET ))=    1;
   *((UWORD*)(new_buffer + _BITCOUNT_OFFSET ))=  24;
   *((UDWORD*) (new_buffer + _COMPRESS_OFFSET ))= BI_RGB;
   *((UDWORD*) (new_buffer + _SIZEIMAGE_OFFSET ))= 0;
   *((SDWORD*) (new_buffer + _XPELSM_OFFSET ))=    0;
   *((SDWORD*) (new_buffer + _YPELSM_OFFSET ))=    0;
   *((UDWORD*) (new_buffer + _CLRUSED_OFFSET ))=   0;
   *((UDWORD*) (new_buffer + _CLRIMPR_OFFSET ))=   0;
 
   new_bitmap = new_buffer + SIZEOF_BITMAPINFOHEADER_V3;
   old_bitmap = buffer + dib_biSize(pDIB);
   {
      SDWORD i,j;
      SDWORD height = BI_HEIGHT(buffer);
      SDWORD width  = BI_WIDTH (buffer);

      for (i=0; i<height; i++)
      {
         for(j=0; j<width; j++)
         {
            SDWORD offset_s = i * dib_StorageWidth(pDIB) + j;
            SDWORD offset_d = i * storage_width_24 + (j*3);
            new_bitmap[offset_d+0] = lut[old_bitmap[offset_s]].rgbRed;
            new_bitmap[offset_d+1] = lut[old_bitmap[offset_s]].rgbGreen;
            new_bitmap[offset_d+2] = lut[old_bitmap[offset_s]].rgbBlue;
         }
      }
   }

   /* clean up */
   if (buffer!=NULL) 
   {
      free(buffer);
   }
   /* update the DIB to hold a V3 24-bit image */
   pDIB->bmiHeader = new_buffer; 
   pDIB->dib_bisize = SIZEOF_BITMAPINFOHEADER_V3;
   return (pDIB);
}


/*
 * dib_load_24bit:
 *
 * here the image is loaded with the colour channels
 * for each pixel stored in order RGB; that is Red (channel) 
 * is stored first, the Green channel, followed by the Blue 
 * channel; as opposed to the 'normal' BMP convention, where 
 * 24-bit pixels are stored with Blue (channel) first, then 
 * Green, followed by the Red channel.
 */
static pdib dib_load_24bit(pdib pDIB)
{
   
   UBYTE *buffer = (UBYTE *) pDIB->bmiHeader;
   SDWORD i,j;
   UBYTE* bitmap = buffer + dib_biSize(pDIB);
   SDWORD height = BI_HEIGHT(buffer);
   SDWORD width  = BI_WIDTH (buffer);
   /*
    * printf("loading 24 bit image : width %d : height %d : planes %d\n",
    *    width,height,BI_PLANES((pdib) buffer));
    */
   for (i=0; i<height; i++)
   {
      for(j=0; j<width; j++)
      {
         SDWORD offset = i * dib_StorageWidth(pDIB) + (j*3);
         UBYTE b1b = bitmap[offset + 0];
         UBYTE b2g = bitmap[offset + 1];
         UBYTE b3r = bitmap[offset + 2];
         bitmap[offset + 0] = b3r;
         bitmap[offset + 1] = b2g;
         bitmap[offset + 2] = b1b;
      }
   }
   return (pDIB);
}


/*
 * dib_load_32bit:
 */
static UDWORD calc_shift(UDWORD val)
{
   UDWORD i = 0;
   while (i < sizeof(SDWORD))
   {
      if ((val & 0x1u) == 0)
      {
         val = val >> 1;
         i++;
      }
      else
      {
         break;
      }
   }
   return i;
}

static pdib dib_load_32bit(pdib pDIB)
{
   UBYTE *buffer = (UBYTE *) pDIB->bmiHeader;
   SDWORD i,j;
   UBYTE* bitmap = buffer + dib_biSize(pDIB);
   SDWORD height = BI_HEIGHT(buffer);
   SDWORD width  = BI_WIDTH (buffer);
   
   /* bit-masks */
   UDWORD red, green, blue, alpha; 
   UDWORD red_shift, green_shift, blue_shift, alpha_shift; 

   if ((BI_HSIZE(pDIB) == SIZEOF_BITMAPINFOHEADER_V3 ||
        BI_HSIZE(pDIB) == SIZEOF_BITMAPINFOHEADER_V4 || 
        BI_HSIZE(pDIB) == SIZEOF_BITMAPINFOHEADER_V5) &&
        BI_COMPRESS(pDIB) == BI_BITFIELDS)
   {
      red   = BI_REDMASK(buffer);
      green = BI_GREENMASK(buffer);
      blue  = BI_BLUEMASK(buffer);
      if (BI_HSIZE(pDIB) == SIZEOF_BITMAPINFOHEADER_V3)
      {
         alpha = ~(0u | red | green | blue);
      }
      else
      {
         alpha = BI_ALPHAMASK(buffer);
      }
      red_shift   = calc_shift(red); 
      green_shift = calc_shift(green); 
      blue_shift  = calc_shift(blue); 
      alpha_shift = calc_shift(alpha); 

      for (i=0; i<height; i++)
      {
         for(j=0; j<width; j++)
         {
            SDWORD offset = i * dib_StorageWidth(pDIB) + (j*4);
            UDWORD val = *((UDWORD*)(bitmap + offset));
            UBYTE r = (UBYTE) ((val & red)   >> red_shift);
            UBYTE g = (UBYTE) ((val & green) >> green_shift); 
            UBYTE b = (UBYTE) ((val & blue)  >> blue_shift);
            UBYTE a = (UBYTE) ((val & alpha) >> alpha_shift); 
            bitmap[offset + 3] = a;
            bitmap[offset + 0] = r;
            bitmap[offset + 1] = g;
            bitmap[offset + 2] = b;
         }
      }
   }
   else 
   {
      /*
       * printf("loading 32 bit image : width %d : height %d : planes %d\n",
       *    width,height,BI_PLANES((pdib) buffer));
       */
      for (i=0; i<height; i++)
      {
         for(j=0; j<width; j++)
         {
            SDWORD offset = i * dib_StorageWidth(pDIB) + (j*4);
            UBYTE b = bitmap[offset + 0];
            UBYTE g = bitmap[offset + 1];
            UBYTE r = bitmap[offset + 2];
            UBYTE a = bitmap[offset + 3];
            bitmap[offset + 3] = a;
            bitmap[offset + 0] = r;
            bitmap[offset + 1] = g;
            bitmap[offset + 2] = b;
         }
      }
   }

   return (pDIB);
}


/*
 * dib_Write:
 *
 * this procedure simply writes a DIB windows bitmap 'pDIB'
 * in the file called 'filename;
 */
void dib_Write(pdib pDIB, const SBYTE *filename)
{
   UBYTE *buffer = pDIB->bmiHeader;
   SDWORD width  = BI_WIDTH(buffer);
   SDWORD height = BI_HEIGHT(buffer);
   SDWORD depth  = BI_BITCOUNT(buffer);
   SDWORD comp   = BI_RGB;
   UBYTE *data   = dib_Image(pDIB);
   UBYTE *pallet = buffer + dib_colourMap(pDIB); /* colour look-up-table */ 

   switch (depth) {
   case 8:
      dib_Make8(filename, width, height, comp, pallet, data);
      break;
   case 24:
      dib_Make24(filename, width, height, comp, data);
      break;
   case 32:
      dib_Make32(filename, width, height, comp, data);
      break;
   default:
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot create image file %s of %d depth \n", filename, depth);
      {SDWORD v=0; assert(v);}
      break;
   }
}

pdib dib_Create(const SBYTE *filename, UDWORD width, UDWORD height, int bitcount)
{
   
   if (bitcount == 8)
   {
      UBYTE* data   = (UBYTE *) malloc(width*height*sizeof(UBYTE)*1);
      UBYTE* pallet = (UBYTE *) malloc(256*sizeof(UBYTE)*3);
      if (data == NULL || pallet == NULL)   
      {
         fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot create image file %s \n", filename);
         {SDWORD v=0; assert(v);}
      }
      memset(data, 0, width*height*sizeof(UBYTE)*1);
      {  /* set a grey-scale pallete */
         int i;
         for (i=0; i < 256; i++)
         {
            pallet[i*3+0] = pallet[i*3+1] = pallet[i*3+2] = i;
         }
      }
      dib_Make8(filename, width, height, BI_RGB, pallet, data);
      /* clean up */
      free(data);
      free(pallet);
   } 
   else if (bitcount == 24)
   {
      UBYTE* data = (UBYTE *) malloc(width*height*sizeof(UBYTE)*3); 
      if (data == NULL)   
      {
         fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot create image file %s \n", filename);
         {SDWORD v=0; assert(v);}
      }
      memset(data, 0, width*height*sizeof(UBYTE)*3);
      dib_Make24(filename, width, height, BI_RGB, data);
      /* clean up */
      free(data);
   } 
   else if (bitcount == 32)
   {
      UBYTE* data = (UBYTE *) malloc(width*height*sizeof(UBYTE)*4); 
      if (data == NULL)   
      {
         fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot create image file %s \n", filename);
         {SDWORD v=0; assert(v);}
      }
      memset(data, 255, width*height*sizeof(UBYTE)*4);
      dib_Make32(filename, width, height, BI_RGB, data);
      /* clean up */
      free(data);
   }
   /* Ok! now lets load the image back in memory */
   return dib_Load(filename);
}

/*
 * Check the byte order  :
 */
static SDWORD CheckByteOrder(void)
{
   SWORD w = 0x0001;
   SBYTE *b = (SBYTE*) &w;
   return ( b[0] ? LSB_FIRST : MSB_FIRST);
}

/*
 * SwapBytes 
 */
static UWORD SwapBytesUWORD(UWORD w)
{
   register UWORD tmp;
   tmp = (UWORD)(w & 0x00ff);
   tmp = (UWORD)(((w & 0xff00) >> 0x08) | (tmp << 0x08));
   return (tmp);
}

static UDWORD SwapBytesUDWORD(UDWORD dw)
{
   register UDWORD tmp;
   tmp = (dw & 0x000000ff);
   tmp = ((dw & 0x0000ff00) >> 0x08) | (tmp << 0x08);
   tmp = ((dw & 0x00ff0000) >> 0x10) | (tmp << 0x08);
   tmp = ((dw & 0xff000000) >> 0x18) | (tmp << 0x08);
   return (tmp);
}

static void ConvertUDWORD(UDWORD *array, SDWORD length) 
{
   SDWORD i;
   for(i=0; i<length; i++)
      (*(array+i)) = SwapBytesUDWORD(*(array+i));
}

static void ConvertUWORD(UWORD *array, SDWORD length)
{
   SDWORD i;
   for(i=0; i<length; i++)
      (*(array+i)) = SwapBytesUWORD(*(array+i));
}



/*
 * dib_Make:
 */
void dib_Make8(
   const SBYTE *filename, 
   SDWORD width, 
   SDWORD height, 
   SDWORD comp,  /* compression scheme  defaults to 0 i.e. BI_RGB no compression */
   void *pallet, /* colour pallet should be made 256 RGB entries */
   void *data    /* colour index data */
) {
   bitmapfileheader hdr;
   SDWORD byteorder = CheckByteOrder();
   SDWORD depth = 8; /* 8-bit pixel values */

   /* Create the .bmp file */
   FILE *fp = fopen(filename, "wb");
   if (fp==NULL) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot open file %s\n", filename);
      {SDWORD v=0; assert(v);}
   }
 
   hdr.bfType = 0x4d42; /* BM */
   /* Compute the size of the entire file */
   hdr.bfSize =SIZEOF_BITMAPFILEHEADER + SIZEOF_BITMAPINFOHEADER + 
      ((1u<<depth) * SIZEOF_RGBQUAD) + (((width + 3) & ~3) * height);

   hdr.bfReserved1 = 0;
   hdr.bfReserved2 = 0;
   /* Compute the offset to the array of color indices */
   hdr.bfOffBits = SIZEOF_BITMAPFILEHEADER + SIZEOF_BITMAPINFOHEADER + ((1u<<depth) * SIZEOF_RGBQUAD);


   /* Copy the BITMAPFILEHEADER into the .bmp file */
   if (byteorder==MSB_FIRST) ConvertUWORD(&(hdr.bfType), 1);
   if (!fwrite(&(hdr.bfType), TWO_BYTES, 1, fp)) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*):\
              Cannot write bmp-header to file %s\n", filename);
      {SDWORD v=0; assert(v);}
   }

   if (byteorder==MSB_FIRST) ConvertUDWORD(&(hdr.bfSize), 1);
   if (!fwrite(&(hdr.bfSize), FOUR_BYTES, 1, fp)) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      {SDWORD v=0; assert(v);}
   }

   if (byteorder==MSB_FIRST) ConvertUWORD(&(hdr.bfReserved1), 1);
   if (!fwrite(&(hdr.bfReserved1), TWO_BYTES, 1, fp)) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      {SDWORD v=0; assert(v);}
   }

   if (byteorder==MSB_FIRST) ConvertUWORD(&(hdr.bfReserved2), 1);
   if (!fwrite(&(hdr.bfReserved2), TWO_BYTES, 1, fp)) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      {SDWORD v=0; assert(v);}
   }

   if (byteorder==MSB_FIRST) ConvertUDWORD(&(hdr.bfOffBits), 1);
   if (!fwrite(&(hdr.bfOffBits), FOUR_BYTES, 1, fp)) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      {SDWORD v=0; assert(v);}
   }

   /* Copy the BITMAPINFOHEADER and in the file */
   
   {
      UBYTE *header = (UBYTE*) malloc(SIZEOF_BITMAPINFOHEADER);
      if (header==NULL) {
         fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                 out of memory %s\n", filename);
         {SDWORD v=0; assert(v);}
      }
      

      /* byte offsets of data in bimapinfoheader structure */
      *((UDWORD*) (header + 0 ))= SIZEOF_BITMAPINFOHEADER;
      *((SDWORD*) (header + _WIDTH_OFFSET ))=  width;
      *((SDWORD*) (header + _HEIGHT_OFFSET ))= height;
      *((UWORD*)(header + _PLANES_OFFSET ))=   1;
      *((UWORD*)(header + _BITCOUNT_OFFSET ))= 8;
      *((UDWORD*) (header + _COMPRESS_OFFSET ))= BI_RGB;
      *((UDWORD*) (header + _SIZEIMAGE_OFFSET ))= 0;
      *((SDWORD*) (header + _XPELSM_OFFSET ))=    0;
      *((SDWORD*) (header + _YPELSM_OFFSET ))=    0;
      *((UDWORD*) (header + _CLRUSED_OFFSET ))=   0;
      *((UDWORD*) (header + _CLRIMPR_OFFSET ))=   0;

       if (byteorder == MSB_FIRST) {
         ConvertUDWORD (((UDWORD*)  (header)), 3);
         ConvertUWORD(((UWORD*) (header + 12)), 2);
         ConvertUDWORD (((UDWORD*)  (header + 16)), 6);
      }

       if (!(fwrite(header, SIZEOF_BITMAPINFOHEADER, 1, fp))) {
         fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                 Cannot write bmp to file %s\n", filename);
         {SDWORD v=0; assert(v);}
       }

       /* clean up */
       free(header);
   } 

   /* copy the colour bitmap */
   {
      UDWORD i, j; /* counters */
      UDWORD numbcolours = 1u << depth; /* number of colours in the colour-look-up table */
      UBYTE *lut = (UBYTE*) pallet; /* look-up-table */
      UBYTE zero=0;
      SDWORD off_0, off_1, off_2;
      if (RGB_ORDER() != 0) {
         off_0 = 2;
         off_1 = 1;
         off_2 = 0;
      } else {
         off_0 = 0;
         off_1 = 1;
         off_2 = 2;
      }
      for (i=0, j=0; i < numbcolours; i++, j+=SIZEOF_RGBQUAD) {
         if (!fwrite(&lut[j+off_0], ONE_BYTES, 1, fp)) { /* write blue byte */
            fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                    Cannot write bmp-colour-map to file %s\n", filename);
            {SDWORD v=0; assert(v);}
         }

         if (!fwrite(&lut[j+off_1], ONE_BYTES, 1, fp)) { /* write green byte */
            fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                    Cannot write bmp-colour-map to file %s\n", filename);
            {SDWORD v=0; assert(v);}
         }

         if (!fwrite(&lut[j+off_2], ONE_BYTES, 1, fp)) { /* write green byte */
            fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                    Cannot write bmp-colour-map to file %s\n", filename);
            {SDWORD v=0; assert(v);}
         }

         if (!fwrite(&zero, ONE_BYTES, 1, fp)) { /* write reserved byte */
            fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                    Cannot write bmp-colour-map to file %s\n", filename);
            {SDWORD v=0; assert(v);}
         }
      }
   }
   
   {
      SDWORD i,j;
      SDWORD storage_w = ((width + 3) & ~3);
      SDWORD padding   = storage_w - width;

      UBYTE *bitmap = data;
      UBYTE *pad = NULL;

      /* allocate padding */
      if (padding!=0)
      {
         pad = (UBYTE*) malloc(padding);
         if (pad==NULL) {
            fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                    out of memory %s\n", filename);
            {SDWORD v=0; assert(v);}
         }
         memset(pad, 0, padding);
      }

      /* copy RGB values to bmp file */
      for (i=0; i<height; i++)
      {
         for(j=0; j<width; j++)
         {
            SDWORD offset = (i*storage_w) + (j);
            /* store indexes to the colour look-up-table */
            if (!fwrite(&bitmap[offset], ONE_BYTES, 1, fp)) {
               fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                       Cannot write bmp to file %s\n", filename);
               {SDWORD v=0; assert(v);}
            }
         }
         /* add padding so scan lines are 4 byte (double word) alligned */
         if (padding!=0)
            if (!fwrite(pad, padding, 1, fp)) {
               fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                       Cannot write bmp to file %s\n", filename);
               {SDWORD v=0; assert(v);}
            }
      }

      /* clean up */
      if (pad!=NULL) free(pad);
   }
   /* close bitmap file */
   fclose( fp );
}

/*
 * dib_Make:
 *   filename : name of the ".bmp" to be created
 *   width    : of dib image
 *   height   : of dib image
 *   depth    : of dib image should be 24-bits
 *   comp     : compression scheme, is always 0
 *   pallet   : colour look up table data if depth is 8-bit, otherwise NULL
 *   data     : image data
 *
 */
void dib_Make(const SBYTE *filename, 
              SDWORD width, SDWORD height, 
              SDWORD depth, 
              SDWORD comp, 
              void *pallet, void *data)
{
   bitmapfileheader hdr;
   SDWORD byteorder = CheckByteOrder();

   /* Create the .bmp file */
   FILE *fp = fopen(filename, "wb");
   if (fp==NULL) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot open file %s\n", filename);
      {SDWORD v=0; assert(v);}
   }
 
   hdr.bfType = 0x4d42; /* BM */
   /* Compute the size of the entire file */
   hdr.bfSize =SIZEOF_BITMAPFILEHEADER + SIZEOF_BITMAPINFOHEADER + 
               (((3*width + 3) & ~3) * height);

   hdr.bfReserved1 = 0;
   hdr.bfReserved2 = 0;
   /* Compute the offset to the array of color indices */
   hdr.bfOffBits = SIZEOF_BITMAPFILEHEADER + SIZEOF_BITMAPINFOHEADER;


   /* Copy the BITMAPFILEHEADER into the .bmp file */
   if (byteorder==MSB_FIRST) ConvertUWORD(&(hdr.bfType), 1);
   if (!fwrite(&(hdr.bfType), TWO_BYTES, 1, fp)) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*):\
              Cannot write bmp-header to file %s\n", filename);
      {SDWORD v=0; assert(v);}
   }

   if (byteorder==MSB_FIRST) ConvertUDWORD(&(hdr.bfSize), 1);
   if (!fwrite(&(hdr.bfSize), FOUR_BYTES, 1, fp)) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      {SDWORD v=0; assert(v);}
   }

   if (byteorder==MSB_FIRST) ConvertUWORD(&(hdr.bfReserved1), 1);
   if (!fwrite(&(hdr.bfReserved1), TWO_BYTES, 1, fp)) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      {SDWORD v=0; assert(v);}
   }

   if (byteorder==MSB_FIRST) ConvertUWORD(&(hdr.bfReserved2), 1);
   if (!fwrite(&(hdr.bfReserved2), TWO_BYTES, 1, fp)) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      {SDWORD v=0; assert(v);}
   }

   if (byteorder==MSB_FIRST) ConvertUDWORD(&(hdr.bfOffBits), 1);
   if (!fwrite(&(hdr.bfOffBits), FOUR_BYTES, 1, fp)) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      {SDWORD v=0; assert(v);}
   }

   /* Copy the BITMAPINFOHEADER and in the file */
   
   {
      UBYTE *header = (UBYTE*) malloc(SIZEOF_BITMAPINFOHEADER);
      if (header==NULL) {
         fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                 out of memory %s\n", filename);
         {SDWORD v=0; assert(v);}
      }
      

      /* byte offsets of data in bimapinfoheader structure */
      *((UDWORD*) (header + 0 ))= SIZEOF_BITMAPINFOHEADER;
      *((SDWORD*) (header + _WIDTH_OFFSET ))=   width;
      *((SDWORD*) (header + _HEIGHT_OFFSET ))=  height;
      *((UWORD*)(header + _PLANES_OFFSET ))=    1;
      *((UWORD*)(header + _BITCOUNT_OFFSET ))=  24;
      *((UDWORD*) (header + _COMPRESS_OFFSET ))=  BI_RGB;
      *((UDWORD*) (header + _SIZEIMAGE_OFFSET ))= 0;
      *((SDWORD*) (header + _XPELSM_OFFSET ))=    0;
      *((SDWORD*) (header + _YPELSM_OFFSET ))=    0;
      *((UDWORD*) (header + _CLRUSED_OFFSET ))=   0;
      *((UDWORD*) (header + _CLRIMPR_OFFSET ))=   0;

       if (byteorder == MSB_FIRST) {
         ConvertUDWORD (((UDWORD*)  (header)), 3);
         ConvertUWORD(((UWORD*) (header + 12)), 2);
         ConvertUDWORD (((UDWORD*)  (header + 16)), 6);
      }

       if (!(fwrite(header, SIZEOF_BITMAPINFOHEADER, 1, fp))) {
         fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                 Cannot write bmp to file %s\n", filename);
         {SDWORD v=0; assert(v);}
       }

       /* clean up */
       free(header);
   } 
   
   {
      SDWORD i,j;
      SDWORD storage_w = ((3*width + 3) & ~3);
      SDWORD padding   = storage_w - 3*width;

      UBYTE *bitmap = data;
      UBYTE bgr[3];
      UBYTE *pad = NULL;

      /* allocate padding */
      if (padding!=0)
      {
         pad = (UBYTE*) malloc(padding);
         if (pad==NULL) {
            fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                    out of memory %s\n", filename);
            {SDWORD v=0; assert(v);}
         }
         memset(pad, 0, padding);
      }

      /* copy RGB values to bmp file */
      for (i=0; i<height; i++)
      {
         for(j=0; j<width; j++)
         {
            SDWORD offset = (i*storage_w) + (j*3);
            if (RGB_ORDER() == 1) {
               /* change RGB values to to BGR */
               bgr[2] = bitmap[offset+0];
               bgr[1] = bitmap[offset+1];
               bgr[0] = bitmap[offset+2];
            } else {
               bgr[2] = bitmap[offset+2];
               bgr[1] = bitmap[offset+1];
               bgr[0] = bitmap[offset+0];
            }
            if (!fwrite(bgr, 3, 1, fp)) {
               fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                       Cannot write bmp to file %s\n", filename);
               {SDWORD v=0; assert(v);}
            }
         }
         /* add padding so scan lines are 4 byte (double word) alligned */
         if (padding!=0)
            if (!fwrite(pad, padding, 1, fp)) {
               fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                       Cannot write bmp to file %s\n", filename);
               {SDWORD v=0; assert(v);}
            }
      }

      /* clean up */
      if (pad!=NULL) free(pad);
   }
   /* close bitmap file */
   fclose( fp );
}

/*
 * dib_Make32:
 *
 * input:
 *   filename : name of the ".bmp" to be created
 *   width    : of dib image
 *   height   : of dib image
 *   depth    : of dib image can be 8,24 or 32-bits
 *   comp     : compression scheme
 *   pallet   : colour look up table data if depth is 8-bit, otherwise NULL
 *   data     : image data
 *
 * output:
 *
 * on exit:
 */

void dib_Make32(
   const SBYTE *filename,
   SDWORD width, 
   SDWORD height,
   SDWORD comp,
   void *data
) {
   bitmapfileheader hdr;
   SDWORD byteorder = CheckByteOrder();

   /* Create the .bmp file */
   FILE *fp = fopen(filename, "wb");
   if (fp==NULL) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot open file %s\n", filename);
      assert(0); exit(1);
   }
 
   hdr.bfType = 0x4d42; /* BM */
   /* Compute the size of the entire file */
   hdr.bfSize =SIZEOF_BITMAPFILEHEADER + SIZEOF_BITMAPINFOHEADER + 
               (4* width * height);
   /*
   hdr.bfSize =SIZEOF_BITMAPFILEHEADER + SIZEOF_BITMAPINFOHEADER + 12 +
               (4* width * height);
   */

   hdr.bfReserved1 = 0;
   hdr.bfReserved2 = 0;
   /* Compute the offset to the array of color indices */
   hdr.bfOffBits = SIZEOF_BITMAPFILEHEADER + SIZEOF_BITMAPINFOHEADER;
   /* hdr.bfOffBits = SIZEOF_BITMAPFILEHEADER + SIZEOF_BITMAPINFOHEADER + 12; */


   /* Copy the BITMAPFILEHEADER into the .bmp file */
   if (byteorder==MSB_FIRST) ConvertUWORD(&(hdr.bfType), 1);
   if (fwrite(&(hdr.bfType), TWO_BYTES, 1, fp)!=1) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*):\
              Cannot write bmp-header to file %s\n", filename);
      assert(0); exit(1);
   }

   if (byteorder==MSB_FIRST) ConvertUDWORD(&(hdr.bfSize), 1);
   if (fwrite(&(hdr.bfSize), FOUR_BYTES, 1, fp)!=1) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      {SDWORD v=0; assert(v);}
   }

   if (byteorder==MSB_FIRST) ConvertUWORD(&(hdr.bfReserved1), 1);
   if (fwrite(&(hdr.bfReserved1), TWO_BYTES, 1, fp)!=1) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      assert(0); exit(1);
   }

   if (byteorder==MSB_FIRST) ConvertUWORD(&(hdr.bfReserved2), 1);
   if (fwrite(&(hdr.bfReserved2), TWO_BYTES, 1, fp)!=1) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      assert(0); exit(1);
   }

   if (byteorder==MSB_FIRST) ConvertUDWORD(&(hdr.bfOffBits), 1);
   if (fwrite(&(hdr.bfOffBits), FOUR_BYTES, 1, fp)!=1) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      assert(0); exit(1);
   }

   /* Copy the BITMAPINFOHEADER and in the file */
   {
      UBYTE *header = (UBYTE*) malloc(SIZEOF_BITMAPINFOHEADER);
      if (header==NULL) {
         fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                 out of memory %s\n", filename);
         assert(0); exit(1);
      }

      /* byte offsets of data in bimapinfoheader structure */
      *((UDWORD*) (header + 0 ))= SIZEOF_BITMAPINFOHEADER;
      *((SDWORD*) (header + _WIDTH_OFFSET ))=  width;
      *((SDWORD*) (header + _HEIGHT_OFFSET ))= height;
      *((UWORD*)(header + _PLANES_OFFSET ))=   1;
      *((UWORD*)(header + _BITCOUNT_OFFSET ))= 32;
      *((UDWORD*) (header + _COMPRESS_OFFSET ))=  BI_RGB; /* BI_BITFIELDS */
      *((UDWORD*) (header + _SIZEIMAGE_OFFSET ))= 0;
      *((SDWORD*) (header + _XPELSM_OFFSET ))=    0;
      *((SDWORD*) (header + _YPELSM_OFFSET ))=    0;
      *((UDWORD*) (header + _CLRUSED_OFFSET ))=   0;
      *((UDWORD*) (header + _CLRIMPR_OFFSET ))=   0;

      if (byteorder == MSB_FIRST) {
         ConvertUDWORD (((UDWORD*)  (header)), 3);
         ConvertUWORD(((UWORD*) (header + 12)), 2);
         ConvertUDWORD (((UDWORD*)  (header + 16)), 6);
      }

      if (fwrite(header, SIZEOF_BITMAPINFOHEADER, 1, fp)!=1) {
         fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                 Cannot write bmp to file %s\n", filename);
         assert(0); exit(1);
      }

      /* clean up */
      free(header);
   }
/*
   UDWORD r_bits = 0x00ff0000u;
   if (byteorder==MSB_FIRST) ConvertUDWORD(&r_bits, 1);
   if (fwrite(&r_bits, FOUR_BYTES, 1, fp)!=1) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      assert(0); exit(1);
   }

   UDWORD g_bits = 0x0000ff00u;
   if (byteorder==MSB_FIRST) ConvertUDWORD(&g_bits, 1);
   if (fwrite(&g_bits, FOUR_BYTES, 1, fp)!=1) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      assert(0); exit(1);
   }
   
   UDWORD b_bits = 0x000000ffu;
   if (byteorder==MSB_FIRST) ConvertUDWORD(&b_bits, 1);
   if (fwrite(&b_bits, FOUR_BYTES, 1, fp)!=1) {
      fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
              Cannot write bmp-header to file %s\n", filename);
      assert(0); exit(1);
   }
*/
   {
      UBYTE *bitmap = data;
      SDWORD scanlinew = 4*width;
      SDWORD i,j;
      /* copy RGBA values to bmp file */
      for (i=0; i<height; i++) {
         for(j=0; j<width; j++) {
            SDWORD offset = (i*scanlinew) + (j*4);
            UBYTE bgra[4];
            if (RGB_ORDER() == 1) {
               /* change RGBA values to to BGRA */
               bgra[3] = bitmap[offset+3];
               bgra[2] = bitmap[offset+0];
               bgra[1] = bitmap[offset+1];
               bgra[0] = bitmap[offset+2];
            } else {
               bgra[3] = bitmap[offset+3];
               bgra[2] = bitmap[offset+2];
               bgra[1] = bitmap[offset+1];
               bgra[0] = bitmap[offset+0];
            }
            /* store ARGB values */
            /*
            if (RGB_ORDER() == 1) {
               bgra[3] = bitmap[offset+3];
               bgra[2] = bitmap[offset+2];
               bgra[1] = bitmap[offset+1];
               bgra[0] = bitmap[offset+0];
            }
            */
            if (fwrite(bgra, 4, 1, fp)!=1) {
               fprintf(stderr, "Error in void dib_write(pdib, SBYTE*): \
                       Cannot write bmp to file %s\n", filename);
               assert(0); exit(1);
            }
         }
      }
   }
   /* close bitmap file */
   fclose( fp );
}


