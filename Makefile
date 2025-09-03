#Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
CXXFLAGS = -I./include 
# ****************************************************
# Targets needed to bring the executable up to date
 
TopWrapper: src/TopWrapper.o src/SubSystemIPC.o
	$(CC) $(CXXFLAGS) $(CFLAGS) -o TopWrapper src/TopWrapper.o src/SubSystemIPC.o

SubSystem: src/SubSystem.o src/SubSystemIPC.o
	$(CC) $(CXXFLAGS) $(CFLAGS) -o SubSystem src/SubSystem.o src/SubSystemIPC.o

FanController: src/FanController.o src/SubSystemIPC.o
	$(CC) $(CXXFLAGS) $(CFLAGS) -o FanController src/FanController.o src/SubSystemIPC.o

all: TopWrapper SubSystem FanController 

clean :
	-rm src/*.o $(objects) TopWrapper SubSystem FanController
