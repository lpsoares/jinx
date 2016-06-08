#
# makefile for testglove, a simple console app to demonstrate reading the
# glove
#
# Copyright (C) 2000, 5DT <Fifth Dimension Technologies>
#

CPP = g++296
CFLAGS = -Wall
BIN = testglove
LIBS = -lfglove
OBJFILES = testglove.o ServerSocket.o Socket.o

default: $(OBJFILES)
	$(CPP) -o $(BIN) $(OBJFILES) $(LIBS)

clean:
	rm -f $(BIN) $(OBJFILES) *~ core #*

%.o: %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

