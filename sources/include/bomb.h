#include <player.h>

struct bomb;

//creates a bomb
struct bomb* bomb_init(struct player* player);
void bomb_free(struct bomb* bomb);

// Display bomb on the screen
void bomb_display(struct map* map,struct bomb* bomb);

//get bomb state
int bomb_get_state(struct bomb* bomb);

//Display bomb explosion
void bomb_explosion_display(struct map* map,struct bomb* bomb);

void bomb_explosion_display_cell(struct map* map,int x,int y);
