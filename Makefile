EXECUTABLE = a.elf
TEST_EXECUTABLE = test.elf

LIB_OBJECTS = timer.o point.o vector.o
OBJECTS = snake.o $(LIB_OBJECTS)
TEST_OBJECTS = test.o $(LIB_OBJECTS)

CFLAGS = -Wall -Wextra -pedantic -std=c11 -lcurses

snake: $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(EXECUTABLE)
test: $(TEST_OBJECTS) test.o
	$(CXX) $(TEST_OBJECTS) $(CFLAGS) -o $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)
clean:
	rm *.o *.elf

