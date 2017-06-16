
NANOMSG_CFLAGS = $(shell pkg-config --cflags libnanomsg)
NANOMSG_LDFLAGS = $(shell pkg-config --libs libnanomsg)

all: sp-server sp-client

sp-server:
	gcc sp-server.c -o sp-server -Wall -std=c11 $(NANOMSG_CFLAGS) $(NANOMSG_LDFLAGS)

sp-client:
	gcc sp-client.c -o sp-client -Wall -std=c11 $(NANOMSG_CFLAGS) $(NANOMSG_LDFLAGS)

clean:
	rm -f sp-server sp-server.o sp-client sp-client.o
