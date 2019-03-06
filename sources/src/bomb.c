#include <SDL/SDL_image.h>
#include <assert.h>

#include <misc.h>
#include <window.h>
#include <player.h>
#include <sprite.h>
#include <constant.h>


struct bomb{
  int x,y;
  int TIME;
  struct player* player;
};

struct bomb* bomb_init(struct player* player){
  struct bomb* bomb=malloc(sizeof(*bomb));
  if (!bomb)
    error("Memory error");

  bomb->x=player_get_x(player);
  bomb->y=player_get_y(player);
  bomb->TIME=3;
  bomb->player=player;
  return bomb;
};


void bomb_free(struct bomb* bomb){
  assert(bomb);
  free(bomb);
};

void bomb_display(struct bomb* bomb){
  assert(bomb);
  window_display_image(sprite_get_bomb(bomb->TIME),
  (bomb->x)* SIZE_BLOC,(bomb->y)* SIZE_BLOC);
};
