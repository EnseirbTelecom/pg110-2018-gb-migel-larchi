/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL.h>
#include <SDL_audio.h>

#include <constant.h>
#include <game.h>
#include <window.h>
#include <misc.h>
#include <save_load.h>
#include <sprite.h>
#include <myfonct.h>

int main(int argc, char *argv[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	struct game* game = game_new();
	/*
	game_free(game);
 	//game =  load_from_file("load.txt");
  game =  load_save("./save/saved.txt");
	*/

	window_create(SIZE_BLOC * STATIC_MAP_WIDTH,
	SIZE_BLOC * STATIC_MAP_HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed,done;

	// game loop
	// static time rate implementation
	done = start_game(&game);
	while (!done) {
		timer = SDL_GetTicks();

		game_display(game);
		done = game_update(&game);

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}
	SDL_Quit();
	game_free(game);

	return EXIT_SUCCESS;
}
