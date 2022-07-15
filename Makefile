CFLAGS=-Wall -g

all: clean build

build: main

clean:
	rm -rf ./main

run: 
	./main
