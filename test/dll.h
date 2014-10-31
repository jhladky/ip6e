#ifndef _DLL_H_
#define _DLL_H_

#define IP_ADDR_LEN 16

struct ip_addr {
   uint8_t bytes[IP_ADDR_LEN];
} ip_addr;

struct l3_funcs {
   int (*l3_rx)(void * packet, int len);
   int (*l3_tx)(void * data, int len, struct ip_addr addr);
} l3_funcs;

void * load_symbol(void * handle, const char * str, void (error)(const char *));
void symbol_error(const char * err);

#endif
