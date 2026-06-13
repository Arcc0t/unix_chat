#include <parser.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

char *load_file(const char *filename)
{
	ssize_t bytes_read;
	int fd;
	off_t size;
	char *result;

	fd = open(filename, O_RDONLY);
	if (fd < 0){
		perror(filename);
		exit(EXIT_FAILURE);
	}

	size = lseek(fd, 0, SEEK_END);
	if (size == (off_t) -1){
		perror(filename);
		exit(EXIT_FAILURE);
	}
	if (lseek(fd, 0, SEEK_SET) < 0){
		perror(filename);
		exit(EXIT_FAILURE);	
	}
	result = calloc(size + 1, 1);
	if (!result){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	bytes_read = read(fd, result, size);
	if (bytes_read != size){
		perror(filename);
		exit(EXIT_FAILURE);
	}
	return result;


}

int main(int argc, char **argv)
{
	char *data;
	struct message *msg = malloc(sizeof(struct message));
	data = load_file(argv[1]);
	if (!parse(data, strlen(data), msg))
		abort();	
	printf("id = 0x%x\n", msg->id);
	printf("reply_id = 0%o\n", msg->reply_id);
	printf("username = %s\n", msg->username);
}
