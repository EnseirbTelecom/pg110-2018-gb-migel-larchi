#include <player.h>
#include <bomb.h>


struct bomb_list;

struct bomb_list* bomb_list_init();

//Free fisrt element of bomb_list
void bomb_list_free_first_ele(struct bomb_list* bomb_list);

// Free all bomb_list
void bomb_list_free(struct bomb_list* bomb_list);

// Display bomb list on the screen
void bomb_list_display(struct bomb_list* bombs);

//add bomb
void bomb_list_add(struct player* player,struct bomb_list* bombs);
