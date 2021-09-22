#.PNY clean exec

CC = gcc
#CC = clang
CFLAGS =  -std=gnu99
FL = -Wall -Werr -ggdb -D_FILEFFSET_BITS=64 -D_GNU_URCE -pthread


#INCLUDE_DIRS = -iqte include

ANSI = -ansi -pedantic

LIBS= -lm

#URCES =  wc.c 

OBJECTS = 1client 1server 4 10



all:  1client 1server 4 10
	
1client: 1client.c
	$(CC) $(CFLAGS) -o 1client 1client.c

1server: 1server.c
	$(CC) $(CFLAGS) -o 1server 1server.c

4: 4.c 
	$(CC) $(CFLAGS)  -o 4  4.c  -lrt 

4: 4.c 
	$(CC) $(CFLAGS)  -o 4  4.c  -lrt -pthread

10: 10.c 
	$(CC) $(CFLAGS)  -o 10 10.c 

extra:
	$(CC)  $(CFLAGS) $(INCLUDE_DIRS)  1 1.c $(LIBS) 

clean:
	rm -rf * $BJECTS)

exec:
	./1 





