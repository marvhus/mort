#!/bin/sh

set -xe

CFLAGS="-Wall -Werror -ggdb `llvm-config-12 --cxxflags --ldflags`"
LIBS="`llvm-config-12 --libs`"

CFILES="src/*.cpp src/lexer/*.cpp src/lexer/*.hpp"


g++ $CFLAGS $CFILES -o bin/mort $LIBS
