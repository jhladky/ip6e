#include <stdio.h>
// #include <stdlib.h>
#include <sys/socket.h>
#include <dlfcn.h>

int socket(int socketFamily, int socketType, int protocol) {
   int (*realSocket)(int socket_family, int socket_type, int protocol);

   realSocket = dlsym(RTLD_NEXT, "socket");
   //include a check for the realSocket here

   if (socketType == SOCK_DGRAM) {
      printf("doin some UDP...\n");
   } else {
      printf("????\n");
   }
   
   return realSocket(socketFamily, socketType, protocol);
}
