/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb_list.h>
#include <monster_list.h>
#include <myfonct.h>

struct game {
	struct map** maps;       // the game's map
	short levels;        // nb maps of the game
	short level;
	struct player* player;
	struct bomb_list* bombs; //list of bombs
};

struct game*
game_new(void) {
	sprite_load(); // load sprites into process memory

	int levels = 0;
	struct game* game = malloc(sizeof(*game));
	game->maps = malloc(sizeof(struct game));
	game->maps = maps_init("./map","easy",&levels);
	game->levels = levels;
	game->level = 0;
	game->bombs=bomb_list_init();
	game->player = player_init(3);

	// Set default location of the player
	player_set_position(game->player, 1, 0);
	// Set default range of the player
	player_set_range(game->player, 1);
	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->level];
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(
		sprite_get_number(player_get_life(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(
		sprite_get_number(player_get_range(game_get_player(game))), x, y);
}

void game_display(struct game* game) {
	assert(game);
	window_clear();
	game_banner_display(game);
	bomb_list_display(game_get_current_map(game),game->bombs);
	map_display(game_get_current_map(game));
	player_display(game->player);
	window_refresh();
}

static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	struct bomb_list* bomb_list=game->bombs;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map);
				door_move(game);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map);
				door_move(game);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map);
				door_move(game);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map);
				door_move(game);
				break;
			case SDLK_SPACE:
				if(player_get_nb_bomb(player)>=1){
					bomb_list_add(map,player,bomb_list);
					player_dec_nb_bomb(player);
				}
				break;
			default:
				break;
			}

			break;
		}
	}
	return 0;
}

int game_update(struct game* game) {
	struct map* map=game_get_current_map(game);
	player_update_state(map,game->player);
	bomb_list_update(map,&(game->bombs));
	monster_list_update(map);

	if (input_keyboard(game))
		return 1; // exit game

	return 0;
}

void game_set_current_lvl(struct game* game,int lvl) {
	assert(game);
	game->level = lvl;
}
