CC = g++
CFLAGS = -I"./" -fPIC -DPIC -O2 -nostdlib
CXXFLAGS = -I"./" -fPIC -DPIC -O2 -nostdlib
LIBS = -lpigpio
DEPS = hashmap.h module.h param.h var.h config.h gpio.h ds18b20 lcd1.h scd30.h ssd1306.h bh1750.h ads1x15
OBJ =  hashmap.o param.o main.o gpio.o ds18b20.o lcd1.o scd30.o ssd1306.o bh1750.o ads1x15.o

%.o: %.c %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)
	
libSmallBasicPIGPIO.so : $(OBJ)
	$(CC) -shared -O2 -nostdlib -o $@ $^ $(CFLAGS) $(LIBS)
	cp libSmallBasicPIGPIO.so ../bin/

clean:
	rm *.o
	rm libSmallBasicPIGPIO.so
	rm ../bin/libSmallBasicPIGPIO.so
