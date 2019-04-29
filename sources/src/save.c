#include <player.h>
#include <stdlib.h>
#include <stdio.h>
#include <game.h>
#include <assert.h>


void save_player(FILE* file, struct player* player) {
  assert(file);
  assert(player);

  fprintf(file, "%d ",player_get_x(player));
  fprintf(file, "%d ",player_get_y(player));
  fprintf(file, "%d ",player_get_key(player));
  fprintf(file, "%d ",player_get_life(player));
  fprintf(file, "%d ",player_get_nb_bomb(player));
  fprintf(file, "%d ",player_get_range(player));
  fputc('\n',file);
}

void save_map(FILE* file,struct map* map) {
  assert(file);
  assert(map);

  int width = map_get_width(map);
  int height = map_get_height(map);
  fprintf(file, "%d ",width);
  fprintf(file, "%d ",height);
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
  levels level player_x player_y player_key player_life player_nb_bomb player_set_range
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
  save_player(file,game_get_player(game));
  save_maps(file,game_get_maps(game),game_get_levels(game));

  fclose(file);

}
