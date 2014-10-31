# ip6e layer 3 & test harness Makefile

CC=gcc
CFLAGS= -Wall -Wextra -Wno-unused-parameter -pedantic -std=c99 -D_POSIX_SOURCE
LDFLAGS=-ldl
# OBJS=l3.o

test: test.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

dll: dll.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

# Remove build files
clean:
	rm -fr *.o dll test
