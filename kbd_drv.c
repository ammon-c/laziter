/*
========================================================================
FILENAME:     kbd_drv.c
DESCRIPTION:  Low-level keyboard driver routines for arcade game
              applications.
AUTHOR:       Ammon R. Campbell

(C) Copyright 1986 Ammon R. Campbell.  All rights reserved.

NOTE:  Modified 1991 to work with newer versions of MSC.
========================================================================
*/

#include <stdlib.h>
#include <dos.h>
#include <conio.h>

#include "kbd_drv.h"

/******************************* DATA ****************************/

/* Storage for address of old keyboard interrupt handler. */
static void (cdecl interrupt far *old_kbd_int9_isr)();

/* Flag; nonzero if last key-interrupt was a key being released. */
static char   was_break;

/* Map of flags indicating which keys are currently pressed. */
static char   keymap[256];

/* Last scancode that was received by the keyboard ISR. */
static unsigned char   v_last_scancode;

/**************************** LOCAL FUNCTIONS *************************/

/*
** __kbd_int9_isr:
** Interrupt service routine for keyboard
** hardware interrupt.
*/
static void cdecl interrupt far
__kbd_int9_isr(void)
{
   unsigned char   scan_code;
   unsigned char   do_chain = 1;   /* Flag is reset if skipping old ISR */

   /* Read scan-code from keyboard controller. */
   scan_code = inp(0x60);
   v_last_scancode = scan_code;

   /* Is it a "key-break" (key lifted) code? */
   if (scan_code == 0xF0)
   {
      /*
      ** A key-break was signalled.
      ** The next interrupt will have the
      ** scan-code of the key that was
      ** lifted.
      */
      was_break = 1;
   }
   /* Is it a 2-byte scancode prefix? */
   else if (scan_code == 0xE0)
   {
      /*
      ** The IBM enhanced keyboard generates
      ** a 0xE0 prefix for certain keys on
      ** the keyboard.  We just ignore these.
      */
      ;
   }
   /* Is it a 3-byte scancode prefix? */
   else if (scan_code == 0xE1)
   {
      /*
      ** The IBM enhanced keyboard generates
      ** a 0xE1 prefix for keys with 3 byte
      ** scancodes (like the PAUSE key).
      */
      ;
   }
   else
   {
      /* Is it a key that the BIOS will screw up? */
      if (scan_code == 0x4C ||   /* Keypad 5 key */
         scan_code == 0x57 ||   /* F11 */
         scan_code == 0x58 ||   /* F12 */
         scan_code == 0x1D ||   /* CTRL */
         scan_code == 0x38)   /* ALT */
      {
         /*
         ** These keys generate temporary lockups
         ** of the system if they are passed on to
         ** the BIOS, so we _won't_ call the old
         ** keyboard BIOS for these keys.
         */
         do_chain = 0;
      }

      /* We have an actual scan-code. */
      /* See if the key-break bit is set. */
      if ((scan_code & 0x0080) == 0x0080)
      {
         /* A key-code with break was signalled. */
         was_break = 1;
      }

      /* Mask off the break bit. */
      scan_code &= 0x007F;

      /* Adjust the keymap for this key. */
      if (was_break)
         keymap[scan_code] = 0;
      else
         keymap[scan_code] = 1;

      /* Reset the flag indicating a key-break. */
      was_break = 0;
   }

   if (!do_chain)
   {
      /* Signal end of interrupt to 8259PIC. */
      outp(0x20, 0x20);
   }
   else
   {
      /* Call the old keyboard interrupt handler. */
      _chain_intr(old_kbd_int9_isr);
   }
}

/************************* PUBLIC FUNCTIONS *************************/

/*
** kbd_init:
** Initializes the keyboard driver.
** This function must be called before any of the other
** kbd_... functions are called.
**
** Parameters:
**   NONE
**
** Returns:
**   NONE
*/
void
kbd_init(void)
{
   int   i;

   kbd_flush();

   /* Initialize the key map. */
   was_break = 0;
   for (i = 0; i < 256; i++)
      keymap[i] = 0;

   /* Save the current interrupt handler for the keyboard. */
   old_kbd_int9_isr = _dos_getvect(9);

   /* Install our interrupt handler for the keyboard. */
   _dos_setvect(9, __kbd_int9_isr);

   kbd_flush();
}

/*
** kbd_deinit:
** Shuts down the keyboard driver.  This must be
** called before the application terminates if a
** previous call to kbd_init() was made.
**
** Parameters:
**   NONE
**
** Returns:
**   NONE
*/
void
kbd_deinit(void)
{
   kbd_flush();

   /* Restore original keyboard interrupt handler. */
   _dos_setvect(9, old_kbd_int9_isr);

   kbd_flush();
}

/*
** kbd_flush:
** Flushes the keyboard buffer.
**
** Parameters:
**   NONE
**
** Returns:
**   NONE
*/
void
kbd_flush(void)
{
   while (kbhit())
      kbd_read();
}

/*
** kbd_read:
** Reads a character from the keyboard.
** If a character is available from the keyboard buffer,
** it will be taken from there, otherwise will wait until
** a key is pressed.
**
** Parameters:
**   NONE
**
** Returns:
**   Value   Meaning
**   -----   -------
**   any     Keycode of key that was pressed.
**           For normal ASCII letters, numbers, and
**           punctuation, the ASCII value of the key
**           will be returned.  For special keys, such
**           as the cursor and function keys, one of
**           the manifest constants KEY_... defined in
**           "kbd_drv.h" will be returned.
*/
int
kbd_read(void)
{
   int   ch;
   union _REGS   regs;

   /* Use PC BIOS interrupt 16h to read the keyboard. */
   regs.x.ax = 0;
   _int86(0x16, &regs, &regs);
   ch = (int)regs.h.al;
   if (ch == 0)
      ch = 0x100 | (int)regs.h.ah;
   return ch;
}

/*
** kbd_avail:
** Determines if a character is currently available
** for reading in the keyboard buffer.
**
** Parameters:
**   NONE
**
** Returns:
**   Value   Meaning
**   -----   -------
**   1       A character is available for reading
**           in the keyboard buffer.
**   0       The keyboard buffer is empty.
*/
int
kbd_avail(void)
{
   return (int)(kbhit() & 1);
}

/*
** kbd_check:
** Checks if a particular key is currently
** pressed.
**
** Parameters:
**   Name   Description
**   ----   -----------
**   key    Scan code of key to be checked.
**          This may be any of the manifest
**          constants SC_... defined
**          in "kbd_drv.h".
**
** Returns:
**   Value   Meaning
**   -----   -------
**   1       The specified is key is currently
**           pressed.
**   0       The specified key is not pressed.
*/
int
kbd_check(int key)
{
   if (keymap[key])
      return 1;
   return 0;
}

/*
** kbd_last_scancode:
** Retrieves the last scancode that was received
** by the keyboard interrupt service routine.
**
** Parameters:
**   NONE
**
** Returns:
**   Value   Meaning
**   -----   -------
**   any     Scancode.
*/
int
kbd_last_scancode(void)
{
   return v_last_scancode;
}

/* End kbd_drv.c */
