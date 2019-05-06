/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

#include <map.h>
#include <game.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <monster_list.h>
#include <bomb_list.h>
#include <map_file.h>


struct map {
	int width;
	int height;
	unsigned char* grid;
	struct monster_list* monster_list;
	struct bomb_list* bombs; //list of bombs
};

#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	map->monster_list= monster_list_init();
	map->bombs=bomb_list_init();
	return map;
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);
	int width=map->width;
	int height=map->height;
	if( (0<=x) && (x<width) && (0<=y) && (y<height) ){ // Les mouvements du joueur sont limités par le cadre de la carte
		return 1;}
	return 0;
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	monster_list_free(&(map->monster_list));
	bomb_list_free(&(map->bombs));

	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

unsigned char* map_get_grid(struct map* map){
	assert(map);
	return map->grid;
}


struct monster_list** map_get_monster_list(struct map* map){
	assert(map);
	assert(map->monster_list);
	return &(map->monster_list);
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}

enum bonus_type map_get_bonus_type(struct map* map, int x, int y){
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0x0f;
}

enum door_type map_get_door_type(struct map* map, int x, int y){
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0x0f;
}

void map_open_the_door(struct map* map,int x,int y) {
  assert(map);
	map->grid[CELL(x,y)] = (map->grid[CELL(x,y)] | 0x01) ;
}

void map_set_bonus_type(struct map* map, int x, int y, enum  bonus_type bonus) {
		assert(map && map_is_inside(map, x, y));
		unsigned char type = map->grid[CELL(x,y)];
		type = (type & 0xf0);
		map->grid[CELL(x,y)] = map->grid[CELL(x,y)] | bonus;
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;

	case BONUS_LIFE:
		window_display_image(sprite_get_bonus(BONUS_LIFE),x,y);
	break;
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	}
}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];

	    switch (type & 0xf0) {
		case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		  break;
	    case CELL_BOX:
	      window_display_image(sprite_get_box(), x, y);
	      break;
	    case CELL_BONUS:
				if (map_get_bonus_type(map,i,j)==BONUS_MONSTER){
					monster_list_add(map->monster_list,i,j);
					map_set_cell_type(map,i,j,CELL_MONSTER);
				}
	      display_bonus(map, x, y, type);
	      break;
	    case CELL_KEY:
	      window_display_image(sprite_get_key(), x, y);
	      break;
	    case CELL_DOOR:
				if( (map_get_door_type(map,i,j) % 2) == 1){ // door is open if 1 digit is 1
					window_display_image(sprite_get_door_opened(), x, y);
				}else{
					window_display_image(sprite_get_door_closed(), x, y);
				}
	      break;
			case CELL_BOMB:
				break;
			case CELL_EXPLOSION:
				window_display_image(sprite_get_bomb(0),x,y);
				break;
	    }
	  }
	}

	bomb_list_display(map,map->bombs);
	monster_list_display(map->monster_list);
}
/*
struct map* map_get_static(void)
{
	struct map* map = map_new(STATIC_MAP_WIDTH, STATIC_MAP_HEIGHT);

	unsigned char themap[STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT] = {
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , 0x52, 0x53 , 0x54 , CELL_MONSTER, 0x56, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER,
	  CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_MONSTER,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_MONSTER,
	  0x25,0x24, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_BOX, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_BOX_LIFE, CELL_EMPTY,
		CELL_EMPTY,  CELL_EMPTY, CELL_DOOR, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
		};

	for (int i = 0; i < STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT; i++)
		map->grid[i] = themap[i];
// a modif
	map->monster_list= monster_list_init();
	monster_init_map(map,map->monster_list);

	return map;
}*/

struct map* map_init(char* path_file){
	assert(path_file);

	int width = map_file_get_width(path_file);
	int height = map_file_get_height(path_file);
	unsigned char* themap = map_file_read(path_file,width,height);
	struct map* map = map_new(width, height);

	for (int i = 0; i < width * height; i++)
		map->grid[i] = themap[i];
	free(themap);

	monster_init_map(map,map->monster_list);
	return map;
}

struct map** maps_init(char* path_dir,char* word_set_name,int* levels){
	char* path_file;
	DIR *d;
	struct dirent *dir;
	d = opendir(path_dir);

	struct map** maps = NULL;
	if (d){	//si le ficher est bien ouvert
			int nb_maps = 0;
			int i = 0;
			int lvl=0;
			while ((dir = readdir(d)) != NULL)	//parcours element (fichier, rep ...) du rep
			{
					if(dir->d_type == DT_REG){ //si l'element est un fichier
							if (check_str(word_set_name,dir->d_name)) {
								lvl=map_file_get_lvl(dir->d_name);

								if((lvl+1)>nb_maps){
									nb_maps=(lvl+1);
								}

								//allocation de la memoire
								if (maps==NULL) {
									maps = malloc((nb_maps)* sizeof(*maps));
								}else{
									maps = realloc(maps,(nb_maps)* sizeof(*maps));
								}


								path_file = get_file_path(path_dir,dir->d_name);
								maps[lvl] = map_init(path_file);
								free(path_file);
								i++;
							}
					}
			}
			closedir(d);
			assert(i==nb_maps);
			(*levels)=i;
	}
	return maps;
}

struct bomb_list** map_get_bombs(struct map* map){
	assert(map);
	return &(map->bombs);
}

void maps_update(struct map** maps,int nb_lvl) {
	assert(maps);
	for (int i = 0; i < nb_lvl; i++) {
		monster_list_update(maps[i]);
		bomb_list_update(maps[i],map_get_bombs(maps[i]));
	}
}

void map_end_pause(struct map* map,int time) {
	assert(map);
	struct bomb_list* bomb_list = map-> bombs;
	struct monster_list* monster_list = map-> monster_list;
	monster_list_change_time(monster_list,time);
	bomb_list_change_time(bomb_list,time);
}

void maps_end_pause(struct map** map, int levels,int time) {
	assert(map);
	for (int i = 0; i < levels; i++) {
		map_end_pause(map[i],time);
	}
}

void map_set_grid(struct map* map,unsigned char cell,int i) {
	assert(map);
	map->grid[i] = cell;
}
