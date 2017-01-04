#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>



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

void set_height(char* filename, unsigned height){
	int fd = open(filename, O_WRONLY, 777);
	write(fd, &height, sizeof(unsigned));
}

void set_width(char* filename, unsigned width){
	int fd = open(filename, O_WRONLY, 777);
	lseek(fd, sizeof(unsigned), SEEK_SET);
	write(fd, &width, sizeof(unsigned));
}


/*fichier passé en argument*/
int main(int argc, char const *argv[])
{
	if(argc<1){
		perror("not enough parameters\n");
		exit(1);
	}	
	char * filename = argv[1];

	/*12 32 1*/
    for (int i = 1; i < argc; i++){  /* Skip argv[0] (program name). */
    	if (strcmp(argv[i], "--getwidth") == 0){  
            get_width(filename);
        }
        else if (strcmp(argv[i], "--getheight") == 0){  
            get_height(filename);
        }
        else if (strcmp(argv[i], "--getobjects") == 0){  
            get_objects(filename);
        }
        else if (strcmp(argv[i], "--getinfo") == 0){  
            get_info(filename);
        }
        else if (strcmp(argv[i], "--setwidth") == 0){  /* Process optional arguments. */
            if (i + 1 <= argc - 1){  /* There are enough arguments in argv. */
    			++i;
                unsigned w = atoi(argv[i]);  /* Convert string to int. */
                set_width(filename, w);
            }
            else{
                perror("Please insert a width.\n");
            }
        }
        else if (strcmp(argv[i], "--setheight") == 0){  
            if (i + 1 <= argc - 1){ 
            	++i; 
                unsigned h = atoi(argv[i]);
                set_height(filename, h);
            }
            else{
                perror("Please insert a height.\n");
            }
        }
        /*else if (strcmp(argv[i], "--setobjects") == 0){  
            if (i + 6 <= argc - 1 ET AUTRE TRUCS){
            	++i;  
                char* file = argv[i];
                ++i;
                int nb_frames = argv[i];
                ++i;
                int solidity = argv[i];
                ++i;
                int destructible = argv[i];
                ++i;
                int collectible = argv[i];
                ++i;
                int generator = argv[i];
                set_objects(file, nb_frames, solidity, destructible, collectible, generator);
            }
            else{
                perror("Please insert the correct arguments.\n")
            }
        }*/
    }
    //get_info(filename);
	return 0;
}

/*
set_objects(file, nb_frames, solidity, destructible, collectible, generator)
prune_objects()

*/