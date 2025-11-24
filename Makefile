CC=arm-none-eabi-gcc
CFLAGS=-I ./includes -Wall -mthumb -mcpu=cortex-m0plus -DCPU_MKL46Z256VLL4
LDFLAGS=-Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Tlink.ld

AS=arm-none-eabi-as
ASFLAGS=-mthumb -mcpu=cortex-m0plus

OBJECTS=startup.o includes/board.o includes/clock_config.o includes/fsl_clock.o includes/fsl_common.o includes/fsl_debug_console.o includes/fsl_gpio.o includes/pin_mux.o includes/fsl_smc.o includes/fsl_log.o includes/fsl_str.o includes/fsl_ftfx_cache.o includes/fsl_ftfx_controller.o includes/fsl_ftfx_flash.o includes/fsl_io.o includes/fsl_uart.o includes/fsl_lpsci.o includes/fsl_assert.o includes/system_MKL46Z4.o

TARGET-1-O0=reverse1_O0.elf
OBJECTS-1-O0=reverse1_O0.o $(OBJECTS)

TARGET-1-OFAST=reverse1_Ofast.elf
OBJECTS-1-OFAST=reverse1_Ofast.o $(OBJECTS)

TARGET-2=main2.elf
OBJECTS-2=main2.o reverse2.o $(OBJECTS)

TARGET-3=main3.elf
OBJECTS-3=main3.o reverse3.o $(OBJECTS)

TARGET-4-O0=reverse4_O0.elf
OBJECTS-4-O0=reverse4_O0.o $(OBJECTS)

TARGET-4-OFAST=reverse4_Ofast.elf
OBJECTS-4-OFAST=reverse4_Ofast.o $(OBJECTS)

all: $(TARGET-1-O0) $(TARGET-1-OFAST) $(TARGET-2) $(TARGET-3) $(TARGET-4-O0) $(TARGET-4-OFAST)

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

reverse1_O0.o: reverse1.c
	$(CC) $(CFLAGS) -O0 -c -o $@ $<

reverse1_Ofast.o: reverse1.c 
	$(CC) $(CFLAGS) -Ofast -c -o $@ $<

main2.o: main2.c 
	$(CC) $(CFLAGS) -Ofast -c -o $@ $<

main3.o: main3.c	
	$(CC) $(CFLAGS) -Ofast -c -o $@ $<

reverse4_O0.o: reverse4.c
	$(CC) $(CFLAGS) -O0 -c -o $@ $<

reverse4_Ofast.o: reverse4.c 
	$(CC) $(CFLAGS) -Ofast -c -o $@ $<

$(TARGET-1-O0): $(OBJECTS-1-O0)
	$(CC) $(LDFLAGS) -O0 -Wl,-Map,reverse1_O0.map $^ -o $@

$(TARGET-1-OFAST): $(OBJECTS-1-OFAST)
	$(CC) $(LDFLAGS) -Ofast -Wl,-Map,reverse1_Ofast.map $^ -o $@

$(TARGET-2): $(OBJECTS-2)
	$(CC) $(LDFLAGS) -Wl,-Map,main2.map $^ -o $@
	
$(TARGET-3): $(OBJECTS-3)
	$(CC) $(LDFLAGS) -Wl,-Map,main3.map $^ -o $@

$(TARGET-4-O0): $(OBJECTS-4-O0)
	$(CC) $(LDFLAGS) -O0 -Wl,-Map,reverse4_O0.map $^ -o $@

$(TARGET-4-OFAST): $(OBJECTS-4-OFAST)
	$(CC) $(LDFLAGS) -Ofast -Wl,-Map,reverse4_Ofast.map $^ -o $@

flash1O0: $(TARGET-1-O0)
	openocd -f openocd.cfg -c "program $< verify reset exit"

flash1Ofast: $(TARGET-1-OFAST)
	openocd -f openocd.cfg -c "program $< verify reset exit"

flash2: $(TARGET-2)
	openocd -f openocd.cfg -c "program $< verify reset exit"

flash3: $(TARGET-3)
	openocd -f openocd.cfg -c "program $< verify reset exit"

flash4O0: $(TARGET-4-O0)
	openocd -f openocd.cfg -c "program $< verify reset exit"

flash4Ofast: $(TARGET-4-OFAST)
	openocd -f openocd.cfg -c "program $< verify reset exit"

clean:
	$(RM) *.o *.map includes/*.o

cleanall:
	$(RM) *.o *.elf *.map includes/*.o

