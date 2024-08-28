/*
=================================================================
FILENAME:     main.c
DESCRIPTION:  Primary source file for "Laziter" game.
AUTHOR:       Ammon R. Campbell
COPYRIGHT:    (C) Copyright 1986 Ammon R. Campbell.
              Binary executable versions of this program may
              be distributed for non-commercial not for profit
              purposes.  The source code may not be distributed.
TOOLS:        Microsoft C Compiler version 3.0
NOTE:  Modified 1991 to work with newer versions of MSC.
=================================================================
*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "tmode.h"
#include "speaker.h"
#include "kbd_drv.h"

/************************** CONSTANTS **************************/

/* Size and location of gameplay viewport on screen. */
#define PLAY_WIDTH      60
#define PLAY_HEIGHT     (SCREEN_HEIGHT - 2)
#define PLAY_X          1
#define PLAY_Y          1

/* Number of enemy ships arranged per horizontal row. */
#define ENEMIES_PER_ROW 6

/* Number of frames per movement of player's ship. */
#define MOVE_SPEED      2

/* Number of frames per movement of player's shots. */
#define SHOT_SPEED      1

/* Maximum number of simultaneous shots by player. */
#define MAX_SHOTS       2

/* Maximum number of enemies per attack wave. */
#define MAX_ENEMIES     (ENEMIES_PER_ROW * 5)

/* Maximum number of simultaneous explosions. */
#define MAX_EXPLODE     4

/* Main loop modes. */
#define MODE_START_WAVE 1
#define MODE_PLAY       2
#define MODE_LOSE_SHIP  3
#define MODE_GAME_OVER  4
#define MODE_WAVE_DONE  5

/* Time delays for certain modes. */
#define TIME_START_WAVE 120
#define TIME_LOSE_SHIP  120
#define TIME_GAME_OVER  200
#define TIME_WAVE_DONE  120

/************************** DATA **************************/

/* Intro screen sound effect. */
static unsigned char *sound_intro = "\x20\
\x61\x52\x62\x51\x63\x50\x64\x4F\x65\x4E\
\x61\x4D\x62\x4C\x63\x4B\x64\x4A\x65\x49\
\x61\x48\x62\x47\x63\x46\x64\x45\x65\x44\
\x61\x43\x62\x42\x63\x41\x64\x40\x65\x3F\
\x61\x3E\x62\x3D\x63\x3C\x64\x3B\x65\x3A\
\x61\x39\x62\x38\x63\x37\x64\x36\x65\x35\
\x61\x34\x62\x33\x63\x32\x64\x31\x65\x30\
\x61\x2F\x62\x2E\x63\x2D\x64\x2C\x65\x2B\
\x61\x2A\x62\x29\x63\x28\x64\x27\x65\x26\
\x61\x25\x62\x24\x63\x23\x64\x22\x65\x21\
\x61\x20\x62\x1F\x63\x1E\x64\x1D\x65\x1C\
\x61\x1B\x62\x1A\x63\x19\x64\x18\x65\x17\
\x61\x16\x62\x15\x63\x14\x64\x13\x65\x12\
";

/* Wave completed sound effect. */
static unsigned char *sound_wave_done = "\x20\
\x20\x02\x20\x03\x20\x04\x20\x05\x20\x06\x20\x07\x20\x08\
\x20\x02\x20\x03\x20\x04\x20\x05\x20\x06\x20\x07\x20\x08\
\x20\x02\x20\x03\x20\x04\x20\x05\x20\x06\x20\x07\x20\x08\
\x20\x02\x20\x03\x20\x04\x20\x05\x20\x06\x20\x07\x20\x08\
\x20\x09\x20\x0A\x20\x0B\x20\x0C\x20\x0D\x20\x0E\x20\x0F\
\x20\x10\x20\x11\x20\x12\x20\x13\x20\x14\x20\x15\x20\x16\
";

/* Player ship hit sound effect. */
static unsigned char *sound_player_hit = "\x20\
\x05\x06\x07\x08\x09\x10\x11\x50\x51\x52\x53\
\x12\x13\x14\x15\x16\x17\x18\x50\x51\x52\x53\
\x05\x06\x07\x08\x09\x10\x11\x50\x51\x52\x53\
\x12\x13\x14\x15\x16\x17\x18\x50\x51\x52\x53\
\x05\x06\x07\x08\x09\x10\x11\x50\x51\x52\x53\
\x12\x13\x14\x15\x16\x17\x18\x50\x51\x52\x53\
\x19\x1A\x1B\x1C\x1D\x1E\x1F\x50\x51\x52\x53\
";

