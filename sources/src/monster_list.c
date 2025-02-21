#include <SDL/SDL_image.h>
#include <assert.h>

#include <misc.h>
#include <monster.h>
#include <monster_list.h>
#include <window.h>
#include <map.h>

struct monster_list{
    struct monster* monster;
    struct monster_list* next;
};

struct monster_list* monster_list_init(){
  struct monster_list* monster_list = malloc(sizeof(*monster_list));
  if (!monster_list)
    error("Memory error");
  monster_list->monster=NULL;
  monster_list->next=NULL;
  return monster_list;
};

void monster_list_free_first_ele(struct monster_list** monster_list){
  struct monster_list* first_ele=*monster_list;
  first_ele=first_ele->next;
  if((*monster_list)->monster)
    monster_free((*monster_list)->monster);
  free(*monster_list);
  *monster_list=first_ele;
};

void monster_init_map(struct map *map,struct monster_list* monster_list) {
  assert(map);
  int width=map_get_width(map);
  int height=map_get_height(map);

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if( map_get_cell_type(map,x,y)==CELL_MONSTER )
        monster_list_add(monster_list,x,y);
    }
  }
}

void monster_list_add(struct monster_list *monster_list,int x,int y) {
  assert(monster_list);

  while(monster_list->monster!=NULL){
    monster_list = monster_list->next;
  }

  monster_list->monster = monster_init();
  monster_set_position(monster_list->monster,x,y);

  monster_list->next=monster_list_init();
}

void monster_list_update(struct player *player, struct map *map) {
  //monster->last_time=0 alors le monstre est mort
  assert(map);
  struct monster_list** pmonster_list=map_get_monster_list(map);
  struct monster_list* monster_list=*pmonster_list;

  while (monster_list->monster!=NULL) {
    monster_update(monster_list->monster,player,map,monster_get_speed(monster_list->monster));
    if (monster_get_lastTime(monster_list->monster)==0) {
      int x = monster_get_x(monster_list->monster);
      int y = monster_get_y(monster_list->monster);
      monster_list=monster_list->next;
      monster_list_del_monster(pmonster_list,x,y);
    }else{
      monster_list=monster_list->next;
    }

  }
}

void monster_list_display(struct monster_list* monster_list) {
  assert(monster_list);
  while (monster_list->monster!=NULL) {
    monster_display(monster_list->monster);
    monster_list=monster_list->next;
  }
}

void monster_list_free(struct monster_list** monster_list){
  assert(*monster_list);
  while (*monster_list!=NULL) {
    monster_list_free_first_ele(monster_list);
  }
};


int monster_list_del_monster(struct monster_list** monster_list,int x,int y) {
  assert(monster_list);
  assert(*monster_list);

  struct monster_list *tmp_monster_list=*monster_list;
  struct monster_list *prev;
  struct monster *tmp_monster=tmp_monster_list->monster;

  if(monster_get_x(tmp_monster)==x && monster_get_y(tmp_monster)==y){
    (*monster_list)=(*monster_list)->next;
    free(tmp_monster);
    free(tmp_monster_list);
    return 1;
  }

  while(tmp_monster_list!=NULL){
    if(monster_get_x(tmp_monster)==x && monster_get_y(tmp_monster)==y)
      break;
    prev=tmp_monster_list;
    tmp_monster_list=(tmp_monster_list->next);
    tmp_monster=(tmp_monster_list->monster);
  }

  if(tmp_monster_list==NULL)
   return 0;

  prev->next=tmp_monster_list->next;
  free(tmp_monster);
  free(tmp_monster_list);
  return 1;

}

void monster_list_change_time(struct monster_list* monster_list,int time){
  assert(monster_list);
  while (monster_list!= NULL) {
    if(monster_list->monster)
      monster_change_time(monster_list->monster,time);
    monster_list = monster_list->next;
  }
}

void monster_list_set_speed(struct monster_list* monster_list,int speed){
  assert(monster_list);
  while (monster_list) {
    if (monster_list->monster) {
      monster_set_speed(monster_list->monster,speed);
    }
    monster_list=monster_list->next;
  }
}
