#pragma once

#define SCREEN_HEIGHT 696
#define SCREEN_WIDTH  448

#define HALF_SCREEN_HEIGHT   (SCREEN_HEIGHT / 2)
#define HALF_SCREEN_WIDTH    (SCREEN_WIDTH / 2)

#define QUATER_SCREEN_HEIGHT (SCREEN_HEIGHT / 4)
#define QUATER_SCREEN_WIDTH  (SCREEN_WIDTH / 4)

#define SPRITE_RESOLUTION            16
#define TWICE_SPRITE_RESOLUTION (SPRITE_RESOLUTION * 2)
#define HALF_SPRITE_RESOLUTION  (SPRITE_RESOLUTION / 2)

#define TIME_IN_POWER_PELLET         3.0f
#define TIME_POINTS_SHOW_FOR         4.0f

#define PLAYER_CHANGE_DIRECTION_DELAY 0.08f
#define GHOST_CHANGE_DIRECTION_DELAY  1.0f

#define PACMAN_MOVEMENT_SPEED    9.1f
#define GHOST_MOVEMENT_SPEED     8.5f
#define GHOST_EYE_MOVEMENT_SPEED 13.0f

#define TIME_IN_PRE_GAME         5.0f

#define POINTS_PER_EXTRA_LIFE_PACMAN    10000
#define POINTS_PER_EXTRA_LIFE_GHOST     30000

#define POINTS_PER_PACMAN_KILL   10000

#define AMOUNT_OF_LEVELS          3
#define STARTING_LIFE_COUNT       2
#define NUMBER_OF_GHOSTS_IN_LEVEL 4

#define FRAME_RATE 60
#define FRAME_TIME (1.0f / FRAME_RATE)