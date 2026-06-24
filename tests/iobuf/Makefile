src_dir = ../../src


iobuf.o: $(src_dir)/iobuf.c
	$(CC) -o $@ -c -I$(src_dir) -g -Wall $^ 

%.x: %.c iobuf.o
	$(CC) -o $@ -I$(src_dir) -g -Wall $^
