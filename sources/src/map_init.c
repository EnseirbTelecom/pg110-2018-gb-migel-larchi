#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

unsigned char map_init_txt(int* width,int* height ){
  FILE* fichier =NULL;
  fichier = fopen("map/map_0.txt","r");
  assert(fichier);

  fscanf(fichier,"%d :%d",width,height);
  printf("%d %d\n",*width,*height);

  unsigned char themap[(*width)*(*height)];
  for (int i = 0; i < (*width)*(*height); i++) {
     fscanf(fichier,"%c",&themap[i]);
  }

  for (size_t i = 0; i < (*width)*(*height); i++) {
    printf("%d ",themap[i] );
  }
  printf("\n" );
  return themap;
}
