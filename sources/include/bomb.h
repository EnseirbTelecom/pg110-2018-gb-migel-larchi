#include <player.h>

struct bomb;

//creates a bomb
struct bomb* bomb_init(struct map* map,struct player* player);
void bomb_free(struct bomb* bomb);

// Get bomb->player
struct player *bomb_get_player(struct bomb *bomb);

//get bomb->player
int bomb_get_exploded(struct bomb *bomb);

//set bomb->exploded
void bomb_set_exploded(struct bomb *bomb,int exploded);

void bomb_explosion_end(struct map *map,struct bomb* bomb);

// Display bomb on the screen
void bomb_display(struct map* map,struct bomb* bomb);

//get bomb state
int bomb_get_state(struct bomb* bomb);

//set bomb explosion on map
void bomb_explosion_map_set(struct map* map,struct bomb* bomb);

int bomb_explosion_map_set_cell(struct map* map,int x,int y);

void bomb_update(struct map* map,struct bomb* bomb);
