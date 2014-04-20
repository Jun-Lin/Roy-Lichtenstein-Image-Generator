#ifndef __DIB_FONT__
#define __DIB_FONT__

#include "dib.h"

/*! \brief Selects a predefined font for drawing text.
 *
 * @param option [in] there are three valid options:
 *                    0 small text
 *                    1 medium text
 *                    2 large text
 *
 * @return It returns 0 if there are no errors, -1 if ones enter an invalid option.
 *
 * \warning By default the option selected is 0 (small text).
 *
 */
extern int dib_ChooseFont(int option);


/*! \brief Draws a string.
 *
 * @param image  [in] a pointer to a device independent bitmap (pDIB)
 * @param str    [in] a pointer to the string constant whose characters we want to draw
 * @param px,py  [in] bottom left corner of the first character in the string 'str'
 * @param colour [in] array of unsigned bytes storing the colour of the srting
 *
 * @return Given a pointer to a device independent bitmap (pDIB) and a string 
 *         on exit the string is drawn across the bitmap starting at (px py).
 *         It returns 0 if there are no errors, -1 otherwise.
 *
 * \warning Note The the 'image' DIB pointer has to be allocated using a did 
 *          library function, such as `dib_create`.
 *
 */
extern int dib_DrawString(pdib image, const char* str, int px, int py, UBYTE* colour);

/*! \brief Gets string drawing dimentions
 *
 * @param str  [in] a pointer to the string constant whose characters we want to draw
 * @param left [out] stores the length of the string in pixesl. It stores how many pixels 
 *              to the left is the leftmost character drawn from the stating drawing 
 *              position. 
 * @param up   [out] stores the height of the string in pixels. It stores how many pixels 
 *              'up' we have moved from the starting drawing position. 
 *
 * @return Calculating from the startign drawing position, it returns how many pixels we move 
 *         down while drawing the text in string 'str' 
 *
 * \warning Note to calculate the height of the drawn strings we need to add the '*up' and 
 *          '*down' distances. For example, to center a string about the y-coordinate
 *          y_center ones needs to specifiy as the start drawing position y_start as follows:
 *          y_start = y_center - up/2 + down/2
 */
extern int dib_DrawStringSize(const char* str, int* left, int* up);

#endif /* __DIB_FONT__ */
