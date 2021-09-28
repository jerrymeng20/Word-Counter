OBJS	= main.o map.o
SOURCE	= main.c map.c
HEADER	= 
OUT	= wordcounter
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 

map.o: map.c
	$(CC) $(FLAGS) map.c 


clean:
	rm -f $(OBJS) $(OUT)
