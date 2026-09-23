CC := gcc
CFlags := -std=c17
DebugFlags := -g -fsanitize=address,undefined -fno-omit-frame-pointer

all: dirsize

dirsize: src/main.c
	mkdir -p build
	$(CC) $(CFlags) $^ -o build/$@

test-dirsize: src/main.c
	mkdir -p build
	$(CC) $(CFlags) $(DebugFlags) $^ -o build/$@

clean:
	rm -rf build/

.PHONY: all clean
