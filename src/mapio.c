#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "error.h"

#ifdef PADAWAN

void assert(int cond, char * msg){
  if(!cond){
    perror(msg);
    exit(1);
  }
}

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

  map_object_add ("images/ground.png", 1, MAP_OBJECT_SOLID);
  map_object_add ("images/wall.png", 1, MAP_OBJECT_SOLID);
  map_object_add ("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
  map_object_add ("images/marble.png", 1, MAP_OBJECT_SOLID | MAP_OBJECT_DESTRUCTIBLE);
  map_object_add ("images/flower.png", 1, MAP_OBJECT_AIR);
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
  char * obj = "util/objets.txt";
  int objects = open(obj, O_RDONLY);
  int map_blocks = open("map_blocks.save", O_WRONLY | O_CREAT | O_TRUNC, 0666);
  lseek(objects, 1, SEEK_SET);
  int r = 1;
  char c;
  int cpt = 0;

  while(r >=1){ //tant qu'on est pas à la fin du fichier
    cpt = 0;
    r = read(objects, &c, sizeof(char));

    while(c!='\"'){ //on compte le nombre de caractère que fait le nom du fichier
      cpt++;
      r = read(objects, &c, sizeof(char));
    }

    write(map_blocks, &cpt, sizeof(int));//on écrit dans le fichier de sauvegarde le nombre de caractères
    
    lseek(objects, cpt*-1 - 1, SEEK_CUR); //on reviens à la position avant qu'on compte le nombre de charactere

    for(int i=0 ; i<cpt ; ++i){ //on ecrit le nom du fichier
      r = read(objects, &c, 1);
      write(map_blocks, &c, 1);
    }
    
    while(c<48 || c>57){ //tant que le caractère lu n'est pas entre 0 et 9
      read(objects, &c, 1);
    }
    
    //CA MARCHE JUSQU'ICI HEIN !!!!

    cpt = 0;
    while(c!=9){ //tant que le character lu n'est pas une tabulation
      read(objects, &c, 1);
      cpt++;
    }

    
    lseek(objects, cpt*-1 -1, SEEK_CUR); //on le remet où il était
    for(int i =0 ; i<cpt ; ++i){
      read(objects, &c, 1);
      printf("FOR c = %c\n", c);
      write(map_blocks, &c, 1);
    }    
    exit(1);

    //la on vient d'écrire le nombre de frames. Youhou.
    lseek(objects, 1, SEEK_CUR); //on saute la tabulation

    r = read(objects, &c, 1);
    char tmp = '0';
    if(c=='a'){
      tmp = '0';
      write(map_blocks, &tmp, 1);
    }
    else{
      read(objects, &c, 1);
      if(c=='e'){
        tmp = '1';
        write(map_blocks, &tmp, 1);
      }
      else{
        tmp='2';
        write(map_blocks, &tmp, 1);
      }
    }

    while(c!=9){
      read(objects, &c, 1);
    }


  
    r = read(objects, &c, 1);
    if(c=='d'){
      tmp='1';
      write(map_blocks, &tmp, 1);
    }
    else{
      tmp = '0';
      write(map_blocks, &tmp, 1);
    }

    while(c!=9){
      read(objects, &c, 1);
    } 

    r = read(objects, &c, 1);
    if(c=='c'){
      tmp='1';
      write(map_blocks, &tmp, 1);
    }
    else{
      tmp = '0';
      write(map_blocks, &tmp, 1);
    }

    while(c!=9){
      read(objects, &c, 1);
    }

    r = read(objects, &c, 1);
    if(c=='g'){
      tmp='1';
      write(map_blocks, &tmp, 1);
    }
    else{
      tmp = '0';
      write(map_blocks, &tmp, 1);
    }

    if(tmp=='1')
      lseek(objects, 9, SEEK_CUR);
    else
      lseek(objects, 13, SEEK_CUR);
  }
  close(fd);
  close(map_blocks);
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
