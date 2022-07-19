CC=g++
CFLAGS=-I "./"
LIBS=-lpigpio
DEPS = hashmap.h module.h param.h var.h config.h gpio.h ds18b20.h lcd1.h scd30.h ssd1306.h bh1750.h ads1x15.h
OBJ =  hashmap.o param.o main.o gpio.o ds18b20.o lcd1.o scd30.o ssd1306.o bh1750.o ads1x15.o

%.o: %.c %.cpp $(DEPS)
	$(CC) -shared -fPIC -DPIC -O2 -nostdlib -o $@ $< $(CFLAGS) $(LIBS)
	
libSmallBasicPIGPIO.so : $(OBJ)
	$(CC) -shared -fPIC -DPIC -O2 -nostdlib -o $@ $^ $(CFLAGS) $(LIBS)
	cp libSmallBasicPIGPIO.so ../bin/

clean:
	rm *.o
	rm libSmallBasicPIGPIO.so
	rm ../bin/libSmallBasicPIGPIO.so
