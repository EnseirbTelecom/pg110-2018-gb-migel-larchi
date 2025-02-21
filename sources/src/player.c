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
	int state; // state=>0 => player est invulneratble ; -1 = vulneratble
	int key;
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
	player->key=0;

	return player;
}
void player_free(struct player* player) {
	assert(player);
	free(player);
}

//fonction link to player->x and player->y
void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}
int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}
int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

//fonction link to player->direction
enum direction player_get_direction(struct player* player){
	assert(player);
	return player->direction;
}
void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}

//fonction link to player->nb_bomb
int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
}
void player_set_nb_bomb(struct player* player,int nb_bomb){
	assert(player);
	player->bombs = nb_bomb;
}
void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->bombs += 1;
}
void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->bombs -= 1;
}

//fonction link to player->life
int player_get_life(struct player *player){
	assert(player);
	return player->life;
}
void player_set_life(struct player* player,int life){
		assert(player);
		player->life = life;
}
void player_inc_life(struct player* player) {
	assert(player);
	player->life +=1;
}
void player_dec_life(struct player *player){
	assert(player);
	player->life -=1;
}

//fonction link to player->range
int player_get_range(struct player* player){
	assert(player != NULL);
	return player->range;
}
void player_set_range(struct player* player,int range){
	assert(player);
	player->range = range;
}
void player_inc_range(struct player * player){
	assert(player);
	player->range += 1;
}
void player_dec_range(struct player * player){
	assert(player);
	player->range -= 1;
}

//fonction link to player->max_bomb
int  player_get_max_bomb(struct player * player){
	assert(player);
	return player->max_bomb;
}
void player_set_max_bomb(struct player* player,int max_bomb){
	assert(player);
	player->max_bomb = max_bomb;
}
void player_inc_max_bomb(struct player* player) {
	assert(player);
	player->max_bomb += 1;
}
void player_dec_max_bomb(struct player* player) {
	assert(player);
	player->max_bomb -= 1;
}

//fonction link to player->key
int player_get_key(struct player* player){
	assert(player);
	return player->key;
}
void player_set_key(struct player* player,int key){
	assert(player);
	player->key = key;
}
void player_inc_key(struct player* player){
	assert(player);
	player->key += 1;
}
void player_dec_key(struct player* player) {
	assert(player);
	player -> key -= 1;
}

void player_set_state(struct player* player,unsigned int time){
	assert(player);
	player->state = time;
}

static int player_move_aux(struct player* player, struct map* map, int x, int y) {
	enum direction direction;
	direction=player->direction;

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		if(map_get_bonus_type(map,x,y)==4) //4 = princess
			return 1;
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

 	case CELL_KEY:
	if (player->key <9) {
		map_set_cell_type(map,x,y,CELL_EMPTY);
		player_inc_key(player);
	}
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
		if((SDL_GetTicks()-state)>2500)
			player->state=-1;
	}
}

void player_display(struct player* player) {
	assert(player);
	if (player->state < 0) {
		window_display_image(sprite_get_player(player->direction),
		player->x * SIZE_BLOC, player->y * SIZE_BLOC);
	}else{
		if ( ((SDL_GetTicks() - player->state)%300)>150 ) {
			window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
		}else{
			window_display_image(sprite_get_player2(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
		}
	}
}
