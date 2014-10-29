#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <linux/if.h>
#include <linux/if_tun.h>
//#include "l3.h"
#include "test.h"

/*
So let's try to make this thing open tun right now
 */

static int tapFd;

int main() {
   struct ifreq ifr;
   int err, i, bytesRead;
   uint8_t buf[BUF_SIZE];

   add_handler(SIGINT, clean_exit);

   if ((tapFd = open("/dev/net/tun", O_RDWR)) == -1) {
      perror("Failed to open tap device");
      abort();
   }

   memset(&ifr, 0, sizeof(struct ifreq));
   ifr.ifr_flags = IFF_TAP;
   strncpy(ifr.ifr_name, "ip6e_test", IFNAMSIZ);

   if ((err = ioctl(tapFd, TUNSETIFF, (void *) &ifr)) == -1) {
      close(tapFd);
      perror("Failed to configure tap device");
      abort();
   }

   printf("Opening tap device successful!\n");

   while (true) {
      if ((bytesRead = read(tapFd, buf, BUF_SIZE)) == 0) {
         perror("Error reading from tap device");
         close(tapFd);
         abort();
      }

      printf("%d bytes read, dumping...\n", bytesRead);
      for (i = 0; i < BUF_SIZE; i++) {
         printf("%02X ", buf[i]);
      }
      printf("\n");
   }

   return 0;
}

static void clean_exit(int unused) {
   printf("Exiting on signal.\n");
   close(tapFd);
   exit(EXIT_SUCCESS);
}

static void add_handler(int signal, void (*handlerFunc)(int)) {
   struct sigaction sa;

   sa.sa_handler = handlerFunc;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   if (sigaction(signal, &sa, NULL) < 0)  {
      perror("Adding signal handler failed");
      abort(); //justified
   }
}
