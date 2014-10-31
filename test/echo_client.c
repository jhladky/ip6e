#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define REQ_ARGS 3
#define ECHO_PORT 21219
#define BUF_LEN 101

static void pexit(const char * message);

int main(int argc, char * argv[]) {
   struct sockaddr_in sa;
   int sock, err;
   FILE * in;
   char sndBuf[BUF_LEN], rcvBuf[BUF_LEN];
   
   if (argc < REQ_ARGS) {
      fprintf(stderr, "Usage: echo_client file server_ip\n");
      exit(EXIT_FAILURE);
   }

   if ((in = fopen(argv[1], "r")) == NULL) {
      pexit("Failed to open file");
   }
   
   if ((sock = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
      pexit("Failed to open socket");
   }

   memset(&sa, 0, sizeof(struct sockaddr_in));
   sa.sin_family = AF_INET;
   sa.sin_port = htons(ECHO_PORT);
   if ((err = inet_pton(AF_INET, argv[2], &sa.sin_addr)) == 0)  {
      fprintf(stderr, "Opening socket failed: invalid network address.\n");
      exit(EXIT_FAILURE);
   } else if (err == -1) {
      pexit("Failed to parse address");
   }

   if (connect(sock, (struct sockaddr *) &sa, sizeof(struct sockaddr_in)) == -1) {
      pexit("Failed to connect to server");
   }

   while (fgets(sndBuf, BUF_LEN, in) != NULL) {
      if ((err = write(sock, sndBuf, strlen(sndBuf) + 1)) < 0) {
         pexit("Failed to write to socket");
      }

      if ((err = read(sock, rcvBuf, BUF_LEN)) < 0) {
         pexit("Failed to read from socket");
      }

      if (strcmp(sndBuf, rcvBuf) != 0) {
         fprintf(stderr, "Diff error: expected: %s, received: %s\n",
                 sndBuf, rcvBuf);
         exit(EXIT_FAILURE);
      }
   }

   if (feof(in) == 0 || ferror(in)) {
      pexit("Failed to read file");
   }

   printf("Finished\n");
   fclose(in);
   close(sock);       

   return 0;
}

static void pexit(const char * message) {
   perror(message);
   exit(EXIT_FAILURE);
}
