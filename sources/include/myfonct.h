#include <map.h>
#include <player.h>
#include <constant.h>

int boxe_move(enum direction direction,struct map* map,int x,int y);

int cell_bonus_move(struct map* map,struct player *player,int x,int y);

void door_move(struct game* game);

void open_the_door(struct game* game);

// pause fonction
int pause();

// game_over fonction
int gover();

// win fonction
int you_win(struct game** game);

// start game finction
struct game* start_game(struct game** game);

// get the file path when the have the dir path and file name
char* get_file_path(char* char1,char* char2);
