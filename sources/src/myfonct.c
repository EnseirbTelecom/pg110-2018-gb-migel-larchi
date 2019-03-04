#include <assert.h>
#include <map.h>
#include <player.h>
#include <constant.h>

int boxe_move(enum direction direction,struct map* map,int x,int y){
  int x2,y2;
  x2=x;
  y2=y;
  switch (direction) {
    case SOUTH:
      y2=y+1;
    break;

    case NORTH:
      y2=y-1;
    break;

    case WEST:
      x2=x-1;
    break;

    case EAST:
      x2=x+1;
    break;
  }
  if (!map_is_inside(map, x2, y2))
		return 0;

  if(map_get_cell_type(map,x2,y2)==CELL_EMPTY){
    map_set_cell_type(map, x, y, CELL_EMPTY);
    map_set_cell_type(map, x2, y2, CELL_BOX);
    return 1;
  }
  return 0;
};
