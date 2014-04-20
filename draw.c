/*
 * The Python Imaging Library.
 *
 * a simple drawing package for the Imaging library
 *
 * Copyright (c) 1996-2006 by Fredrik Lundh
 * Copyright (c) 1997-2006 by Secret Labs AB.
 *
 * 2014-02-09 Conversion by A Sarafopoulos to work with the own dib library 
 *
 * See the README file for information on usage and redistribution.
 */

/* FIXME: support fill/outline attribute for all filled shapes */
/* FIXME: support zero-winding fill */
/* FIXME: add drawing context, support affine transforms */
/* FIXME: support clip window (and mask?) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "dib.h"

#define CEIL(v)  (int) ceil(v)
#define FLOOR(v) ((v) >= 0.0 ? (int) (v) : (int) floor(v))

/* like (a * b + 127) / 255), but much faster on most platforms         */
/*------------------------------------------------------------------------
 * Cannot make sense of this ... Why add 128 to tmp, then divide with 256 
 * and add tmp and finally divide with 256 again?
 *
#define MULDIV255(a, b, tmp)\
        (tmp = (a) * (b) + 128, ((((tmp) >> 8) + (tmp)) >> 8))

#define BLEND(mask, in1, in2, tmp1, tmp2)\
        (MULDIV255(in1, 255 - mask, tmp1) + MULDIV255(in2, mask, tmp2))
-------------------------------------------------------------------------*/

/*! \brief Performs (a * b) / 255, but much faster on most platforms 
 * @see BLEND marco
 * @param a [in] input pixel value
 * @param b [in] input alpha value
 * @param tmp [in] temporary variable used in the blend calculation
 * @return a * b/255
 */
#define MULDIV255(a, b, tmp) (tmp = (a) * (b), (tmp) >> 8)

/*! \brief opacity bled fucntion 
 * @param mask [in] alpha channel mask
 * @param in1 [in] source pixel value
 * @param in2 [in] destination pixel value
 * @param tmp1, tmp2 [in] temporary parameters used in the calculation
 * @see MULDIV255
 * @return (source * alpha) +  destination * (1 - alpha); blend function calculation
 */
#define BLEND(mask, in1, in2, tmp1, tmp2)\
        (MULDIV255(in1, 255 - mask, tmp1) + MULDIV255(in2, mask, tmp2))

/*! \brief Calculates the scanline offset
 * @param y [in] y-scanline number
 * @param im [in] pointer to DIB 
 * @return Scanline offset value 
 */
#define SCANL(y, im) ((y) * dib_StorageWidth(im))

/* -------------------------------------------------------------------- */
/* Primitives                                                           */
/* -------------------------------------------------------------------- */

typedef struct {
    /* edge descriptor for polygon engine */
    int d;
    int x0, y0;
    int xmin, ymin, xmax, ymax;
    float dx;
} Edge;

static inline void 
ImagingError_MemoryError()
{
   printf("Imaging Memory Error: Can't allocate edge list\n");
}

static inline void
point8(pdib im, int x, int y, UBYTE* colour)
{
   if (x >= 0 && x < dib_Width(im) && y >= 0 && y < dib_Height(im))
   {
      UBYTE* buffer = dib_Image(im);
      buffer[ SCANL(y, im) + x ]  = colour[0];
   }
}

static inline void
point24(pdib im, int x, int y, UBYTE* colour)
{
   if (x >= 0 && x < dib_Width(im) && y >= 0 && y < dib_Height(im))
   {
      UBYTE* buffer = dib_Image(im);
      buffer[ SCANL(y, im) + x * 3 + 0 ]  = colour[0];
      buffer[ SCANL(y, im) + x * 3 + 1 ]  = colour[1];
      buffer[ SCANL(y, im) + x * 3 + 2 ]  = colour[2];
   }
}

static inline void
point32(pdib im, int x, int y, UBYTE* colour)
{
   if (x >= 0 && x < dib_Width(im) && y >= 0 && y < dib_Height(im))
   {
      UBYTE* buffer = dib_Image(im);
      buffer[ SCANL(y, im) + x * 4 + 0 ]  = colour[0];
      buffer[ SCANL(y, im) + x * 4 + 1 ]  = colour[1];
      buffer[ SCANL(y, im) + x * 4 + 2 ]  = colour[2];
      buffer[ SCANL(y, im) + x * 4 + 3 ]  = colour[3];
   }
}

