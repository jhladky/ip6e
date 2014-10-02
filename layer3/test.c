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

static int tunFd;

int main() {
   struct ifreq ifr;
   int err;

   add_handler(SIGINT, clean_exit);

   if ((tunFd = open("/dev/net/tun", O_RDWR)) == -1) {
      perror("Failed to open tap device");
      abort();
   }

   memset(&ifr, 0, sizeof(struct ifreq));
   ifr.ifr_flags = IFF_TUN;
   strncpy(ifr.ifr_name, "ip6e_test", IFNAMSIZ);

   if ((err = ioctl(tunFd, TUNSETIFF, (void *) &ifr)) == -1) {
      close(tunFd);
      perror("Failed to configure tap device");
      abort();
   }

   printf("Opening tap device successful!\n");

   while (true) {

   }

   return 0;
}

static void clean_exit(int unused) {
   printf("Exiting on signal.\n");
   close(tunFd);
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