/* Player shooting sound effect. */
static unsigned char *sound_shoot = "\x01\
\x05\x10\x06\x11\x07\x12\x08\x13\x09\x14";

/* Enemy destroyed sound effect. */
static unsigned char *sound_enemy_hit = "\x02\
\x20\x19\x18\x17\x16\x15\x14\x13\x12\x11\x10\
";

/* Player spaceship picture. */
static BITM pic_ship =
{
    5, 5,
    "\
\x00\x00\x01\x00\x00\
\x00\x01\x01\x01\x00\
\x00\x01\x03\x01\x00\
\x01\x01\x03\x01\x01\
\x01\x04\x01\x04\x01\
"
};

/* Enemy spaceship picture. */
static BITM pic_enemy1 =
{
    5, 5,
    "\
\x00\x06\x06\x06\x00\
\x06\x01\x06\x06\x06\
\x06\x06\x06\x06\x06\
\x00\x06\x06\x06\x00\
\x06\x00\x06\x00\x06\
"
};
static BITM pic_enemy2 =
{
    5, 5,
    "\
\x00\x06\x06\x06\x00\
\x06\x06\x01\x06\x06\
\x06\x06\x06\x06\x06\
\x00\x06\x06\x06\x00\
\x06\x00\x06\x00\x06\
"
};
static BITM pic_enemy3 =
{
    5, 5,
    "\
\x00\x06\x06\x06\x00\
\x06\x06\x06\x01\x06\
\x06\x06\x06\x06\x06\
\x00\x06\x06\x06\x00\
\x06\x00\x06\x00\x06\
"
};
static BITM *pic_enemies[3] =
{
    &pic_enemy1, &pic_enemy2, &pic_enemy3
};

/* Player shot pictures. */
static BITM pic_player_shot1 =
{
    1, 3, "\x04\x05\x06",
};
static BITM pic_player_shot2 =
{
    1, 3, "\x05\x06\x04",
};
static BITM pic_player_shot3 =
{
    1, 3, "\x06\x04\x05",
};
static BITM *pic_player_shots[3] =
{
    &pic_player_shot1, &pic_player_shot2, &pic_player_shot3
};

/* List of possible fill colors (for screen borders, etc). */
static char colors[] =
{
    CLR_RED, CLR_GREEN, CLR_BLUE, CLR_CYAN,
    CLR_PURPLE, CLR_ORANGE
};

/* Player's current score. */
static int    score;

/* Number of ships player has left. */
static int    num_ships;

/* Current attack wave number. */
static int    attack_wave;

/* Current mode for main game loop. */
static int    loop_mode;

/* Flag; nonzero if running in demo mode. */
static int    demo_mode;

/* Position of player's ship in viewport. */
static int    player_x;
static int    player_y;

/* Position of player's shots in viewport (-1 means not active). */
static int    shot_x[MAX_SHOTS];
static int    shot_y[MAX_SHOTS];

/* Flag; nonzero if player started a shot last frame. */
static int    did_shoot;

/* Counter for synchronizing and timing game activities. */
static int    loop_counter;

/* Information about enemy ships. */
static struct
{
    int    x;    /* Horizontal position (-1 means not active). */
    int    y;    /* Vertical position. */
} enemies[MAX_ENEMIES];

/* Information about explosions. */
static struct
{
    int    x;    /* Horizontal position (-1 means not active). */
    int    y;    /* Vertical position. */
    int    age;  /* Age of explosion (# of frames active). */
} explode[MAX_EXPLODE];

/* Last explosion started in explode[] array. */
static int    last_explode;

/* Approximate number of active enemies remaining. */
static int    approx_num_enemies;

/* Current direction for enemy movement (1 or -1). */
static int    enemy_direction;

/************************** FUNCTIONS **************************/

