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

#define STATEDELAY	1000
#define EXPDELAY	1000

struct bomb{
  int x,y;
  int TIME;
  int range;
  int exploded; // 0 pas exploser, 1 vient exploded, 2 au moin 2 bombe on exploser sur la map
  struct player* player;
  int state; // 4 a 0: 1 a 4 =>longeur de la meche, 0 => explotion, -1 => a del
  int rn,rs,re,rw; //la range range sur les 4 directions
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
   bomb->rn=1;
   bomb->rs=1;
   bomb->re=1;
   bomb->rw=1;
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

int bomb_get_time(struct bomb* bomb){
  assert(bomb);
  return  bomb->TIME;
}

void bomb_set_time(struct bomb* bomb,int time){
  assert(bomb);
  bomb->TIME = time;
}

void bomb_display(struct map *map,struct bomb *bomb){
  assert(bomb);
  int x=bomb->x;
  int y=bomb->y;
  int state=bomb_get_state(bomb);
  if (state >= 0)
    window_display_image(sprite_get_bomb(state),x* SIZE_BLOC,y* SIZE_BLOC);
};

void bomb_update_state(struct map* map,struct bomb* bomb) {
  assert(map);
  assert(bomb);
  int x = bomb->x;
  int y = bomb->y;
  int currentTIME = SDL_GetTicks();
  int DELAY = currentTIME - bomb->TIME;

  if (map_get_cell_type(map,x,y) == CELL_EXPLOSION
      && bomb->state >0) {
        //bomb na pas exploser => on change sont etat
    bomb->TIME = currentTIME;
    bomb->state = 0;
  } else {
    if (DELAY > STATEDELAY && bomb->state > 0) {
      bomb->TIME = currentTIME;
      bomb->state -= 1;
    }
    else if(DELAY > EXPDELAY && bomb->state == 0){
      bomb->state = -1;
    }
  }
}
/*
int bomb_explosion_map_set_cell(struct map *map,int x,int y) {
    enum cell_type cell_type = map_get_cell_type(map,x,y);
    enum bonus_type bonus_type=map_get_bonus_type(map,x,y);

    switch (cell_type) {
      case CELL_EMPTY:
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        return 1;
        break;

      case CELL_BONUS:
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        return 1;
        break;

      case CELL_MONSTER:
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        return 1;
        break;

      case CELL_EXPLOSION:
        return 1;
      break;

      case CELL_BOMB:
       map_set_cell_type(map,x,y,CELL_EXPLOSION);
        return 1;
        break;

    case CELL_BOX:
      map_set_cell_type(map,x,y,CELL_EXPLOSION);
      map_set_bonus_type(map,x,y,bonus_type);
      return 2;
      break;

    default:
    break;
    }
  return 0;
}
*/

int bomb_explosion_map_set_cell_2(struct map *map,int x,int y) {
    enum cell_type cell_type = map_get_cell_type(map,x,y);
    enum bonus_type bonus_type = map_get_bonus_type(map,x,y);
    switch (cell_type) {
      case CELL_EMPTY:
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        return 1;
        break;

      case CELL_BONUS:
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        return 1;
        break;

      default: return 0;
    }
}

int bomb_explosion_map_set_aux(struct map *map,int x,int y){
  //retourne int!=0 si explosion peut continuer de se propager
  enum cell_type cell_type = map_get_cell_type(map,x,y);

  switch (cell_type) {
    case CELL_EMPTY: return 1;
    case CELL_BONUS: return 1;
    case CELL_MONSTER: return 1;
    case CELL_EXPLOSION: return 1;
    case CELL_BOMB: return 1;
    case CELL_BOX: return 2;
    default:return 0;;
  }
}
void bomb_explosion_map_set(struct map* map,struct bomb* bomb){
  int x=bomb->x;
  int y=bomb->y;
  int range=bomb->range;
  int value;
  enum bonus_type bonus_type=map_get_bonus_type(map,x,y);
  for (int i = 1; i < range+1; i++) {
    y--;
    if (map_is_inside(map,x,y)) {
      value=bomb_explosion_map_set_aux(map,x,y);
      if(value ==1){
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        (bomb->rn)++;
      }else if (value ==2) {
        bonus_type=map_get_bonus_type(map,x,y);
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        map_set_bonus_type(map,x,y,bonus_type);
        (bomb->rn)++;
        break;
      }else{
        break;
      }
    }
  }

  x=bomb->x;
  y=bomb->y;
  for (int i = 1; i < range+1; i++) {
    y++;
    if (map_is_inside(map,x,y)) {
      value=bomb_explosion_map_set_aux(map,x,y);
      if(value ==1){
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        (bomb->rs)++;
      }else if (value ==2) {
        bonus_type=map_get_bonus_type(map,x,y);
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        map_set_bonus_type(map,x,y,bonus_type);
        (bomb->rs)++;
        break;
      }else{
        break;
      }
    }else{
      break;
    }
  }

  x=bomb->x;
  y=bomb->y;
  for (int i = 1; i < range+1; i++) {
      x++;
      if (map_is_inside(map,x,y)) {
        value=bomb_explosion_map_set_aux(map,x,y);
        if(value ==1){
          map_set_cell_type(map,x,y,CELL_EXPLOSION);
          (bomb->re)++;
        }
        else if (value ==2) {
          bonus_type=map_get_bonus_type(map,x,y);
          map_set_cell_type(map,x,y,CELL_EXPLOSION);
          map_set_bonus_type(map,x,y,bonus_type);
          (bomb->re)++;
          break;
        }else{
          break;
        }
      }else{
        break;
      }
  }

  x=bomb->x;
  y=bomb->y;
  for (int i = 1; i < range+1; i++) {
    x--;
    if (map_is_inside(map,x,y)) {
      value=bomb_explosion_map_set_aux(map,x,y);
      if(value ==1){
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        (bomb->rw)++;
      }
      else if (value ==2) {
        bonus_type=map_get_bonus_type(map,x,y);
        map_set_cell_type(map,x,y,CELL_EXPLOSION);
        map_set_bonus_type(map,x,y,bonus_type);
        (bomb->rw)++;
        break;
      }else{
        break;
      }
    }else{
      break;
    }
  }


}

void bomb_explosion_map_set_2(struct map* map,struct bomb* bomb){
  assert (map);
  assert(bomb);
  int x=bomb->x;
  int y=bomb->y;
  map_set_cell_type(map,x,y,CELL_EMPTY);
  for (int i = 1; i < (bomb->rn); i++) {
    bomb_explosion_map_set_cell_2(map,x,y-i);
  }
  for (int i = 1; i < (bomb->rs); i++) {
    bomb_explosion_map_set_cell_2(map,x,y+i);
  }
  for (int i = 1; i < (bomb->re); i++) {
    bomb_explosion_map_set_cell_2(map,x+i,y);
  }
  for (int i = 1; i < (bomb->rw); i++) {
    bomb_explosion_map_set_cell_2(map,x-i,y);
  }
}

void bomb_explosion_end_aux(struct map* map, int x, int y) {
    enum bonus_type bonus_type=map_get_bonus_type(map,x,y);
    if (bonus_type) {
      map_set_cell_type(map,x,y,CELL_BONUS);
      map_set_bonus_type(map,x,y,bonus_type);
    }else{
      map_set_cell_type(map,x,y,CELL_EMPTY);
    }
}

void bomb_explosion_end(struct map *map,struct bomb* bomb) {
  int x=bomb->x;
  int y=bomb->y;
  map_set_cell_type(map,x,y,CELL_EMPTY);
  for (int i = 1; i < (bomb->rn); i++) {
    bomb_explosion_end_aux(map,x,y-i);
  }
  for (int i = 1; i < (bomb->rs); i++) {
    bomb_explosion_end_aux(map,x,y+i);
  }
  for (int i = 1; i < (bomb->re); i++) {
    bomb_explosion_end_aux(map,x+i,y);
  }
  for (int i = 1; i < (bomb->rw); i++) {
    bomb_explosion_end_aux(map,x-i,y);
  }
}

void bomb_update(struct map* map,struct bomb* bomb){
  assert(map);
  assert(bomb);
  bomb_update_state(map,bomb);
  int state = bomb_get_state(bomb);
  if(state==0 && (bomb->exploded)==0){
    //si la bombe vient exploser
    bomb_explosion_map_set(map,bomb);
    bomb->exploded=1;
  }else if (state == 0 && (bomb->exploded)==2) {
    //si une bombe vient de finir d'exploser, on rafraichie toutes les explosions
    bomb_explosion_map_set_2(map,bomb);
    (bomb->exploded)=1;
  }else if (state < 0) {
    bomb_explosion_end(map,bomb);
    player_inc_nb_bomb(bomb->player);
  }
}

void bomb_change_time(struct bomb* bomb,int time) {
  assert(bomb);
  if (bomb->state>0) {
    bomb->TIME = SDL_GetTicks() - (time - bomb->TIME);
  }else{
    bomb->TIME = SDL_GetTicks() - (time - bomb->TIME);
  }
}
