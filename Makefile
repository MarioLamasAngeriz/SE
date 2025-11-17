CC=arm-none-eabi-gcc
LDFLAGS=-O2 -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main.map,-Tlink.ld
CFLAGS=$(INCLUDES) -O2 -Wall -mthumb -mcpu=cortex-m0plus -DCPU_MKL46Z256VLL4
INCLUDES = -I ./includes -I ./drivers -I ./includes/simpleChannel -I ./includes/doubleChannel

PWM=tpm_simple_pwm.elf
OBJECTS_PWM=tpm_simple_pwm.o startup.o drivers/fsl_tpm.o drivers/fsl_gpio.o drivers/fsl_clock.o drivers/fsl_common.o includes/simpleChannel/board.o includes/simpleChannel/pin_mux.o includes/simpleChannel/clock_config.o includes/system_MKL46Z4.o drivers/fsl_debug_console.o drivers/fsl_smc.o drivers/fsl_log.o drivers/fsl_str.o drivers/fsl_ftfx_cache.o drivers/fsl_ftfx_controller.o drivers/fsl_io.o drivers/fsl_uart.o drivers/fsl_lpsci.o drivers/fsl_assert.o

PWM_TWO=tpm_pwm_twochannel.elf
OBJECTS_PWM_TWO=tpm_pwm_twochannel.o startup.o drivers/fsl_tpm.o drivers/fsl_gpio.o drivers/fsl_clock.o drivers/fsl_common.o includes/doubleChannel/board.o includes/doubleChannel/pin_mux.o includes/doubleChannel/clock_config.o includes/system_MKL46Z4.o drivers/fsl_debug_console.o drivers/fsl_smc.o drivers/fsl_log.o drivers/fsl_str.o drivers/fsl_ftfx_cache.o drivers/fsl_ftfx_controller.o drivers/fsl_io.o drivers/fsl_uart.o drivers/fsl_lpsci.o drivers/fsl_assert.o


all: $(PWM) $(PWM_TWO) 

$(PWM): $(OBJECTS_PWM)
	$(CC) $(LDFLAGS) $^ -o $@

$(PWM_TWO): $(OBJECTS_PWM_TWO)
	$(CC) $(LDFLAGS) $^ -o $@

flash_pwm_two: $(PWM_TWO)
	openocd -f openocd.cfg -c "program $< verify reset exit"

flash_pwm: $(PWM)
	openocd -f openocd.cfg -c "program $< verify reset exit"

clean:
	$(RM) *.o drivers/*.o includes/*.o

cleanall:
	$(RM) *.o *.elf *.map drivers/*.o includes/*.o


