#include <SDL/SDL_image.h>
#include <assert.h>

#include <bomb.h>
#include <misc.h>
#include <window.h>
#include <player.h>
#include <sprite.h>
#include <constant.h>
#include  <map.h>
#include <monster_list.h>

#define EXPTIME1	600
#define EXPTIME2	1200
#define EXPTIME3	1800
#define EXPTIME4	2400
#define EXPTIME5	3000

struct bomb{
  int x,y;
  int TIME;
  int range;
  int exploded;
  struct player* player;
  int state;
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
  bomb->range=player_get_range(player);
  bomb->state=4;
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
  return bomb->state;
};

void bomb_display(struct map *map,struct bomb *bomb){
  assert(bomb);
  int x=bomb->x;
  int y=bomb->y;
  int state=bomb_get_state(bomb);
  window_display_image(sprite_get_bomb(state),x* SIZE_BLOC,y* SIZE_BLOC);
};

void bomb_update_state(struct bomb* bomb) {
  assert(bomb);
  int TIME = SDL_GetTicks() - bomb->TIME;
  if (TIME<EXPTIME1) {
    bomb->state = 4;
  } else if (TIME<EXPTIME2) {
    bomb->state = 3;
  } else if (TIME<EXPTIME3) {
    bomb->state = 2;
  } else if (TIME<EXPTIME4) {
    bomb->state = 1;
  } else if (TIME<EXPTIME5) {
    bomb->state = 0;
  } else{
      bomb->state = -1;
  }
}

void bomb_update(struct map* map,struct bomb* bomb){
  assert(map);
  assert(bomb);
  bomb_update_state(bomb);
  int state = bomb_get_state(bomb);
  if(state==0 && (bomb->exploded)==0){
    //si la bombe vient exploser
    bomb_explosion_map_set(map,bomb);
    bomb->exploded=1;
  }else if (state == 0 && (bomb->exploded)==2) {
    //si une bombe a exploser, on rafraichie les explosion
    bomb_explosion_map_set(map,bomb);
  }
}

int bomb_explosion_map_set_cell(struct map *map,int x,int y) {
    enum cell_type cell_type = map_get_cell_type(map,x,y);
    enum bonus_type bonus_type=map_get_bonus_type(map,x,y);

    switch (cell_type) {
      case CELL_EMPTY:
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        return 1;
        break;


      case CELL_KEY:
        break;

      case CELL_BONUS:
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        return 1;
        break;

      case CELL_MONSTER:
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        return 1;
        break;

      case CELL_SCENERY:
        break;

      case CELL_DOOR:
        break;

      case CELL_EXPLOSION:
        return 1;
        //window_display_image(sprite_get_bomb(0),x*SIZE_BLOC,y*SIZE_BLOC);
      break;

      case CELL_BOMB:
        return 1;
        break;

    case CELL_BOX:
      map_set_cell_type(map,x,y,CELL_EXPLOSION);
      map_set_bonus_type(map,x,y,bonus_type);
      return 0;
      break;
    }
  return 0;
}

int bomb_explosion_map_set_cell_2(struct map *map,int x,int y) {
    enum cell_type cell_type = map_get_cell_type(map,x,y);

    switch (cell_type) {
      case CELL_EMPTY:
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        return 1;
        break;


      case CELL_KEY:
        break;

      case CELL_BONUS:
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        return 1;
        break;

      case CELL_MONSTER:

        break;

      case CELL_SCENERY:
        break;

      case CELL_DOOR:
        break;

      case CELL_EXPLOSION:
        return 1;
        //window_display_image(sprite_get_bomb(0),x*SIZE_BLOC,y*SIZE_BLOC);
      break;

      case CELL_BOMB:
        return 1;
        break;

    case CELL_BOX:
      return 0;
      break;
    }
  return 0;
}