static inline void
point32rgba(pdib im, int x, int y, UBYTE* colour)
{
   unsigned int tmp1, tmp2;

   if (x >= 0 && x < dib_Width(im) && y >= 0 && y < dib_Height(im))
   {
      UBYTE* buffer = dib_Image(im);
      UBYTE* out = buffer + SCANL(y, im) + x * 4 ;
      UBYTE* in  = colour;
      out[0] = BLEND(in[3], out[0], in[0], tmp1, tmp2);
      out[1] = BLEND(in[3], out[1], in[1], tmp1, tmp2);
      out[2] = BLEND(in[3], out[2], in[2], tmp1, tmp2);
   }
}

static inline void
hline8(pdib im, int x0, int y0, int x1, UBYTE* colour)
{
    int tmp;
    UBYTE* buffer = dib_Image(im);

    if (y0 >= 0 && y0 < dib_Height(im)) {
        if (x0 > x1)
            tmp = x0, x0 = x1, x1 = tmp;
        if (x0 < 0)
            x0 = 0;
        else if (x0 >= dib_Width(im))
            return;
        if (x1 < 0)
            return;
        else if (x1 >= dib_Width(im))
            x1 = dib_Width(im)-1;
        if (x0 <= x1)
            memset(buffer + SCANL(y0, im) + x0, colour[0], x1 - x0 + 1);
    }
}


static inline void
hline24(pdib im, int x0, int y0, int x1, UBYTE* colour)
{
    int tmp;
    UBYTE* buffer = dib_Image(im);
    UBYTE* p;

    if (y0 >= 0 && y0 < dib_Height(im)) {
        if (x0 > x1)
            tmp = x0, x0 = x1, x1 = tmp;
        if (x0 < 0)
            x0 = 0;
        else if (x0 >= dib_Width(im))
            return;
        if (x1 < 0)
            return;
        else if (x1 >= dib_Width(im))
            x1 = dib_Width(im)-1;
        p = buffer + SCANL(y0, im) + x0*3;
        while (x0 <= x1) {
            p[0]  = colour[0];
            p[1]  = colour[1];
            p[2]  = colour[2];
            x0++; p += 3;
        }
    }
}

static inline void
hline32xx(pdib im, int x0, int y0, int x1, UBYTE* colour)
{
    int tmp;
    UBYTE* buffer = dib_Image(im);
    UBYTE* p;

    if (y0 >= 0 && y0 < dib_Height(im)) {
        if (x0 > x1)
            tmp = x0, x0 = x1, x1 = tmp;
        if (x0 < 0)
            x0 = 0;
        else if (x0 >= dib_Width(im))
            return;
        if (x1 < 0)
            return;
        else if (x1 >= dib_Width(im))
            x1 = dib_Width(im)-1;
        p = buffer + SCANL(y0, im) + x0*4;
        while (x0 <= x1) {
            p[0]  = colour[0];
            p[1]  = colour[1];
            p[2]  = colour[2];
            p[3]  = colour[3];
            x0++; p += 4;
        }
    }
}


static inline void
hline32(pdib im, int x0, int y0, int x1, UBYTE* colour)
{
    int tmp;
    UBYTE* buffer = dib_Image(im);
    UDWORD* p;

    if (y0 >= 0 && y0 < dib_Height(im)) {
        if (x0 > x1)
            tmp = x0, x0 = x1, x1 = tmp;
        if (x0 < 0)
            x0 = 0;
        else if (x0 >= dib_Width(im))
            return;
        if (x1 < 0)
            return;
        else if (x1 >= dib_Width(im))
            x1 = dib_Width(im)-1;
        p = (UDWORD*) ((UBYTE*)buffer + SCANL(y0, im));
        while (x0 <= x1)
            p[x0++] = *((UDWORD*)colour);
    }
}

static inline void
hline32rgba(pdib im, int x0, int y0, int x1, UBYTE* colour)
{
    int tmp;
    unsigned int tmp1, tmp2;
    UBYTE* buffer = dib_Image(im);

    if (y0 >= 0 && y0 < dib_Height(im)) {
        if (x0 > x1)
            tmp = x0, x0 = x1, x1 = tmp;
        if (x0 < 0)
            x0 = 0;
        else if (x0 >= dib_Width(im))
            return;
        if (x1 < 0)
            return;
        else if (x1 >= dib_Width(im))
            x1 = dib_Width(im)-1;
        if (x0 <= x1) {
            UBYTE* out = buffer + SCANL(y0, im) + x0*4;
            UBYTE* in = colour;
            while (x0 <= x1) {
                out[0] = BLEND(in[3], out[0], in[0], tmp1, tmp2);
                out[1] = BLEND(in[3], out[1], in[1], tmp1, tmp2);
                out[2] = BLEND(in[3], out[2], in[2], tmp1, tmp2);
                x0++; out += 4;
            }
        }
    }
}

