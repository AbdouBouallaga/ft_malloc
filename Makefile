SRC_DIR	= ./src/
INC_DIR	= ./inc/
LIB_DIR = ./lib/
SRC	= $(SRC_DIR)malloc.c
OBJ	= $(SRC:.c=.o)
CFLAGS = -fPIC -Wall -Wextra -Werror

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

all: $(OBJ)
	gcc -o libft_malloc_$(HOSTTYPE).so $(SRC_DIR)malloc.c $(CFLAGS) -shared
	-ln -s libft_malloc_$(HOSTTYPE).so libft_malloc.so

clean:
	-rm $(SRC_DIR)malloc.o

fclean: clean
	-rm libft_malloc.so
	-rm libft_malloc_$(HOSTTYPE).so

re: fclean all