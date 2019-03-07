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


void bomb_list_update(struct bomb_list** bomb_list){
  if((*bomb_list)->bomb){
    if (bomb_get_state((*bomb_list)->bomb)<0) {
      bomb_list_free_first_ele(bomb_list);
    }
  }
};

void bomb_list_display(struct map* map,struct bomb_list** bomb_list){
  //utilisé dans game.c
  bomb_list_update(bomb_list);
  struct bomb_list* bombs=*bomb_list;
  while (bombs->bomb) {
      bomb_display(map,bombs->bomb);
      bombs=(bombs->next);
  }
};

void bomb_list_add(struct player* player,struct bomb_list* bomb_list){
  struct bomb* bomb=bomb_init(player);
  struct bomb_list* bomb_list_aux=bomb_list;
  while (bomb_list_aux->bomb) {
    bomb_list_aux=bomb_list_aux->next;
  }
  bomb_list_aux->bomb=bomb;
  bomb_list_aux->next=bomb_list_init();

};