/*
** draw_dtext:
** Draws small text centered horizontally in the
** gameplay viewport.
**
** Parameters:
**    Name    Description
**    ----    -----------
**    y       Vertical position for text.
**    clr     Color of text.
**    text    String to be drawn.
**
** Returns:
**    NONE
*/
static void
draw_dtext(int y, COLOR clr, unsigned char *text)
{
    /* Make sure text is on an even numbered line. */
    y &= ~1;

    /* Draw black behind the text. */
    tm_rect(PLAY_X + PLAY_WIDTH / 2 - strlen(text) / 2,
        y,
        strlen(text),
        1, CLR_BLACK);

    /* Draw the text. */
    tm_text(PLAY_X + PLAY_WIDTH / 2 - strlen(text) / 2,
        y,
        clr, text);
}

/*
** draw_explosion:
** Draws an explosion.  Assumes clipping is enabled.
**
** Parameters:
**    Name    Description
**    ----    -----------
**    x       Horizontal position of explosion center.
**    y       Vertical position of explosion center.
**    age     Age of explosion.
**
** Returns:
**    NONE
*/
static void
draw_explosion(int x, int y, int age)
{
    int    distance = age >> 1;
    COLOR    c = colors[x % 6]; /* Color depends on center position. */

    tm_pixel(x + distance, y + distance, c);
    tm_pixel(x + distance, y - distance, c);
    tm_pixel(x - distance, y + distance, c);
    tm_pixel(x - distance, y - distance, c);
    tm_pixel(x + distance, y, c);
    tm_pixel(x - distance, y, c);
    tm_pixel(x, y + distance, c);
    tm_pixel(x, y - distance, c);
}

