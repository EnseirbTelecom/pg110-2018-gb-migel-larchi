/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <myfonct.h>

struct player {
	int life;
	int x, y;
	enum direction direction;
	int bombs; //nb of bomb that player can put right now
	int max_bomb;
	int  range;
	int state; // state=>0 => player est invulneratble
};

struct player* player_init(int bombs) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->direction = NORTH;
	player->bombs = bombs;
	player->max_bomb = bombs;
	player->life=3;
	player->state=-1;
	return player;
}

int player_get_life(struct player *player){
	assert(player);
	return player->life;
}

void player_inc_life(struct player* player) {
	assert(player);
	player->life +=1;
}

void player_dec_life(struct player *player){
	assert(player);
	player->life -=1;
}

void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}

void player_set_range(struct player *player, int range){
	assert(player);
	player->range =range;
}

void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

int player_get_range(struct player* player){
	assert(player != NULL);
	return player->range;
}


void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->bombs -= 1;
}

int  player_get_max_bomb(struct player * player){
		assert(player);
		return player->max_bomb;
}

void player_inc_max_bomb(struct player* player) {
	assert(player);
	player->max_bomb += 1;
}

void player_dec_max_bomb(struct player* player) {
	assert(player);
	player->max_bomb -= 1;
}

void player_inc_range(struct player * player){
	assert(player);
	player->range += 1;
}

void player_dec_range(struct player * player){
	assert(player);
	player->range -= 1;
}

static int player_move_aux(struct player* player, struct map* map, int x, int y) {
	enum direction direction;
	direction=player->direction;

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;		// Les mouvements du joueur sont limités les éléments de décors
		break;

	case CELL_BOMB:
		return 0;
		break;

	case CELL_BOX:
		return boxe_move(direction,map,x,y);
		break;

	case CELL_BONUS:
		cell_bonus_move(map,player,x,y);
		map_set_cell_type(map,x,y,CELL_EMPTY);
		break;

	case CELL_MONSTER:
		break;

	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1)) {
			player->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1)) {
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y)) {
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y)) {
			player->x++;
			move = 1;
		}
		break;
	}

	if (move && (map_get_cell_type(map,x,y)!=CELL_BOMB) ) {
		//apres pose de la bomb puis deplacement la bombe ne disparait pas
		map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return move;
}

void player_update_state(struct map *map,struct player* player) {

	int x=player->x;
	int y=player->y;
	int state=player->state;
	enum cell_type cell_type=map_get_cell_type(map,x,y);

	if (state < 0) {
		switch (cell_type) {
			case CELL_MONSTER:
				player_dec_life(player);
				player->state=SDL_GetTicks();

			break;

			case CELL_EXPLOSION:
				player_dec_life(player);
				player->state=SDL_GetTicks();
			default:
			break;
		}
	}else{
		if((SDL_GetTicks()-state)>1000)
			player->state=-1;
	}
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}
