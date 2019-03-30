#ifndef MAP_FILE_H

// get width of a map from file
int map_file_get_width(char* dir);

// get height of a map from file
int map_file_get_height(char* dir);

// read a map file
unsigned char* map_file_read(char* dir,int width,int height);

// read the contained of directory
struct map** map_file_read_dir();

#define MAP_FILE_H
#endif
