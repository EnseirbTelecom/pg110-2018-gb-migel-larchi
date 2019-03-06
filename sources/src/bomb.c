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
  bomb->TIME=SDL_GetTicks() ;
  bomb->player=player;
  return bomb;
};


void bomb_free(struct bomb* bomb){
  assert(bomb);
  free(bomb);
};

int bomb_get_state(struct bomb* bomb){
  assert(bomb);
  int state=SDL_GetTicks() - bomb->TIME;
  state=state/1000;
  if(state>3)
    state=3;
  return 3-state;
};

void bomb_display(struct bomb* bomb){
  assert(bomb);
  int state=bomb_get_state(bomb);
  window_display_image(sprite_get_bomb(state),
  (bomb->x)* SIZE_BLOC,(bomb->y)* SIZE_BLOC);
};
