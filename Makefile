EXECUTABLE = snake
TEST_EXECUTABLE = test.elf

INSTALL_DIR = /usr/local/bin/
INSTALL_DIR_LOCAL = ~/.local/bin/
INSTALL_COMMAND = install

LIB_OBJECTS = timer.o point.o vector.o snake.o error.o highscore.c
OBJECTS = main.o $(LIB_OBJECTS)
TEST_OBJECTS = test.o $(LIB_OBJECTS)

CFLAGS = -Wall -Wextra -pedantic -std=gnu11 -lcurses

MAKE = $(CC) $(OBJECTS) $(CFLAGS) -o $(EXECUTABLE)


debug: CFLAGS += -Og -g3
debug: executable

release: CFLAGS += -O2 --lto
release: executable

executable: $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(EXECUTABLE)

test: $(TEST_OBJECTS) test.o
	$(CC) $(TEST_OBJECTS) $(CFLAGS) -o $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

install_local: INSTALL_DIR = $(INSTALL_DIR_LOCAL)
install_local: install

uninstall_local: INSTALL_DIR = $(INSTALL_DIR_LOCAL)
uninstall_local: uninstall

install:release
install:
	$(INSTALL_COMMAND) $(EXECUTABLE) $(INSTALL_DIR)

uninstall:
	rm $(INSTALL_DIR)/$(EXECUTABLE)

clean:
	rm -f *.o *.elf $(EXECUTABLE)

