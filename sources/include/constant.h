/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef CONSTANT_H_
#define CONSTANT_H_

#define WINDOW_NAME "[PG110] Bombeirb 2018-2019"

// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       40

// Size (# of pixels) of banner
#define LINE_HEIGHT	4
#define BANNER_HEIGHT	40

// Time management
#define DEFAULT_GAME_FPS 30

// map size
#define STATIC_MAP_WIDTH  12
#define STATIC_MAP_HEIGHT 12

#define GAME_OVER_WIDTH 472
#define GAME_OVER_HEIGHT 441

#define GAME_PAUSE_WIDTH 472
#define GAME_PAUSE_HEIGHT 441

#define GAME_START_WIDTH 472
#define GAME_START_HEIGHT 441

#define MONSTER_MAX_SPEED 400
#define MONSTER_MIN_SPEED 1000



enum direction {
	NORTH = 0,
	EAST,  // 1
	SOUTH, // 2
	WEST   // 3
};

#endif /* CONSTANT */
