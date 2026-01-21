CC=arm-none-eabi-gcc
CFLAGS=-I ./includes -I ./drivers -O2 -Wall -mthumb -mcpu=cortex-m0plus -DCPU_MKL46Z256VLL4
LDFLAGS=-O2 -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map,main.map,-Tlink.ld

TARGET=main.elf
OBJECTS=main.o startup.o 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ 

flash: $(TARGET)
	openocd -f openocd.cfg -c "program $< verify reset exit"

clean:
	$(RM) *.o *.map includes/*.o drivers/*.o

cleanall:
	$(RM) *.o *.elf *.map includes/*.o drivers/*.o

