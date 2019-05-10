#include <map.h>
#include <player.h>
#include <constant.h>

int boxe_move(enum direction direction,struct map* map,int x,int y);

int cell_bonus_move(struct map* map,struct player *player,int x,int y);

void door_move(struct game* game);

void open_the_door(struct game* game);

int pause();
int gover();
int you_win(struct game** game);
struct game* start_game(struct game** game);
