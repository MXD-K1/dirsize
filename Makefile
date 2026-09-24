CC := gcc
CFlags := -std=c17
LDFlags := -lm
DebugFlags := -g -fsanitize=address,undefined,leak -fno-omit-frame-pointer

SOURCES = main.c dir_utils.c path.c fs.c

DEBUG_OBJECTS = $(SOURCES:%.c=build/debug/%.o)
RELEASE_OBJECTS = $(SOURCES:%.c=build/release/%.o)

all: dirsize

dirsize: $(RELEASE_OBJECTS)
	mkdir -p build/release
	$(CC) $(CFlags) $^ $(LDFlags) -o build/release/$@

test-dirsize: $(DEBUG_OBJECTS)
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
