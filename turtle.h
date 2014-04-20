/*
 * turtle.h
 *
 *  Created on: 7 Mar 2014
 *      Author: Jon
 */

#ifndef TURTLE_H_
#define TURTLE_H_

extern "C" {
#include "dib.h"
#include "draw.h"
#include "dib_Fonts.h"
}


/* \brief Turtle data record
 */
struct Turtle
{
   double x, y;  /**< turtle's position */
   double angle; /**< turtle's orientation */
   bool pen;     /**< if 0 pen up otherwise pen down */
   pdib image;   /**< DIB surface to draw on */
};

void Turtle_clear(Turtle *t);

void Turtle_init(Turtle *t, pdib im, double x, double y, double angle, bool pen);

void Turtle_pendown(Turtle *t);

void Turtle_penup(Turtle *t);

void Turtle_turn(Turtle *t, double theta);

void Turtle_move(Turtle *t, double step, UBYTE* colour);

double radians2degrees(double v);

void three_segment_koch_curve(Turtle *t,
                              double scale,   /**< [in] Initiator's length */
                              int depth,      /**< [in] number of recursive steps so far */
                              UBYTE *colour);


#endif /* TURTLE_H_ */
