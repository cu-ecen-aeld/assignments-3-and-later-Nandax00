CC=gcc
ifdef CROSS_COMPILE
	CC=aarch64-none-linux-gnu-gcc
endif

writer : writer.o
	$(CC) -o writer writer.o

writer.o : writer.c
	$(CC) -c writer.c

clean :
	rm -f writer writer.o
