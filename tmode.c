/*
==================================================================
FILENAME:     tmode.c
DESCRIPTION:  Routines for text-mode games on IBM-PC
AUTHOR:       Ammon R. Campbell
COPYRIGHT:    (C) Copyright 1986 Ammon R. Campbell.
              All rights reserved.
TOOLS:        Microsoft C Compiler version 3.0
NOTE:  Modified 1991 to work with newer versions of MSC.
==================================================================
*/

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <dos.h>

#include "tmode.h"

/**************************** DATA *************************/

/* Array of character maps for banner-size text. */
static struct
{
   unsigned char   code;
   unsigned char   *pixels;
} char_maps[] =
{
   {   ' ', "\
------\
------\
------\
------\
------\
------\
"
   },

   {   'A', "\
-###--\
#---#-\
#####-\
#---#-\
#---#-\
------\
"
   },

   {   'B', "\
####--\
#---#-\
####--\
#---#-\
####--\
------\
"
   },

   {   'C', "\
-###--\
#---#-\
#-----\
#---#-\
-###--\
------\
"
   },

   {   'D', "\
####--\
#---#-\
#---#-\
#---#-\
####--\
------\
"
   },

   {   'E', "\
#####-\
#-----\
####--\
#-----\
#####-\
------\
"
   },

   {   'F', "\
#####-\
#-----\
####--\
#-----\
#-----\
------\
"
   },

   {   'G', "\
-####-\
#-----\
#-###-\
#---#-\
-###--\
------\
"
   },

   {   'H', "\
#---#-\
#---#-\
#####-\
#---#-\
#---#-\
------\
"
   },

   {   'I', "\
-###--\
--#---\
--#---\
--#---\
-###--\
------\
"
   },

   {   'J', "\
---##-\
----#-\
----#-\
#---#-\
-###--\
------\
"
   },

   {   'K', "\
#---#-\
#--#--\
###---\
#--#--\
#---#-\
------\
"
   },

   {   'L', "\
#-----\
#-----\
#-----\
#-----\
#####-\
------\
"
   },

   {   'M', "\
#---#-\
##-##-\
#-#-#-\
#---#-\
#---#-\
------\
"
   },

   {   'N', "\
#---#-\
##--#-\
#-#-#-\
#--##-\
#---#-\
------\
"
   },

   {   'O', "\
-###--\
#---#-\
#---#-\
#---#-\
-###--\
------\
"
   },

   {   'P', "\
####--\
#---#-\
####--\
#-----\
#-----\
------\
"
   },

   {   'Q', "\
-###--\
#---#-\
#-#-#-\
#--#--\
-##-#-\
------\
"
   },

   {   'R', "\
####--\
#---#-\
####--\
#---#-\
#---#-\
------\
"
   },

   {   'S', "\
-####-\
#-----\
-###--\
----#-\
####--\
------\
"
   },

   {   'T', "\
#####-\
--#---\
--#---\
--#---\
--#---\
------\
"
   },

   {   'U', "\
#---#-\
#---#-\
#---#-\
#---#-\
-###--\
------\
"
   },

   {   'V', "\
#---#-\
#---#-\
#---#-\
-#-#--\
--#---\
------\
"
   },

   {   'W', "\
#---#-\
#---#-\
#-#-#-\
##-##-\
#---#-\
------\
"
   },

   {   'X', "\
#---#-\
-#-#--\
--#---\
-#-#--\
#---#-\
------\
"
   },

   {   'Y', "\
#---#-\
-#-#--\
--#---\
--#---\
--#---\
------\
"
   },

   {   'Z', "\
#####-\
---#--\
--#---\
-#----\
#####-\
------\
"
   },

#if 0
/* Don't enable the lower-case letters for now.  They don't look very good. */
   {   'a', "\
------\
####--\
-####-\
##--#-\
-####-\
------\
"
   },

   {   'b', "\
#-----\
#-----\
####--\
#---#-\
####--\
------\
"
   },

   {   'c', "\
------\
------\
-###--\
#-----\
-###--\
------\
"
   },

   {   'd', "\
----#-\
----#-\
-####-\
#---#-\
-####-\
------\
"
   },

   {   'e', "\
------\
-###--\
#--##-\
###---\
-####-\
------\
"
   },

   {   'f', "\
--###-\
-#----\
####--\
-#----\
-#----\
------\
"
   },

   {   'g', "\
------\
-####-\
#---#-\
-####-\
----#-\
-###--\
"
   },

   {   'h', "\
#-----\
#-----\
####--\
#---#-\
#---#-\
------\
"
   },

   {   'i', "\
--#---\
------\
--#---\
--#---\
--#---\
------\
"
   },

   {   'j', "\
--#---\
------\
--#---\
--#---\
--#---\
##----\
"
   },

   {   'k', "\
#-----\
#--#--\
###---\
#-#---\
#--#--\
------\
"
   },

   {   'l', "\
#-----\
-#----\
-#----\
-#----\
--#---\
------\
"
   },

   {   'm', "\
------\
------\
####--\
#-#-#-\
#-#-#-\
------\
"
   },

   {   'n', "\
------\
------\
####--\
#---#-\
#---#-\
------\
"
   },

   {   'o', "\
------\
------\
-###--\
#---#-\
-###--\
------\
"
   },

   {   'p', "\
------\
------\
####--\
#---#-\
####--\
#-----\
"
   },

   {   'q', "\
------\
------\
-####-\
#---#-\
-####-\
----#-\
"
   },

   {   'r', "\
------\
------\
-###--\
#-----\
#-----\
------\
"
   },

   {   's', "\
------\
--###-\
-##---\
---##-\
####--\
------\
"
   },

   {   't', "\
--#---\
-###--\
--#---\
--#---\
---#--\
------\
"
   },

   {   'u', "\
------\
------\
#---#-\
#---#-\
-####-\
------\
"
   },

   {   'v', "\
------\
------\
#---#-\
-#-#--\
--#---\
------\
"
   },

   {   'w', "\
------\
------\
#-#-#-\
#-#-#-\
-#-#--\
------\
"
   },

   {   'x', "\
------\
------\
##-##-\
--#---\
##-##-\
------\
"
   },

   {   'y', "\
------\
------\
#---#-\
-#-#--\
--#---\
-#----\
"
   },

   {   'z', "\
------\
####--\
--#---\
-#----\
####--\
------\
"
   },
#endif

   {   '0', "\
-###--\
#---#-\
#-#-#-\
#---#-\
-###--\
------\
"
   },

   {   '1', "\
-##---\
--#---\
--#---\
--#---\
-###--\
------\
"
   },

   {   '2', "\
####--\
----#-\
-###--\
#-----\
#####-\
------\
"
   },

   {   '3', "\
####--\
----#-\
-###--\
----#-\
####--\
------\
"
   },

   {   '4', "\
--#-#-\
-#--#-\
#####-\
----#-\
----#-\
------\
"
   },

   {   '5', "\
####--\
#-----\
####--\
----#-\
####--\
------\
"
   },

   {   '6', "\
-###--\
#-----\
####--\
#---#-\
-###--\
------\
"
   },

   {   '7', "\
#####-\
#---#-\
---#--\
--#---\
--#---\
------\
"
   },

   {   '8', "\
-###--\
#---#-\
-###--\
#---#-\
-###--\
------\
"
   },

   {   '9', "\
-###--\
#---#-\
-####-\
----#-\
-###--\
------\
"
   },

   {   '-', "\
------\
------\
#####-\
------\
------\
------\
"
   },

   {   '+', "\
--#---\
--#---\
#####-\
--#---\
--#---\
------\
"
   },

   {   '.', "\
------\
------\
------\
------\
--##--\
------\
"
   },

   {   ',', "\
------\
------\
------\
--##--\
---#--\
------\
"
   },

   {   ':', "\
--##--\
------\
------\
------\
--##--\
------\
"
   },

   {   '!', "\
--##--\
--##--\
--##--\
------\
--##--\
------\
"
   },

   {   '?', "\
-###--\
#---#-\
---#--\
--#---\
--#---\
------\
"
   },

   {   '@', "\
-###--\
#-###-\
#-###-\
#-----\
-###--\
------\
"
   },

   {   '=', "\
------\
-####-\
------\
-####-\
------\
------\
"
   },

   /* End of list. */
   {   '\0',   ""
   }
};

