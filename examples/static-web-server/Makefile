UV_PATH=$(shell pwd)/../libuv
HTTP_PARSER_PATH=$(shell pwd)/../http-parser

UV_LIB=$(UV_PATH)/out/Debug/libuv.a
HTTP_PARSER=$(HTTP_PARSER_PATH)/http_parser.o

CFLAGS=-g -Wall -I$(UV_PATH)/include/ $(UV_LIB) $(HTTP_PARSER)
LIBS=

MAIN=main.c
MAIN_OBJECTS=$(MAIN:.c=.o)
EXECUTABLE=main

REQUEST_PARSER_SRC=request-parser.c
REQUEST_PARSER=request-parser.o
CFLAGS+=$(REQUEST_PARSER_SRC)

RESOLVE_RESOURCE_SRC=resolve-resource.c
RESOLVE_RESOURCE=resolve-resource.o
CFLAGS+=$(RESOLVE_RESOURCE_SRC)

PIPE_FILE_SRC=pipe-file.c
PIPE_FILE=pipe-file.o
CFLAGS+=$(PIPE_FILE_SRC)

uname_S=$(shell uname -s)

ifeq (Darwin, $(uname_S))
CFLAGS+=-framework CoreServices
endif

ifeq (Linux, $(uname_S))
LIBS=-lrt -ldl -lm -pthread
PLUGIN_EXE_FLAGS=-Wl,-export-dynamic
endif

all: clean $(UV_LIB) $(HTTP_PARSER) $(EXECUTABLE)

run: all
	@echo "\n\033[1;33m>>>\033[0m"
	./$(EXECUTABLE)
	@echo "\n\033[1;33m<<<\033[0m\n"
	make clean

.c.o:
	$(CC) $(CFLAGS) $(UV_LIB) $< -o $@ 

$(UV_LIB):
	make -C ../libuv/out

$(HTTP_PARSER):
	make -C ../http-parser http_parser.o

clean:
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
	rm -f $(EXECUTABLE) $(MAIN_OBJECTS) $(REQUEST_PARSER_OBJECT) $(RESOLVE_RESOURCE)


.PHONY: all clean
