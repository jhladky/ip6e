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
#include <linux/if_packet.h>
#include "test.h"

/* 
 * We need these as global so we can close them properly when we terminate.
 */
static int tap, packetSock, rawSock;

int main() {
   struct ifreq ifr;
   int err, bytesRead;
   uint8_t buf[BUF_SIZE];

   add_handler(SIGINT, clean_exit);

   /* /dev/net/tun is known as the 'clone device'. Although we
    * receive a file descriptor from it, we can't use it until
    * we make the ioctl call.
    */
   if ((tap = open("/dev/net/tun", O_RDWR)) == -1) {
      perror("Failed to open tap device");
      abort();
   }

   /* When we make the ioctl call, this will
    * make a TAP device, and call it `ip6e`.
    * IFF_NO_PI removes a byte the kernel insert before all packets,
    * giving us the "pure" packet.
    */
   memset(&ifr, 0, sizeof(struct ifreq));
   ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
   strncpy(ifr.ifr_name, "ip6e", IFNAMSIZ);

   // The iocal call with the magical TUNSETIFF flag
   if ((err = ioctl(tap, TUNSETIFF, (void *) &ifr)) == -1) {
      close(tap);
      perror("Failed to configure tap device");
      abort();
   }

   printf("(1) Open tap device\n");

   packetSock = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ALL));
   rawSock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

   printf("(2) Open sockets\n");

   while (true) {

      /*if ((bytesRead = read(tap, buf, BUF_SIZE)) < 0) {
        perror("Error reading from tap device");
        close(tap);
        abort();
        }

        printf("Read %d bytes from ip6e\n", bytesRead); */
      
      if ((bytesRead = read(packetSock, buf, BUF_SIZE)) < 0) {
         perror("Error reading from socket");
         close(packetSock);
         abort();
      }

      printf("Read %d bytes from packet socket\n", bytesRead);
      if ((bytesRead = read(rawSock, buf, BUF_SIZE)) < 0) {
         perror("Error reading from socket");
         close(rawSock);
         abort();
      }
      printf("Read %d bytes from raw socket\n", bytesRead);
      ip_parse(buf, NULL);
   }
   
   printf("Exiting on finish.\n");
   close(tap);
   close(packetSock);
   close(rawSock);
   return 0;
}

static void print_ip(const char* title, const uint8_t* addr) {
   int i;
   
   printf("(%s", title);
   for(i = 0; i < IP_LEN; i++) {
      printf("%03d", addr[i]);
      if(i != (IP_LEN - 1)) {
         printf(".");
      }
   }
   printf(") ");
}

void ip_parse(const uint8_t* bytes, void* extra) {
   struct ip_header header;
   uint32_t headerSize;
   
   memcpy(&header, bytes, sizeof(struct ip_header));
   header.totalLen = ntohs(header.totalLen);
   header.id  = ntohs(header.id);
   header.checksum  = ntohs(header.checksum);
   headerSize = ((long) sizeof(struct ip_header)) - ((header.verAndIHL & 0x0F) > 5 ? 0 : 4);

   printf("IP Header: ");
   printf("(TOS: 0x%x) ", header.TOS);
   printf("(TTL: %d) ", header.TTL);
   printf("(Protocol: TCP) ");
   printf("(Checksum: not calculated) ");
   print_ip("Sender IP: ", header.srcIP);
   print_ip("Dest IP: ", header.destIP);
   printf("\n");
   tcp_parse(bytes + headerSize, NULL);
}

static void tcp_parse(const uint8_t* bytes, void* extra) {
   struct tcp_header header;
   
   memcpy(&header, bytes, sizeof(tcp_header));
   header.srcPort = ntohs(header.srcPort);
   header.destPort = ntohs(header.destPort);
   header.seqNum = ntohl(header.seqNum);
   header.ackNum = ntohl(header.ackNum);
   header.windowSize = ntohs(header.windowSize);
   header.checksum = ntohs(header.checksum);
   header.urgentPtr = ntohs(header.urgentPtr);

   printf("TCP Header: ");
   printf("(Source Port: %05d) ", header.srcPort);
   printf("(Dest Port: %05d) ", header.destPort);
   printf("(Sequence Number: %010u) ", header.seqNum);
   printf("(ACK Number: %010u) ", header.ackNum);
   printf("(SYN Flag: %s) ", ynBoolString(header.flags & 0x02));
   printf("(RST Flag: %s) ", ynBoolString(header.flags & 0x04));
   printf("(FIN Flag: %s) ", ynBoolString(header.flags & 0x01));
   printf("(Window Size: %d) ", header.windowSize);
   printf("(Checksum: not calculated) ");
   printf("\n");
}

static void clean_exit(int unused) {
   printf("Exiting on signal.\n");
   close(tap);
   close(packetSock);
   close(rawSock);
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
