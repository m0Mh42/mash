FILES=./src/main.cpp
FLAGS=-lstdc++ -lm -Wall -Wextra -g -O4

OUTPUT=./bin/mash
TESTFILE=./bin/testfile

all:
	gcc $(FILES) $(FLAGS) -o $(OUTPUT)

run:
	$(OUTPUT) $(TESTFILE)

difficulty:
	$(OUTPUT) $(TESTFILE) 6

dirs:
	mkdir ./bin

