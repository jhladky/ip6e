# ip6e layer 3 & test harness Makefile

CC=gcc
CFLAGS= -Wall -Wextra -Wno-unused-parameter -pedantic -std=c99 -D_POSIX_SOURCE
# OBJS=l3.o

test: test.o
	$(CC) $(CFLAGS) -o $@ $<

dll: dll.o
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove build files
clean:
	rm -fr *.o
