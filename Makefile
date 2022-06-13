# Name: Makefile
# Author: Neb Konstantin Viktorovich
EXECUTABLE := counters
BUILDDIR := build

CC = g++
CFLAGS = -c -Wall
LDFLAGS = -l sqlite3

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(BUILDDIR)/main.o $(BUILDDIR)/functions.o $(BUILDDIR)/list.o
	$(CC) $(LDFLAGS) $^ -o $(BUILDDIR)/$@

$(BUILDDIR)/main.o: main.cpp
	$(CC) $(CFLAGS) $< -o $@

$(BUILDDIR)/functions.o: functions.cpp functions.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILDDIR)/list.o: list.cpp list.h
	$(CC) $(CFLAGS) $< -o $@

clear:
	rm -f $(BUILDDIR)/*
