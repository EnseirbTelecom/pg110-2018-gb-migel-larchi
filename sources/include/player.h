/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>

struct player;


// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number);
void   player_free(struct player* player);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);
// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
enum direction player_get_direction(struct player* player);
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_set_nb_bomb(struct player* player,int nb_bomb);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);

// Set, Increase, Decrease life of the player
int  player_get_life(struct player * player);
void player_set_life(struct player* player,int life);
void player_inc_life(struct player * player);
void player_dec_life(struct player * player);

// Set the range of the player
// Returns the current range of the player
int player_get_range(struct player* player);
// Increase, Decrease the range
void player_set_range(struct player* player,int range);
void player_inc_range(struct player * player);
void player_dec_range(struct player * player);

// Increase, Decrease the max_bomb
int player_get_max_bomb(struct player* player);
void player_set_max_bomb(struct player* player,int max_bomb);
void player_inc_max_bomb(struct player* player);
void player_dec_max_bomb(struct player* player);

int player_get_key(struct player* player);
void player_set_key(struct player* player,int key);
void player_inc_key(struct player * player);
void player_dec_key(struct player * player);

void player_set_state(struct player* player,unsigned int time);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);

void player_update_state(struct map *map,struct player* player);

#endif /* PLAYER_H_ */
