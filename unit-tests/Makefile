CC=g++ -std=c++11
CFLAGS=-c -O2 -Wall
SRCDIR=unit-tests/src
BINDIR=unit-tests/bin
TARGET=main-tests

all: $(TARGET)
	
$(TARGET): main-tests.o calculator.o
	$(CC) $(BINDIR)/main-tests.o $(BINDIR)/BasicCalculator.o -o $(BINDIR)/$(TARGET)
main-tests.o:
	$(CC) $(CFLAGS) $(SRCDIR)/main-tests.cpp -o $(BINDIR)/main-tests.o
calculator.o:
	$(CC) $(CFLAGS) src/BasicCalculator.cpp -o $(BINDIR)/BasicCalculator.o
clean:
	rm -rf $(BINDIR)/*.o $(BINDIR)/$(TARGET)
