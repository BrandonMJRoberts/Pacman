#pragma once

// ------------------------------------------------------------------ //

enum class FACING_DIRECTION : char
{
	UP    = 0x00,
	DOWN  = 0x01,
	LEFT  = 0x02,
	RIGHT = 0x04,
	NONE  = 0x08
};

// ------------------------------------------------------------------ //

enum class SELECTION_OPTIONS : char
{
	START_GAME    = 0x00,
	HIGHSCORES    = 0x01,
	CHANGE_PLAYER = 0x02,
	CHANGE_NAME   = 0x04,
	QUIT          = 0x08,
	NONE          = 0x16
};

// ------------------------------------------------------------------ //

enum class PLAYER_CHARACTER_TYPE : char
{
	PACMAN       = 0x00,
	RED_GHOST    = 0x01,
	BLUE_GHOST   = 0x02,
	ORANGE_GHOST = 0x04,
	PINK_GHOST   = 0x08
};

// ------------------------------------------------------------------ //

enum class PICKUP_TYPES : char
{
	CHERRY     = 0,
	STRAWBERRY = 1,
	PEACH      = 2,
	APPLE      = 3,
	LIME       = 4,
	CUP        = 5,
	BELL       = 6,
	KEY        = 7
};

// ------------------------------------------------------------------ //

enum class SCREENS : char
{
	MAIN_MENU   = 0x00,
	PAUSE_MENU  = 0x01,
	IN_GAME     = 0x02,
	HIGH_SCORES = 0x04,
	SAME        = 0x08,
	PRIOR       = 0x16,
	QUIT        = 0x32
};

// ------------------------------------------------------------------ //