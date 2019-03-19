#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void map_init_txt(void){
  FILE* fichier =NULL;
  fichier = fopen("map/map_0.txt","r");
  assert(fichier);

  int width=fgetc(fichier);
  fgetc(fichier);
  int height=fgetc(fichier);
  int car =fgetc(fichier);

  unsigned char themap[width * height];

  int nb_car = 0;
  while (nb_car<(width*height)) {
    nb_car_line++;
    car=fgetc;
    if(car!='' || car!='/n'){
      themap[nb_car]=car;
      printf("%d\n",car);
    }
  }
}
