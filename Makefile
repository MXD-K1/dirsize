CC := gcc
CFlags := -std=c17
LDFlags := -lm
DebugFlags := -g -fsanitize=address,undefined,leak -fno-omit-frame-pointer

all: dirsize

dirsize: build/release/main.o build/release/dir_utils.o
	mkdir -p build/release
	$(CC) $(CFlags) $^ $(LDFlags) -o build/release/$@

test-dirsize: build/debug/main.o build/debug/dir_utils.o
	mkdir -p build/debug
	$(CC) $(CFlags) $(DebugFlags) $^ $(LDFlags) -o build/debug/$@

build/release/%.o: src/%.c
	mkdir -p build/release
	$(CC) $(CFlags) $< -c -o $@

build/debug/%.o: src/%.c
	mkdir -p build/debug
	$(CC) $(CFlags) $(DebugFlags) $< -c -o $@

clean:
	rm -rf build/

.PHONY: all clean
