UV_PATH=$(shell pwd)/../libuv
UV_LIB=$(UV_PATH)/out/Debug/libuv.a
CFLAGS=-g -Wall -I$(UV_PATH)/include/
LIBS=

uname_S=$(shell uname -s)

ifeq (Darwin, $(uname_S))
CFLAGS+=-framework CoreServices
endif

ifeq (Linux, $(uname_S))
LIBS=-lrt -ldl -lm -pthread
endif

MAIN=main.c
EXECUTABLE=main

$(EXECUTABLE): $(UV_LIB)
	$(CC) $(CFLAGS) $^ -o $@


all: clean $(EXECUTABLE)

run: all
	@echo "\n\033[1;33m>>>\033[0m"
	./main
	@echo "\n\033[1;33m<<<\033[0m\n"
	make clean

runerr: all
	@echo "\n\033[1;33m>>>\033[0m"
	./main 1>/dev/null
	@echo "\n\033[1;33m<<<\033[0m\n"
	make clean

runout: all
	@echo "\n\033[1;33m>>>\033[0m"
	./main 2>/dev/null 
	@echo "\n\033[1;33m<<<\033[0m\n"
	make clean

.c.o:
	$(CC) $(CFLAGS) $< -o $@

$(UV_LIB):
	$(shell pushd pwd)
	$(shell cd $(UV_PATH))
	git clone https://git.chromium.org/external/gyp.git build/gyp
	./gyp_uv.py -f make
	make -C out
	$(shell cd -)

clean:
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
	rm -f main main.o

.PHONY: all clean
