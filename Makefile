CC=arm-none-eabi-gcc
LDFLAGS=-O2 -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main.map,-Tlink.ld
CFLAGS=-I ./includes -O2 -Wall -mthumb -mcpu=cortex-m0plus

TARGET=main.elf
OBJECTS=main.o startup.o 

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

flash: $(TARGET)
	openocd -f openocd.cfg -c "program main.elf verify reset exit"

clean:
	$(RM) *.o

cleanall:
	$(RM) *.o *.elf *.map


