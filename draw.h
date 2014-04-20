#ifndef __DRAW_H__
#define __DRAW_H__

#include "dib.h" 

/*! \brief Draws a point.
 * 
 * @param im [in] a pointer to a BMP device independent bitmap
 * @param x0, yo [in] pixel coordinates
 * @param ink_[in] array of UBYTE colours 
 * @param op [in] if 1 and this is an RGBA image use opacity bled operation, 
 *                otherwise don't use opacity colour blending   
 */
extern int
ImagingDrawPoint(pdib im, int x0, int y0, const void* ink_, int op);

/*! \brief Draws a line.
 * 
 * @param im [in] a pointer to a BMP device independent bitmap
 * @param x0, yo [in] pixel coordinates of line's start position
 * @param x1, y1 [in] pixel coordinates of line's end position
 * @param ink_[in] array of UBYTE colours 
 * @param op [in] if 1 and this is an RGBA image use opacity bled operation, 
 *                otherwise don't use opacity colour blending   
 */
extern int
ImagingDrawLine(pdib im, int x0, int y0, int x1, int y1, const void* ink_,
                int op);

/*! \brief Draws a wide-line.
 * 
 * @param im [in] a pointer to a BMP device independent bitmap
 * @param x0, yo [in] pixel coordinates of line's start position
 * @param x1, y1 [in] pixel coordinates of line's end position
 * @param ink_ [in] array of UBYTE colours 
 * @param width [in] line pixel width
 * @param op [in] if 1 and this is an RGBA image use opacity bled operation, 
 *                otherwise don't use opacity colour blending   
 */
extern int
ImagingDrawWideLine(pdib im, int x0, int y0, int x1, int y1,
                    const void* ink_, int width, int op);

/*! \brief Draws a rectangle. 
 * 
 * @param im [in] a pointer to a BMP device independent bitmap
 * @param x0, yo [in] pixel coordinates of top-left corner
 * @param x1, y1 [in] pixel coordinates of bottom-right corner
 * @param ink_ [in] array of UBYTE colours 
 * @param fill [in] if 1 filled otherwise outlined
 * @param op [in] if 1 and this is an RGBA image use opacity bled operation, 
 *                otherwise don't use opacity colour blending   
 */
extern int
ImagingDrawRectangle(pdib im, int x0, int y0, int x1, int y1,
                     const void* ink_, int fill, int op);

/*! \brief Draws a polygon. 
 * 
 * @param im [in] a pointer to a BMP device independent bitmap 
 * @param count [in] number of vertexies
 * @param xy [in] int array of ``count ''vertex coordinates
 * @param ink_ [in] array of UBYTE colours 
 * @param fill [in] if 1 filled otherwise outlined
 * @param op [in] if 1 and this is an RGBA image use opacity bled operation, 
 *                otherwise don't use opacity colour blending   
 */
extern int
ImagingDrawPolygon(pdib im, int count, int* xy, const void* ink_,
                   int fill, int op);

/*! \brief Draws a bitmap. 
 * 
 * @param im [in] a pointer to the destination BMP device independent bitmap 
 * @param x0, yo [in] pixel coordinates of top-left cotrner of where in the 
 *                    destination bitmap we will draw the source bitmap
 * @param bitmap [in] a pointer to the source BMP device independent bitmap 
 * @param ink [in] array of UBYTE colours, should be NULL or 0
 * @param op [in] if 1 and this is an RGBA image use opacity bled operation, 
 *                otherwise don't use opacity colour blending   
 */
extern int
ImagingDrawBitmap(pdib im, int x0, int y0, pdib bitmap, const void* ink,
                  int op);

/*! \brief Draws an arc. 
 * 
 * @param im [in] a pointer to a BMP device independent bitmap
 * @param x0, yo [in] pixel coordinates of top-left bounding-box corner
 * @param x1, y1 [in] pixel coordinates of bottom-right bounding-box corner
 * @param start, end [in] arc's starting and ending angles in degrees.
 * @param ink [in] array of UBYTE colours 
 * @param fill [in] if 1 filled otherwise outlined
 * @param op [in] if 1 and this is an RGBA image use opacity bled operation, 
 *                otherwise don't use opacity colour blending   
 */
extern int
ImagingDrawArc(pdib im, int x0, int y0, int x1, int y1,
               int start, int end, const void* ink, int op);

/*! \brief Draws a chord. 
 * 
 * @param im [in] a pointer to a BMP device independent bitmap
 * @param x0, yo [in] pixel coordinates of top-left bounding-box corner
 * @param x1, y1 [in] pixel coordinates of bottom-right bounding-box corner
 * @param start, end [in] chords's starting and ending angles in degrees.
 * @param ink [in] array of UBYTE colours 
 * @param fill [in] if 1 filled otherwise outlined
 * @param op [in] if 1 and this is an RGBA image use opacity bled operation, 
 *                otherwise don't use opacity colour blending   
 */
extern int
ImagingDrawChord(pdib im, int x0, int y0, int x1, int y1,
               int start, int end, const void* ink, int fill, int op);

/*! \brief Draws an ellipse. 
 * 
 * @param im [in] a pointer to a BMP device independent bitmap
 * @param x0, yo [in] pixel coordinates of top-left bounding-box corner
 * @param x1, y1 [in] pixel coordinates of bottom-right bounding-box corner
 * @param ink [in] array of UBYTE colours 
 * @param fill [in] if 1 filled otherwise outlined
 * @param op [in] if 1 and this is an RGBA image use opacity bled operation, 
 *                otherwise don't use opacity colour blending   
 */
extern int
ImagingDrawEllipse(pdib im, int x0, int y0, int x1, int y1,
                   const void* ink, int fill, int op);

/*! \brief Draws a pie-sclice. 
 * 
 * @param im [in] a pointer to a BMP device independent bitmap
 * @param x0, yo [in] pixel coordinates of top-left bounding-box corner
 * @param x1, y1 [in] pixel coordinates of bottom-right bounding-box corner
 * @param start, end [in] sclice's starting and ending angles in degrees.
 * @param ink [in] array of UBYTE colours 
 * @param fill [in] if 1 filled otherwise outlined
 * @param op [in] if 1 and this is an RGBA image use opacity bled operation, 
 *                otherwise don't use opacity colour blending   
 */
extern int
ImagingDrawPieslice(pdib im, int x0, int y0, int x1, int y1,
                    int start, int end, const void* ink, int fill, int op);

#endif /* __DRAW_H__ */

