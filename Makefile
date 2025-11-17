CC=arm-none-eabi-gcc
LDFLAGS=-O2 -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main.map,-Tlink.ld 
CFLAGS=$(INCLUDES) -O2 -Wall -mthumb -mcpu=cortex-m0plus -DCPU_MKL46Z256VLL4 -DSDK_I2C_BASED_COMPONENT_USED 
INCLUDES = -I ./includes -I ./drivers -I ./includes/bubble/ -I ./includes/bubble_per/

BUBBLE=bubble.elf 
OBJECTS_BUBBLE=bubble.o startup.o drivers/fsl_gpio.o drivers/fsl_tsi_v4.o drivers/fsl_clock.o drivers/fsl_common.o includes/bubble/board.o includes/bubble/pin_mux.o includes/bubble/clock_config.o includes/system_MKL46Z4.o drivers/fsl_debug_console.o drivers/fsl_smc.o drivers/fsl_log.o drivers/fsl_str.o drivers/fsl_ftfx_cache.o drivers/fsl_ftfx_controller.o drivers/fsl_io.o drivers/fsl_uart.o drivers/fsl_lpsci.o drivers/fsl_assert.o drivers/fsl_mma.o drivers/fsl_i2c.o drivers/fsl_tpm.o


BUBBLE_PER=bubble_per.elf 
OBJECTS_BUBBLE_PER=bubble_per.o startup.o drivers/fsl_gpio.o drivers/fsl_tsi_v4.o drivers/fsl_clock.o drivers/fsl_common.o includes/bubble_per/board.o includes/bubble_per/pin_mux.o includes/bubble_per/clock_config.o includes/system_MKL46Z4.o drivers/fsl_debug_console.o drivers/fsl_smc.o drivers/fsl_log.o drivers/fsl_str.o drivers/fsl_ftfx_cache.o drivers/fsl_ftfx_controller.o drivers/fsl_io.o drivers/fsl_uart.o drivers/fsl_lpsci.o drivers/fsl_assert.o drivers/fsl_mma.o drivers/fsl_i2c.o includes/bubble_per/peripherals.o drivers/fsl_tpm.o

all: $(BUBBLE) $(BUBBLE_PER) 

$(BUBBLE): $(OBJECTS_BUBBLE)
	$(CC) $(LDFLAGS) $^ -lm -o $@

$(BUBBLE_PER): $(OBJECTS_BUBBLE_PER)
	$(CC) $(LDFLAGS) $^ -lm -o $@

flash_bubble: $(BUBBLE)
	openocd -f openocd.cfg -c "program $< verify reset exit"

flash_bubble_per: $(BUBBLE_PER) 
	openocd -f openocd.cfg -c "program $< verify reset exit"

clean:
	$(RM) *.o drivers/*.o includes/*.o

cleanall:
	$(RM) *.o *.elf *.map drivers/*.o includes/*.o


