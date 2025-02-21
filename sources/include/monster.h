#ifndef MONSTER_H_
#define MONSTER_H_
#include <constant.h>
#include <map.h>
#include <player.h>

struct monster;

// Creates a new monster
struct monster* monster_init();
//set the monster speed;
void  monster_set_speed(struct monster* monster,int speed);
int monster_get_speed(struct monster* monster);
void monster_free();

// Set the position of the monster
void monster_set_position(struct monster *monster, int x, int y);
// Returns the current position of the monster
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);
int monster_get_lastTime(struct monster* monster);

// Set the direction of the next move of the monster
void monster_set_current_way(struct monster *monster, enum direction direction);

// Move the monster according to the current direction
int monster_move(struct monster* monster, struct map* map);
static int monster_move_door(struct monster* monster, struct map* map, int x, int y) ;
void monster_death(struct monster* monster);
// Display the monster on the screen
void monster_display(struct monster* monster);
void monster_update(struct monster* monster, struct player *player, struct map* map, unsigned int speed);
void monster_new_pos(struct monster* monster, struct player *player, struct map* map);
void monster_change_time(struct monster* monster,int time);

#endif