/*
** draw_frame:
** Draws one frame of gameplay.
**
** Parameters:
**    NONE
**
** Returns:
**    NONE
*/
static void
draw_frame(void)
{
    int    i;
    COLOR    color;
    char    msg[80];

    /* Determine color for flashing messages. */
    color = (COLOR)(((loop_counter / 4) % 8));

    /* Draw borders. */
    tm_clear(colors[attack_wave % 6]);
    tm_rect(PLAY_X, PLAY_Y, PLAY_WIDTH, PLAY_HEIGHT, CLR_BLACK);

    /* Draw score. */
    tm_text(PLAY_X + PLAY_WIDTH + 1, PLAY_Y + 1, CLR_BLACK, "SCORE");
    sprintf(msg, "%03d", score);
    tm_banner(PLAY_X + PLAY_WIDTH + 1,
        PLAY_Y + 3,
        CLR_BLACK, msg);

    /* Draw number of ships. */
    tm_text(PLAY_X + PLAY_WIDTH + 1, PLAY_Y + 10, CLR_BLACK, "SHIPS");
    sprintf(msg, "%01d", num_ships);
    tm_banner(PLAY_X + PLAY_WIDTH + 1,
        PLAY_Y + 12,
        CLR_BLACK, msg);

    /* Draw attack wave number. */
    tm_text(PLAY_X + PLAY_WIDTH + 1, PLAY_Y + 20, CLR_BLACK, "ATTACK WAVE");
    sprintf(msg, "%02d", attack_wave + 1);
    tm_banner(PLAY_X + PLAY_WIDTH + 1,
        PLAY_Y + 22,
        CLR_BLACK, msg);

    /* Turn clipping on. */
    tm_clip(PLAY_X, PLAY_Y, PLAY_WIDTH, PLAY_HEIGHT);

    /* Draw player's ship. */
    tm_pic_trans(PLAY_X + player_x,
        PLAY_Y + player_y,
        &pic_ship);

    /* Draw explosions. */
    for (i = 0; i < MAX_EXPLODE; i++)
    {
        if (explode[i].x != -1)
        {
            draw_explosion(
                PLAY_X + explode[i].x,
                PLAY_Y + explode[i].y,
                explode[i].age);
        }
    }

    /* Draw enemy ships. */
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].x != -1)
        {
            tm_pic_trans(PLAY_X + enemies[i].x,
                PLAY_Y + enemies[i].y,
                pic_enemies[(loop_counter / 4) % 3]);
        }
    }

    /* Draw player's shots. */
    for (i = 0; i < MAX_SHOTS; i++)
    {
        if (shot_x[i] != -1)
        {
            tm_pic_trans(PLAY_X + shot_x[i],
                PLAY_Y + shot_y[i],
                pic_player_shots[loop_counter % 3]);
        }
    }

    /* Turn clipping back off. */
    tm_noclip();

    /* Draw new attack wave message on screen. */
    if (loop_mode == MODE_START_WAVE && !demo_mode)
    {
        strcpy(msg, "ATTACK");
        tm_banner(PLAY_X + PLAY_WIDTH / 2 - (CHAR_WIDTH * strlen(msg)) / 2,
            PLAY_Y + 5 + CHAR_HEIGHT * 2,
            color,
            msg);
        strcpy(msg, "WAVE");
        tm_banner(PLAY_X + PLAY_WIDTH / 2 - (CHAR_WIDTH * strlen(msg)) / 2,
            PLAY_Y + 5 + CHAR_HEIGHT * 3,
            color,
            msg);
        sprintf(msg, "%d", attack_wave + 1);
        tm_banner(PLAY_X + PLAY_WIDTH / 2 - (CHAR_WIDTH * strlen(msg)) / 2,
            PLAY_Y + 5 + CHAR_HEIGHT * 4,
            color,
            msg);
    }

    /* Draw GAME OVER message on screen. */
    if (loop_mode == MODE_GAME_OVER && !demo_mode)
    {
        strcpy(msg, "GAME OVER");
        tm_banner(PLAY_X + PLAY_WIDTH / 2 - (CHAR_WIDTH * strlen(msg)) / 2,
            PLAY_Y + 5 + CHAR_HEIGHT * 1,
            color,
            msg);
    }

    /* Draw end of wave message on screen. */
    if (loop_mode == MODE_WAVE_DONE && !demo_mode)
    {
        strcpy(msg, "COMPLETED");
        tm_banner(PLAY_X + PLAY_WIDTH / 2 - (CHAR_WIDTH * strlen(msg)) / 2,
            PLAY_Y + 5 + CHAR_HEIGHT * 0,
            color,
            msg);
        strcpy(msg, "ATTACK");
        tm_banner(PLAY_X + PLAY_WIDTH / 2 - (CHAR_WIDTH * strlen(msg)) / 2,
            PLAY_Y + 5 + CHAR_HEIGHT * 1,
            color,
            msg);
        strcpy(msg, "WAVE");
        tm_banner(PLAY_X + PLAY_WIDTH / 2 - (CHAR_WIDTH * strlen(msg)) / 2,
            PLAY_Y + 5 + CHAR_HEIGHT * 2,
            color,
            msg);
    }

    /* Draw player's ship dying message. */
    if (loop_mode == MODE_LOSE_SHIP && !demo_mode)
    {
        strcpy(msg, "OOPS!");
        tm_banner(PLAY_X + PLAY_WIDTH / 2 - (CHAR_WIDTH * strlen(msg)) / 2,
            PLAY_Y + 5 + CHAR_HEIGHT * 1,
            color,
            msg);
    }

    /* Draw demo mode stuff. */
    if (!demo_mode)
        return;

    /* Draw name of game in big text. */
    tm_banner(
        PLAY_X + PLAY_WIDTH / 2 - (CHAR_WIDTH * strlen("LAZITER")) / 2,
        PLAY_HEIGHT / 2, color,
        "LAZITER");

    /* Draw copyright notice. */
    draw_dtext(PLAY_Y + PLAY_HEIGHT / 2 + CHAR_HEIGHT + 2, color,
            "(C) Copyright 1986 by Ammon R. Campbell.");
    draw_dtext(PLAY_Y + PLAY_HEIGHT / 2 + CHAR_HEIGHT + 4, color,
            "This program may be distributed freely, ");
    draw_dtext(PLAY_Y + PLAY_HEIGHT / 2 + CHAR_HEIGHT + 6, color,
            "but may not be sold for profit.         ");

    /* Draw flashing demo message. */
    if (loop_counter & 0x20)
    {
        draw_dtext(PLAY_Y + PLAY_HEIGHT / 2 + CHAR_HEIGHT + 10, color,
            "PRESS SPACE TO BEGIN");
    }
}

