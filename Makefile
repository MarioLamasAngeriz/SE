CC=arm-none-eabi-gcc
LDFLAGS=-O2 -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main.map,-Tlink.ld
CFLAGS=$(INCLUDES) -O2 -Wall -mthumb -mcpu=cortex-m0plus -DCPU_MKL46Z256VLL4

INCLUDES=-I. -I./includes/ -I./freertos/ -I./freertos/include/ -I./freertos/include/private/ -I./freertos/portable/GCC/ARM_CM0/ -I./freertos/portable/low_power_tickless/ -I./freertos/portable/MemMang/ 

TARGET=main.elf
OBJECTS=main.o startup.o $(OBJECTS_RTOS) $(OBJECTS_LED) 

OBJECTS_RTOS=./freertos/list.o ./freertos/queue.o ./freertos/tasks.o ./freertos/portable/MemMang/heap_2.o ./freertos/portable/GCC/ARM_CM0/port.o
OBJECTS_LED=includes/fsl_gpio.o includes/fsl_clock.o includes/fsl_common.o includes/board.o includes/led/pin_mux.o includes/clock_config.o includes/system_MKL46Z4.o includes/fsl_debug_console.o includes/fsl_smc.o includes/fsl_log.o includes/fsl_str.o includes/fsl_ftfx_cache.o includes/fsl_ftfx_controller.o includes/fsl_io.o includes/fsl_uart.o includes/fsl_lpsci.o includes/fsl_assert.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

flash: $(TARGET)
	openocd -f openocd.cfg -c "program $< verify reset exit"

clean:
	$(RM) *.o *.map

cleanall:
	$(RM) *.o *.elf *.map $(OBJECTS)



