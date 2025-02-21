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
#include <save_load.h>
#include <map.h>
#include <constant.h>

struct game {
	struct map** maps;       // the game's map
	short levels;        // nb maps of the game
	short level;
	struct player* player;
};

struct game*
game_new(void) {
	sprite_load(); // load sprites into process memory

	int levels = 0;
	struct game* game = malloc(sizeof(*game));
	game->maps = maps_init("./map","easy",&levels);
	game->levels = levels;
	game->level = 0;
	game->player = player_init(3);
	// Set default location of the player
	player_set_position(game->player, 1, 0);
	// Set default range of the player
	player_set_range(game->player, 4);
	return game;
}

struct game* game_load(struct map** maps, short levels,short level,struct player* player){
	//sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->maps = maps;
	game->levels = levels;
	game->level = level;
	game->player = player;
	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
}

struct map** game_get_maps(struct game* game){
	assert(game);
	return game->maps;
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

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 12;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	int x;

	x = 0 * white_bloc;
	window_display_image(sprite_get_banner_flag(),x,y);

	x = 0 * white_bloc + 1 * SIZE_BLOC;
	window_display_image(sprite_get_number(game->level),x,y);

	x = 1 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_key(),x,y);

	x = 1 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_key(game_get_player(game))),x,y);

	x = 2 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_life(), x, y);

	x = 2 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(
		sprite_get_number(player_get_life(game_get_player(game))), x, y);

	x = 3 * white_bloc + 6 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 3 * white_bloc + 7 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 4 * white_bloc + 8 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 4 * white_bloc + 9 * SIZE_BLOC;
	window_display_image(
		sprite_get_number(player_get_range(game_get_player(game))), x, y);
}

void game_display(struct game* game) {
	assert(game);
	window_clear();

	game_banner_display(game);

	map_display(game_get_current_map(game));

	player_display(game->player);
	window_refresh();
}

static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	struct bomb_list* bomb_list=*(map_get_bombs(map));
	int levels = game->levels;
	int x = player_get_x(player);
	int y = player_get_y(player);
	int p;
	int time;
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
				if (map_get_cell_type(map,x,y) == CELL_DOOR){
					if(map_get_door_type(map,x,y)%2 == 0)
						open_the_door(game);
				} else {
					if(player_get_nb_bomb(player)>=1){
						if (map_get_cell_type(map,x,y) != CELL_BOMB) {
							bomb_list_add(map,player,bomb_list);
							player_dec_nb_bomb(player);
						}
					}
				}
				break;

			case SDLK_p:
				time = SDL_GetTicks();
				p = pause();
				if (p==2) {
					save_create(game);
				}
				else if (p==1) {
					return 1;
				}
				window_create(SIZE_BLOC*map_get_width(map)
										 ,SIZE_BLOC*map_get_height(map) + BANNER_HEIGHT + LINE_HEIGHT);

				maps_end_pause(game->maps,levels,time);
				break;
			default:
				break;
			}

			break;
		}
	}
	return 0;
}

int game_update(struct game** game) {
	// game over
	if (player_get_life((*game)->player)==0) {
		return gover(game);
	}

	//win situation
	struct map* map=game_get_current_map(*game);
	if(map_get_cell_type(map,player_get_x((*game)->player),player_get_y((*game)->player))==CELL_SCENERY)
		return you_win(game);


	int speed = (game_get_current_lvl(*game))*((MONSTER_MAX_SPEED-MONSTER_MIN_SPEED)/(game_get_levels(*game)-1))+MONSTER_MIN_SPEED;
	map_set_monsters_speed(map,speed);
	player_update_state(map,(*game)->player);
	maps_update((*game)->player,(*game)->maps,(*game)->levels);
	if (input_keyboard((*game)))
		return 1; // exit game

	return 0;
}

int game_get_levels(struct game* game){
	assert(game);
	return game->levels;
}

int game_get_current_lvl(struct game* game){
	assert(game);
	return game->level;
}
void game_set_current_lvl(struct game* game,int lvl) {
	assert(game);
	game->level = lvl;
	struct map* map = game_get_current_map(game);
	window_create(SIZE_BLOC*map_get_width(map)
	             ,SIZE_BLOC*map_get_height(map) + BANNER_HEIGHT + LINE_HEIGHT);

}
