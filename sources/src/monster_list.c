#include <SDL/SDL_image.h>
#include <assert.h>

#include <misc.h>
#include <monster.h>
#include <window.h>

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
