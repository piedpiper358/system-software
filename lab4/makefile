#.PHONY clean exec

CC = gcc
#CC = clang
CFLAGS = -ggdb  -std=gnu99 -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE
FL = -Wall -Werror


#INCLUDE_DIRS = -iquote include

ANSI = -ansi -pedantic

LIBS= -lm

SOURCES =  wc.c 

OBJECTS = wc


all: wc
	
wc: $(SOURCES) 
	$(CC)  $(CFLAGS) $(INCLUDE_DIRS) -o $(OBJECTS) $(SOURCES) $(LIBS) 

clean:
	rm -rf $(OBJECTS)

exec:
	./build/wc 





