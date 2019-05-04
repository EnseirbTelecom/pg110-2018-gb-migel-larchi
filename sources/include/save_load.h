#ifndef SAVE_LOAD_H_
#define SAVE_LOAD_H_

#include <game.h>

void save_create(struct game* game);

struct game* load_save(char* path_save);

#endif
