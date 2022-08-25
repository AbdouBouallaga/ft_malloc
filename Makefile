SRC_DIR=./src/
INC_DIR=./inc/

all: $(SRC_DIR)malloc.o
	gcc -g -I $(INC_DIR)malloc.h $(SRC_DIR)malloc.o
	rm $(SRC_DIR)malloc.o

clean:
	rm a.out

test: all
	./a.out

re: clean all
