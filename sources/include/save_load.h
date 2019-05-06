#ifndef SAVE_LOAD_H_
#define SAVE_LOAD_H_

#include <game.h>

// create a save file
void save_create(struct game* game);

// read a save file
struct game* load_save(char* path_save);

// read chargement file
struct game* load_from_file(char* path_save);

#endif
