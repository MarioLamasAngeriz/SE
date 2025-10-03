CC=arm-none-eabi-gcc
LDFLAGS=-O2 -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main.map,-Tlink.ld
CFLAGS=-I ./includes -I ./drivers -O2 -Wall -mthumb -mcpu=cortex-m0plus -DCPU_MKL46Z256VLL4

LED=led.elf
OBJECTS_LED=led_blinky.o startup.o drivers/fsl_gpio.o drivers/fsl_clock.o drivers/fsl_common.o includes/board.o includes/pin_mux.o includes/clock_config.o includes/system_MKL46Z4.o drivers/fsl_debug_console.o drivers/fsl_smc.o drivers/fsl_log.o drivers/fsl_str.o drivers/fsl_ftfx_cache.o drivers/fsl_ftfx_controller.o drivers/fsl_ftfx_flash.o drivers/fsl_io.o drivers/fsl_uart.o drivers/fsl_lpsci.o

HI=hello.elf
OBJECTS_HI=hello_world.o startup.o drivers/fsl_gpio.o drivers/fsl_clock.o drivers/fsl_common.o includes/board.o includes/pin_mux.o includes/clock_config.o includes/system_MKL46Z4.o drivers/fsl_debug_console.o drivers/fsl_smc.o drivers/fsl_log.o drivers/fsl_str.o drivers/fsl_ftfx_cache.o drivers/fsl_ftfx_controller.o drivers/fsl_ftfx_flash.o drivers/fsl_io.o drivers/fsl_uart.o drivers/fsl_lpsci.o

$(LED): $(OBJECTS_LED)
	$(CC) $(LDFLAGS) $^ -o $@

$(HI): $(OBJECTS_HI)
	$(CC) $(LDFLAGS) $^ -o $@

flash_led: $(LED)
	openocd -f openocd.cfg -c "program led.elf verify reset exit"

flash_hello: $(HI)
	openocd -f openocd.cfg -c "program hello.elf verify reset exit"

clean:
	$(RM) *.o drivers/*.o includes/*.o

cleanall:
	$(RM) *.o *.elf *.map drivers/*.o includes/*.o


