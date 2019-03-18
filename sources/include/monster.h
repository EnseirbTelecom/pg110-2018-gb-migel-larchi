#ifndef MONSTER_H_
#define MONSTER_H_
#include <constant.h>
#include <map.h>

struct monster;

// Creates a new monster
struct monster* monster_init();
void monster_free();

// Set the position of the monster
void monster_set_position(struct monster *monster, int x, int y);

// Returns the current position of the monster
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);

// Set the direction of the next move of the monster
void monster_set_current_way(struct monster *monster, enum direction direction);

// Move the monster according to the current direction
int monster_move(struct monster* monster, struct map* map);

// Display the monster on the screen
void monster_display(struct monster* monster);
#endif
