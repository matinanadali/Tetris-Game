# paths
LIB = ../lib
INCLUDE = ../include

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall -Werror -g -I$(INCLUDE)
LDFLAGS = -lm

# Αρχεία .o
OBJS = state.o ./vec2.o $(LIB)/k08.a $(LIB)/libraylib.a

# Το εκτελέσιμο πρόγραμμα
EXEC = state

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

clean:
	rm -f $(OBJS) $(EXEC)

# Εκτέλεση αρχικού προγράμματος
run: $(EXEC)
	./$(EXEC) $(ARGS)

# Για να φτιάξουμε τα k08.a/libraylib.a τρέχουμε το make στο lib directory.
$(LIB)/%.a:
	$(MAKE) -C $(LIB) $*.a
