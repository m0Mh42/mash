FILES=./src/main.cpp
FLAGS=-lm -Wall -Wextra -g -O4

OUTPUT=./bin/mash
TESTFILE=./bin/testfile

all:
	g++ $(FILES) $(FLAGS) -o $(OUTPUT)

run:
	$(OUTPUT) $(TESTFILE)

difficulty:
	$(OUTPUT) $(TESTFILE) 6

stresstest:
	$(OUTPUT) $(TESTFILE) 63

dirs:
	mkdir ./bin

