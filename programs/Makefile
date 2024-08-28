# paths
LIB = ../lib
INCLUDE = -I ../include 
MODULES = ../modules
INTERFACE = ./interface

# compiler
CC = gcc

# Compile options
CFLAGS = -Wall -Werror -g $(INCLUDE)
LDFLAGS = -lm

OBJS = game.o $(MODULES)/state.o ./interface.o \
					$(MODULES)/vec2.o \
					$(LIB)/k08.a \
					$(LIB)/libraylib.a \

EXEC = game

ARGS =

include $(LIB)/libraylib.mk

all: $(EXEC) $(ALT_EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(ALT_OBJS) $(EXEC) $(ALT_EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

$(LIB)/%.a:
	$(MAKE) -C $(LIB) $*.a
