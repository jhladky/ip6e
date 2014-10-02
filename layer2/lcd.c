#include <stdbool.h>
//#include <limits.h> //has length of a char as CHAR_BIT
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

///private prototypes///
void strobe();
void write(uint8_t byte, bool isData);

void lcd_init() {
   DDRD |= DATA_PINS | _BV(RS) | _BV(RW) | _BV(STROBE);
   PORTD |= _BV(STROBE);
   PORTD &= ~_BV(RW) & ~_BV(RS);

   _delay_ms(STARTUP_WAIT_INT);
   PORTD = (PORTD & ~DATA_PINS) | (FUNCTION_SET & 0XF0) >> 1;
   strobe();
   write(FUNCTION_SET, false);
   write(DISPLAY_CONTROL, false);
   write(DISPLAY_CLEAR, false);
   _delay_us(OK_WAIT_INT - CTRL_WAIT_INT);
   write(ENTRY_MODE_SET, false);
   write(RETURN_HOME, false); 
}

void lcd_string(const char* str) {
   while(*str) {
      write(*str++, true);
   }
}


///helper functions///
void strobe() {
   PORTD &= ~_BV(STROBE);
   _delay_us(1); //wait 20 cycles
   PORTD |= _BV(STROBE);
}

void write(uint8_t byte, bool isData) {
   PORTD ^= (-isData ^ PORTD) & _BV(RS);
   PORTD = (PORTD & ~DATA_PINS) | (byte & 0XF0) >> 1; //so this is wrong now
   strobe();
   PORTD = (PORTD & ~DATA_PINS) | (byte & 0X0F) << 3; //wrong as well
   strobe();
   _delay_us(DATA_WAIT_INT);
}
