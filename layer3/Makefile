# ip6e layer 3 Makefile

CC=gcc
CFLAGS= -Wall -Wextra -Wno-unused-parameter -pedantic -std=c99 -D_POSIX_SOURCE
OS = $(shell uname -s)

ifeq ("$(OS)", "Darwin")
   OSFLAGS=-DDARWIN
	OSLDFLAGS=-install_name
else
   OSFLAGS=-DLINUX
	OSLDFLAGS=-soname
endif

l3_obj: l3.c
	$(CC) $(CFLAGS) -fPIC -c $<
	$(CC) $(CFLAGS) -shared -Wl,$(OSLDFLAGS),libip6e.so -o libip6e.so l3.o

l3: l3.o
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove build files
clean:
	rm -fr *.o