static inline void
line8(pdib im, int x0, int y0, int x1, int y1, UBYTE* ink)
{
    int i, n, e;
    int dx, dy;
    int xs, ys;

    /* normalize coordinates */
    dx = x1-x0;
    if (dx < 0)
        dx = -dx, xs = -1;
    else
        xs = 1;
    dy = y1-y0;
    if (dy < 0)
        dy = -dy, ys = -1;
    else
        ys = 1;

    n = (dx > dy) ? dx : dy;

    if (dx == 0)

        /* vertical */
        for (i = 0; i < dy; i++) {
            point8(im, x0, y0, ink);
            y0 += ys;
        }

    else if (dy == 0)

        /* horizontal */
        for (i = 0; i < dx; i++) {
            point8(im, x0, y0, ink);
            x0 += xs;
        }

    else if (dx > dy) {

        /* bresenham, horizontal slope */
        n = dx;
        dy += dy;
        e = dy - dx;
        dx += dx;

        for (i = 0; i < n; i++) {
            point8(im, x0, y0, ink);
            if (e >= 0) {
                y0 += ys;
                e -= dx;
            }
            e += dy;
            x0 += xs;
        }

    } else {

        /* bresenham, vertical slope */
        n = dy;
        dx += dx;
        e = dx - dy;
        dy += dy;

        for (i = 0; i < n; i++) {
            point8(im, x0, y0, ink);
            if (e >= 0) {
                x0 += xs;
                e -= dy;
            }
            e += dx;
            y0 += ys;
        }

    }
}



static inline void
line24(pdib im, int x0, int y0, int x1, int y1, UBYTE* ink)
{
    int i, n, e;
    int dx, dy;
    int xs, ys;

    /* normalize coordinates */
    dx = x1-x0;
    if (dx < 0)
        dx = -dx, xs = -1;
    else
        xs = 1;
    dy = y1-y0;
    if (dy < 0)
        dy = -dy, ys = -1;
    else
        ys = 1;

    n = (dx > dy) ? dx : dy;

    if (dx == 0)

        /* vertical */
        for (i = 0; i < dy; i++) {
            point24(im, x0, y0, ink);
            y0 += ys;
        }

    else if (dy == 0)

        /* horizontal */
        for (i = 0; i < dx; i++) {
            point24(im, x0, y0, ink);
            x0 += xs;
        }

    else if (dx > dy) {

        /* bresenham, horizontal slope */
        n = dx;
        dy += dy;
        e = dy - dx;
        dx += dx;

        for (i = 0; i < n; i++) {
            point24(im, x0, y0, ink);
            if (e >= 0) {
                y0 += ys;
                e -= dx;
            }
            e += dy;
            x0 += xs;
        }

    } else {

        /* bresenham, vertical slope */
        n = dy;
        dx += dx;
        e = dx - dy;
        dy += dy;

        for (i = 0; i < n; i++) {
            point24(im, x0, y0, ink);
            if (e >= 0) {
                x0 += xs;
                e -= dy;
            }
            e += dx;
            y0 += ys;
        }

    }
}


static inline void
line32(pdib im, int x0, int y0, int x1, int y1, UBYTE* ink)
{
    int i, n, e;
    int dx, dy;
    int xs, ys;

    /* normalize coordinates */
    dx = x1-x0;
    if (dx < 0)
        dx = -dx, xs = -1;
    else
        xs = 1;
    dy = y1-y0;
    if (dy < 0)
        dy = -dy, ys = -1;
    else
        ys = 1;

    n = (dx > dy) ? dx : dy;

    if (dx == 0)

        /* vertical */
        for (i = 0; i < dy; i++) {
            point32(im, x0, y0, ink);
            y0 += ys;
        }

    else if (dy == 0)

        /* horizontal */
        for (i = 0; i < dx; i++) {
            point32(im, x0, y0, ink);
            x0 += xs;
        }

    else if (dx > dy) {

        /* bresenham, horizontal slope */
        n = dx;
        dy += dy;
        e = dy - dx;
        dx += dx;

        for (i = 0; i < n; i++) {
            point32(im, x0, y0, ink);
            if (e >= 0) {
                y0 += ys;
                e -= dx;
            }
            e += dy;
            x0 += xs;
        }

    } else {

        /* bresenham, vertical slope */
        n = dy;
        dx += dx;
        e = dx - dy;
        dy += dy;

        for (i = 0; i < n; i++) {
            point32(im, x0, y0, ink);
            if (e >= 0) {
                x0 += xs;
                e -= dy;
            }
            e += dx;
            y0 += ys;
        }

    }
}

