# Paths
LIB = lib
INCLUDE = -I./include -I../include
MODULES = ./modules
PROGRAMS = ./programs

# Compilers
CC = gcc
EMCC = emcc

# Compile options
CFLAGS = -Wall -Werror -g $(INCLUDE)
LDFLAGS = -lm

# Object files for native build
OBJS = $(PROGRAMS)/game.o $(MODULES)/state.o $(PROGRAMS)/interface.o \
       $(MODULES)/vec2.o

# Object files for WebAssembly build
WASM_OBJS = $(PROGRAMS)/game_wasm.o $(MODULES)/state_wasm.o $(PROGRAMS)/interface_wasm.o \
            $(MODULES)/vec2_wasm.o

# Static libraries for WebAssembly
WASM_LIBS = $(LIB)/k08_wasm.a $(LIB)/libraylib_wasm.a  # Ensure these are WebAssembly-compatible

EXEC = game
WASM_EXEC = game.html

include $(LIB)/libraylib.mk

# Default target (native build with GCC)
all: $(EXEC)

# Target for native GCC build
$(EXEC): $(OBJS) $(LIB)/k08.a $(LIB)/libraylib.a
	$(CC) $(OBJS) $(LIB)/k08.a $(LIB)/libraylib.a -o $(EXEC) $(LDFLAGS)

# Target for WebAssembly build with emcc
wasm: $(WASM_EXEC)

$(WASM_EXEC): $(WASM_OBJS) $(WASM_LIBS)
	$(EMCC) $(WASM_OBJS) $(WASM_LIBS) -o $(WASM_EXEC) $(CFLAGS) $(LDFLAGS) -s WASM=1 -s USE_GLFW=3 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY

# Rule to compile .o files for native build
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# Rule to compile .o files for WebAssembly build
%_wasm.o: %.c
	$(EMCC) -c $(CFLAGS) $< -o $@

# Clean up object files and executables
clean:
	rm -f $(OBJS) $(WASM_OBJS) $(EXEC) $(WASM_EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

run-wasm: $(WASM_EXEC)
	emrun --no_browser --port 8080 .
	# Access the game at http://localhost:8080/$(WASM_EXEC)

# Build static libraries for native build
$(LIB)/%.a:
	$(MAKE) -C $(LIB) $*.a

# Ensure WebAssembly-compatible libraries exist
$(LIB)/%_wasm.a:
	@echo "Error: No rule to build $@. Ensure WebAssembly-compatible libraries are available."
