/*
=======================================================================
FILENAME:     speaker.c
DESCRIPTION:  Routines for playing sounds on the IBM PC internal
              speaker.
AUTHOR:       Ammon R. Campbell
COPYRIGHT:    (C) Copyright 1985 Ammon R. Campbell.
              All rights reserved.
TOOLS:        Microsoft C version 3.0.
NOTE:  Modified 1991 to work with newer versions of MSC.
=======================================================================
*/

#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

#include "speaker.h"

/******************************* CONSTANTS ***************************/

/* Maximum pitch bytes in sample. */
#define MAX_QUEUE    256

/*
** I/O port address of the register that contains the
** speaker on/off bit.  The low two bits determine
** whether the timer is gated through to the speaker
** or not.  The other bits should be left alone.
*/
#define SPKR_ENABLE_PORT    0x61

/*
** I/O port address of the count register for the
** channel two timer that drives the speaker.
*/
#define TIMER2_COUNT_REG    0x42

/*
** I/O port address of the mode register for the
** 8253 timer chip.
*/
#define TIMER2_MODE_REG        0x43

/*
** Mode command to set timer two to square wave
** output mode.
*/
#define TIMER2_MODE_CMD        0xB6

/******************************* DATA ***************************/

/*
** This is the priority of the currently playing
** sample.  Only a sample of equal or higher
** priority can preempt the currently playing
** sample.
*/
static unsigned char    v_priority;

/*
** This is a list of pitch values for the pitches
** that are to be played at each timer tick.
** A value of zero means silence.
*/
static unsigned char    v_list[MAX_QUEUE];

/* Number of pitches queued in v_list. */
static int        v_size;

/* Current index into v_list[]. */
static int        v_index;

/******************************* FUNCTIONS ***************************/

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
int
spk_init(void)
{
    /* Mark everything as unbusy. */
    v_size = 0;
    v_index = 0;
    v_priority = 0;

    /* Success! */
    return 1;
}

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
void
spk_deinit(void)
{
    /* Mark everything as unbusy. */
    v_size = 0;
    v_index = 0;
    v_priority = 0;

    /* Turn off the speaker. */
    outp(SPKR_ENABLE_PORT, inp(SPKR_ENABLE_PORT) & (~3));
}

/*
** spk_poll:
** This is the service routine for the speaker routines.
** It must be called at some regular interval, i.e.
** 18.2 times per second with real-time clock ticks,
** or 60 times per second with vertical retrace.
*/
void
spk_poll(void)
{
    /* Playing? */
    if (v_index < v_size)
    {
        /* Play the current pitch. */
        if (v_list[v_index] == 0)
        {
            /* Silence:  Stop the speaker. */
            outp(SPKR_ENABLE_PORT, inp(SPKR_ENABLE_PORT) & (~3));
        }
        else
        {
            /* Set new pitch for speaker. */
            outp(TIMER2_MODE_REG, TIMER2_MODE_CMD);
            outp(TIMER2_COUNT_REG, 0);
            outp(TIMER2_COUNT_REG, v_list[v_index]);

            /* Turn speaker on if it isn't. */
            outp(SPKR_ENABLE_PORT, inp(SPKR_ENABLE_PORT) | 3);
        }

        /* Step to next byte of sample. */
        v_index++;
    }
    else
    {
        /* End of sample. */
        v_size = 0;
        v_index = 0;

        /* Stop the speaker. */
        outp(SPKR_ENABLE_PORT, inp(SPKR_ENABLE_PORT) & (~3));
    }
}

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
int
spk_busy(void)
{
    if (v_index < v_size)
        return 1;
    return 0;
}

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
void
spk_play(unsigned char *data, unsigned int size)
{
    if (data == NULL || size < 2)
        return;

    if (data[0] >= v_priority || v_index >= v_size)
    {
        if (size >= MAX_QUEUE)
            size = MAX_QUEUE;
        v_priority = data[0];
        v_index = 0;
        v_size = size - 1;
        memcpy(v_list, &data[1], size - 1);
    }
}

/*
** spk_stop:
** Stops any sample that is playing.
**
** Parameters:
**
** Returns:
**    NONE
*/
void
spk_stop(void)
{
    v_size = 0;
    v_index = 0;
    v_priority = 0;

    /* Turn off the speaker. */
    outp(SPKR_ENABLE_PORT, inp(SPKR_ENABLE_PORT) & (~3));
}

/* End speaker.c */
