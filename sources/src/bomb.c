#include <SDL/SDL_image.h>
#include <assert.h>

#include <bomb.h>
#include <misc.h>
#include <window.h>
#include <player.h>
#include <sprite.h>
#include <constant.h>
#include  <map.h>

#define portee 4

struct bomb{
  int x,y;
  int TIME;
  int exploded;
  struct player* player;
};

struct bomb* bomb_init(struct map* map,struct player* player){
  //utiliser dans bomb_list_add
  struct bomb* bomb=malloc(sizeof(*bomb));
  if (!bomb)
    error("Memory error");

  int x=player_get_x(player);
  int y=player_get_y(player);

  map_set_cell_type(map,x,y,CELL_BOMB);
  bomb->x=x;
  bomb->y=y;
  bomb->TIME=SDL_GetTicks() ;
  bomb->exploded=0;
  bomb->player=player;
  return bomb;
};


void bomb_free(struct bomb* bomb){
  assert(bomb);
  free(bomb);
};

struct player *bomb_get_player(struct bomb *bomb){
  assert (bomb);
  return bomb->player;
};

int bomb_get_exploded(struct bomb *bomb){
  assert(bomb);
  return bomb->exploded;
};


void bomb_set_exploded(struct bomb *bomb,int exploded){
  assert(bomb);
  bomb->exploded=exploded;
};

int bomb_get_state(struct bomb* bomb){
  assert(bomb);
  int state=SDL_GetTicks() - bomb->TIME;
  state=state/1000;
  return 4-state;
};

void bomb_display(struct map* map,struct bomb* bomb){
  assert(bomb);
  int state=bomb_get_state(bomb);
  window_display_image(sprite_get_bomb(state),
  (bomb->x)* SIZE_BLOC,(bomb->y)* SIZE_BLOC);
  if(state==0)
    bomb_explosion_display(map,bomb);
};

void bomb_explosion_display_cell(struct map* map,int x,int y) {
    enum cell_type cell_type = map_get_cell_type(map,x,y);
    switch (cell_type) {
      case CELL_EMPTY:
      window_display_image(sprite_get_bomb(0),x*SIZE_BLOC,y*SIZE_BLOC);
      break;
      case CELL_BOX:
      break;
      case CELL_KEY:
      break;
      case CELL_BONUS:
      break;
      case CELL_MONSTER:
      break;
      case CELL_BOMB:
      break;
      case CELL_SCENERY:
      break;
      case CELL_DOOR:
      break;
    }
}

void bomb_explosion_display(struct map* map,struct bomb* bomb){
  int x=bomb->x;
  int y=bomb->y;
  for (int i = 1; i < 2; i++) {
    bomb_explosion_display_cell(map,x,y+i);
    bomb_explosion_display_cell(map,x,y-i);
    bomb_explosion_display_cell(map,x+i,y);
    bomb_explosion_display_cell(map,x-i,y);
  }
};