static inline void
line32rgba(pdib im, int x0, int y0, int x1, int y1, UBYTE* ink)
{
    int i, n, e;
    int dx, dy;
    int xs, ys;

    /* normalize coordinates */
    dx = x1-x0;
    if (dx < 0)
        dx = -dx, xs = -1;
    else
        xs = 1;
    dy = y1-y0;
    if (dy < 0)
        dy = -dy, ys = -1;
    else
        ys = 1;

    n = (dx > dy) ? dx : dy;

    if (dx == 0)

        /* vertical */
        for (i = 0; i < dy; i++) {
            point32rgba(im, x0, y0, ink);
            y0 += ys;
        }

    else if (dy == 0)

        /* horizontal */
        for (i = 0; i < dx; i++) {
            point32rgba(im, x0, y0, ink);
            x0 += xs;
        }

    else if (dx > dy) {

        /* bresenham, horizontal slope */
        n = dx;
        dy += dy;
        e = dy - dx;
        dx += dx;

        for (i = 0; i < n; i++) {
            point32rgba(im, x0, y0, ink);
            if (e >= 0) {
                y0 += ys;
                e -= dx;
            }
            e += dy;
            x0 += xs;
        }

    } else {

        /* bresenham, vertical slope */
        n = dy;
        dx += dx;
        e = dx - dy;
        dy += dy;

        for (i = 0; i < n; i++) {
            point32rgba(im, x0, y0, ink);
            if (e >= 0) {
                x0 += xs;
                e -= dy;
            }
            e += dx;
            y0 += ys;
        }

    }
}

static int
x_cmp(const void *x0, const void *x1)
{
    float diff = *((float*)x0) - *((float*)x1);
    if (diff < 0)
        return -1;
    else if (diff > 0)
        return 1;
    else
        return 0;
}

static inline int
polygon8(pdib im, int n, Edge *e, UBYTE* ink, int eofill)
{
    int i, j;
    float *xx;
    int ymin, ymax;
    float y;

    if (n <= 0)
        return 0;

    /* Find upper and lower polygon boundary (within image) */

    ymin = e[0].ymin;
    ymax = e[0].ymax;
    for (i = 1; i < n; i++) {
        if (e[i].ymin < ymin) ymin = e[i].ymin;
        if (e[i].ymax > ymax) ymax = e[i].ymax;
    }

    if (ymin < 0)
        ymin = 0;
    if (ymax >= dib_Height(im))
        ymax = dib_Height(im)-1;

    /* Process polygon edges */

    xx = malloc(n * sizeof(float));
    if (!xx)
        return -1;

    for (;ymin <= ymax; ymin++) {
        y = ymin+0.5F;
        for (i = j = 0; i < n; i++) 
            if (y >= e[i].ymin && y <= e[i].ymax) {
                if (e[i].d == 0)
                    hline8(im, e[i].xmin, ymin, e[i].xmax, ink);
                else
                    xx[j++] = (y-e[i].y0) * e[i].dx + e[i].x0;
            }
        if (j == 2) {
            if (xx[0] < xx[1])
                hline8(im, CEIL(xx[0]-0.5), ymin, FLOOR(xx[1]+0.5), ink);
            else
                hline8(im, CEIL(xx[1]-0.5), ymin, FLOOR(xx[0]+0.5), ink);
        } else {
            qsort(xx, j, sizeof(float), x_cmp);
            for (i = 0; i < j-1 ; i += 2)
                hline8(im, CEIL(xx[i]-0.5), ymin, FLOOR(xx[i+1]+0.5), ink);
        }
    }

    free(xx);

    return 0;
}

