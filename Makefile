# ip6e layer 3 & test harness Makefile

OS = $(shell uname -s)

# test dir vars
CC=gcc
# CFLAGS= -Wall -Wextra -Wno-unused-parameter -pedantic -std=c99 -D_POSIX_SOURCE
CFLAGS= -Wall -Wextra -Wno-unused-parameter -std=c99 -D_POSIX_SOURCE
LDFLAGS=-ldl

ifeq ("$(OS)", "Darwin")
   OSFLAGS=-DDARWIN -D_BSD_SOURCE
	OSLDFLAGS=-install_name
else
   OSFLAGS=-DLINUX -D_GNU_SOURCE
	OSLDFLAGS=-soname
endif

test: test/test.o
	$(CC) $(CFLAGS) $(OSFLAGS) -o ip6e_test $< $(LDFLAGS)

dll: test/dll.o
	$(CC) $(CFLAGS) $(OSFLAGS) -o $@ $< $(LDFLAGS)

preload: test/preload.c
	$(CC) $(CFLAGS) $(OSFLAGS) -o libpreload.so $< -fPIC -shared $(LDFLAGS)

echo_client: test/echo_client.o
	$(CC) $(CFLAGS) $(OSFLAGS) -o $@ $<

echo_server: test/echo_server.o
	$(CC) $(CFLAGS) $(OSFLAGS) -o $@ $<

echo: echo_client echo_server

l3: layer3/l3.c
	$(CC) $(CFLAGS) -fPIC -c $<
	$(CC) $(CFLAGS) -shared -Wl,$(OSLDFLAGS),libip6e.so -o libip6e.so l3.o

all: l3 test dll echo_client echo_server

# Object file dependencies
%.o: %.c
	$(CC) $(CFLAGS) $(OSFLAGS) -c $< -o $@ $(LDFLAGS)

# Remove build files
clean:
	rm -fr *.o dll ip6e_test echo_client echo_server libip6e.so test/*.o layer3/*.o
