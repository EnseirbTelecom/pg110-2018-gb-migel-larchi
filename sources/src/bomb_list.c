#include <SDL/SDL_image.h>
#include <assert.h>

#include <misc.h>
#include <window.h>
#include <player.h>
#include <sprite.h>
#include <bomb.h>


struct bomb_list{
    struct bomb* bomb;
    struct bomb_list* next;
};

struct bomb_list* bomb_list_init(){
  struct bomb_list* bombs=malloc(sizeof(*bombs));
  if (!bombs)
    error("Memory error");
  bombs->bomb=NULL;
  bombs->next=NULL;
  return bombs;
};

void bomb_list_free_first_ele(struct bomb_list** bomb_list){
  struct bomb_list* first_ele=*bomb_list;
  first_ele=first_ele->next;
  bomb_free((*bomb_list)->bomb);
  free(*bomb_list);
  *bomb_list=first_ele;
};


void bomb_list_free(struct bomb_list** bomb_list){
  assert(*bomb_list);
  while (*bomb_list!=NULL) {
    bomb_list_free_first_ele(bomb_list);
  }
};

void bomb_list_set_exploded_to_2(struct bomb_list *bomb_list){
//set bomb->exploded from 1 to 2
  assert(bomb_list);
  while(bomb_list->bomb){
    if (bomb_get_exploded(bomb_list->bomb)==1) {
      bomb_set_exploded(bomb_list->bomb,2);
    }
    bomb_list=bomb_list->next;
  }
};


void bomb_list_update(struct map *map, struct bomb_list** pbomb_list){
  assert(map);
  assert(pbomb_list);
  assert(*pbomb_list);
  struct bomb_list* bomb_list = *pbomb_list;
  struct bomb *bomb=bomb_list->bomb;
  int bol=1;
  while (bomb) {
    if(bomb && bol==1 ){
      if ( bomb_get_state(bomb)<0 ){
        struct player *player=bomb_get_player(bomb);
        bomb_explosion_end(map,bomb);
        bomb_list_set_exploded_to_2( bomb_list->next);
        bomb_list_free_first_ele(pbomb_list);
        if(player_get_max_bomb(player)>player_get_nb_bomb(player))
        player_inc_nb_bomb(player);
        bol = 0;
      }
    }
    bomb_update(map,bomb);
    bomb_list = bomb_list->next;
    bomb=bomb_list->bomb;
  }
}

void bomb_list_display(struct map* map,struct bomb_list* bomb_list){
  while (bomb_list->bomb) {
      bomb_display(map,bomb_list->bomb);
      bomb_list=(bomb_list->next);
  }
}

void bomb_list_add(struct map* map,struct player* player,struct bomb_list* bomb_list){
  struct bomb* bomb=bomb_init(map,player);
  struct bomb_list* bomb_list_aux=bomb_list;
  while (bomb_list_aux->bomb) {
    bomb_list_aux=bomb_list_aux->next;
  }
  bomb_list_aux->bomb=bomb;
  bomb_list_aux->next=bomb_list_init();

};