static inline int
polygon24(pdib im, int n, Edge *e, UBYTE* ink, int eofill)
{
    int i, j;
    float *xx;
    int ymin, ymax;
    float y;

    if (n <= 0)
        return 0;

    /* Find upper and lower polygon boundary (within image) */

    ymin = e[0].ymin;
    ymax = e[0].ymax;
    for (i = 1; i < n; i++) {
        if (e[i].ymin < ymin) ymin = e[i].ymin;
        if (e[i].ymax > ymax) ymax = e[i].ymax;
    }

    if (ymin < 0)
        ymin = 0;
    if (ymax >= dib_Height(im))
        ymax = dib_Height(im)-1;

    /* Process polygon edges */

    xx = malloc(n * sizeof(float));
    if (!xx)
        return -1;

    for (;ymin <= ymax; ymin++) {
        y = ymin+0.5F;
        for (i = j = 0; i < n; i++) {
            if (y >= e[i].ymin && y <= e[i].ymax) {
                if (e[i].d == 0)
                    hline24(im, e[i].xmin, ymin, e[i].xmax, ink);
                else
                    xx[j++] = (y-e[i].y0) * e[i].dx + e[i].x0;
            }
        }
        if (j == 2) {
            if (xx[0] < xx[1])
                hline24(im, CEIL(xx[0]-0.5), ymin, FLOOR(xx[1]+0.5), ink);
            else
                hline24(im, CEIL(xx[1]-0.5), ymin, FLOOR(xx[0]+0.5), ink);
        } else {
            qsort(xx, j, sizeof(float), x_cmp);
            for (i = 0; i < j-1 ; i += 2)
                hline24(im, CEIL(xx[i]-0.5), ymin, FLOOR(xx[i+1]+0.5), ink);
        }
    }

    free(xx);

    return 0;
}


static inline int
polygon32(pdib im, int n, Edge *e, UBYTE* ink, int eofill)
{
    int i, j;
    float *xx;
    int ymin, ymax;
    float y;

    if (n <= 0)
        return 0;

    /* Find upper and lower polygon boundary (within image) */

    ymin = e[0].ymin;
    ymax = e[0].ymax;
    for (i = 1; i < n; i++) {
        if (e[i].ymin < ymin) ymin = e[i].ymin;
        if (e[i].ymax > ymax) ymax = e[i].ymax;
    }

    if (ymin < 0)
        ymin = 0;
    if (ymax >= dib_Height(im))
        ymax = dib_Height(im)-1;

    /* Process polygon edges */

    xx = malloc(n * sizeof(float));
    if (!xx)
        return -1;

    for (;ymin <= ymax; ymin++) {
        y = ymin+0.5F;
        for (i = j = 0; i < n; i++) {
            if (y >= e[i].ymin && y <= e[i].ymax) {
                if (e[i].d == 0)
                    hline32(im, e[i].xmin, ymin, e[i].xmax, ink);
                else
                    xx[j++] = (y-e[i].y0) * e[i].dx + e[i].x0;
            }
        }
        if (j == 2) {
            if (xx[0] < xx[1])
                hline32(im, CEIL(xx[0]-0.5), ymin, FLOOR(xx[1]+0.5), ink);
            else
                hline32(im, CEIL(xx[1]-0.5), ymin, FLOOR(xx[0]+0.5), ink);
        } else {
            qsort(xx, j, sizeof(float), x_cmp);
            for (i = 0; i < j-1 ; i += 2)
                hline32(im, CEIL(xx[i]-0.5), ymin, FLOOR(xx[i+1]+0.5), ink);
        }
    }

    free(xx);

    return 0;
}

static inline int
polygon32rgba(pdib im, int n, Edge *e, UBYTE* ink, int eofill)
{
    int i, j;
    float *xx;
    int ymin, ymax;
    float y;

    if (n <= 0)
        return 0;

    /* Find upper and lower polygon boundary (within image) */

    ymin = e[0].ymin;
    ymax = e[0].ymax;
    for (i = 1; i < n; i++) {
        if (e[i].ymin < ymin) ymin = e[i].ymin;
        if (e[i].ymax > ymax) ymax = e[i].ymax;
    }

    if (ymin < 0)
        ymin = 0;
    if (ymax >= dib_Height(im))
        ymax = dib_Height(im)-1;

    /* Process polygon edges */

    xx = malloc(n * sizeof(float));
    if (!xx)
        return -1;

    for (;ymin <= ymax; ymin++) {
        y = ymin+0.5F;
        for (i = j = 0; i < n; i++) {
            if (y >= e[i].ymin && y <= e[i].ymax) {
                if (e[i].d == 0)
                    hline32rgba(im, e[i].xmin, ymin, e[i].xmax, ink);
                else
                    xx[j++] = (y-e[i].y0) * e[i].dx + e[i].x0;
            }
        }
        if (j == 2) {
            if (xx[0] < xx[1])
                hline32rgba(im, CEIL(xx[0]-0.5), ymin, FLOOR(xx[1]+0.5), ink);
            else
                hline32rgba(im, CEIL(xx[1]-0.5), ymin, FLOOR(xx[0]+0.5), ink);
        } else {
            qsort(xx, j, sizeof(float), x_cmp);
            for (i = 0; i < j-1 ; i += 2)
                hline32rgba(im, CEIL(xx[i]-0.5), ymin, FLOOR(xx[i+1]+0.5), ink);
        }
    }

    free(xx);

    return 0;
}

