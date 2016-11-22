CFLAGS= -std=gnu99
CC=gcc $(CFLAGS)
OBJECTS=sync.o main.o

mylocks: $(OBJECTS) 
	$(CC) $(OBJECTS) -lpthread -o mylocks
clean:
	rm -f mylocks $(OBJECTS) 
run: mylocks
	./mylocks
