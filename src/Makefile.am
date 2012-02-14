all: *.c
	$(CC) -Os -Wall -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE *.c -lncurses -lmicrohttpd -lpthread -lxml2 -I/usr/include/libxml2 -ljson -lcurl -o disknukem

install: disknukem
	mkdir -p $(prefix)/share/disknukem/bin
	mkdir -p $(prefix)/share/disknukem/web
	install -m 0755 disknukem $(prefix)/bin
	install -m 0644 web/* $(prefix)/share/disknukem/web

.PHONY: install

clean:
	rm -f a.out disknukem
	rm -f *.o

clean-local:
	rm -rf a.out disknukem *.o

# eof

bin_PROGRAMS = disknukem
disknukem_SOURCES = device.c dwipe.c gui.c httpd.c isaac_rand.c json.c logging.c method.c mt19937ar-cok.c notify.c options.c pass.c prng.c xml.c
AM_CFLAGS = -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -Os -Wall -lncurses -lmicrohttpd -lpthread -lxml2 -I/usr/include/libxml2 -ljson -lcurl
