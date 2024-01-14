CC = g++
CFLAGS = -flto -std=c++20 -Ofast -funroll-loops -finline-functions

all: runme

runme: runme.o
	$(CC) $(CFLAGS) -o $@ $^

main: main.o
	$(CC) $(CFLAGS) -o $@ $^


runme.o: edit.cpp includes.h
	$(CC) $(CFLAGS) -c $< -o $@


main.o: main.cpp includes.h data.h dim.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f runme.o main.o

test: runme
	@echo "Running test on [1 0 0] [0 1 0] [0 0 1], Expected value: 1"
	@LASTLINE=$$(./runme [1 0 0] [0 1 0] [0 0 1] | tail -n 1);\
	if [ "$$LASTLINE" = "1" ]; then \
		echo "Test passed"; \
	else \
		echo "Test Failed"; \
	fi

