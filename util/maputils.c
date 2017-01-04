unsigned get_height(char* filename){
	int fd = open(filename, O_RDONLY);
	unsigned height = 0;
	int r = read(fd, &height, sizeof(usigned));
	return height;
}

unsigned get_width(char* filename){
	int fd = open(filename, O_RDONLY);
	unsigned width = 0;
	lseek(fd, sizeof(usigned), SEEK_SET);
	int r = read(fd, &width, sizeof(usigned));
	return width;
}

unsigned get_objects(char* filename){
	int fd = open(filename, O_RDONLY);
	unsigned objects = 0;
	lseek(fd, 2*sizeof(usigned), SEEK_SET);
	int r = read(fd, &objects, sizeof(usigned));
	return objects;
}

void get_info(char* filename){
	printf("Height : %u, Width : %u, Obects : %u.\n", get_height(filename), get_width(filename), get_object(filename));
}


/*fichier passé en argument*/
int main(int argc, char const *argv[])
{
	return 0;
}

/*
set_width()
set_height()

set_objects(file, nb_frames, solidity, destructible, collectible, generator)
prune_objects()

*/