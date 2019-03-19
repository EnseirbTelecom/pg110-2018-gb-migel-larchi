#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  FILE* fichier =NULL;
  fichier = fopen("map_0.txt","r");

  if (fichier == NULL){
    printf("fichier == NULL\n");
    return 0;
  }

  int car=fgetc(fichier);

  printf("%c\n",car );
  fgetc(fichier);
  car=fgetc(fichier);
  printf("%c\n",car );

  car=fgetc(fichier);
  while (car != EOF) {
    printf("%c",car);
    car = fgetc(fichier);
  }

  fclose(fichier);
  return 0;
}
