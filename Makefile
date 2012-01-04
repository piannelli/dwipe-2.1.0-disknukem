all: *.c
	$(CC) -Os -Wall -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE *.c -lncurses libmicrohttpd.a -lpthread -lxml2 -I/usr/include/libxml2 -ljson -o dwipe
	$(STRIP) dwipe

clean:
	rm -f a.out dwipe
	rm -f *.o

# eof
