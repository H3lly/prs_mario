unsigned get_height(char* filename){
	int fd = open(filename, O_RDONLY);
	unsigned height = 0;
	int r = read(fd, &height, sizeof(usigned));
	return height;
}

/*fichier passé en argument*/
int main(int argc, char const *argv[])
{
	
	return 0;
}

/*
get_width()
get_height()
get_objects() //nb objets contenus
get_info() //affiche toutes les informations (facile...)

set_width()
set_height()

set_objects(file, nb_frames, solidity, destructible, collectible, generator)
prune_objects()

*/