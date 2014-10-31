#include <stdint.h>
#include <stdio.h>
#include "l3.h"

int l3_rx(void * packet, int len) {

   printf("I'm l3_rx!\n");
   
   return 0;
}

int l3_tx(void * data, int len, struct ip_addr addr) {

   printf("I'm l3_tx!\n");
   
   return 0;
}
