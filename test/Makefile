# ip6e layer 3 & test harness Makefile

CC=gcc
CFLAGS= -Wall -Wextra -pedantic -std=c99 -D_POSIX_SOURCE
OBJS=l3.o

ip6e_test: test.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $< $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove build files
clean:
	rm -fr *.o
