#include <SDL/SDL_image.h>
#include <assert.h>

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
  monster->x=3;
  monster->y=3;
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

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}

static int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {
	enum direction direction;
	direction=monster->direction;

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
      map_set_cell_type(map,x,y-1,CELL_MONSTER);
			move = 1;
		}
		break;

	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1)) {
			monster->y++;
      map_set_cell_type(map,x,y+1,CELL_MONSTER);
			move = 1;
		}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y)) {
			monster->x--;
      map_set_cell_type(map,x-1,y,CELL_MONSTER);
			move = 1;
		}
		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y)) {
			monster->x++;
      map_set_cell_type(map,x-1,y,CELL_MONSTER);
			move = 1;
		}
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return move;
}

void monster_display(struct monster* monster) {
	assert(monster);
	window_display_image(sprite_get_monster(monster->direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}

void monster_set(struct monster* monster, struct map* map, unsigned int speed) {
  unsigned int lastTime=monster->lastTime;
  unsigned int  currentTime;
  currentTime = SDL_GetTicks();
  printf("lastTime : %d\n",lastTime);
  printf("currentTime : %d\n",currentTime);
  if (currentTime > (lastTime + speed)) {
    printf("okok\n");
    move_monster(monster,map);
    monster->lastTime = currentTime;
  }
}

void move_monster(struct monster* monster, struct map* map) {
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
