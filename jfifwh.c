#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int
process (char const *srcfn)
{
	int fd;
	u_char *data;
	size_t data_length, i;
	struct stat fs;
	
	fd = open(srcfn,  O_RDONLY);
	if (fd == -1)
	{
		perror("ERROR: Can't open the file for reading");
		return 2;
	}
	
	fstat(fd, &fs);
	
	data_length = fs.st_size;
	
	data = mmap(0, data_length, PROT_READ, MAP_SHARED, fd, 0);
	
	i = 0;
	
	if (*((int32_t *) &data[i]) != 0xe0ffd8ff)
	{
		fprintf(stderr, "ERROR: File has no proper file header\n");
		return 3;
	}
	
	i += 4;
	
	if (*((int32_t *) &data[i+2]) != 'FIFJ' || data[i+6] != 0)
	{
		fprintf(stderr, "ERROR: File has no proper JFIF header\n");
		return 3;
	}
	
	
	
	return 0;
}

int main (int argc, char const *argv[])
{
	if (argc != 2)
	{
		printf("%s\n", "Usage:\n  jfifwh image.jpg");
		return 1;
	}
	
	return process(argv[1]);
}
