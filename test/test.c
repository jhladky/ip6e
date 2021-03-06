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
// #include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/if_packet.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "test.h"

//WORKS, HORRIBLE
//extern unsigned int if_nametoindex(const char*);

/* 
 * We need these as global so we can close them properly when we terminate.
 */
static int tap, packetSock;

int main() {
   const char* tapName = "ip6e";
   struct ifreq ifr;
   struct sockaddr_ll sa;
   int err, bytesRead, ifNdx;
   uint8_t buf[BUF_SIZE];

   add_handler(SIGINT, clean_exit);
   fprintf(stderr, "Open tap device ");
   /*
    * /dev/net/tun is known as the 'clone device'. Although we
    * receive a file descriptor from it, we can't use it until
    * we make the ioctl call.
    */
   if ((tap = open("/dev/net/tun", O_RDWR)) == -1) {
      perror("Opening tap fd failed");
      abort();
   }

   /*
    * When we make the ioctl call, this will
    * make a TAP device, and call it `ip6e`.
    * IFF_NO_PI removes a byte the kernel insert before all packets,
    * giving us the "pure" packet.
    */
   memset(&ifr, 0, sizeof(struct ifreq));
   ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
   strncpy(ifr.ifr_name, tapName, IFNAMSIZ);

   // The ioctl call with the magical TUNSETIFF flag
   if ((err = ioctl(tap, TUNSETIFF, (void *) &ifr)) == -1) {
      close(tap);
      perror("Tap initialization failed");
      abort();
   }
   
   /*
    * Two `system` calls that set up the ip6e link. Replaces using
    * ioctl
    */
   if ((err = system("/sbin/ip link set ip6e up")) == -1) {
      close(tap);
      perror("Tap network configuration failed");
      abort();
   }

   if ((err = system("/sbin/ip address add dev ip6e 10.100.200.1/24")) == -1) {
      close(tap);
      perror("Tap network configuration failed");
      abort();
   }

   if ((err = system("/sbin/ip route add 10.200.200.0/24 via "
                     "10.100.200.1")) == -1) {
      close(tap);
      perror("Tap network configuration failed");
      abort();
   }

   printDone();
   fprintf(stderr, "Open packet socket ");

   memset(&sa, 0, sizeof(struct sockaddr_ll));
   if ((ifNdx = if_nametoindex("ip6e")) == 0) {
      close(tap);
      perror("Get tap device index failed");
      abort();
   }
   sa.sll_family = PF_PACKET;
   sa.sll_protocol = htons(ETH_P_ALL);
   sa.sll_ifindex = if_nametoindex("ip6e");
   packetSock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
   
   if ((err = bind(packetSock,
                   (struct sockaddr *) &sa,
                   sizeof(struct sockaddr_ll))) < 0) {
      close(tap);
      close(packetSock);
      perror("Bind to tap interface failed");
      abort();
   }

   printDone();
   printf("Ready.\n");

   while (true) {
      if ((bytesRead = read(packetSock, buf, BUF_SIZE)) < 0) {
         perror("Error reading from socket");
         close(packetSock);
         abort();
      }
      printf("\nRead %d bytes from packet socket\n", bytesRead);
      eth_parse(buf, NULL);
   }
   
   printf("Exiting on finish.\n");
   close(tap);
   close(packetSock);
   return 0;
}

static char* ip_to_string(const uint8_t* addr) {
   static char str[IP_STR_LEN];
      
   snprintf(str, IP_STR_LEN, "%03d.%03d.%03d.%03d",
            addr[0], addr[1], addr[2], addr[3]);
   return str;
}

static char* mac_to_string(const uint8_t* addr) {
   static char str[MAC_STR_LEN];

   snprintf(str, MAC_STR_LEN, "%02X:%02X:%02X:%02X:%02X:%02X",
            addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
   return str;
}

static void eth_parse(const uint8_t* bytes, void* extra) {
   struct eth_header header;

   memcpy(&header, bytes, sizeof(struct eth_header));
   header.type = ntohs(header.type);

   printf("Eth Header 0014 bytes: ");
   printf("(Dest MAC: %s) ", mac_to_string(header.destMAC));
   printf("(Source MAC: %s) ", mac_to_string(header.srcMAC));
   
   printf("(Type: ");
   if(header.type == 0x806) {
      printf("ARP)\n");
      // arpParse(bytes + sizeof(ethHeader), NULL);
   } else if(header.type == 0x800) {
      printf("IP)\n");
      ip_parse(bytes + sizeof(struct eth_header), NULL);
   } else {
      printf("Unknown %d)\n", header.type);
   }
}

static void ip_parse(const uint8_t* bytes, void* extra) {
   struct ip_header header;
   uint32_t headerSize;
   
   memcpy(&header, bytes, sizeof(struct ip_header));
   header.totalLen = ntohs(header.totalLen);
   header.id  = ntohs(header.id);
   header.checksum  = ntohs(header.checksum);
   headerSize = ((long) sizeof(struct ip_header)) -
      ((header.verAndIHL & 0x0F) > 5 ? 0 : 4);

   if (strcmp("010.100.200.001", ip_to_string(header.srcIP)) == 0) {
      printf("--> ");
   } else {
      printf("<-- ");
   }
   printf("IP4 Header %04d bytes: ", headerSize);
   printf("(TOS: 0x%x) ", header.TOS);
   printf("(TTL: %d) ", header.TTL);
   printf("(Protocol: %d) ", header.protocol);
   // printf("(Checksum: not calculated) ");
   printf("(Sender IP: %s) ", ip_to_string(header.srcIP));
   printf("(Dest IP: %s) ", ip_to_string(header.destIP));
   printf("\n");
   if (header.protocol == 0x06) {
      tcp_parse(bytes + headerSize, NULL);
   }
}

static void tcp_parse(const uint8_t* bytes, void* extra) {
   uint8_t headerSize;
   struct tcp_header header;
   
   memcpy(&header, bytes, sizeof(tcp_header));
   header.srcPort = ntohs(header.srcPort);
   header.destPort = ntohs(header.destPort);
   header.seqNum = ntohl(header.seqNum);
   header.ackNum = ntohl(header.ackNum);
   header.windowSize = ntohs(header.windowSize);
   header.checksum = ntohs(header.checksum);
   header.urgentPtr = ntohs(header.urgentPtr);

   headerSize = (header.offset >> 4) * sizeof(uint32_t);

   printf("TCP Header %04d bytes: ", headerSize);
   printf("(Source Port: %05d) ", header.srcPort);
   printf("(Dest Port: %05d) ", header.destPort);
   printf("(Sequence Number: %010u) ", header.seqNum);
   printf("(ACK Number: %010u) ", header.ackNum);
   printf("(SYN Flag: %s) ", ynBoolString(header.flags & 0x02));
   printf("(RST Flag: %s) ", ynBoolString(header.flags & 0x04));
   printf("(FIN Flag: %s) ", ynBoolString(header.flags & 0x01));
   printf("(Window Size: %d) ", header.windowSize);
   // printf("(Checksum: not calculated) ");
   printf("\n");
}

static void clean_exit(int unused) {
   printf("Exiting on signal.\n");
   close(tap);
   close(packetSock);
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
