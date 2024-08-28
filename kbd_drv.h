/*
========================================================================
FILENAME:     kbd_drv.h
DESCRIPTION:  Include file for keyboard driver module.
AUTHOR:       Ammon R. Campbell

(C) Copyright 1986 Ammon R. Campbell.  All rights reserved.

NOTE:  Modified 1991 to work with newer versions of MSC.
========================================================================
*/

/* Special keycodes used for the kbd_read() function: */
#define KEY_BACKSPACE   0x08
#define KEY_TAB         0x09
#define KEY_ENTER       0x0D
#define KEY_ESCAPE      0x1B
#define KEY_UP          0x148
#define KEY_DOWN        0x150
#define KEY_LEFT        0x14B
#define KEY_RIGHT       0x14D
#define KEY_HOME        0x147
#define KEY_END         0x14F
#define KEY_F1          0x13B
#define KEY_PGUP        0x149
#define KEY_PGDN        0x151
#define KEY_INSERT      0x152
#define KEY_DELETE      0x153

/* Scan codes used for the kbd_check() function: */
#define SC_1         /*   1   */    0x02
#define SC_2         /*   2   */    0x03
#define SC_3         /*   3   */    0x04
#define SC_4         /*   4   */    0x05
#define SC_5         /*   5   */    0x06
#define SC_6         /*   6   */    0x07
#define SC_7         /*   7   */    0x08
#define SC_8         /*   8   */    0x09
#define SC_9         /*   9   */    0x0A
#define SC_0         /*   10  */    0x0B
#define SC_MINUS                    0x0C
#define SC_EQUALS                   0x0D
#define SC_BACKSPACE                0x0E
#define SC_F1                       0x3B
#define SC_F2                       0x3C
#define SC_F3                       0x3D
#define SC_F4                       0x3E
#define SC_F5                       0x3F
#define SC_F6                       0x40
#define SC_F7                       0x41
#define SC_F8                       0x42
#define SC_F9                       0x43
#define SC_F10                      0x44
#define SC_UP        /*   96   */   0x48
#define SC_DOWN      /*   98   */   0x50
#define SC_LEFT      /*   92   */   0x4B
#define SC_RIGHT     /*   102  */   0x4D
#define SC_SPACE     /*   61   */   0x39
#define SC_ENTER     /*   43   */   0x1C
#define SC_ESCAPE    /*   110  */   0x01
#define SC_LSHIFT    /*   44   */   0x2A
#define SC_RSHIFT    /*   57   */   0x36
#define SC_CTRL      /*   58   */   0x1D
#define SC_ALT       /*   60   */   0x38
#define SC_TAB       /*   16   */   0x0F
#define SC_HOME                     0x47
#define SC_END                      0x4F
#define SC_PGDN                     0x51
#define SC_PGUP                     0x49

#define SC_A   0x1E
#define SC_B   0x30
#define SC_C   0x2E
#define SC_D   0x20
#define SC_E   0x12
#define SC_F   0x21
#define SC_G   0x22
#define SC_H   0x23
#define SC_I   0x17
#define SC_J   0x24
#define SC_K   0x25
#define SC_L   0x26
#define SC_M   0x32
#define SC_N   0x31
#define SC_O   0x18
#define SC_P   0x19
#define SC_Q   0x10
#define SC_R   0x13
#define SC_S   0x1F
#define SC_T   0x14
#define SC_U   0x16
#define SC_V   0x2F
#define SC_W   0x11
#define SC_X   0x2D
#define SC_Y   0x15
#define SC_Z   0x2C

#define SC_SEMICOLON    0x27
#define SC_SINGLEQUOTE  0x28
#define SC_COMMA        0x33
#define SC_PERIOD       0x34
#define SC_SLASH        0x35
#define SC_BACKSLASH    0x2B
#define SC_LEFTBRACKET  0x1A
#define SC_RIGHTBRACKET 0x1B
#define SC_BACKQUOTE    0x29
#define SC_CAPSLOCK     0x3A
#define SC_NUMLOCK      0x45

#define SC_KEYPAD_0        0x52
#define SC_KEYPAD_1        0x4F
#define SC_KEYPAD_2        0x50
#define SC_KEYPAD_3        0x51
#define SC_KEYPAD_4        0x4B
#define SC_KEYPAD_5        0x4C
#define SC_KEYPAD_6        0x4D
#define SC_KEYPAD_7        0x47
#define SC_KEYPAD_8        0x48
#define SC_KEYPAD_9        0x49
#define SC_KEYPAD_STAR     0x37
#define SC_KEYPAD_MINUS    0x4A
#define SC_KEYPAD_PLUS     0x4E
#define SC_KEYPAD_PERIOD   0x53

/* Public functions: */

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
void  kbd_init(void);

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
void  kbd_deinit(void);

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
void  kbd_flush(void);

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
int   kbd_read(void);

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
int   kbd_avail(void);

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
int   kbd_check(int key);

/* end kdb_drv.h */