void bomb_explosion_map_set(struct map* map,struct bomb* bomb){

  int x=bomb->x;
  int y=bomb->y;
  int range=bomb->range;

  int xi=x+1;
  int x_i=x-1;
  int yi=y+1;
  int y_i=y-1;

  if (bomb_get_exploded(bomb)==0) {
    int i=0;
    while (i<range) {
      if(map_is_inside(map,x,yi) && bomb_explosion_map_set_cell(map,x,yi)){
        yi++;
        i++;
      }else{
        break;
      }
    }

    i=0;
    while (i<range) {
      if(map_is_inside(map,x,y_i) && bomb_explosion_map_set_cell(map,x,y_i)){
        y_i--;
        i++;
      }else{
        break;
      }
    }

    i=0;
    while (i<range) {
      if(map_is_inside(map,xi,y) && bomb_explosion_map_set_cell(map,xi,y)){
        xi++;
        i++;
      }else{
        break;
      }
    }

    i=0;
    while (i<range) {
      if(map_is_inside(map,x_i,y) && bomb_explosion_map_set_cell(map,x_i,y)){
        x_i--;
        i++;
      }else{
        break;
      }
    }
  }
  else if (bomb_get_exploded(bomb)==2) {
    int i=0;
    while (i<range) {
      if(map_is_inside(map,x,yi) && bomb_explosion_map_set_cell_2(map,x,yi)){
        yi++;
        i++;
      }else{
        break;
      }
    }

    i=0;
    while (i<range) {
      if(map_is_inside(map,x,y_i) && bomb_explosion_map_set_cell_2(map,x,y_i)){
        y_i--;
        i++;
      }else{
        break;
      }
    }

    i=0;
    while (i<range) {
      if(map_is_inside(map,xi,y) && bomb_explosion_map_set_cell_2(map,xi,y)){
        xi++;
        i++;
      }else{
        break;
      }
    }

    i=0;
    while (i<range) {
      if(map_is_inside(map,x_i,y) && bomb_explosion_map_set_cell_2(map,x_i,y)){
        x_i--;
        i++;
      }else{
        break;
      }
    }
  }

};

void bomb_explosion_end(struct map *map,struct bomb* bomb){
  assert(bomb);
  assert(map);

  int x=bomb->x;
  int y=bomb->y;
  int range=bomb->range;

  int xi=x+1;
  int x_i=x-1;
  int yi=y+1;
  int y_i=y-1;
  map_set_cell_type(map,x,y,CELL_EMPTY);
  for ( int i=0 ; i < range; i++) {
    if(map_is_inside(map,x,yi) && (map_get_cell_type(map,x,yi)==CELL_EXPLOSION
      || map_get_cell_type(map,x,yi)==CELL_BOMB
      || map_get_cell_type(map,x,yi)==CELL_EMPTY)){
      enum bonus_type bonus_type=map_get_bonus_type(map,x,yi);
      if (bonus_type) {
        map_set_cell_type(map,x,yi,CELL_BONUS);
        map_set_bonus_type(map,x,yi,bonus_type);
      }else{
        map_set_cell_type(map,x,yi,CELL_EMPTY);
      }
      yi++;
    }

    if(map_is_inside(map,xi,y) && (map_get_cell_type(map,xi,y)==CELL_EXPLOSION
    || map_get_cell_type(map,xi,y)==CELL_BOMB
    || map_get_cell_type(map,xi,y)==CELL_EMPTY)){
      enum bonus_type bonus_type=map_get_bonus_type(map,xi,y);
      if (bonus_type) {
        map_set_cell_type(map,xi,y,CELL_BONUS);
        map_set_bonus_type(map,xi,y,bonus_type);
      }else{
        map_set_cell_type(map,xi,y,CELL_EMPTY);
      }
      xi++;
    }

    if(map_is_inside(map,x,y_i) && (map_get_cell_type(map,x,y_i)==CELL_EXPLOSION
    || map_get_cell_type(map,x,y_i)==CELL_BOMB
    || map_get_cell_type(map,x,y_i)==CELL_EMPTY)){
      enum bonus_type bonus_type=map_get_bonus_type(map,x,y_i);
      if (bonus_type) {
        map_set_cell_type(map,x,y_i,CELL_BONUS);
        map_set_bonus_type(map,x,y_i,bonus_type);
      }else{
        map_set_cell_type(map,x,y_i,CELL_EMPTY);
      }

      y_i--;
    }
    if(map_is_inside(map,x_i,y) && (map_get_cell_type(map,x_i,y)==CELL_EXPLOSION
    || map_get_cell_type(map,x_i,y)==CELL_BOMB
    || map_get_cell_type(map,x_i,y)==CELL_EXPLOSION)){
      enum bonus_type bonus_type=map_get_bonus_type(map,x_i,y);
      if (bonus_type) {
        map_set_cell_type(map,x_i,y,CELL_BONUS);
        map_set_bonus_type(map,x_i,y,bonus_type);
      }else{
        map_set_cell_type(map,x_i,y,CELL_EMPTY);
      }
      x_i--;
    }
  }
};
