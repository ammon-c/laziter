/*
==================================================================
FILENAME:     tmode.h
DESCRIPTION:  Include file for tmode.c and callers.
AUTHOR:       Ammon R. Campbell
COPYRIGHT:    (C) Copyright 1986 Ammon R. Campbell.
              All rights reserved.
TOOLS:        Microsoft C Compiler version 3.0
NOTE:  Modified 1991 to work with newer versions of MSC.
==================================================================
*/

/* Width and height of screen in 'pixels' */
#define SCREEN_WIDTH    80
#define SCREEN_HEIGHT   50

/* Size of each banner character in 'pixels' */
#define CHAR_WIDTH      6
#define CHAR_HEIGHT     6

/* Screen colors. */
#define CLR_BLACK       0
#define CLR_BLUE        1
#define CLR_GREEN       2
#define CLR_CYAN        3
#define CLR_RED         4
#define CLR_PURPLE      5
#define CLR_ORANGE      6
#define CLR_WHITE       7


/* Data type used for a color/pixel. */
typedef unsigned char COLOR;

/* Data type used for a picture. */
typedef struct
{
   int      width;
   int      height;
   COLOR *  pixels;
} BITM;


/* Public functions: */

/*
** tm_init:
** Initializes the tmode library for use.
** This must be called prior to calling any other
** functions in this module.
**
** Parameters:
**   NONE
**
** Returns:
**      Value   Meaning
**      -----   -------
**      1       Successful.
**      0       Error occured.
*/
int    tm_init(void);

/*
** tm_deinit:
** Shuts down the tmode module.
** This should be called at the end of the program
** if the previous call to tm_init() was successful.
**
** Parameters:
**   NONE
**
** Returns:
**   NONE
*/
void   tm_deinit(void);

/*
** tm_update:
** Copies the contents of the temporary frame to
** the screen.
**
** Parameters:
**   NONE
**
** Returns:
**   NONE
*/
void   tm_update(void);

/*
** tm_clear:
** Clears the temporary frame to a specified color.
**
** Parameters:
**   Name   Description
**   ----   -----------
**   color   Color to draw with.
**
** Returns:
**   NONE
*/
void   tm_clear(COLOR color);

/*
** tm_pixel:
** Draws a pixel to the temporary frame buffer.
**
** Parameters:
**   Name   Description
**   ----   -----------
**   x      Horizontal position of pixel.
**   y      Vertical position of pixel.
**   color  Color to draw with.
**
** Returns:
**   NONE
*/
void   tm_pixel(int x, int y, COLOR color);

/*
** tm_rect:
** Draws a rectangle on the temporary frame buffer.
**
** Parameters:
**   Name   Description
**   ----   -----------
**   x      Horizontal upper-left of rectangle.
**   y      Vertical upper-left of rectangle.
**   width  Width of rectangle in pixels.
**   height Height of rectangle in pixels.
**
** Returns:
**   NONE
*/
void   tm_rect(int x, int y, int width, int height, COLOR color);

/*
** tm_banner:
** Draws banner-size text to the temporary frame buffer.
**
** Parameters:
**   Name   Description
**   ----   -----------
**   x      Horizontal upper-left of drawing area.
**   y      Vertical upper-left of drawing area.
**   color  Color of text to draw.
**   text   String of text characters to be drawn.
**
** Returns:
**   NONE
*/
void   tm_banner(int x, int y, COLOR color, unsigned char *text);

/*
** tm_cbanner:
** Draws banner-size text, centered horizontally on the screen.
**
** Parameters:
**   Name   Description
**   ----   -----------
**   y      Vertical top of drawing area.
**   color  Color of text to draw.
**   text   String of text characters to be drawn.
**
** Returns:
**   NONE
*/
void   tm_cbanner(int y, COLOR color, unsigned char *text);

/*
** tm_text:
** Draws text to the temporary frame buffer.
**
** Parameters:
**   Name   Description
**   ----   -----------
**   x      Horizontal upper-left of drawing area.
**   y      Vertical upper-left of drawing area.
**   color  Color of text to draw.
**   text   String of text characters to be drawn.
**
** Returns:
**   NONE
*/
void   tm_text(int x, int y, COLOR color, unsigned char *text);

/*
** tm_ctext:
** Draws text, centered horizontally on the screen.
**
** Parameters:
**   Name   Description
**   ----   -----------
**   y      Vertical top of drawing area.
**   color  Color of text to draw.
**   text   String of text characters to be drawn.
**
** Returns:
**   NONE
*/
void   tm_ctext(int y, COLOR color, unsigned char *text);

/*
** tm_scanline:
** Copies colors to a portion of a scanline in
** the temporary frame buffer.
**
** Parameters:
**   Name   Description
**   ----   -----------
**   x      Horizontal position of left-end of scanline.
**   y      Vertical position of scanline.
**   width  Width of scanline area to fill in pixels.
**   colors Array of colors to be copied to scanline.
**
** Returns:
**   NONE
*/
void   tm_scanline(int x, int y, int width, COLOR *colors);

/*
** tm_pic:
** Copies a picture to the temporary frame buffer.
**
** Parameters:
**   Name   Description
**   ----   -----------
**   x      Horizontal upper-left of drawing position.
**   y      Vertical upper-left of drawing position.
**   bitm   Pointer to picture to be drawn.
**
** Returns:
**   NONE
*/
void   tm_pic(int x, int y, BITM *bitm);

/*
** tm_pic_trans:
** Copies a picture to the temporary frame buffer,
** except doesn't copy black pixels (they are left
** transparent).
**
** Parameters:
**   Name   Description
**   ----   -----------
**   x      Horizontal upper-left of drawing position.
**   y      Vertical upper-left of drawing position.
**   bitm   Pointer to picture to be drawn.
**
** Returns:
**   NONE
*/
void   tm_pic_trans(int x, int y, BITM *bitm);

/*
** tm_clip:
** Specifies a clipping rectangle for drawing operations.
** Any drawing requested outside the specified clipping
** rectangle will not occur.
**
** This applies to pixel, scanline, rectangle, and picture
** operations, but not to text, banner, or clear
** operations.
**
** Parameters:
**   Name   Description
**   ----   -----------
**   x      Horizontal coordinate of left edge of clip rect.
**   y      Vertical coordinate of top edge of clip rect.
**   width  Width of clip rect.
**   heigh  Height of clip rect.
**
** Returns:
**   NONE
*/
void   tm_clip(int x, int y, int width, int height);

/*
** tm_noclip:
** Specifies that drawing by tm_pic() should not be clipped.
**
** Parameters:
**   NONE
**
** Returns:
**   NONE
*/
void   tm_noclip(void);

/* End tmode.h */
