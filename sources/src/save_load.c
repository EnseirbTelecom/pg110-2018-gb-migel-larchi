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
#include <stdlib.h>

void go_to_next_line(FILE* file){
  int foo = fgetc(file);
  if (foo==EOF) {
    printf("erreur go_to_next_line: end of file\n");
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
    if ((grid[i]&CELL_BOMB)==CELL_BOMB || (grid[i]&CELL_EXPLOSION)==CELL_EXPLOSION)  {
      if (grid[i]&0x0f) {
        grid[i]=(grid[i]&0x0f)|CELL_BONUS;
      }else{
        grid[i] = CELL_EMPTY;
      }
    }
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
  FILE *file = fopen("save/saved.txt", "w");
  assert(file);

  // first line of save file
  //  nb_level current_lvl
  fprintf(file, "%d ",game_get_levels(game));
  fprintf(file, "%d ",game_get_current_lvl(game));
  fputc('\n',file);

  //seconde line: player information
  // x y key life nb_bomb range max_bomb direction
  save_player(file,game_get_player(game));

  //next line: maps information
  // each line represent one map
  save_maps(file,game_get_maps(game),game_get_levels(game));

  fclose(file);

}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


struct player* load_player(char* path_save) {
  //load player information
  struct player* player = player_init(0);
  FILE* fichier = fopen(path_save,"r");
  assert(fichier);

  go_to_next_line(fichier);
  int x,y,key,life,range,max_bomb,direction;
  fscanf(fichier,"%d %d %d %d %d %d %d",&x,&y,&key,&life,&range,&max_bomb,&direction);
  player_set_position(player,x,y);
  player_set_current_way(player,direction);
  player_set_key(player,key);
  player_set_max_bomb(player,max_bomb);
  player_set_nb_bomb(player,max_bomb);
  player_set_range(player,range);
  player_set_life(player,life);
  player_set_state(player,-1);

  fclose(fichier);
  return player;
}

struct map* load_map(FILE* fichier){
  //charge le contenu une ligne map
  //fichier doit pointer sur le debut de la ligne
  assert(fichier);
  int width,height,cell;
  fscanf(fichier," %d : %d ",&width,&height);
  struct map* map = map_new(width,height);
  for (int i = 0; i < (width*height); i++) {
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

  struct map** maps = malloc(levels*sizeof(*maps));
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
  struct map** maps = load_maps(path_save,levels);
  struct player* player = load_player(path_save);
  struct game* game = game_load(maps,levels,level,player);
  return  game;
}

struct game* load_from_file(char* path_save) {
  FILE* fichier = fopen(path_save,"r");

  int levels = fgetc(fichier);

  int level,x,y;
  fscanf(fichier,"%d:%d,%d",&level,&x,&y);

  char prefix[255];
  fgets(prefix,255,fichier);
  struct map** maps = maps_init("./map","easy",&levels);
  struct player* player = player_init(3);
  player_set_position(player,x,y);

  struct game* game = game_load(maps,levels,level,player);
  struct map* map = game_get_current_map(game);
  return game;
}

struct game* load_loadfile(char* path_save) {
  FILE* fichier = fopen(path_save,"r");

  int levels;
  fscanf(fichier,"%d\n",&levels);
  int level,x,y;
  fscanf(fichier,"%d:%d,%d",&level,&x,&y);

  go_to_next_line(fichier);//sauter une ligne

  char prefix[255];
  fgets(prefix,255,fichier);
  struct map** maps = maps_init_2(prefix,levels);
  struct player* player = player_init(3);
  player_set_position(player,x,y);

  struct game* game = game_load(maps,levels,level,player);
  struct map* map = game_get_current_map(game);
  return game;
}
