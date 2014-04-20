/** \file Turtle.cpp 
 * Turle graphics
 */

//
//g++ -c -g turtle.cpp  -o main.o
//gcc -c -g dib.c       -o dib.o
//gcc -c -g draw.c      -o draw.o
//gcc -c -g dib_Fonts.c -o dib_Fronts.o
//g++ -o turtle main.o dib.o draw.o dib_Fronts.o -lm
//
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "turtle.h"

extern "C" {
#include "dib.h"
#include "draw.h"
#include "dib_Fonts.h"
}

//struct Turtle
//{
//   double x, y;  /**< turtle's position */
//   double angle; /**< turtle's orientation */
//   bool pen;     /**< if 0 pen up otherwise pen down */
//   pdib image;   /**< DIB surface to draw on */
//};


/*! \brief Turtle clear all struct fields 
 * @param t [in/out] pointer/reference to an instance of a Turtle
 *
 * \note you have to pass the turle instance by reference and 
 *       on Exit all of the Turtle's data fields are set to 0
 */
void Turtle_clear(Turtle *t) 
{ 
   t->x=t->y=t->angle=0.0; 
   t->pen=false;
   t->image = NULL;
}

/*! \brief Turtle initialisation 
 *
 * @param t [in/out] pointer/reference to an instance of a Turtle
 * @param im    [in] pointer to DIB/BMP image data
 * @param x,y   [in] the Turtle's position
 * @param angle [in] orientation in degress
 * @param pen   [in] Turtle's pen is up if pen is false, 
 *                   other wise pen is down
 * \note you have to pass the turle instance by reference and 
 *       on Exit all of the Turtle's data fields are intialise
 *       to the corresponding parameter values.
 */
void Turtle_init(Turtle *t, pdib im, double x, double y, double angle, bool pen) 
{
      t->image = im;
      t->x = x; 
      t->y = y;
      t->angle = angle;
      t->pen = pen;
}

/*! \brief Turtle's pen is set to down 
 *
 * @param t [in/out] pointer/reference to an instance of a Turtle
 */
void Turtle_pendown(Turtle *t) 
{
   t->pen=true;
}

/*! \brief Turtle's pen is set to up 
 *
 * @param t [in/out] pointer/reference to an instance of a Turtle
 */
void Turtle_penup(Turtle *t) 
{
   t->pen=false;
}

/*! \brief Turtle turns 'theta' degress
 *
 * @param t [in/out] pointer/reference to an instance of a Turtle
 * @param theta [in] change orientation by 'theta' degress
 *
 * \note you have to pass the turle instance by reference and 
 *       on Exit all of the Turtle's data orientatin has changed.
 *
 */
void Turtle_turn(Turtle *t, double theta) {
      t->angle +=theta;
      t->angle = fmod(t->angle, 360);
}

/* return the closest integer 
static inline int round(double v) {
   return ((v - floor(v)) > 0.5) ? (int)ceil(v): (int)floor(v);
}
*/

/*! \brief Turtle moves 'step' length
 *
 * @param t [in/out] pointer/reference to an instance of a Turtle
 * @param steps [in] move 'step' length towards orientation 
 *
 * \note you have to pass the turle instance by reference and 
 *       on Exit all of the Turtle's position has changed
 *
 */
void Turtle_move(Turtle *t, double step, UBYTE* colour) 
{      
      const double pi = 3.1415926535;
      double theta = t->angle*pi/180.0;  /* convert to radians */
      double px = step *  cos(theta);
      double py = step * -sin(theta);
      /* draw */
      if (t->pen == true) 
      {
         ImagingDrawLine(t->image,
      	    round(t->x),    round(t->y), 
             round(t->x+px), round(t->y+py),
             colour, 0);
      }
      t->x += px;
      t->y += py;
}

/* \brief converts degree to radians
 * 
 * @param t [in] degrees
 *
 * @return radians
 */
double radians2degrees(double v) {
   const double pi = 3.1415926535;
   return v*180.0/pi;
}


/*! \brief Draws three-segment Koch curve\brief
 *
 * @param t [in/out] pointer/reference to an instance of a Turtle
 *
 * \note see Minkowski Sausage at :
 *       http://mathworld.wolfram.com/MinkowskiSausage.html
 */
void three_segment_koch_curve(Turtle *t,
                              double scale,   /**< [in] Initiator's length */
                              int depth,      /**< [in] number of recursive steps so far */
                              UBYTE *colour)  /**< [in] colour of the pen */
{   
   double theta = radians2degrees(atan(0.5));
   scale *= sin(atan(0.5));
   if (depth<=1) 
   {
      Turtle_turn(t, theta);
      Turtle_move(t, scale, colour);
      Turtle_turn(t, -90);
      Turtle_move(t, scale, colour);
      Turtle_turn(t, 90);
      Turtle_move(t, scale, colour);
      Turtle_turn(t, -theta);

   } 
   else 
   {
      Turtle_turn(t, theta);
      three_segment_koch_curve(t, scale, depth-1, colour);
      Turtle_turn(t, -90);
      three_segment_koch_curve(t, scale, depth-1, colour);
      Turtle_turn(t, 90);
      three_segment_koch_curve(t, scale, depth-1, colour);
      Turtle_turn(t, -theta);

   }
}

//const char* string =
//"Hello World!";
//
//int main()
//{
//   pdib image_0 = dib_Create("ari32.bmp", 512, 512, 32);
//   UBYTE colourText1[] = {0, 0, 255, 255};
//   UBYTE colour0[] = {  0, 0, 0, 255};
//   UBYTE colour1[] = {255, 0, 0, 255};
//
//   dib_ChooseFont(0);
//   dib_DrawString(image_0, string, 512/2, 512/2, colourText1);
//
//   const double pi = 3.1415926535;
//   double hypot = 512;
//   double theta = atan(0.5)*180.0/pi;
//   double sinTheta = sin(atan(0.5));
//   double scale = sinTheta * hypot;
//
//   Turtle c1;
//   Turtle *t = &c1;
//   Turtle_init(t,
//          image_0,
//          10, 80, /* initial position    */
//          -45,    /* initial orientation */
//          0);     /* the pen is down     */
//
//   Turtle_pendown(t);
//   Turtle_turn(t, theta);
//   Turtle_move(t, scale, colour0);
//   Turtle_turn(t, -90);
//   Turtle_move(t, scale, colour0);
//   Turtle_turn(t, 90);
//   Turtle_move(t, scale, colour0);
//   Turtle_turn(t, -theta);
//   Turtle_penup(t);
//
//   Turtle c2;
//   Turtle_init(&c2, image_0, 10, 80, -45, 1);
//   /* Minkowski Sausage
//    * http://mathworld.wolfram.com/MinkowskiSausage.html
//    * */
//   three_segment_koch_curve(&c2, 512, 4, colour1);
//
//   dib_Write(image_0, "ari32.bmp");
//
//   // clean up
//   dib_Destroy(image_0);
//
//   // system("display \"ari32.bmp\"&");
//   system("ari32.bmp");
//   return EXIT_SUCCESS;
//}

