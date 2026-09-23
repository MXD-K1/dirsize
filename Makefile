CC := gcc
CFlags := -std=c17
LDFlags := -lm
DebugFlags := -g -fsanitize=address,undefined -fno-omit-frame-pointer

all: dirsize

dirsize: src/main.c
	mkdir -p build
	$(CC) $(CFlags) $(LDFlags) $^ -o build/$@

test-dirsize: src/main.c
	mkdir -p build
	$(CC) $(CFlags) $(DebugFlags) $(LDFlags) $^ -o build/$@

clean:
	rm -rf build/

.PHONY: all clean
