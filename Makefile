CC=arm-none-eabi-gcc
LDFLAGS=-O2 -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main.map,-Tlink.ld
CFLAGS=$(INCLUDES) -O2 -Wall -mthumb -mcpu=cortex-m0plus
INCLUDES=-I ./includes/ -I ./freertos/

TARGET=main.elf
OBJECTS=main.o startup.o $(OBJECTS_LCD) # $(OBJECTS_RTOS)

OBJECTS_RTOS=./freertos/list.o ./freertos/queue.o ./freertos/tasks.o ./freertos/heap_2.o ./freertos/port.o ./freertos/timers.o

OBJECTS_LCD=./includes/lcd.o

all: $(TARGET) flash

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

flash: $(TARGET)
	openocd -f openocd.cfg -c "program $< verify reset exit"

clean:
	$(RM) *.o *.map ./freertos/*.o

cleanall:
	$(RM) *.o *.elf *.map $(OBJECTS)

