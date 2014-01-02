UV_PATH=$(shell pwd)/../libuv
UV_LIB=$(UV_PATH)/out/Debug/libuv.a

CFLAGS=-g -O2 -Wall -I$(UV_PATH)/include 
LIBS=

uname_S=$(shell uname -s)

ifeq (Darwin, $(uname_S))
CFLAGS+=-framework CoreServices
SHARED_LIB_FLAGS=-bundle -undefined dynamic_lookup -o libhello.dylib
endif

ifeq (Linux, $(uname_S))
LIBS=-lrt -ldl -lm -pthread
SHARED_LIB_FLAGS=-shared -Wl,-soname,libhello.so -o libhello.so
PLUGIN_EXE_FLAGS=-Wl,-export-dynamic
endif

all: clean plugin

run: all
	@echo "\n\033[1;33m>>>\033[0m"
	plugin libhello.dylib
	@echo "\n\033[1;33m<<<\033[0m\n"
	make clean

plugin: ./*.c
	gcc $(CFLAGS) $(PLUGIN_EXE_FLAGS) -o plugin main.c plugin.c $(UV_LIB) $(LIBS)
	gcc -g -Wall -c -fPIC -o hello.o hello.c
	gcc $(SHARED_LIB_FLAGS) hello.o

clean:
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
	rm -f main main.o plugin plugin.o hello hello.o libhello.*

.PHONY: all clean run
