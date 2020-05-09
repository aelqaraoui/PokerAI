TARGETS=main.cpp interface.cpp controller.cpp data.cpp pokerAI.cpp cards.cpp
 
CC_CPP = g++
CC_C = gcc

CFLAGS = -Wall -Werror -g -lSDL2  -lSDL2_image -lSDL2_ttf

all :
	$(CC_CPP) $(CFLAGS) $(TARGETS) -o main

