FLAGS = -g
LIBS = $(shell pkg-config libedit --libs)
INC = $(shell pkg-config libedit --cflags)
a.out : main.c
	gcc -g  $(LIBS) $(INC) main.c -o a.out
