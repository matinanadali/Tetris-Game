# Compiler
CC = gcc
EMCC = emcc

# Compile options
CFLAGS = -Wall -Werror -g
INCLUDE = -I../include

# Object files for native build
OBJS = ADTVector.o

# Object files for WebAssembly build
WASM_OBJS = ADTVector_wasm.o

# Targets
all: k08.a

wasm: k08_wasm.a

# Native build for k08.a
k08.a: $(OBJS)
	ar rcs $@ $(OBJS)

# WebAssembly build for k08_wasm.a
k08_wasm.a: $(WASM_OBJS)
	$(EMCC) $(WASM_OBJS) -o $@ -r

# Compile .o files for native build
%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

# Compile .o files for WebAssembly build
%_wasm.o: %.c
	$(EMCC) -c $(CFLAGS) $(INCLUDE) $< -o $@

# Clean up object files and libraries
clean:
	rm -f $(OBJS) $(WASM_OBJS) 
