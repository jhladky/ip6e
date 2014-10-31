# ip6e layer 3 & test harness Makefile

CC=gcc
CFLAGS= -Wall -Wextra -Wno-unused-parameter -pedantic -std=c99 -D_POSIX_SOURCE
LDFLAGS=-ldl
# OBJS=l3.o

OS = $(shell uname -s)

ifeq ("$(OS)", "Darwin")
   OSFLAGS=-DDARWIN -D_BSD_SOURCE
	OSLDFLAGS=-install_name
else
   OSFLAGS=-DLINUX
	OSLDFLAGS=-soname
endif

test: test.o
	$(CC) $(CFLAGS) $(OSFLAGS) -o $@ $< $(LDFLAGS)

dll: dll.o
	$(CC) $(CFLAGS) $(OSFLAGS) -o $@ $< $(LDFLAGS)

echo_client: echo_client.o
	$(CC) $(CFLAGS) $(OSFLAGS) -o $@ $<

echo_server: echo_server.o
	$(CC) $(CFLAGS) $(OSFLAGS) -o $@ $<

echo: echo_client echo_server

all: test dll echo_client echo_server

%.o: %.c
	$(CC) $(CFLAGS) $(OSFLAGS) -c $< -o $@ $(LDFLAGS)

# Remove build files
clean:
	rm -fr *.o dll test echo_client echo_server
