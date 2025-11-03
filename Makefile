CC=arm-none-eabi-gcc
#LDFLAGS=-Ofast -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main.map,-Tlink.ld
CFLAGS=-I ./includes -Ofast -Wall -mthumb -mcpu=cortex-m0plus -DCPU_MKL46Z256VLL4

LDFLAGS-C=-Ofast -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main-c.map,-Tlink.ld
#CFLAGS-C=-I ./includes -Ofast -Wall -mthumb -mcpu=cortex-m0plus -DCPU_MKL46Z256VLL4

TARGET-C=main-c.elf
OBJECTS-C= main-c.o startup.o includes/board.o includes/clock_config.o includes/fsl_clock.o includes/fsl_common.o includes/fsl_debug_console.o includes/fsl_gpio.o includes/pin_mux.o includes/fsl_smc.o includes/fsl_log.o includes/fsl_str.o includes/fsl_ftfx_cache.o includes/fsl_ftfx_controller.o includes/fsl_ftfx_flash.o includes/fsl_io.o includes/fsl_uart.o includes/fsl_lpsci.o includes/fsl_assert.o includes/system_MKL46Z4.o

$(TARGET-C): $(OBJECTS-C)
	$(CC) $(LDFLAGS-C) $^ -o $@

flash-c: $(TARGET-C)
	openocd -f openocd.cfg -c "program main-c.elf verify reset exit"

LDFLAGS-IN-LINE=-Ofast -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main-inline.map,-Tlink.ld
#CFLAGS-IN-LINE=-I ./includes -Ofast -Wall -mthumb -mcpu=cortex-m0plus -DCPU_MKL46Z256VLL4

TARGET-INLINE=main-inline.elf
OBJECTS-INLINE= main-inline.o startup.o includes/board.o includes/clock_config.o includes/fsl_clock.o includes/fsl_common.o includes/fsl_debug_console.o includes/fsl_gpio.o includes/pin_mux.o includes/fsl_smc.o includes/fsl_log.o includes/fsl_str.o includes/fsl_ftfx_cache.o includes/fsl_ftfx_controller.o includes/fsl_ftfx_flash.o includes/fsl_io.o includes/fsl_uart.o includes/fsl_lpsci.o includes/fsl_assert.o includes/system_MKL46Z4.o

$(TARGET-INLINE): $(OBJECTS-INLINE)
	$(CC) $(LDFLAGS-IN-LINE) $^ -o $@

flash-inline: $(TARGET-INLINE)
	openocd -f openocd.cfg -c "program main-inline.elf verify reset exit"

#LDFLAGS-LINKED-ASM=
#CFLAGS-LINKED-ASM=






#TARGET=main.elf
#OBJECTS=main.o startup.o includes/board.o includes/clock_config.o includes/fsl_clock.o includes/fsl_common.o includes/fsl_debug_console.o includes/fsl_gpio.o includes/pin_mux.o includes/fsl_smc.o includes/fsl_log.o includes/fsl_str.o includes/fsl_ftfx_cache.o includes/fsl_ftfx_controller.o includes/fsl_ftfx_flash.o includes/fsl_io.o includes/fsl_uart.o includes/fsl_lpsci.o includes/fsl_assert.o includes/system_MKL46Z4.o

#all: $(TARGET)

#$(TARGET): $(OBJECTS)
#	$(CC) $(LDFLAGS) $^ -o $@

#flash: $(TARGET)
#	openocd -f openocd.cfg -c "program main.elf verify reset exit"

clean:
	$(RM) *.o *.map includes/*.o

cleanall:
	$(RM) *.o *.elf *.map includes/*.o


