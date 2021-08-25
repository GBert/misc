
CC=g++

#CPPFLAGS=-g -O2 -Wall
#CPPFLAGS=-g -O0 -Wall -std=c++11
CPPFLAGS=-I. -g -O0 -Wall -std=c++11
LDFLAGS=-g
LIBS=-lpthread -ldl

TESTS= \
	testloco

all: $(TESTS)
	@./testloco

clean:
	rm -f $(TESTS) *.o

