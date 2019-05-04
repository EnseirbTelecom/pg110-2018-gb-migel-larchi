#include <assert.h>
#include <time.h>

#include <player.h>
#include <map.h>
#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb_list.h>
#include <monster_list.h>
#include <myfonct.h>

void go_to_next_line(FILE* file){
  int foo = fgetc(file);
  if (foo==EOF) {
    printf("error EOF\n");
    return;
  }

  if(foo=='\n'){
    return;
  }else{
    return go_to_next_line(file);
  }
}

void save_player(FILE* file, struct player* player) {
  assert(file);
  assert(player);

  fprintf(file, "%d ",player_get_x(player));
  fprintf(file, "%d ",player_get_y(player));
  fprintf(file, "%d ",player_get_key(player));
  fprintf(file, "%d ",player_get_life(player));
  fprintf(file, "%d ",player_get_nb_bomb(player));
  fprintf(file, "%d ",player_get_range(player));
  fprintf(file, "%d ",player_get_max_bomb(player));
  fprintf(file, "%d ",player_get_direction(player));

  fputc('\n',file);
}

void save_map(FILE* file,struct map* map) {
  assert(file);
  assert(map);

  int width = map_get_width(map);
  int height = map_get_height(map);
  fprintf(file, "%d ",width);
  fprintf(file, ": %d ",height);
  unsigned char* grid = map_get_grid(map);
  for (int i = 0; i < width*height; i++) {
    fprintf(file, "%d ",grid[i]);
  }
  fputc('\n',file);

}

void save_maps(FILE* file, struct map** maps,int levels) {
  for (int i = 0; i < levels; i++) {
    save_map(file,maps[i]);
  }
  fputc('\n',file);

}

void save_create(struct game* game) {
  /*
  levels level player_x player_y player_key player_life player_nb_bomb player_set_range,
  map_widht map_height map_grid ...
  map_widht map_height map_grid ...
  */
  char name[255];
  printf("Enter the save name: \n");
  scanf("%s", name);

  FILE *file = fopen("save/1.txt", "w");
  assert(file);
  fprintf(file, "%d ",game_get_levels(game));
  fprintf(file, "%d ",game_get_current_lvl(game));
  fputc('\n',file);
  save_player(file,game_get_player(game));
  save_maps(file,game_get_maps(game),game_get_levels(game));

  fclose(file);

}

struct player* load_player(char* path_save) {
  struct player* player = malloc(sizeof(player));
  FILE* fichier = fopen(path_save,"r");
  assert(fichier);

  go_to_next_line(fichier);
  int x,y,key,life,nb_bomb,range,max_bomb,direction;
  fscanf(fichier,"%d %d %d %d %d %d %d %d",&x,&y,&key,&life,&nb_bomb,&range,&max_bomb,&direction);
  player_set_position(player,x,y);
  player_set_current_way(player,direction);
  player_set_key(player,key);
  player_set_max_bomb(player,max_bomb);
  player_set_nb_bomb(player,nb_bomb);
  player_set_range(player,range);
  player_set_life(player,life);
  player_set_state(player,-1);

  fclose(fichier);
  return player;
}

struct map* load_map(FILE* fichier){
  assert(fichier);
  int width,height,cell;
  fscanf(fichier," %d : %d ",&width,&height);
  struct map* map = map_new(width,height);
  for (int i = 0; i < (width)*(height); i++) {
    fscanf(fichier,"%d ",&cell);
    map_set_grid(map,cell,i);
  }
  monster_init_map(map,*(map_get_monster_list(map)));
  return map;
}

struct map** load_maps(char* path_save,int levels){
  assert(path_save);
  FILE* fichier = fopen(path_save,"r");
  assert(fichier);

  go_to_next_line(fichier);
  go_to_next_line(fichier);

  struct map** maps = (malloc(levels*sizeof( struct map *) ) );
  printf("okoko levels: %d\n",levels);
  for (int i = 0; i < levels; i++) {
    maps[i] = load_map(fichier);
  }
  fclose(fichier);
  return maps;
}

void  load_lvl_s(char* path_save,int* levels,int* level ) {
  assert(path_save);
  FILE* fichier = fopen(path_save,"r");
  assert(fichier);

  fscanf(fichier,"%d %d ",levels,level);
  fclose(fichier);
}
struct game* load_save(char* path_save){
  int level,levels;
  load_lvl_s(path_save,&levels,&level);
  struct player* player = load_player(path_save);
  struct map** maps = load_maps(path_save,levels);
  struct game* game = game_load(maps,levels,level,player);

  return  game;
}
