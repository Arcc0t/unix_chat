#include "iobuf.h"

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

	result = (IOBUF *) calloc(sizeof(IOBUF));
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
