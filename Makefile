NAME := mort
CFLAGS := -I src/incl/ -Wall -Werror -std=c++14 -pedantic
CC := g++

cpp_source_files := $(shell find src/impl/ -name *.cpp)

.PHONY: build_debug

build_debug:
	echo $(cpp_source_files)
	mkdir -p build/debug
	$(CC) $(cpp_source_files) $(CFLAGS) -ggdb -o build/debug/$(NAME).bin

build_release:
	echo $(cpp_source_files)
	mkdir -p build/release
	$(CC) $(cpp_source_files) $(CFLAGS) -s -o build/release/$(NAME).bin

run:
	./build/debug/$(NAME).bin $(file)

run_release:
	./build/release/$(NAME).bin $(file)

clean:
	rm -rf build
