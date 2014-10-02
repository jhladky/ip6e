#ifndef __LCD_H__
#define __LCD_H__

///LCD interaction time constants///
#define STARTUP_WAIT_INT 20 //ms
#define CTRL_WAIT_INT 39    //us
#define OK_WAIT_INT 1530    //us
#define DATA_WAIT_INT 43    //us
#define OSC_WAIT_INT 375    //us


///lcd command constants///
#define FUNCTION_SET 0x2FU
#define DISPLAY_CONTROL 0x0FU
#define DISPLAY_CLEAR 0x01U
#define ENTRY_MODE_SET 0x60U
#define RETURN_HOME 0x02U

//this section needs to be worked on a LOT
#define DATA_PINS 0x78   //PORTD.6 to PORTD.3
#define RS 0             //PORTD.0
#define RW 1             //PORTD.1
#define STROBE 2         //PORTD.2


///prototypes///
void lcd_init();
void lcd_string(const char* str);

#endif
