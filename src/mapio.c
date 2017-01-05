#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "error.h"

#ifdef PADAWAN

void map_new (unsigned width, unsigned height)
{
  map_allocate (width, height);

  for (int x = 0; x < width; x++){
    map_set (x, height - 1, 0); // Ground
    if ( x%4==0 && x!=0)
      map_set(x,height-2,4); //flower
    if ( x%6==0 && x!=0)
      map_set(x,height-3,5); //pieces
  }
for (int y = 0; y < height - 1; y++) {
    map_set (0, y, 1); // Wall
    map_set (width - 1, y, 1); // Wall
  }
  
 

  map_object_begin (6);

  // Texture pour le sol
  map_object_add ("images/ground.png", 1, MAP_OBJECT_SOLID);
  // Mur
  map_object_add ("images/wall.png", 1, MAP_OBJECT_SOLID);
  // Gazon
  map_object_add ("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
  // Marbre
  map_object_add ("images/marble.png", 1, MAP_OBJECT_SOLID | MAP_OBJECT_DESTRUCTIBLE);


  //fleur
  map_object_add ("images/flower.png", 1, MAP_OBJECT_AIR);

  //pieces
  map_object_add("images/coin.png", 20, MAP_OBJECT_AIR | MAP_OBJECT_COLLECTIBLE);
  map_object_end ();

}

void map_save (char *filename)
{
  // TODO
  /*fprintf (stderr, "Sorry: Map save is not yet implemented\n");*/
  int fd = open(filename,O_CREAT | O_TRUNC | O_WRONLY, 777);
  unsigned height = map_height();
  unsigned width = map_width();
  unsigned nb_objects = map_objects();
  write(fd, &height, sizeof(height));
  write(fd, &width, sizeof(width));
  write(fd, &nb_objects, sizeof(nb_objects));
  for (int y = 0; y < height; y++){
    for(int x = 0; x < width; x++){
      int data = map_get(x,y);
      write(fd, &data, sizeof(data));
    }
  }
  // ############################################# À VÉRIFIER ##############################################
  int objects = open("../util/objets.txt", O_RDONLY);
  lseek(objects, sizeof(char), SEEK_SET);
  char c;
  int cpt = 0;
  while(1/*not end of file*/){
    c = 'a';
    read(objects, &c, sizeof(char));
    while(c!='\"'){
      cpt++;
      read(objects, &c, sizeof(char));
    }
    cpt--;//vérifier si on est pas décalé !
  }
  lseek(objects, cpt*-1, SEEK_SET); //vérifier si on est pas décalé !
  write(fd, &cpt, sizeof(int)); 

  for(int i=0 ; i<cpt ; ++i){
    read(objects, &c, 1);
    write(fd, &c, 1);
  }
  lseek(objects, 2, SEEK_SET); //faut compter la tabulation aussi
  read(objects, &c, 1);
  write(fd, &c, 1);
  lseek(objects, 1, SEEK_SET);
  read(objects, &c, 1);
  char tmp = '0';
  if(c=='a'){
    tmp = '0';
    write(fd, &tmp, 1);
  }
  else{
    read(objects, &c, 1);
    if(c=='e'){
      tmp = '1';
      write(fd, &tmp, 1);
    }
    else{
      tmp='2';
      write(fd, &tmp, 1);
    }
  }
  //continuer de lire jusqu'à la tabulation
  //si le charactere lu est 'd'
    //write 1
  // sinon 
    //write 0
  //continuer de lire juqu'à la tabulation
  //si le char lu est 'c'
    //write 1
  //sinon
    //write 0
  //continuer jusqu'à la tabulation
  //si le character lu est 'g'
    //write 1
  //sinon
    //write 0

  //FAIRE hexdump -c objects.txt
  //PRENDRE EN COMTPE QUE LE NOMBRE DE FRAME PEUT ËTRE PLUS LONG LEL !!!!!!!!!!!!!!
  close(fd);
}
/*
AIR = 0
SEMISOLID = 1
SOLID = 2
*/

void map_load (char *filename)
{
  
int load = open(filename, O_RDONLY);
  unsigned height, width,nb_objects;
  int type;
  read(load, &height, sizeof(height));
  read(load, &width, sizeof(width));
  read(load, &nb_objects,sizeof(nb_objects));
  map_allocate (width, height);
  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      read(load, &type, sizeof(type));
      map_set(x, y, type);
    }
  }
  close(load);

  map_object_begin (6);

  // Texture pour le sol
  map_object_add ("images/ground.png", 1, MAP_OBJECT_SOLID);
  // Mur
  map_object_add ("images/wall.png", 1, MAP_OBJECT_SOLID);
  // Gazon
  map_object_add ("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
  // Marbre
  map_object_add ("images/marble.png", 1, MAP_OBJECT_SOLID | MAP_OBJECT_DESTRUCTIBLE);
  // Fleur
  map_object_add ("images/flower.png", 1, MAP_OBJECT_AIR);
  // Piece
  map_object_add ("images/coin.png", 20, MAP_OBJECT_AIR | MAP_OBJECT_COLLECTIBLE);

  //exit_with_error ("Map load is not yet implemented\n");
}

#endif
