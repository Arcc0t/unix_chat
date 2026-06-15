#ifndef IOBUF_H_GUARD
#define IOBUF_H_GUARD
#include <stdlib.h>
#include "defines.h"
#include <sys/types.h>

typedef struct iobuf_ IOBUF;

enum {IOBUF_NOFILE = -1};
typedef enum {IOBUF_ERR = -1, IOBUF_FULL = 0, IOBUF_PARTIAL = 1} iobuf_res;

IOBUF *iobuf_create(int infd, int outfd);

void iobuf_destroy(IOBUF *buf);


/* write to buffer */
size_t iobuf_write(IOBUF *obuf, void *ibuf, size_t ibuf_size);

/* read from buffer */
size_t iobuf_read(IOBUF *ibuf, void *obuf, size_t obuf_size);

int iobuf_getchar(IOBUF *buf);

int iobuf_peek(IOBUF *buf);

BOOL iobuf_putchar(IOBUF *buf, char c);

size_t iobuf_getline(IOBUF *ibuf, char *obuf, size_t obuf_size);

iobuf_res iobuf_putline(IOBUF *buf, char *str);

/* write to file descriptor*/
size_t iobuf_fwrite(IOBUF *buf, int fd); 

/*read from file descriptor*/
iobuf_res iobuf_fread(IOBUF *buf, int fd);

iobuf_res iobuf_flush(IOBUF *buf);

#endif /* IOBUF_H_GUARD */