/*
** start_attack_wave:
** Starts a new attack wave in the game.
**
** Parameters:
**    NONE
**
** Returns:
**    NONE
*/
static void
start_attack_wave(void)
{
    int    i;

    /* Reset timing stuff. */
    loop_counter = 0;
    loop_mode = MODE_START_WAVE;

    /* Assume the player just launched a shot. */
    did_shoot = 1;

    /* Set initial position of player's ship. */
    player_x = PLAY_WIDTH / 2 - pic_ship.width / 2;
    player_y = PLAY_HEIGHT - pic_ship.height - 1;

    /* Reset all player shots. */
    for (i = 0; i < MAX_SHOTS; i++)
    {
        shot_x[i] = -1;
        shot_y[i] = -1;
    }

    /* Reset all explosions. */
    last_explode = 0;
    for (i = 0; i < MAX_EXPLODE; i++)
    {
        explode[i].x = -1;
        explode[i].y = -1;
    }

    /* Initialize enemy ship positions. */
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].x = -1;
        enemies[i].y = -1;
    }
    approx_num_enemies = ENEMIES_PER_ROW * 3;
    approx_num_enemies += ENEMIES_PER_ROW * (attack_wave / 15);
    if (approx_num_enemies > MAX_ENEMIES)
        approx_num_enemies = MAX_ENEMIES;
    for (i = 0; i < approx_num_enemies; i++)
    {
        enemies[i].x = 2 + (i % ENEMIES_PER_ROW) * ((PLAY_WIDTH - 10) / ENEMIES_PER_ROW);
        enemies[i].y = 2 + (i / ENEMIES_PER_ROW) * (2 + pic_enemy1.height);
    }
    enemy_direction = 1;

    /* Play intro sound effect. */
    if (!demo_mode)
        spk_play(sound_intro, strlen(sound_intro));
}

/*
** player_hit:
** This is called if the player's ship is hit.
**
** Parameters:
**    NONE
**
** Returns:
**    NONE
*/
static void
player_hit(void)
{
    /* Reduce number of player's ships. */
    num_ships--;

    /* Play descruction sound effect. */
    spk_play(sound_player_hit, strlen(sound_player_hit));

    /* Start explosion at player ship position. */
    last_explode = (last_explode + 1) % MAX_EXPLODE;
    explode[last_explode].x = player_x + pic_ship.width / 2;
    explode[last_explode].y = player_y + pic_ship.height / 2;
    explode[last_explode].age = 0;

    /* See if player is out of ships. */
    if (num_ships < 1)
    {
        /* Display game over. */
        loop_counter = 0;
        loop_mode = MODE_GAME_OVER;
    }
    else
    {
        /* Show player's ship being destroyed. */
        loop_counter = 0;
        loop_mode = MODE_LOSE_SHIP;
    }
}

/*
** explode_actions:
** Processes explosion updating for a single frame.
**
** Parameters:
**    NONE
**
** Returns:
**    NONE
*/
static void
explode_actions(void)
{
    int    i;

    /* Loop for each possible explosion. */
    for (i = 0; i < MAX_EXPLODE; i++)
    {
        /* Is this explosion active? */
        if (explode[i].x == -1)
            continue; /* No. */

        /* Update explosion's age. */
        explode[i].age++;

        /* If explosion is too old, stop it. */
        if (explode[i].age > 40)
            explode[i].x = -1;
    }
}

/*
** enemy_actions:
** Processes the enemy ship actions for a single frame.
**
** Parameters:
**    NONE
**
** Returns:
**    NONE
*/
static void
enemy_actions(void)
{
    int    enx;
    int    change_dir = 0;
    int    enemy_speed;

    /* Determine how fast to move the enemies. */
    enemy_speed = approx_num_enemies / (attack_wave + 1) + 1;

    /* Loop for each enemy ship. */
    for (enx = 0; enx < MAX_ENEMIES; enx++)
    {
        /* Is this enemy active? */
        if (enemies[enx].x == -1)
            continue;

        /* Move the enemy. */
        if (loop_counter % enemy_speed == 0)
        {
            enemies[enx].x += enemy_direction;
            if (enemies[enx].x < 1)
            {
                change_dir = 1;
            }
            if (enemies[enx].x + pic_enemy1.width - 1 >= PLAY_WIDTH)
            {
                change_dir = 1;
            }
        }

        /* Did this enemy hit the player? */
        if (player_x + pic_ship.width > enemies[enx].x &&
            player_x < enemies[enx].x + pic_enemy1.width &&
            player_y + pic_ship.height > enemies[enx].y &&
            player_y < enemies[enx].y + pic_enemy1.height)
        {
            /* Enemy hit player. */
            player_hit();
        }
    }

    /* See if enemies should change direction yet. */
    if (change_dir)
    {
        /* Change the direction of the enemies. */
        enemy_direction = -enemy_direction;

        /* Move the enemies down the screen a pixel. */
        for (enx = 0; enx < MAX_ENEMIES; enx++)
        {
            /* Is this enemy active? */
            if (enemies[enx].x == -1)
                continue;

            /* Move the enemy down. */
            enemies[enx].y++;

            /* Did enemy go off bottom of screen. */
            if (enemies[enx].y >= PLAY_HEIGHT)
                player_hit();
        }
    }
}