static inline void
add_edge(Edge *e, int x0, int y0, int x1, int y1)
{
    /* printf("edge %d %d %d %d\n", x0, y0, x1, y1); */

    if (x0 <= x1)
        e->xmin = x0, e->xmax = x1;
    else
        e->xmin = x1, e->xmax = x0;

    if (y0 <= y1)
        e->ymin = y0, e->ymax = y1;
    else
        e->ymin = y1, e->ymax = y0;
    
    if (y0 == y1) {
        e->d = 0;
        e->dx = 0.0;
    } else {
        e->dx = ((float)(x1-x0)) / (y1-y0);
        if (y0 == e->ymin)
            e->d = 1;
        else
            e->d = -1;
    }

    e->x0 = x0;
    e->y0 = y0;
}

typedef struct {
    void (*point)(pdib im, int x, int y, UBYTE* ink);
    void (*hline)(pdib im, int x0, int y0, int x1, UBYTE* ink);
    void (*line)(pdib im, int x0, int y0, int x1, int y1, UBYTE* ink);
    int (*polygon)(pdib im, int n, Edge *e, UBYTE* ink, int eofill);
} DRAW;

DRAW draw8 = { point8,  hline8,  line8,  polygon8 };
DRAW draw24 = { point24, hline24, line24, polygon24 };
DRAW draw32 = { point32, hline32, line32, polygon32 };
DRAW draw32rgba = { point32rgba, hline32rgba, line32rgba, polygon32rgba };

/* -------------------------------------------------------------------- */
/* Interface                                                            */
/* -------------------------------------------------------------------- */

/*! \brief Marco which selects which version of the draw fucntions to use */
#define DRAWINIT()\
    if (dib_Bitcount(im) == 8) {\
        draw = &draw8;\
        ink = (UBYTE*)(ink_);\
    } else if (dib_Bitcount(im) == 24) {\
        draw = &draw24;\
        ink = (UBYTE*)(ink_);\
    } else if (dib_Bitcount(im) == 32) {\
        draw = (op) ? &draw32rgba : &draw32;\
        ink = (UBYTE*)(ink_);\
    } else {\
      printf("Error in Imaging: Unsupported image bit-count: %d \n", dib_Bitcount(im));\
      assert(0);\
    }

int
ImagingDrawPoint(pdib im, int x0, int y0, const void* ink_, int op)
{
    DRAW* draw;
    UBYTE* ink;

    DRAWINIT();

    draw->point(im, x0, y0, ink);

    return 0;
}

int
ImagingDrawLine(pdib im, int x0, int y0, int x1, int y1, const void* ink_,
                int op)
{
    DRAW* draw;
    UBYTE* ink;

    DRAWINIT();

    draw->line(im, x0, y0, x1, y1, ink);

    return 0;
}

int
ImagingDrawWideLine(pdib im, int x0, int y0, int x1, int y1,
                    const void* ink_, int width, int op)
{
    DRAW* draw;
    UBYTE* ink;

    Edge e[4];

    int dx, dy;
    double d;

    DRAWINIT();

    if (width <= 1) {
        draw->line(im, x0, y0, x1, y1, ink);
        return 0;
    }

    dx = x1-x0;
    dy = y1-y0;

    if (dx == 0 && dy == 0) {
        draw->point(im, x0, y0, ink);
        return 0;
    }

    d = width / sqrt((float) (dx*dx + dy*dy)) / 2.0;

    dx = (int) floor(d * (y1-y0) + 0.5);
    dy = (int) floor(d * (x1-x0) + 0.5);


    add_edge(e+0, x0 - dx,  y0 + dy, x1 - dx,  y1 + dy);
    add_edge(e+1, x1 - dx,  y1 + dy, x1 + dx,  y1 - dy);
    add_edge(e+2, x1 + dx,  y1 - dy, x0 + dx,  y0 - dy);
    add_edge(e+3, x0 + dx,  y0 - dy, x0 - dx,  y0 + dy);
    
    draw->polygon(im, 4, e, ink, 0);

    return 0;
}

