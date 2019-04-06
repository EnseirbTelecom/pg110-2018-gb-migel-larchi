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

void bomb_list_clean(struct bomb_list** pbomb_list) {
    assert(pbomb_list);
    assert(*pbomb_list);
    struct bomb_list* bomb_list = *pbomb_list;
    struct bomb_list* prev;
    struct bomb* bomb = bomb_list->bomb;

    if (!bomb && bomb_list->next) {
      (*pbomb_list) = (*pbomb_list)->next;
      free(bomb_list);
    }

    bomb_list = (*pbomb_list);
    prev = bomb_list;
    bomb_list = bomb_list->next;

    while (bomb_list != NULL) {
        bomb = bomb_list->bomb;
      if (bomb == NULL && bomb_list->next != NULL) {
        prev->next = bomb_list->next;
        free(bomb_list);
        bomb_list = prev->next;
      }else{
        prev = bomb_list;
        bomb_list = bomb_list->next;
      }

    }
}

void bomb_list_aux(struct map* map, struct bomb_list* bomb_list) {
  assert(map);
  assert(bomb_list);

  while (bomb_list->bomb) {
    bomb_update(map,bomb_list->bomb);
    bomb_list = bomb_list->next;
  }

}

void bomb_list_update(struct map *map, struct bomb_list** pbomb_list){
  assert(map);
  assert(pbomb_list);
  assert(*pbomb_list);

  struct bomb_list* bomb_list = *pbomb_list;
  struct bomb *bomb;
  int need_to_refresh = 0;

  while (bomb_list) {
    bomb=bomb_list->bomb;
    if (bomb) {
      bomb_update(map,bomb);
      if (bomb_get_state(bomb) < 0) {
        need_to_refresh = 1;
        bomb_free(bomb);
        bomb_list -> bomb = NULL;
      }
    }
    bomb_list = bomb_list->next;
  }

  bomb_list_clean(pbomb_list);
  if (need_to_refresh) {
    bomb_list_set_exploded_to_2(*pbomb_list);
    bomb_list_aux(map,*pbomb_list);
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
