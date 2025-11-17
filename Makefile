CC=arm-none-eabi-gcc
LDFLAGS=-O2 -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main.map,-Tlink.ld
CFLAGS=$(INCLUDES) -O2 -Wall -mthumb -mcpu=cortex-m0plus -DCPU_MKL46Z256VLL4
INCLUDES = -I ./includes -I ./drivers

TOUCH=tsi_v4_normal.elf
OBJECTS_TOUCH=tsi_v4_normal.o startup.o drivers/fsl_gpio.o drivers/fsl_tsi_v4.o drivers/fsl_clock.o drivers/fsl_common.o includes/board.o includes/pin_mux.o includes/clock_config.o includes/system_MKL46Z4.o drivers/fsl_debug_console.o drivers/fsl_smc.o drivers/fsl_log.o drivers/fsl_str.o drivers/fsl_ftfx_cache.o drivers/fsl_ftfx_controller.o drivers/fsl_io.o drivers/fsl_uart.o drivers/fsl_lpsci.o drivers/fsl_assert.o drivers/fsl_lptmr.o

all: $(TOUCH) 

$(TOUCH): $(OBJECTS_TOUCH)
	$(CC) $(LDFLAGS) $^ -o $@

flash_touch: $(TOUCH)
	openocd -f openocd.cfg -c "program $< verify reset exit"

clean:
	$(RM) *.o drivers/*.o includes/*.o

cleanall:
	$(RM) *.o *.elf *.map drivers/*.o includes/*.o


