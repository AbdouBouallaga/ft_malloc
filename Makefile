SRC_DIR	= ./src/
INC_DIR	= ./inc/
LIB_DIR = $(CURDIR)/lib/
SRC	= $(SRC_DIR)malloc.c
OBJ	= $(SRC:.c=.o)
LFT = ./libft/libft.a
CFLAGS = -fPIC -Wall -Wextra -Werror

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = $(CURDIR)/lib/libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

CC = rm $(NAME) 2>/dev/null | cc

all: $(OBJ) $(NAME) $(LINK)

$(NAME): $(LFT)
	mkdir -p $(LIB_DIR)
	gcc -o $(NAME) $(OBJ) $(LFT) $(CFLAGS) -shared

$(LINK):
	-ln -s $(NAME) $(LINK)

$(LFT):
	make -C ./libft

clean:
	-rm $(SRC_DIR)malloc.o

fclean: clean
	-rm $(NAME)
	-rm $(LINK)
	make -C ./libft fclean

re: fclean all
