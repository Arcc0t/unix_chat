#include "iobuf.h"
#include <unistd.h>
#include <stdio.h>
int main(void)
{
	char str[100] = {0};
	IOBUF *buf = iobuf_create(0, IOBUF_NOFILE);
	iobuf_read(buf, str, sizeof(str) - 1);
	fputs(str, stdout);

}
