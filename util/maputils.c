#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


unsigned get_height(char* filename){
	int fd = open(filename, O_RDONLY);
	unsigned height = 0;
	int r = read(fd, &height, sizeof(unsigned));
	return height;
}

unsigned get_width(char* filename){
	int fd = open(filename, O_RDONLY);
	unsigned width = 0;
	lseek(fd, sizeof(unsigned), SEEK_SET);
	int r = read(fd, &width, sizeof(unsigned));
	return width;
}

unsigned get_objects(char* filename){
	int fd = open(filename, O_RDONLY);
	unsigned objects = 0;
	lseek(fd, 2*sizeof(unsigned), SEEK_SET);
	int r = read(fd, &objects, sizeof(unsigned));
	return objects;
}

void get_info(char* filename){
	printf("Height : %u, Width : %u, Objects : %u.\n", get_height(filename), get_width(filename), get_objects(filename));
}


/*fichier passé en argument*/
int main(int argc, char const *argv[])
{
	if(argc<1){
		perror("not enough parameters");
		exit(1);
	}	
	char * filename = argv[1];
	get_info(filename);

	return 0;
}

/*
set_width()
set_height()

set_objects(file, nb_frames, solidity, destructible, collectible, generator)
prune_objects()

*/