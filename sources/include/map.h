/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_

enum cell_type {
	CELL_EMPTY=0x00,   		//  0000 0000				0
	CELL_SCENERY=0x10, 		//  0001 0000				16
	CELL_BOX=0x20,   		//  0010 0000					32
	CELL_DOOR=0x30,      	//  0011 0000				48
	CELL_KEY=0x40,       	//  0100 0000				64
	CELL_BONUS=0x50, 		// 	0101 0000					80
	CELL_MONSTER=0x60, 		// 	0110 0000				96
	CELL_BOMB=0x70, 	   		// 	0111 0000			112
	CELL_EXPLOSION=0x80, 	   		// 	1000 0000	128

};

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1,
	BONUS_BOMB_RANGE_INC,
	BONUS_BOMB_NB_DEC,
	BONUS_BOMB_NB_INC,
	BONUS_MONSTER,
	BONUS_LIFE
};

enum door_type{
	DOOR_LVL0=0,
	DOOR_LVL1,
	DOOR_LVL2,
	DOOR_LVL3,
	DOOR_LVL4,
	DOOR_LVL5,
	DOOR_LVL6,
	DOOR_LVL7,
};

enum scenery_type {
	SCENERY_STONE = 1,    // 0001
	SCENERY_TREE  = 2,    // 0010
	SCENERY_PRINCESS = 4  // 0100
};

enum compose_type {
	CELL_TREE     = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE    = CELL_SCENERY | SCENERY_STONE,
	CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS,

    CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
    CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_INC,
	CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_DEC,
    CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_INC,
    CELL_BOX_LIFE     = CELL_BOX | BONUS_MONSTER,
    CELL_BOX_MONSTER  = CELL_BOX | BONUS_LIFE,
};

struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);

// Return the type of a bonus
enum bonus_type map_get_bonus_type(struct map* map, int x, int y);

// Return the type of a bonus
enum door_type map_get_door_type(struct map* map, int x, int y);

//set the bonus type
void map_set_bonus_type(struct map* map, int x, int y, enum  bonus_type bonus) ;

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default static map
struct map* map_get_static();

// Display the map on the screen
void map_display(struct map* map);

// return the monster_list of the map
struct monster_list** map_get_monster_list(struct map* map);

// init struct map* from a file
struct map* map_init(char* path_file);

// init  a array of map  from a path of a directory and word_set_name
struct map** maps_init(char* path_dir,char* word_set_name,int* levels);

void maps_update(struct map** maps,int nb_lvl);

void map_open_the_door(struct map* map,int x,int y) ;

#endif /* MAP_H_ */