/* Array of indeces into char_maps[] */
static unsigned char ch_table[256];

/*
** Grid of pixel colors.  Drawing is done to here,
** before being copied to the actual screen by
** tm_update().
*/
static COLOR frame[SCREEN_WIDTH * SCREEN_HEIGHT];

/* Pointer to the video memory on the actual screen. */
static unsigned char far *screen = (unsigned char far *)0xB8000000L;

/* Size and position of clipping rectangle. */
static int   clip_x;      /* NOTE: -1 indicates no clipping. */
static int   clip_y;
static int   clip_width;
static int   clip_height;

/**************************** FUNCTIONS *************************/

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
int
tm_init(void)
{
   union REGS   aregs;
   int          i;

   /*
   ** Initialize screen to mode 3 (80 x 25 chars color).
   */
   aregs.x.ax = 0x0003;          /* Function 0, subfunc 3 is 80x25chars. */
   int86(0x10, &aregs, &aregs);  /* Call PC video BIOS. */

   /*
   ** Move cursor off screen.
   */
   aregs.h.ah = 2;         /* Function 2 is move cursor. */
   aregs.h.bh = 0;         /* Page # */
   aregs.h.dl = (unsigned char)0;   /* X */
   aregs.h.dh = (unsigned char)25;   /* Y */
   int86(0x10, &aregs, &aregs);   /* Call PC video BIOS. */

   /*
   ** Initialize table for mapping character codes to
   ** character map entries.
   */
   for (i = 0; i < 256; i++)
      ch_table[i] = 0xFF;
   i = 0;
   while (char_maps[i].code != '\0')
   {
      ch_table[char_maps[i].code] = i;
      i++;
   }

   /* Clipping is off by default. */
   clip_x = -1;
   clip_y = -1;
   clip_width = -1;
   clip_height = -1;

   /* Clear the screen. */
   memset(frame, 0, (SCREEN_WIDTH * SCREEN_HEIGHT));
   tm_update();

   /* Success! */
   return 1;
}

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
void
tm_deinit(void)
{
   union REGS   aregs;

   /*
   ** Reset screen.
   */
   aregs.x.ax = 0x0003;
   int86(0x10, &aregs, &aregs);
}

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
void
tm_update(void)
{
   int      x, y;
   int      frame_offset;
   unsigned char   attrib;   /* Attribute byte for screen. */

   /* Wait for vertical retrace on video card. */
   while (!(inp(0x3DA) & 0x08))
   {
      /* Wait. */
   }

   /* Copy frame to screen. */
   for (y = 0; y < SCREEN_HEIGHT / 2; y++)
   {
      for (x = 0; x < SCREEN_WIDTH; x++)
      {
         frame_offset = y * SCREEN_WIDTH * 2 + x;
         if (frame[frame_offset] > 0xF)
         {
            /*
            ** There is a character here.
            */

            /* Set character byte. */
            screen[y * 160 + x * 2] = frame[frame_offset];

            /* Set attribute byte. */
            screen[y * 160 + x * 2 + 1] = frame[frame_offset + SCREEN_WIDTH];
         }
         else
         {
            /*
            ** There are two pixels here.
            */

            /* Set character byte. */
            screen[y * 160 + x * 2] = 0xDF;

            /* Set attribute byte. */
            attrib = 
               (frame[frame_offset]) +
               (frame[frame_offset + SCREEN_WIDTH] << 4);
            screen[y * 160 + x * 2 + 1] = attrib;
         }
      }
   }
}

