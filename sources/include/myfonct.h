#ifndef MYFONC_
#define MYFONC_

#include <map.h>
#include <player.h>
#include <constant.h>


// fonction to check if a boxe can be mouve
int boxe_move(enum direction direction,struct map* map,int x,int y);

// modification of player state when he move to a CELL_BONUS
int cell_bonus_move(struct map* map,struct player *player,int x,int y);

// change the current level when player move to a door
void door_move(struct game* game);

// to open the door
void open_the_door(struct game* game);

int pause();
int gover();
int you_win(struct game** game);
struct game* start_game(struct game** game);
#endif
