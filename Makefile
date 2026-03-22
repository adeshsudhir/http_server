CC = gcc
CFLAGS = -Wall -Wextra -g

all: server
	

server: src/server.c
	$(CC) $(CFLAGS) src/server.c -o server

clean:
	rm -f server
