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
  struct monster_list* monster_list=malloc(sizeof(*monster_list));
  if (!monster_list)
    error("Memory error");
  monster_list->monster=NULL;
  monster_list->next=NULL;
  return monster_list;
};

void monster_list_free_first_ele(struct monster_list** monster_list){
  struct monster_list* first_ele=*monster_list;
  first_ele=first_ele->next;
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

void monster_list_update(struct map *map, struct monster_list *monster_list) {
  assert(monster_list);

  while (monster_list->monster!=NULL) {
    monster_set(monster_list->monster,map,1000);
    monster_list=monster_list->next;

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

struct monster_list *monster_list_find_monster(struct monster_list *monster_list,int x,int y){
  assert(monster_list);

  struct monster *monster=monster_list->monster;
  while (monster!=NULL) {
    if( monster_get_x(monster)==x && monster_get_y(monster)==y ){
      return monster_list;
    }else{
      monster_list=monster_list->next;
      monster=monster_list->monster;
    }
  }
  return NULL;
}
