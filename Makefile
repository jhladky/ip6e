# Compile
tube: main.c
	avr-gcc -mmcu=atmega644 -DF_CPU=16000000 -Os -std=c99 -o main.elf main.c
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

# Flash
flash: main.hex
	avrdude -pm644 -c usbtiny -F -u -U flash:w:main.hex

# Remove build files
clean:
	rm -fr *.elf *.hex *.o
