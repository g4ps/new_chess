CFLAGS = -g
SRCS = game.c   main.c   pieces.c utils.c
OBJS = $(SRCS:%.c=%.o)
LIBS = $(shell pkg-config libedit --libs)
INC = $(shell pkg-config libedit --cflags)
a.out : $(OBJS)
	gcc -g  $(LIBS) $(INC) $(OBJS) -o a.out
