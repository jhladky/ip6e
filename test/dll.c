#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "dll.h"

int main() {
   int err;
   // const char * sterr;
   void * handle;
   struct l3_funcs l3Funcs;
   struct ip_addr addr;

   // Modify this to RTLD_LAZY when we start using the testbench.
   if ((handle = dlopen("./libip6e.so", RTLD_NOW)) == NULL) {
      fprintf(stderr, "error opening library: %s\n", dlerror());
      abort();
   }
   
   printf("DLL!\n");
   l3Funcs.l3_rx = load_symbol(handle, "l3_rx", symbol_error);
   l3Funcs.l3_tx = load_symbol(handle, "l3_tx", symbol_error);

   l3Funcs.l3_rx(NULL, 0);
   l3Funcs.l3_tx(NULL, 0, addr);
   
   if ((err = dlclose(handle)) != 0) {
      fprintf(stderr, "error closing library: %s\n", dlerror());
      abort();
   }
   
   return 0;
}

void * load_symbol(void * handle, const char * str, void (errorHandler)(const char *)) {
   const char * err;
   void * symbol;

   // Clear out any old errors
   dlerror();
   symbol = dlsym(handle, str);
   if ((err = dlerror()) != NULL) {
      errorHandler(err);
      return NULL;
   } else {
      return symbol;
   }
}

void symbol_error(const char * err) {
   fprintf(stderr, "error loading symbol: %s\n", err);
   abort();
}
