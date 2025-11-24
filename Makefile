CC=arm-none-eabi-gcc
CFLAGS=-I ./includes -Ofast -Wall -mthumb -mcpu=cortex-m0plus -DCPU_MKL46Z256VLL4
LDFLAGS=-Ofast -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Tlink.ld

AS=arm-none-eabi-as
ASFLAGS=-mthumb -mcpu=cortex-m0plus

OBJECTS=startup.o includes/board.o includes/clock_config.o includes/fsl_clock.o includes/fsl_common.o includes/fsl_debug_console.o includes/fsl_gpio.o includes/pin_mux.o includes/fsl_smc.o includes/fsl_log.o includes/fsl_str.o includes/fsl_ftfx_cache.o includes/fsl_ftfx_controller.o includes/fsl_ftfx_flash.o includes/fsl_io.o includes/fsl_uart.o includes/fsl_lpsci.o includes/fsl_assert.o includes/system_MKL46Z4.o

TARGET-1=reverse1.elf
OBJECTS-1=reverse1.o $(OBJECTS)

TARGET-2=main2.elf
OBJECTS-2=main2.o reverse2.o $(OBJECTS)

TARGET-3=main3.elf
OBJECTS-3=main3.o reverse3.o $(OBJECTS)

TARGET-4=reverse4.elf
OBJECTS-4=reverse4.o $(OBJECTS)

all: $(TARGET-1) $(TARGET-2) $(TARGET-3) $(TARGET-4)

#introducir variaciones de flags de optimización

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

$(TARGET-1): $(OBJECTS-1)
	$(CC) $(LDFLAGS) -Wl,-Map,reverse1.map $^ -o $@

$(TARGET-2): $(OBJECTS-2)
	$(CC) $(LDFLAGS) -Wl,-Map,main2.map $^ -o $@
	
$(TARGET-3): $(OBJECTS-3)
	$(CC) $(LDFLAGS) -Wl,-Map,main3.map $^ -o $@

$(TARGET-4): $(OBJECTS-4)
	$(CC) $(LDFLAGS) -Wl,-Map,reverse4.map $^ -o $@

flash1: $(TARGET-1)
	openocd -f openocd.cfg -c "program $< verify reset exit"

flash2: $(TARGET-2)
	openocd -f openocd.cfg -c "program $< verify reset exit"

flash3: $(TARGET-3)
	openocd -f openocd.cfg -c "program $< verify reset exit"

flash4: $(TARGET-4)
	openocd -f openocd.cfg -c "program $< verify reset exit"

clean:
	$(RM) *.o *.map includes/*.o

cleanall:
	$(RM) *.o *.elf *.map includes/*.o

