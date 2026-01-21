CC=arm-none-eabi-gcc
CFLAGS=-I ./includes -I ./drivers -O2 -Wall -mthumb -mcpu=cortex-m0plus -DCPU_MKL46Z256VLL4
LDFLAGS=-O2 -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main.map,-Tlink.ld

TARGET=main.elf
OBJECTS=main.o startup.o drivers/fsl_assert.o includes/system_MKL46Z4.o drivers/fsl_debug_console.o drivers/fsl_common.o drivers/fsl_clock.o drivers/fsl_gpio.o drivers/fsl_log.o drivers/fsl_str.o drivers/fsl_io.o drivers/fsl_uart.o drivers/fsl_lpsci.o includes/board.o includes/pin_mux.o includes/clock_config.o drivers/fsl_smc.o drivers/fsl_ftfx_cache.o drivers/fsl_ftfx_controller.o drivers/fsl_ftfx_flash.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -lm -o $@ 

flash: $(TARGET)
	openocd -f openocd.cfg -c "program $< verify reset exit"

clean:
	$(RM) *.o *.map includes/*.o drivers/*.o

cleanall:
	$(RM) *.o *.elf *.map includes/*.o drivers/*.o

