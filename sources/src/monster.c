#include <SDL/SDL_image.h>
#include <assert.h>

#include <monster.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <myfonct.h>
#include <time.h>

struct monster{
  int x,y;
  unsigned int lastTime;
  enum direction direction;
};

struct monster* monster_init() {
	struct monster* monster = malloc(sizeof(*monster));
	if (!monster)
		error("Memory error");
  monster->direction = SOUTH;
  monster->lastTime=SDL_GetTicks();
	return monster;
}

void monster_set_position(struct monster *monster, int x, int y) {
	assert(monster);
	monster->x = x;
	monster->y = y;
}

void monster_free(struct monster* monster) {
	assert(monster);
	free(monster);
}

int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

int monster_get_lastTime(struct monster* monster){
    assert(monster);
    return monster->lastTime;
}

void monster_death(struct monster* monster) {
  assert(monster);
  monster->lastTime=0;
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}

static int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {
	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;		// Les mouvements du joueur sont limités les éléments de décors
	break;

	case CELL_BOMB:
		return 0;
	break;

	case CELL_BOX:
		return 0;
	break;

	case CELL_BONUS:
    return 0;
	break;

	case CELL_MONSTER:
    return 0;
	break;

	case CELL_DOOR:
    return 0;
	break;

  case CELL_EXPLOSION:
    return 0;
  break;

	default:
	break;
	}

	// Monster has moved
	return 1;
}

int monster_move(struct monster* monster, struct map* map) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;

	switch (monster->direction) {
	case NORTH:
		if (monster_move_aux(monster, map, x, y - 1)) {
			monster->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1)) {
			monster->y++;
			move = 1;
		}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y)) {
			monster->x--;
			move = 1;
		}
		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y)) {
			monster->x++;
			move = 1;
		}
		break;
	}
	if (move && map_get_cell_type(map,x,y)==CELL_MONSTER)
		map_set_cell_type(map, x, y, CELL_EMPTY);

  x = monster_get_x(monster);
  y = monster_get_y(monster);
  if(map_get_cell_type(map,x,y)!=CELL_EXPLOSION)
    map_set_cell_type(map,x,y,CELL_MONSTER);
	return move;
}

void monster_display(struct monster* monster) {
	assert(monster);
	window_display_image(sprite_get_monster(monster->direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}

void monster_update(struct monster* monster, struct map* map, unsigned int speed) {
  unsigned int lastTime=monster->lastTime;
  unsigned int  currentTime;
  currentTime = SDL_GetTicks();
  if (currentTime > (lastTime + speed)) {
    monster->lastTime = currentTime;
    monster_new_pos(monster,map);
  }
  int x=monster_get_x(monster);
  int y=monster_get_y(monster);
  if (map_get_cell_type(map,x,y)==CELL_EXPLOSION) {
    monster_death(monster);
  }
}

void monster_new_pos(struct monster* monster, struct map* map) {
  	int random_number = rand();
    int random_move = random_number%4 ;
    switch (random_move) {
      case 0:
        monster_set_current_way(monster, NORTH);
        monster_move(monster, map);
        break;
      case 1:
        monster_set_current_way(monster, SOUTH);
        monster_move(monster, map);
        break;
      case 2:
        monster_set_current_way(monster, EAST);
        monster_move(monster, map);
        break;
      case 3:
        monster_set_current_way(monster, WEST);
        monster_move(monster, map);
        break;
      }
}
