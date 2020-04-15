TARGETS=main.cpp game.cpp pokerAI.cpp cards.cpp
 
CC_CPP = g++
CC_C = gcc

CFLAGS = -Wall -Werror -g 

all :
	$(CC_CPP) $(CFLAGS) $(TARGETS) -o main

