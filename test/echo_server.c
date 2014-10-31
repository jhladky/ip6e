#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define ECHO_PORT 21219
#define BUF_LEN 101

static void pexit(const char * message);
static void clean_exit(int unused);
static void add_handler(int signal, void (*handlerFunc)(int));

static int sock;

int main() {
   struct sockaddr_in sa, client;
   int bytesRead, sockLen = sizeof(struct sockaddr_in);
   char buf[BUF_LEN];

   add_handler(SIGINT, clean_exit);
   
   if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) == -1) {
      pexit("Failed to open socket");
   }

   memset(&sa, 0, sizeof(struct sockaddr_in));
   memset(&client, 0, sizeof(struct sockaddr_in));
   sa.sin_family = AF_INET;
   sa.sin_port = htons(ECHO_PORT);
   sa.sin_addr.s_addr = htonl(INADDR_ANY);

   if (bind(sock, (struct sockaddr *) &sa, sizeof(struct sockaddr_in)) == -1) {
      pexit("Failed to connect to server");
   }

   printf("data[[\n");
   
   while (true) {
      if ((bytesRead = recvfrom(sock, buf, BUF_LEN, 0,
                                (struct sockaddr *) &client,
                                (socklen_t *) &sockLen)) == -1) {
         pexit("Failed to read from socket");
      }

      printf("%s\n" , buf);
      
      if (sendto(sock, buf, bytesRead, 0, (struct sockaddr *) &client,
                 (socklen_t) sockLen) == -1) {
         pexit("Failed to write to socket");
      }
   }
   
   close(sock);
   
   return 0;
}

static void clean_exit(int unused) {
   printf("Exiting on signal.\n");
   close(sock);
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


static void pexit(const char * message) {
   perror(message);
   exit(EXIT_FAILURE);
}
