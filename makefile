CC = gcc
CFLAGS = -Wall -g
DEPS = Inventory.h FileHandler.h DataUnits.h Menu.h
LIBS = -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Inventory: Inventory.o FileHandler.o Menu.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o Inventory