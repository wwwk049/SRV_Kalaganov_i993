CC = g++
CFLAGS =
TARGETS = main test

all: $(TARGETS)

debug: CFLAGS += -g
debug: $(TARGETS)

main: main.cpp
	$(CC) $(CFLAGS) -o main main.cpp

test: test.cpp
	$(CC) $(CFLAGS) -o test test.cpp

clean:
	rm -f $(TARGETS)
