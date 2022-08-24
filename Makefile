SRC_DIR=./src/
INC_DIR=./inc/

all: $(SRC_DIR)malloc.o
	gcc -I $(INC_DIR)malloc.h $(SRC_DIR)malloc.o
	rm $(SRC_DIR)malloc.o

clean:
	rm a.out

re: clean all