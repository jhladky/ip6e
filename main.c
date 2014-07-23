#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#define LED (1 << 6)

int main() {
   DDRD |= LED;
   
   while (true) {
      PORTD |= LED;
      _delay_ms(1000);
      PORTD &= ~LED;
      _delay_ms(1000);
   }

   return 0;
}
