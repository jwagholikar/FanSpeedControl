#Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
TopWrapper: TopWrapper.o SubSystemIPC.o
	$(CC) $(CFLAGS) -o TopWrapper TopWrapper.o SubSystemIPC.o

SubSystem: SubSystem.o SubSystemIPC.o
	$(CC) $(CFLAGS) -o SubSystem SubSystem.o SubSystemIPC.o

FanController: FanController.o SubSystemIPC.o
	$(CC) $(CFLAGS) -o FanController FanController.o SubSystemIPC.o

all: TopWrapper SubSystem FanController 

clean :
	-rm *.o $(objects) TopWrapper SubSystem FanController
