CFLAGS = -g
SRCS = game.c   main.c   pieces.c utils.c list.c
OBJS = $(SRCS:%.c=%.o)
LIBS = $(shell pkg-config libedit --libs)
INC = $(shell pkg-config libedit --cflags)
a.out : $(OBJS) chess.h
	gcc -g  $(LIBS) $(INC) $(OBJS) -o a.out
