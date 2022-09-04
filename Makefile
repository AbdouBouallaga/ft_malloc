SRC_DIR	= ./src/
INC_DIR	= ./inc/
LIB_DIR = ./lib/
SRC	= $(SRC_DIR)malloc.c
OBJ	= $(SRC:.c=.o)
LFT = ./libft/libft.a
CFLAGS = -fPIC -Wall -Wextra -Werror

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif


all: $(OBJ) $(LFT)
	gcc -o libft_malloc_$(HOSTTYPE).so $(OBJ) $(LFT) $(CFLAGS) -shared
	-ln -s libft_malloc_$(HOSTTYPE).so libft_malloc.so

$(LFT):
	make -C ./libft

clean:
	-rm $(SRC_DIR)malloc.o
	make -C ./libft clean
	make -C ./test clean

fclean: clean
	-rm libft_malloc.so
	-rm libft_malloc_$(HOSTTYPE).so
	make -C ./libft fclean
	make -C ./test fclean


tst: all
	make -C ./test
	./malloc_test

re: fclean all