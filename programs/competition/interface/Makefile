# paths
LIB = ../../../lib
INCLUDE = -I../../../include -I../include
MODULES = ../../../modules
INTERFACE = ../interface

# compiler
CC = gcc

# Compile options
CFLAGS = -Wall -Werror -g $(INCLUDE)
LDFLAGS = -lm

# Αρχεία .o
OBJS = game_screen.o welcome_screen.o ../state.o $(MODULES)/vec2.o $(LIB)/k08.a $(LIB)/libraylib.a 
# Εκτελέσιμα αρχεία
EXEC =

# Παράμετροι για δοκιμαστική εκτέλεση
ARGS =

# Παράμετροι της βιβλιοθήκης raylib
include $(LIB)/libraylib.mk

all: $(EXEC) $(ALT_EXEC)

# Αρχική υλοποίηση
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Εναλλακτική υλοποίηση
$(ALT_EXEC): $(ALT_OBJS)
	$(CC) $(ALT_OBJS) -o $(ALT_EXEC) $(LDFLAGS)

# Clean up object files and executables
clean:
	rm -f $(OBJS) $(ALT_OBJS) $(EXEC) $(ALT_EXEC)

# Εκτέλεση αρχικού προγράμματος
run: $(EXEC)
	./$(EXEC) $(ARGS)

# Εκτέλεση εναλλακτικής υλοποίησης
alt_run: $(ALT_EXEC)
	./$(ALT_EXEC) $(ARGS)

# Βιβλιοθήκη
$(LIB)/%.a:
	$(MAKE) -C $(LIB) $*.a
