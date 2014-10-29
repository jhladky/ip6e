#ifndef _IP6E_TEST_
#define _IP6E_TEST_

#define BUF_SIZE 2048
#define IP_LEN 4
#define MAC_LEN 6
#define ynBoolString(b) ((b) ? "yes" : " no")
#define printDone() fprintf(stderr, "[X]\n")

static void clean_exit(int unused);
static void add_handler(int signal, void (*handlerFunc)(int));
static void print_ip(const char* title, const uint8_t* addr);
static void print_mac(const char* title, const uint8_t* mac);
static void tcp_parse(const uint8_t* bytes, void* extra);
static void ip_parse(const uint8_t* bytes, void* extra);
static void eth_parse(const uint8_t* bytes, void* extra);

struct __attribute__ ((__packed__)) tcp_header {
   uint16_t srcPort;
   uint16_t destPort;
   uint32_t seqNum;
   uint32_t ackNum;
   uint8_t offset;
   uint8_t flags;
   uint16_t windowSize;
   uint16_t checksum;
   uint16_t urgentPtr;
   uint8_t options[40]; /*MAY BE A FUTURE BUG. ALT, I MIGHT GET AWAY WITH IT HAHA*/
} tcp_header;

struct __attribute__ ((__packed__)) ip_header {
   uint8_t verAndIHL;
   uint8_t TOS;
   uint16_t totalLen;
   uint16_t id;
   uint16_t flags; /* not quite right?...*/
   uint8_t TTL;
   uint8_t protocol;
   uint16_t checksum;
   uint8_t srcIP[IP_LEN];
   uint8_t destIP[IP_LEN];
   uint8_t options[4];
} ip_header;

struct __attribute__ ((__packed__)) eth_header {
   uint8_t destMAC[MAC_LEN];
   uint8_t srcMAC[MAC_LEN];
   uint16_t type;
} eth_header;

#endif
