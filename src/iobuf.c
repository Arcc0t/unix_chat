#include "iobuf.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

enum {IOBUF_HALF_SIZE = 2048};

struct iobuf_ {
	int infd;
	int outfd;
	/* queue using two stacks */
	size_t front_size;
	size_t back_size;
	char front[IOBUF_HALF_SIZE];
	char back[IOBUF_HALF_SIZE];
};


IOBUF *iobuf_create(int infd, int outfd)
{
	IOBUF *result;

	result = (IOBUF *) calloc(sizeof(char), sizeof(IOBUF));
	if (!result)
		return NULL;
	result->infd = infd;
	result->outfd = outfd;
	return result;
}

void iobuf_destroy(IOBUF *buf)
{
	free(buf);
}

static
void transfer_buf_(IOBUF *buf)
{
	if (buf->front_size)
		return;	
	while (buf->back_size)
		buf->front[buf->front_size++] = buf->back[--buf->back_size];
}

ssize_t iobuf_fread(IOBUF *buf, int fd)
{
	size_t to_read;
	ssize_t bytes_read;

	transfer_buf_(buf);
	to_read = IOBUF_HALF_SIZE - buf->back_size;

	bytes_read = read(fd, buf->back + buf->back_size, to_read);
	if (bytes_read <= 0)
		return bytes_read;
	

	buf->back_size += bytes_read;

	return bytes_read;	

}

size_t iobuf_read(IOBUF *ibuf, void *obuf, size_t obuf_size)
{
	size_t bytes_read = 0;
	size_t to_read;
	char *obuf_ch = obuf;
	ssize_t fread_res;

	/* buffer is empty */
	if (ibuf->front_size == 0 && ibuf->back_size == 0){
		/* nowhere to get new data */
		if (ibuf->infd == -1)
			return bytes_read;

		/* otherwise */
		fread_res = iobuf_fread(ibuf, ibuf->infd);
		if (fread_res <= 0)
			return bytes_read;			
	}

	while (obuf_size) { 
		transfer_buf_(ibuf);
		to_read = MIN(obuf_size, ibuf->front_size);
		if (to_read == 0)
			break;
		while (to_read){
			*obuf_ch++ = ibuf->front[--ibuf->front_size];
			--obuf_size;
			--to_read;
		}
	}
	return bytes_read;
}

size_t iobuf_write(IOBUF *obuf, void *ibuf, size_t ibuf_size){
	size_t to_write = MIN(ibuf_size, IOBUF_HALF_SIZE - obuf->back_size);
	memcpy(ibuf, obuf->back, to_write);
	obuf->back_size+=to_write;
	return to_write;
}

int iobuf_getchar(IOBUF *buf){
	if(buf->front_size)
		return buf->front[--buf->front_size];
	else
		return IOBUF_EOF;
}

int iobuf_peek(IOBUF *buf){
	if(buf->front_size)
		return buf->front[buf->front_size - 1];
	else
		return IOBUF_EOF;
}

BOOL iobuf_putchar(IOBUF *buf, char c){
	if(buf->back_size == IOBUF_HALF_SIZE)
		return FALSE;
	else{
		buf->back[buf->backsize++] = c;
		return TRUE;
	}
}

size_t iobuf_getline(IOBUF *ibuf, char *obuf, size_t obuf_size){
	char c;
	size_t len = 0;
	while(obuf_size && ibuf->front_size){
		c = iobuf_getchar(ibuf);
		if(c){
			*(obuf++) = c;
			obuf_size--;
			len++;
			if(c == '\n')break;
		}
		else break;
	}
	return len;
}

iobuf_res iobuf_putline(IOBUF *buf, char *str){
	char  c;
	while(TRUE){
		c=*(str++);
		if(c)
			if(!iobuf_putchar(buf, c))return IOBUF_FULL;
			else if(c== '\n')return IOBUF_PARTIAL;
		else
			return IOBUF_PARTIAL;
	}
}

size_t iobuf_fwrite(IOBUF *buf, int fd){
	if(!buf->front_size)return 0;	
	char tmp[buf->front_size];
	for(char *i = tmp, *j = buf->front + buf->front_size - 1; j >= buf->front; i++, j--)
		*i=*j;
	size_t len = write(fd, tmp, buf->front_size);
	buf->front_size-=len;
	return len;
}

iobuf_res iobuf_flush(IOBUF *buf){
	while(buf->back_size && buf->front_size < IOBUF_HALF_SIZE)
		buf->front[buf->front_size++] = buf->back[--buf->back_size];
	if(buf->back_size)return IOBUF_FULL;
	return IOBUF_PARTIAL;
}