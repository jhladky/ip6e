#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"

#define LED (1 << 7)

int main() {
   DDRD |= LED;
   
   lcd_init();
   lcd_string("Hello!");

   while (true) {
      PORTD |= LED;
      _delay_ms(1000);
      PORTD &= ~LED;
      _delay_ms(1000);
   }

   return 0;
}