int
ImagingDrawRectangle(pdib im, int x0, int y0, int x1, int y1,
                     const void* ink_, int fill, int op)
{
    int y;
    int tmp;
    DRAW* draw;
    UBYTE* ink;

    DRAWINIT();

    if (y0 > y1)
        tmp = y0, y0 = y1, y1 = tmp;

    if (fill) {

        if (y0 < 0)
            y0 = 0;
        else if (y0 >= dib_Height(im))
            return 0;

        if (y1 < 0)
            return 0;
        else if (y1 > dib_Height(im))
            y1 = dib_Height(im);

        for (y = y0; y <= y1; y++)
            draw->hline(im, x0, y, x1, ink);

    } else {

        /* outline */
        draw->line(im, x0, y0, x1, y0, ink);
        draw->line(im, x1, y0, x1, y1, ink);
        draw->line(im, x1, y1, x0, y1, ink);
        draw->line(im, x0, y1, x0, y0, ink);

    }

    return 0;
}

int
ImagingDrawPolygon(pdib im, int count, int* xy, const void* ink_,
                   int fill, int op)
{
    int i, n;
    DRAW* draw;
    UBYTE* ink;

    if (count <= 0)
        return 0;

    DRAWINIT();

    if (fill) {

        /* Build edge list */
        Edge* e = malloc(count * sizeof(Edge));
        if (!e) {
            (void) ImagingError_MemoryError();
            return -1;
        }
        for (i = n = 0; i < count-1; i++)
            add_edge(&e[n++], xy[i+i], xy[i+i+1], xy[i+i+2], xy[i+i+3]);
        if (xy[i+i] != xy[0] || xy[i+i+1] != xy[1])
            add_edge(&e[n++], xy[i+i], xy[i+i+1], xy[0], xy[1]);
        draw->polygon(im, n, e, ink, 0);
        free(e);

    } else {

        /* Outline */
        for (i = 0; i < count-1; i++)
            draw->line(im, xy[i+i], xy[i+i+1], xy[i+i+2], xy[i+i+3], ink);
        draw->line(im, xy[i+i], xy[i+i+1], xy[0], xy[1], ink);

    }
        
    return 0;
}

int
ImagingDrawBitmap(pdib im, int x0, int y0, pdib bitmap, const void* ink,
                  int op)
{
   int width  = dib_Width(im);
   int height = dib_Height(im);
   int storage_width = dib_StorageWidth(im);
   int depth = dib_Bitcount(im);
   unsigned char* buffer = dib_Image(im);

   int widthS  = dib_Width(bitmap);
   int heightS = dib_Height(bitmap);
   int storage_widthS = dib_StorageWidth(bitmap);
   int depthS = dib_Bitcount(bitmap);
   unsigned char* bufferS = dib_Image(bitmap);
   int x, y; /* destination counters */
   int v, w; /* source counters */

   if (depthS != depth)
   {
      printf("Error in ImagingDrawBitmap. ");
      printf("Source depth: %d not eqaul to destination depth %d\n", 
              depthS, depth);
      return -1;
   }

   if (depthS != 24 && depthS != 32)
   {
      printf("Error in ImagingDrawBitmap. ");
      printf("Source depth: %d is not supported.\n", depthS);
      return -1;
   }

   int byte_count = depth/8;
   for (y=y0, w=0; y < height && w < heightS; y++, w++)
   {
      int scanline  = y * storage_width;
      int scanlineS = w * storage_widthS;
      for (x=x0, v=0; x < width && v < widthS; x++, v++)
      {
         if (byte_count == 3)
         {
            int offset  = x*byte_count;
            int offsetS = v*byte_count;
            buffer[ scanline + offset+0]  = bufferS[ scanlineS + offsetS+0];
            buffer[ scanline + offset+1]  = bufferS[ scanlineS + offsetS+1];
            buffer[ scanline + offset+2]  = bufferS[ scanlineS + offsetS+2];
         }
         else if (byte_count == 4)
         {
            if (op == 0)
            {
               int offset  = x*byte_count;
               int offsetS = v*byte_count;
               buffer[ scanline + offset+0]  = bufferS[ scanlineS + offsetS+0];
               buffer[ scanline + offset+1]  = bufferS[ scanlineS + offsetS+1];
               buffer[ scanline + offset+2]  = bufferS[ scanlineS + offsetS+2];
               buffer[ scanline + offset+3]  = bufferS[ scanlineS + offsetS+3];
            }
            else if (op == 1)
            {
               unsigned int tmp1, tmp2;
               UBYTE* out = buffer  + SCANL(y, im) + x*4;
               UBYTE* in =  bufferS + SCANL(w, bitmap) + v*4;
               out[0] = BLEND(in[3], out[0], in[0], tmp1, tmp2);
               out[1] = BLEND(in[3], out[1], in[1], tmp1, tmp2);
               out[2] = BLEND(in[3], out[2], in[2], tmp1, tmp2);
            }
            else
            {
               int offset  = x*byte_count;
               int offsetS = v*byte_count;
               buffer[ scanline + offset+0]  = bufferS[ scanlineS + offsetS+0];
               buffer[ scanline + offset+1]  = bufferS[ scanlineS + offsetS+1];
               buffer[ scanline + offset+2]  = bufferS[ scanlineS + offsetS+2];
               buffer[ scanline + offset+3]  = op;
              
            }
         }
      }
   }
   return 0; 
}

