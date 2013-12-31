CFLAGS=-g -O2 -Wall -I../libuv/include ../libuv/out/Debug/libuv.a 

all: clean main tick

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

main: 
	$(CC) $(CFLAGS) main.c -o main

clean:
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
	rm -f main main.o tick tick.o

.PHONY: all clean
