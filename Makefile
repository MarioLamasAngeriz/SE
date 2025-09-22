CC=arm-none-eabi-gcc
LDFLAGS=-O2 -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl, --gc-sections, -Map, main.map, -Tlink.ld
CFLAGS=-I ./includes -O2 -Wall -mthumb -mcpu=cortex-m0plus

flash: main.o startup.o

clean:
	$(RM) *.o

cleanall:
	$(RM) *.o *.elf *.map


