EXECUTABLE = a.elf
TEST_EXECUTABLE = test.elf

LIB_OBJECTS = timer.o point.o vector.o snake.o error.o
OBJECTS = main.o $(LIB_OBJECTS)
TEST_OBJECTS = test.o $(LIB_OBJECTS)

CFLAGS = -Wall -Wextra -pedantic -std=c11 -lcurses

MAKE = $(CC) $(OBJECTS) $(CFLAGS) -o $(EXECUTABLE)


debug: CFLAGS += -Og
debug: executable
release: CFLAGS += -O2 --lto
release: executable
executable: $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(EXECUTABLE)
test: $(TEST_OBJECTS) test.o
	$(CC) $(TEST_OBJECTS) $(CFLAGS) -o $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)
clean:
	rm -f *.o *.elf

