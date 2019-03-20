#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int map_init_txt_get_width(){
  FILE* fichier = NULL;
  fichier = fopen("map/map_0.txt","r");
  assert(fichier);

  int width,height;
  fscanf(fichier,"%d :%d",&width,&height);
  fclose(fichier);
  return width;
}

int map_init_txt_get_height(){
  FILE* fichier = NULL;
  fichier = fopen("map/map_0.txt","r");
  assert(fichier);

  int width,height;
  fscanf(fichier,"%d :%d",&width,&height);
  fclose(fichier);
  return height;
}

unsigned char* map_init_txt(int width,int height){

  FILE* fichier =NULL;
  fichier = fopen("map/map_0.txt","r");
  assert(fichier);

  int a,b;
  fscanf(fichier,"%d :%d",&a,&b);

  unsigned char *themap = malloc(sizeof(*themap)* (width*height) );
  for (int i = 0; i < (width)*(height); i++) {
    fscanf(fichier,"%d ",&a);
    themap[i]=a;
  }

  for (int i = 0; i < (width)*(height); i++) {
    printf("%d:%x ",i,themap[i] );
  }
  printf("\n" );
  fclose(fichier);
  return themap;
}
