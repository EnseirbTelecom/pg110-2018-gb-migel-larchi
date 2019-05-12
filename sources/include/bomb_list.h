#ifndef BOMBL_
#define  BOMBL_

#include <player.h>
#include <bomb.h>


struct bomb_list;

struct bomb_list* bomb_list_init();

//Free fisrt element of bomb_list
void bomb_list_free_first_ele(struct bomb_list* bomb_list);

// Free all bomb_list
void bomb_list_free(struct bomb_list** bomb_list);

// Display bomb list on the screen
void bomb_list_display(struct map* map,struct bomb_list* bombs);

//add bomb
void bomb_list_add(struct map* map,struct player* player,struct bomb_list* bombs);

// set bomb->exploded on bomb_list
void bomb_list_set_exploded_to_2(struct bomb_list *bomb_list,int exploded);

void bomb_list_update(struct map *map, struct bomb_list** pbomb_list);

void bomb_list_change_time(struct bomb_list* bomb_list,int time);
#endif
