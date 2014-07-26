# ip6e Makefile

CC=avr-gcc
CFLAGS=-mmcu=atmega644 -DF_CPU=20000000 -Os -std=c99
OBJS=lcd.o

# Compile
tube: main.o $(OBJS)
	$(CC) $(CFLAGS) -o main.elf main.o $(OBJS)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Flash
flash: main.hex
	avrdude -pm644 -c usbtiny -F -u -U flash:w:main.hex

# Remove build files
clean:
	rm -fr *.elf *.hex *.o