/*
** check_enemy_hit:
** Checks to see if an enemy was hit by one of the player's
** shots, and performs the appropriate actions if so.
**
** Parameters:
**    NONE
**
** Returns:
**    NONE
*/
static void
check_enemy_hit(void)
{
    int    enx;
    int    shotndx;

    /* Check each of the player's shots. */
    for (shotndx = 0; shotndx < MAX_SHOTS; shotndx++)
    {
        /* Is this shot active? */
        if (shot_x[shotndx] == -1)
            continue;

        /* Check each enemy. */
        approx_num_enemies = 0;
        for (enx = 0; enx < MAX_ENEMIES; enx++)
        {
            /* Is this enemy active? */
            if (enemies[enx].x == -1)
                continue;

            /* Dit shot hit enemy? */
            if (shot_x[shotndx] + pic_player_shot1.width > enemies[enx].x &&
                shot_x[shotndx] < enemies[enx].x + pic_enemy1.width &&
                shot_y[shotndx] + pic_player_shot1.height > enemies[enx].y &&
                shot_y[shotndx] < enemies[enx].y + pic_enemy1.height)
            {
                /* Shot hit enemy! */
                /* Play sound effect. */
                spk_play(sound_enemy_hit, strlen(sound_enemy_hit));

                /* Turn on new explosion. */
                last_explode = (last_explode + 1) % MAX_EXPLODE;
                explode[last_explode].x = enemies[enx].x + pic_enemy1.width / 2;
                explode[last_explode].y = enemies[enx].y + pic_enemy1.height / 2;
                explode[last_explode].age = 0;

                /* Turn off the enemy. */
                enemies[enx].x = -1;

                /* Turn off the shot. */
                shot_x[shotndx] = -1;

                /* Update player's score. */
                score += 1;
            }
            approx_num_enemies++;
        }
    }
}

/*
** check_end_wave:
** Checks to see if all enemies have been destroyed,
** and performs appropriate actions if so.
**
** Parameters:
**    NONE
**
** Returns:
**    NONE
*/
static void
check_end_wave(void)
{
    int    enx;

    /* Check if there are any active enemies. */
    for (enx = 0; enx < MAX_ENEMIES; enx++)
    {
        if (enemies[enx].x != -1)
        {
            /* Enemy still alive; wave is not done yet. */
            return;
        }
    }

    /* Play wave completed sound. */
    spk_play(sound_wave_done, strlen(sound_wave_done));

    /* Start the next attack wave. */
    loop_counter = 0;
    loop_mode = MODE_WAVE_DONE;
}

/*
** start_new_game:
** Starts a new game.  The demo_mode variable
** should be set prior to calling this function.
**
** Parameters:
**    NONE
**
** Returns:
**    NONE
*/
static void
start_new_game(void)
{
    score = 0;
    attack_wave = 0;
    num_ships = 3;
    start_attack_wave();
}

