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
  close(fd);
}

void map_load (char *filename)
{
  
int load = open(filename, O_RDONLY);
  unsigned height, width,nb_objects;
  int r, type;
  r = read(load, &height, sizeof(height));
  r = read(load, &width, sizeof(width));
  r = read(load, &nb_objects,sizeof(nb_objects));
  map_allocate (width, height);
  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      r = read(load, &type, sizeof(type));
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
