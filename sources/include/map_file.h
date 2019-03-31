#ifndef MAP_FILE_H

// get width of a map from file
int map_file_get_width(char* dir);

// get height of a map from file
int map_file_get_height(char* dir);

// read a map file
unsigned char* map_file_read(char* dir,int width,int height);


int check_str(char* char1,char* char2);

// get the file path when the have the dir path and file name
char* get_file_path(char* char1,char* char2);


#define MAP_FILE_H
#endif
