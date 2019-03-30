#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <assert.h>
#include <map.h>
#include <string.h>

int map_file_get_width(char* dir){
  FILE* fichier = NULL;
  fichier = fopen(dir,"r");
  assert(fichier);

  int width,height;
  fscanf(fichier,"%d :%d",&width,&height);
  fclose(fichier);
  return width;
}

int map_file_get_height(char* dir){
  FILE* fichier = NULL;
  fichier = fopen(dir,"r");
  assert(fichier);

  int width,height;
  fscanf(fichier,"%d :%d",&width,&height);
  fclose(fichier);
  return height;
}

unsigned char* map_file_read(char* dir,int width,int height){
  FILE* fichier =NULL;
  fichier = fopen(dir,"r");
  assert(fichier);

  int a,b;
  fscanf(fichier,"%d :%d",&a,&b);

  unsigned char *themap = malloc(sizeof(*themap)* (width*height) );
  for (int i = 0; i < (width)*(height); i++) {
    fscanf(fichier,"%d ",&a);
    themap[i]=a;
  }

  fclose(fichier);
  return themap;
}

int check_str(char* char1,char* char2){
  // return 1 if char2 begin with the same char than char1
  while ((*char1)!='\0') {
    if ((*char1) != (*char2))
      return 0;
    char1++;
    char2++;
    }
    return 1;
}

char* get_file_path(char* char1,char* char2){
  //return char* with the relative path of a file by concatenating char1 and char2
  char* char3 =  malloc( sizeof(char)* ( strlen(char1)+strlen(char2) + 2) ); // +2 = '/' + '\0'
  char* char_aux=char3;

  while ((*char1)!='\0') {
    (*char_aux) = (*char1);
    (char1)++;
    (char_aux)++;
  }

  (*char_aux) = '/';
  (char_aux)++;

  while ((*char2)!='\0') {
    (*char_aux) = (*char2);
    (char2)++;
    (char_aux)++;
  }
  (*char_aux) = '\0';

  return char3;
}

struct map** map_file_read_dir(char* path_dir, char* word_set_name ) {
//  char* path_dir="./map";
  char* path_file;
  DIR *d;
  struct dirent *dir;
  d = opendir("./map");

  struct map **maps = NULL;
  if (d){
      int i=0;
      while ((dir = readdir(d)) != NULL)
      {
          if(dir->d_type == DT_REG){
              if (check_str(word_set_name,dir->d_name)) {

                //allocation de la memoire
                if (maps==NULL) {
                  maps = malloc( sizeof(*maps) );
                }else{
                  maps = realloc(maps,i* sizeof(*maps));
                }

                path_file = get_file_path(path_dir,dir->d_name);
                maps[i] = map_load_map(path_file);
                i++;
                free(path_file);
              }
          }
      }
      closedir(d);
  }
  return maps;
}
