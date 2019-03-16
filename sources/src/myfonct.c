#include <stdio.h>
#include <assert.h>
#include <map.h>
#include <player.h>
#include <constant.h>

int boxe_move(enum direction direction,struct map* map,int x,int y){
  int x1,y1;
  x1=x;
  y1=y;

  switch (direction) {
    case SOUTH:
      y1=y+1;
    break;

    case NORTH:
      y1=y-1;
    break;

    case WEST:
      x1=x-1;
    break;

    case EAST:
      x1=x+1;
    break;
  }
  if (!map_is_inside(map, x1, y1))
		return 0;

  if(  map_get_cell_type(map,x1,y1)==CELL_EMPTY){
    map_set_cell_type(map, x1, y1, map_get_cell_type(map,x,y));
    map_set_cell_type(map, x, y, CELL_EMPTY);
    return 1;
  }
  return 0;
}

void cell_bonus_move(struct map* map,struct player *player,int x,int y){
  enum bonus_type bonus_type= map_get_bonus_type(map,x,y);
  printf("%d\n",bonus_type );
  switch (bonus_type) {
    case BONUS_BOMB_RANGE_INC:
      printf("ok\n");
      player_inc_range(player);
    break;

    case BONUS_BOMB_RANGE_DEC:
    if(player_get_range(player)>1)
      player_dec_range(player);
    printf("ko\n");
    break;

    case BONUS_BOMB_NB_INC:
    player_inc_nb_bomb(player);
    printf("ko\n");
    break;

    case BONUS_BOMB_NB_DEC:
    player_dec_nb_bomb(player);
    printf("ko\n");
    break;

    case BONUS_LIFE:
    printf("ko\n");
    break;

    case BONUS_MONSTER:
    printf("ko\n");
    break;
  }
}
