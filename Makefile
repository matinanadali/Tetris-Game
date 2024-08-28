# paths
LIB = lib
INCLUDE = -I /include ../include
MODULES = ./modules
PROGRAMS = ./programs

# compiler
CC = gcc

# Compile options
CFLAGS = -Wall -Werror -g $(INCLUDE)
LDFLAGS = -lm

# Αρχεία .o
OBJS = $(PROGRAMS)/game.o $(MODULES)/state.o $(PROGRAMS)/interface.o \
					$(MODULES)/vec2.o \
					$(LIB)/k08.a \
					$(LIB)/libraylib.a \

EXEC = game

include $(LIB)/libraylib.mk

all: $(EXEC) $(ALT_EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Clean up object files and executables
clean:
	rm -f $(OBJS) $(ALT_OBJS) $(EXEC) $(ALT_EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

$(LIB)/%.a:
	$(MAKE) -C $(LIB) $*.a
