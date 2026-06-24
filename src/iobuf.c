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

iobuf_res iobuf_fread(IOBUF *buf, int fd)
{
	size_t to_read;
	ssize_t bytes_read;

	transfer_buf_(buf);
	to_read = IOBUF_HALF_SIZE - buf->back_size;
	while (to_read){
		bytes_read = read(fd, buf->back + buf->back_size, to_read);
		if (bytes_read < 0){
			if (errno == EWOULDBLOCK)
				return IOBUF_PARTIAL;
			else 
				return IOBUF_ERR;
		}
		if (bytes_read == 0)
			return IOBUF_PARTIAL;

		to_read -= bytes_read; 
		buf->back_size += bytes_read;
	}
	return IOBUF_FULL;

}

size_t iobuf_read(IOBUF *ibuf, void *obuf, size_t obuf_size)
{
	size_t bytes_read = 0;
	size_t to_read;
	char *obuf_ch = obuf;
	iobuf_res err;

	while (obuf_size) { 
		/* buffer is empty but we are not done yet */
		if (ibuf->front_size == 0 && ibuf->back_size == 0){
			/* nowhere to get new data */
			if (ibuf->infd == -1)
				return bytes_read;

			/* otherwise */
			err = iobuf_fread(ibuf, ibuf->infd);
			if (err == IOBUF_ERR || err == IOBUF_PARTIAL)
				return bytes_read;			
		}
		transfer_buf_(ibuf);
		to_read = MIN(obuf_size, ibuf->front_size);
		if (to_read == 0)
			break;
		while (to_read){
			*obuf_ch++ = ibuf->front[--ibuf->front_size];
			--obuf_size;
		}
	}
	return bytes_read;
}
