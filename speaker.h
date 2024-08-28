/*
=======================================================================
FILENAME:     speaker.h
DESCRIPTION:  Include file for speaker.c and calling programs.
AUTHOR:       Ammon R. Campbell
COPYRIGHT:    (C) Copyright 1985 Ammon R. Campbell.
              All rights reserved.
TOOLS:        Microsoft C version 3.0.
NOTE:  Modified 1991 to work with newer versions of MSC.
=======================================================================
*/

/*
** spk_init:
** Initializes the PC Speaker hardware module.
** A timer interrupt service routine is installed to
** handle background playing of pitches on the speaker.
** This will continue until spk_deinit() is called.
**
** Parameters:
**    NONE
**
** Returns:
**    Value    Meaning
**    -----    -------
**    1        Successful.
**    0        Error occurred; don't call spk_deinit().
*/
int   spk_init(void);

/*
** spk_deinit:
** Deinitializes the PC Speaker hardware module.
**
** Parameters:
**    NONE
**
** Returns:
**    NONE
*/
void  spk_deinit(void);

/*
** spk_poll:
** This is the service routine for the speaker routines.
** It must be called at some regular interval, i.e.
** 18.2 times per second with real-time clock ticks,
** or 60 times per second with vertical retrace.
*/
void  spk_poll(void);

/*
** spk_busy:
** Determines if a sound is being played or not.
**
** Parameters:
**    NONE
**
** Returns:
**    Value    Meaning
**    -----    -------
**    1        playing a sample.
**    0        idle.
*/
int   spk_busy(void);

/*
** spk_play:
** Begins playing a sample on the PC Speaker if
** its priority is equal to or lower than the
** currently playing sample.
**
** The format of a sample is one byte of priority,
** followed by some bytes of pitches.  Each pitch
** byte specifies a pitch that will be played for
** 1/18th of a second (use zero for silence), with
** lower numbers giving higher pitches.
**
** Parameters:
**    Name    Description
**    ----    -----------
**    data    Pointer to audio data to be played.
**    size    Size in bytes of audio data to be played.
**
** Returns:
**    NONE
*/
void  spk_play(unsigned char *data, unsigned int size);

/*
** spk_stop:
** Stops any sample that is playing.
**
** Parameters:
**
** Returns:
**    NONE
*/
void  spk_stop(void);

/* End speaker.h */