/*
** tm_clear:
** Clears the temporary frame to a specified color.
**
** Parameters:
**   Name   Description
**   ----   -----------
**   color  Color to draw with.
**
** Returns:
**   NONE
*/
void
tm_clear(COLOR color)
{
   memset(frame, color, SCREEN_WIDTH * SCREEN_HEIGHT);
}

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
void
tm_pixel(int x, int y, COLOR color)
{
   /* Check if clipping is enabled. */
   if (clip_x != -1)
   {
      /* Check if point is outside clipping rectangle. */
      if (x < clip_x || x >= clip_x + clip_width ||
         y < clip_y || y >= clip_y + clip_height)
      {
         return; /* Point is clipped out. */
      }
   }

   /* Draw the pixel. */
   frame[y * SCREEN_WIDTH + x] = color;
}

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
void
tm_rect(int x, int y, int width, int height, COLOR color)
{
   int   yy;

   /* Clip to the clipping rectangle. */
   if (clip_x != -1)
   {
      /* Clip to right edge of clipping rectangle. */
      if (width > clip_x + clip_width - x)
         width = clip_x + clip_width - x;
      if (width < 1)
         return;

      /* Clip to bottom edge of clipping rectangle. */
      if (height > clip_y + clip_height - y)
         height = clip_y + clip_height - y;
      if (height < 1)
         return;

      /* Clip to left edge of clipping rectangle. */
      if (x < clip_x)
      {
         width -= (clip_x - x);
         x = clip_x;
         if (width < 1)
            return;
      }

      /* Clip to top edge of clipping rectangle. */
      if (y < clip_y)
      {
         height -= (clip_y - y);
         y = clip_y;
         if (height < 1)
            return;
      }
   }

   /* Draw the rectangle. */
   for (yy = 0; yy < height; yy++)
   {
      /* Draw one scanline of the rectangle. */
      memset(&frame[(y + yy) * SCREEN_WIDTH + x], color, width);
   }
}

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
void
tm_banner(int x, int y, COLOR color, unsigned char *text)
{
   int      yy, xx;
   unsigned char   *cptr;

   /* Draw each character in the string. */
   while (*text != '\0')
   {
      /* Draw this character. */
      cptr = char_maps[ch_table[*text]].pixels;
      for (yy = 0; yy < CHAR_HEIGHT; yy++)
      {
         for (xx = 0; xx < CHAR_WIDTH; xx++)
         {
            if (cptr[yy * CHAR_WIDTH + xx] == '#')
               frame[(y + yy) * SCREEN_WIDTH + (x + xx)] = color;
         }
      }

      /* Step to next character in string. */
      text++;
      x += CHAR_WIDTH;
   }
}

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
void
tm_cbanner(int y, COLOR color, unsigned char *text)
{
   tm_banner(SCREEN_WIDTH / 2 - (CHAR_WIDTH * strlen(text)) / 2,
      y, color, text);
}

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
void
tm_text(int x, int y, COLOR color, unsigned char *text)
{
   int      xx;
   unsigned char   b;

   /* Text can only be drawn to even numbered lines. */
   y &= ~1;

   /* Draw each character in the string. */
   for (xx = 0; text[xx] != '\0'; xx++)
   {
      /* Mix text color with existing color. */
      b = frame[y * SCREEN_WIDTH + x + xx];
      b = (b << 4) & 0xF0;
      b |= color;
      frame[(y + 1) * SCREEN_WIDTH + x + xx] = b;

      /* Place character code into frame. */
      frame[y * SCREEN_WIDTH + x + xx] = text[xx];
   }
}

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
void
tm_ctext(int y, COLOR color, unsigned char *text)
{
   tm_text(SCREEN_WIDTH / 2 - strlen(text) / 2, y, color, text);
}

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
void
tm_scanline(int x, int y, int width, COLOR *colors)
{
   int   srcx = 0;

   /* Clip to the clipping rectangle. */
   if (clip_x != -1)
   {
      /* Clip vertically. */
      if (y < clip_y || y >= clip_y + clip_height)
         return;

      /* Clip to right edge of clipping rectangle. */
      if (width > clip_x + clip_width - x)
         width = clip_x + clip_width - x;
      if (width < 1)
         return;

      /* Clip to left edge of clipping rectangle. */
      if (x < clip_x)
      {
         srcx += (clip_x - x);
         width -= (clip_x - x);
         x = clip_x;
         if (width < 1)
            return;
      }
   }

   memcpy(&frame[y * SCREEN_WIDTH + x], &colors[srcx], width);
}

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
void
tm_pic(int x, int y, BITM *bitm)
{
   int   yy;
   int   srcx, srcy, srcw, srch;

   /* Assume the entire picture will be copied. */
   srcx = 0;
   srcy = 0;
   srcw = bitm->width;
   srch = bitm->height;

   /* Assume the entire picture will be copied. */
   srcx = 0;
   srcy = 0;
   srcw = bitm->width;
   srch = bitm->height;

   /* Clip to the clipping rectangle. */
   if (clip_x != -1)
   {
      /* Clip to right edge of clipping rectangle. */
      if (srcw > clip_x + clip_width - x)
         srcw = clip_x + clip_width - x;
      if (srcw < 1)
         return;

      /* Clip to bottom edge of clipping rectangle. */
      if (srch > clip_y + clip_height - y)
         srch = clip_y + clip_height - y;
      if (srch < 1)
         return;

      /* Clip to left edge of clipping rectangle. */
      if (x < clip_x)
      {
         srcx += (clip_x - x);
         srcw -= (clip_x - x);
         x = clip_x;
         if (srcw < 1)
            return;
      }

      /* Clip to top edge of clipping rectangle. */
      if (y < clip_y)
      {
         srcy += (clip_y - y);
         srch -= (clip_y - y);
         y = clip_y;
         if (srch < 1)
            return;
      }
   }

   /* Transfer a region from the picture to the frame. */
   for (yy = 0; yy < srch; yy++)
   {
      memcpy(&frame[(y + yy) * SCREEN_WIDTH + x],
         &bitm->pixels[(srcy + yy) * bitm->width + srcx],
         srcw);
   }
}

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
void
tm_pic_trans(int x, int y, BITM *bitm)
{
   int   xx;
   int   yy;
   int   srcx, srcy, srcw, srch;
   COLOR   b;

   /* Assume the entire picture will be copied. */
   srcx = 0;
   srcy = 0;
   srcw = bitm->width;
   srch = bitm->height;

   /* Clip to the clipping rectangle. */
   if (clip_x != -1)
   {
      /* Clip to right edge of clipping rectangle. */
      if (srcw > clip_x + clip_width - x)
         srcw = clip_x + clip_width - x;
      if (srcw < 1)
         return;

      /* Clip to bottom edge of clipping rectangle. */
      if (srch > clip_y + clip_height - y)
         srch = clip_y + clip_height - y;
      if (srch < 1)
         return;

      /* Clip to left edge of clipping rectangle. */
      if (x < clip_x)
      {
         srcx += (clip_x - x);
         srcw -= (clip_x - x);
         x = clip_x;
         if (srcw < 1)
            return;
      }

      /* Clip to top edge of clipping rectangle. */
      if (y < clip_y)
      {
         srcy += (clip_y - y);
         srch -= (clip_y - y);
         y = clip_y;
         if (srch < 1)
            return;
      }
   }

   /* Transfer a region from the picture to the frame. */
   for (yy = 0; yy < srch; yy++)
   {
      for (xx = 0; xx < srcw; xx++)
      {
         b = bitm->pixels[(srcy + yy) * bitm->width + srcx + xx];
         if (b)
         {
            frame[(y + yy) * SCREEN_WIDTH + x + xx] = b;
         }
      }
   }
}

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
void
tm_clip(int x, int y, int width, int height)
{
   clip_x = x;
   clip_y = y;
   clip_width = width;
   clip_height = height;
}

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
void
tm_noclip(void)
{
   clip_x = -1;
   clip_y = -1;
   clip_width = -1;
   clip_height = -1;
}

/* End tmode.c */
