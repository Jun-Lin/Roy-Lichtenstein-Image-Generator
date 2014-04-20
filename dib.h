#ifndef __DIB_H__
#define __DIB_H__

#include "dib_types.h"

/*
 * forward declarations
 */

/*! \brief A pointer to BMP device independent bitmap data.
 *         
 * \warning This pointer is an important data type used to access all DIB/BMP bitmap data. 
 */
typedef struct bmi *pdib;

/*
 * prototypes
 */

/*! \brief Returns the BMP scanline storage with in bytes.
 *
 * @param pDIB [in] a pointer to a device independent bitmap (pDIB)
 *
 * @return Given a pointer to a device independent bitmap (pDIB) it 
 *         returns the number of bytes in each scanline of this bitmap. 
 *
 * \warning Note that BMP files have to have a scanline width that is a 
 *          multiple of 4. For this reason, the scanline storage width is not 
 *          necessarily the same as the number of bytes that store pixel data
 *          across a scanline.
 *
 */
extern SDWORD dib_StorageWidth(pdib pDIB);

/*! \brief Returns the bitmap's width. 
 * 
 * @param pDIB [in] a pointer to a device independent bitmap (pDIB)
 *
 * @return Given a pointer to a device independent bitmap (pDIB) it returns 
 *        the number of pixels in a scanline. 
 */
extern SDWORD dib_Width (pdib pDIB);

/*! \brief Returns the bitmap's height. 
 * 
 * @param pDIB [in] a pointer to a device independent bitmap (pDIB)
 *
 * @return Given a pointer to a device independent bitmap (pDIB) it returns
 *         its number of scanlines. 
 */
extern SDWORD dib_Height(pdib pDIB);

/*! \brief Deallocates the bitmap data. 
 * 
 * @param pDIB [in] a pointer to a device independent bitmap (pDIB)
 * 
 * @return Given a pointer to a BMP device independent bitmap (pDIB) 
 *         it deallocates the memory allocated in pDIB. On Exit the bitmap data 
 *         in pDIB are deleted. 
 */
extern void dib_Destroy(pdib pDIB);

/*! \brief Returns the bitmap's bits per pixel. 
 * 
 * @param pDIB [in] a pointer to a device independent bitmap (pDIB)
 *
 * @return Given a pointer to a BMP device independent bitmap (pDIB) it returns
 *         the number of bits used to store the data of each pixel. The number of bytes 
 *         that store a pixel is given by dib_Bitcount(pdib pDIB)/8
 */
extern UWORD dib_Bitcount(pdib pDIB);

/*! \brief Returns returns a pointer to the image data. 
 * 
 * @param pDIB [in] a pointer to a device independent bitmap (pDIB)
 * 
 * @return Given a pointer to a device independent bitmap (pDIB) it returns
 *         the pixel data in an array of unsigned bytes (UBYTE*). 
 *
 * @warning All pixel data are held consecutively in a one dimensional array of unsigned bytes (UBYTE *).
 *         The pixel data in the array are stored in (horizontal) scanlines, with the scanline at the bottom 
 *         of the bitmap  appearing first in the array, then the second scanline from the bottom, followed by the 
 *         third scanline from the bottom of the bitmap, then the fourth and so on ..., 
 *         and with each scanline storing pixel values consecutively from left to right.
 *
 * @see If dib_Bitcount(pDIB) returns 8 then we have an array of grey scale pixels, 
 *      each pixel comprising of one byte, that is; a single value between 0 and 255. 
 * @see If dib_Bitcount(pDIB) returns 24 then we have an array of RGB pixels, each
 *      pixel comprising of three bytes, one byte for red, one byte for green, and one for blue.
 * @see If dib_Bitcount(pDIB) returns 32 then we have an array of RGBA pixels, each
 *      pixel comprising of four bytes, red, green, blue, and alpha. The alpha value 
 *      is used to store pixel opacity. 
 *
 * @see The red, green, blue and alpha values that describe the colour of a pixel are also referred to as colour channels.  
 */
