CC=arm-none-eabi-gcc
LDFLAGS=-O2 -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main.map,-Tlink.ld
CFLAGS=-I ./includes -I ./drivers -O2 -Wall -mthumb -mcpu=cortex-m0plus

LED=led.elf
OBJECTS_LED=led_blinky.o startup.o

HI=hello.elf
OBJECTS_HI=hello_world.o startup.o

$(LED): $(OBJECTS_LED)
	$(CC) $(LDFLAGS) $^ -o $@

$(HI): $(OBJECTS_HI)
	$(CC) $(LDFLAGS) $^ -o $@

flash_led: $(LED)
	openocd -f openocd.cfg -c "program led.elf verify reset exit"

flash_hello: $(HI)
	openocd -f openocd.cfg -c "program hello.elf verify reset exit"

clean:
	$(RM) *.o

cleanall:
	$(RM) *.o *.elf *.map


