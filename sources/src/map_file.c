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

int map_file_get_lvl(char* fname){
  assert(fname);
  char fname_cp[256];
  strcpy(fname_cp, fname);
  char* char_lvl;

  char_lvl = strtok(fname_cp,"_");
  char_lvl = strtok(NULL,".");
  int result=atoi(char_lvl);

  return result;
}
