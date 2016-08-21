CC=g++ -std=c++11
CFLAGS=-c -O2 -Wall
SRCDIR=src
BINDIR=bin
TARGET=calculator

all: $(TARGET)
	
$(TARGET): main.o calculator.o
	$(CC) $(BINDIR)/main.o $(BINDIR)/BasicCalculator.o -o $(BINDIR)/$(TARGET)
main.o:
	$(CC) $(CFLAGS) $(SRCDIR)/main.cpp -o $(BINDIR)/main.o
calculator.o:
	$(CC) $(CFLAGS) $(SRCDIR)/BasicCalculator.cpp -o $(BINDIR)/BasicCalculator.o
unit-tests: calculator.o
	make -f unit-tests/Makefile

clean:
	rm -rf $(BINDIR)/*.o $(BINDIR)/$(TARGET)
