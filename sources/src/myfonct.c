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
};
