#ifndef MONSTER_LIST_H_
#define MONSTER_LIST_H_
#include <map.h>

struct monster_list;
struct monster_list* monster_list_init();
void monster_list_free_first_ele(struct monster_list** monster_list);
void monster_init_map(struct map *map,struct monster_list* monster_list);
void monster_list_add(struct monster_list *monster_list,int x,int y);
void monster_list_update(struct map *map, struct monster_list *monster_list);
void monster_list_display(struct monster_list* monster_list);
void monster_list_free(struct monster_list** monster_list);
struct monster_list *monster_list_find_monster(struct monster_list *monster_list,int x,int y);
#endif
