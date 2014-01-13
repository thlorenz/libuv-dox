UV_PATH=$(shell pwd)/../libuv
HTTP_PARSER_PATH=$(shell pwd)/../http-parser

UV_LIB=$(UV_PATH)/out/Debug/libuv.a
HTTP_PARSER=$(HTTP_PARSER_PATH)/http_parser.o

CFLAGS=-g -O2 -Wall -I$(UV_PATH)/include/ -I$(HTTP_PARSER_PATH)/
LIBS=

MAIN=main.c
MAIN_OBJECT=main.o
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

all: clean $(EXECUTABLE)

run: all
	@echo "\n\033[1;33m>>>\033[0m"
	./$(EXECUTABLE)
	@echo "\n\033[1;33m<<<\033[0m\n"
	make clean

.c.o:
	$(CC) $(CFLAGS) $< -o $@

$(EXECUTABLE): $(HTTP_PARSER) $(UV_LIB) $(MAIN) 
	$(CC) $(CFLAGS) $^ -o $@

$(UV_LIB):
	cd $(UV_PATH) && \
	test -d ./build/gyp || (mkdir -p ./build && git clone https://git.chromium.org/external/gyp.git ./build/gyp) && \
	./gyp_uv.py -f make && \
	$(MAKE) -C ./out

$(HTTP_PARSER):
	cd $(HTTP_PARSER_PATH) \
	make -C  

clean:
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
	rm -f $(EXECUTABLE) $(MAIN_OBJECT) $(REQUEST_PARSER) $(RESOLVE_RESOURCE) $(PIPE_FILE)


.PHONY: all clean
