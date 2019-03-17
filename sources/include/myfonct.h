#include <map.h>
#include <player.h>
#include <constant.h>

int boxe_move(enum direction direction,struct map* map,int x,int y);

int cell_bonus_move(struct map* map,struct player *player,int x,int y);
