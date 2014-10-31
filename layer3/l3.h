#ifndef __L3_H__
#define __L3_H__

#define IP_ADDR_LEN 16

struct ip_addr {
   uint8_t bytes[IP_ADDR_LEN];
} ip_addr;

int l3_rx(void * packet, int len);
int l3_tx(void * data, int len, struct ip_addr addr);

#endif
