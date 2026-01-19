CC=arm-none-eabi-gcc
CFLAGS=-I ./includes -O2 -Wall -mthumb -mcpu=cortex-m0plus -DCPU_MKL46Z256VLL4
LDFLAGS=-O2 -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main.map,-Tlink.ld

TARGET=main.elf
OBJECTS=main.o startup.o ./includes/board.o ./includes/clock_config.o ./includes/fsl_assert.o ./includes/fsl_clock.o ./includes/fsl_common.o ./includes/fsl_debug_console.o ./includes/fsl_gpio.o ./includes/fsl_log.o ./includes/fsl_smc.o ./includes/fsl_str.o ./includes/fsl_tpm.o ./includes/pin_mux.o ./includes/fsl_io.o ./includes/fsl_ftfx_cache.o ./includes/fsl_ftfx_controller.o ./includes/fsl_uart.o ./includes/fsl_lpsci.o ./includes/system_MKL46Z4.o ./includes/lcd.o ./includes/fsl_mma.o ./includes/fsl_i2c.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -lm -o $@ 

flash: $(TARGET)
	openocd -f openocd.cfg -c "program $< verify reset exit"

clean:
	$(RM) *.o *.map includes/*.o

cleanall:
	$(RM) *.o *.elf *.map includes/*.o