extern UBYTE* dib_Image(pdib pDIB);

/*! \brief Loads a bitmap from a file.
 * 
 * @param filename [in] a string that holds a BMP file name
 * 
 * @return Given the string (``filename'') holding the name of a device independent 
 *         bitmap it loads the bitmap in memory and returns a pdib pointer holding 
 *         the bitmap's data.   
 */
extern pdib dib_Load(const SBYTE *filename);

/*! \brief Writes the DIB pointer data to a file.
 * 
 * @param bmp      [in] a pointer to a device independent bitmap (pDIB)
 * @param filename [in] the name of a BMP file to write in disk
 * 
 * @return Given a pointer to a BMP device independent bitmap (bmp) and a string holding
 *         a filename (``filename'') it writes the data stored in ``bmp'' in ``filename''.   
 */
extern void dib_Write(pdib bmp, const SBYTE *filename);

/*! \brief Creates a new BMP width X height device independent bitmap and stores it on disk.
 * 
 * @param filename [in] the name of a BMP file to create and write in disk
 * @param width, height [in] width and height of the bitmap to be created
 * @param bitcount [in] the number of bits per-pixel 
 *
 * @return Given the string (``filename'') holding the name of BMP device independent bitmap
 *         as well as its ``width'', ``height'' and its number of bits per pixel (``bitcount'') 
 *         it creates a new width X height BMP device independent bitmap data object and stores 
 *         it on disk as ``filename''. On Exit it returns a DIB pointer holding the newly created 
 *         bitmap data.  
 */
extern pdib dib_Create(const SBYTE *filename, UDWORD width, UDWORD height, int bitcount);

/*! \brief Stores a 8-bit BMP file to disk from `data` 
 * 
 * @return On exit a 8-bit BMP file is written to disk
 */ 
extern void dib_Make8(  
   const SBYTE *filename,  /**< [out]the name the output file */ 
   SDWORD width,     /**< [in] image dimensions */
   SDWORD height,    /**< [in] image dimensions */
   SDWORD comp,      /**< [in] compression scheme  defaults to 0 i.e. BI_RGB no compression */
   void *pallet,     /**< [in] colour pallet should be made of a 1D  array of 3*256 bytes, 
                               with RGB triples representing a colour look-up table */
   void *data        /**< [in] bitmap data which are stored as width X height bytes of colour indexes */
);
/*! \brief Stores 24-bit BMP file to disk from `data`
 * 
 * @return On exit a 24-bit BMP file is written to disk
 */ 
extern void dib_Make(   
   const SBYTE *filename,  /**< [out] name of the output file */ 
   SDWORD width,     /**< [in] image dimensions */
   SDWORD height,    /**< [in] image dimensions */
   SDWORD depth,     /**< [in] in this case it should be 24 */ 
   SDWORD comp,      /**< [in] compression scheme  defaults to 0 i.e. BI_RGB no compression */
   void *pallet,     /**< [in] in this case it should be NULL */
   void *data        /**< [in] bitmap data which are stored as RGB triples of bytes */
);

/*! \brief macro that definesthe interface of writing a 24-bit 
 *         BMP files which is similar to writing 8-bit and 32-bit 
 *         BMPs
 */
#define dib_Make24(filename, width, height, comp, data) \
   dib_Make((filename), (width), (height), (24), (comp), (NULL), (data))


/*! \brief Stores a 32-bit BMP file to disk from  `data` 
 * 
 * @return On exit a 32-bit BMP file is written to disk
 */
extern void dib_Make32( 
   const SBYTE *filename,  /**< [out] output filename */ 
   SDWORD width,     /**< [in] image dimensions */
   SDWORD height,    /**< [in] image dimensions */
   SDWORD comp,      /**< [in] compression scheme  defaults to 0 i.e. BI_RGB no compression */
   void *data        /**< [in] bitmap data which are stored as RGBA quads*/
);


#endif /* __DIB_H__ */

