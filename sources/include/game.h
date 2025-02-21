/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>

// Abstract data type
struct game;

// Create a new game
struct game* game_new();
struct game* game_load(struct map** maps, short levels,short level,struct player* player);

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);

struct map** game_get_maps(struct game* game);
struct map* game_get_current_map(struct game* game);

// Display the game on the screen
void game_display(struct game* game);

// update
int game_update(struct game** game);

//set current map
int game_get_levels(struct game* game);
int game_get_current_lvl(struct game* game);
void game_set_current_lvl(struct game* game,int lvl);

struct bomb_list** map_get_bombs(struct map* map);

#endif /* GAME_H_ */
