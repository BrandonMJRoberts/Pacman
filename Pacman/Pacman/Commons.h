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
	START_GAME = 0x00,
	HIGHSCORES = 0x01,
	CHANGE_PLAYER = 0x02,
	QUIT = 0x04,
	NONE = 0x08
};

// ------------------------------------------------------------------ //

enum class PLAYER_CHARACTER_TYPE : char
{
	PACMAN = 0x00,
	RED_GHOST = 0x01,
	BLUE_GHOST = 0x02,
	ORANGE_GHOST = 0x04,
	PINK_GHOST = 0x08
};

// ------------------------------------------------------------------ //

enum class PICKUP_TYPES : char
{
	CHERRY     = 0x00,
	STRAWBERRY = 0x01,
	PEACH      = 0x02,
	APPLE      = 0x04,
	LIME       = 0x08,
	CUP        = 0x10,
	BELL       = 0x20,
	KEY        = 0x40
};