/* -------------------------------------------------------------------- */
/* standard shapes */

#define ARC 0
#define CHORD 1
#define PIESLICE 2

static int
ellipse(pdib im, int x0, int y0, int x1, int y1,
        int start, int end, const void* ink_, int fill,
        int mode, int op)
{
    int i, n;
    int cx, cy;
    int w, h;
    int x = 0, y = 0;
    int lx = 0, ly = 0;
    int sx = 0, sy = 0;
    DRAW* draw;
    UBYTE* ink;

    w = x1 - x0;
    h = y1 - y0;
    if (w < 0 || h < 0)
        return 0;

    DRAWINIT();

    cx = (x0 + x1) / 2;
    cy = (y0 + y1) / 2;

    while (end < start)
        end += 360;

    if (mode != ARC && fill) {

        /* Build edge list */
        Edge* e = malloc((end - start + 3) * sizeof(Edge));
        if (!e) {
            ImagingError_MemoryError();
            return -1;
        }

        n = 0;

        for (i = start; i <= end; i++) {
            x = FLOOR((cos(i*M_PI/180) * w/2) + cx + 0.5);
            y = FLOOR((sin(i*M_PI/180) * h/2) + cy + 0.5);
            if (i != start)
                add_edge(&e[n++], lx, ly, x, y);
            else
                sx = x, sy = y;
            lx = x, ly = y;
        }

        if (n > 0) {
            /* close and draw polygon */
            if (mode == PIESLICE) {
                if (x != cx || y != cy) {
                    add_edge(&e[n++], x, y, cx, cy);
                    add_edge(&e[n++], cx, cy, sx, sy);
                }
            } else {
                if (x != sx || y != sy)
                    add_edge(&e[n++], x, y, sx, sy);
            }
            draw->polygon(im, n, e, ink, 0);
        }

        free(e);
        
    } else {

        for (i = start; i <= end; i++) {
            x = FLOOR((cos(i*M_PI/180) * w/2) + cx + 0.5);
            y = FLOOR((sin(i*M_PI/180) * h/2) + cy + 0.5);
            if (i != start)
                draw->line(im, lx, ly, x, y, ink);
            else
                sx = x, sy = y;
            lx = x, ly = y;
        }

        if (i != start) {
            if (mode == PIESLICE) {
                if (x != cx || y != cy) {
                    draw->line(im, x, y, cx, cy, ink);
                    draw->line(im, cx, cy, sx, sy, ink);
                }
            } else if (mode == CHORD) {
                if (x != sx || y != sy)
                    draw->line(im, x, y, sx, sy, ink);
            }
        }
    }

    return 0;
}

int
ImagingDrawArc(pdib im, int x0, int y0, int x1, int y1,
               int start, int end, const void* ink, int op)
{
    return ellipse(im, x0, y0, x1, y1, start, end, ink, 0, ARC, op);
}

int
ImagingDrawChord(pdib im, int x0, int y0, int x1, int y1,
               int start, int end, const void* ink, int fill, int op)
{
    return ellipse(im, x0, y0, x1, y1, start, end, ink, fill, CHORD, op);
}

int
ImagingDrawEllipse(pdib im, int x0, int y0, int x1, int y1,
                   const void* ink, int fill, int op)
{
    return ellipse(im, x0, y0, x1, y1, 0, 360, ink, fill, CHORD, op);
}

int
ImagingDrawPieslice(pdib im, int x0, int y0, int x1, int y1,
                    int start, int end, const void* ink, int fill, int op)
{
    return ellipse(im, x0, y0, x1, y1, start, end, ink, fill, PIESLICE, op);
}

/* -------------------------------------------------------------------- */

