#!/bin/sh

set -xe

CFLAGS="-Wall -Werror -std=c++11 -pedantic -ggdb"
LIBS=""

CFILES="src/*.cpp src/*.hpp"


g++ $CFLAGS $CFILES -o bin/mort $LIBS
