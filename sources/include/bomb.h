#include <player.h>

struct bomb;

//creates a bomb
struct bomb* bomb_init(struct player* player);
void bomb_free(struct bomb* bomb);

// Display bomb on the screen
void bomb_display(struct bomb* bomb);

//get bomb state
int bomb_get_state(struct bomb* bomb);