/*
** player_actions:
** Processes player actions for one frame.
**
** Parameters:
**    NONE
**
** Returns:
**    NONE
*/
static void
player_actions(void)
{
    int    i;

    /* Process player movement. */
    if (loop_counter % MOVE_SPEED == 0)
    {
        if (!demo_mode)
        {
            /* Get movement inputs. */
            if (kbd_check(SC_LEFT))
                player_x--;
            if (kbd_check(SC_RIGHT))
                player_x++;
        }

        /* Make sure player's ship stays on screen. */
        if (player_x < -2)
            player_x = -2;
        if (player_x > PLAY_WIDTH - 3)
            player_x = PLAY_WIDTH - 3;
    }

    /* Move any active player shots. */
    if (loop_counter % SHOT_SPEED == 0)
    {
        /* Check each possible shot. */
        for (i = 0; i < MAX_SHOTS; i++)
        {
            /* Is this shot active? */
            if (shot_x[i] != -1)
            {
                /* Active shot; move it. */
                shot_y[i] -= 1;
                if (shot_y[i] < -5)
                {
                    /* Turn off the shot. */
                    shot_x[i] = -1;
                }
            }
        }
    }

    /* Process player shooting. */
    if (!demo_mode)
    {
        if (!kbd_check(SC_SPACE))
            did_shoot = 0;
        if (did_shoot == 0 && kbd_check(SC_SPACE))
        {
            /* Find an inactive shot. */
            for (i = 0; i < MAX_SHOTS; i++)
            {
                /* Is this shot active? */
                if (shot_x[i] == -1)
                    break;
            }
            if (i < MAX_SHOTS)
            {
                /* Start a new shot. */
                shot_x[i] = player_x + 2;
                shot_y[i] = player_y - 3;

                /* Play shooting sound effect. */
                spk_play(sound_shoot, strlen(sound_shoot));

                /* Set flag to keep shots from stringing. */
                did_shoot = 1;
            }
        }
    }
}

/*
** game_loop:
** Runs the main loop for the game.
**
** Parameters:
**    NONE
**
** Returns:
**    NONE
*/
static void
game_loop(void)
{
    int    done = 0;

    demo_mode = 1;
    start_new_game();
    while (!done)
    {
        /* Generate screen. */
        draw_frame();
        tm_update();

        /* Play sound. */
        spk_poll();

        /* Flush the keyboard buffer. */
        kbd_flush();

        /* Handle the different game modes. */
        if (loop_mode == MODE_START_WAVE &&
            loop_counter > TIME_START_WAVE)
        {
            /* Switch from START_WAVE to PLAY. */
            loop_mode = MODE_PLAY;
            loop_counter = 0;
        }
        else if (loop_mode == MODE_WAVE_DONE)
        {
            player_actions();
            explode_actions();
            if (loop_counter > TIME_WAVE_DONE)
            {
                attack_wave++;
                start_attack_wave();
            }
        }
        else if (loop_mode == MODE_LOSE_SHIP &&
            loop_counter > TIME_LOSE_SHIP)
        {
            /* Restart on same wave. */
            start_attack_wave();
        }
        else if (loop_mode == MODE_PLAY)
        {
            /* Normal game play stuff. */
            player_actions();
            if (loop_mode != MODE_PLAY)
                continue;
            enemy_actions();
            if (loop_mode != MODE_PLAY)
                continue;
            check_enemy_hit();
            if (loop_mode != MODE_PLAY)
                continue;
            explode_actions();
            check_end_wave();
        }
        else if (loop_mode == MODE_GAME_OVER &&
            loop_counter > TIME_GAME_OVER)
        {
            /* Switch from GAME_OVER back to demo game. */
            demo_mode = 1;
            start_new_game();
        }

        /* Check if user wants to start a new game. */
        if (demo_mode)
        {
            if (kbd_check(SC_SPACE))
            {
                /* Start a new game. */
                demo_mode = 0;
                start_new_game();
            }
        }

        /* Check if user wants to quit. */
        if (kbd_check(SC_ESCAPE))
        {
            while (kbd_check(SC_ESCAPE))
                ; /* Wait. */
            if (demo_mode)
            {
                /* Exit program. */
                done = 1;
            }
            else
            {
                /* Quit current game - return to demo. */
                if (loop_mode != MODE_GAME_OVER)
                {
                    loop_counter = 0;
                    loop_mode = MODE_GAME_OVER;
                }
            }
        }

        /* Update the number of frames drawn so far. */
        loop_counter++;
    }
}

/*
** main:
** C application entry point.
*/
int cdecl
main(int argc, char **argv)
{
    /* Startup. */
    printf("Initializing...\n");
    if (!tm_init())
    {
        printf("\nFailed initializing video card.\n");
        printf("This program requires a color display.\n");
        return EXIT_FAILURE;
    }
    spk_init();
    kbd_init();

    /* Process. */
    game_loop();

    /* Shutdown. */
    kbd_flush();
    kbd_deinit();
    spk_stop();
    spk_deinit();
    tm_deinit();
    printf("\nBye...\n\n");

    return EXIT_SUCCESS;
}

/* End main.c */
