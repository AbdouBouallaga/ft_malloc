SRC_DIR	= ./src/
INC_DIR	= ./inc/
LIB_DIR = $(CURDIR)/lib/
SRC	= $(SRC_DIR)malloc.c
OBJ	= $(SRC:.c=.o)
LFT = ./libft/libft.a
# CFLAGS = -fPIC -Wall -Wextra -Werror
CFLAGS = -fPIC
KERNEL = $(shell uname -s)
HOSTTYPE = $(shell uname -m)_$(KERNEL)

NAME = $(CURDIR)/lib/libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

CC = rm $(NAME) 2>/dev/null | cc

# ifeq ($(KERNEL),Darwin)
# 	COMP = clang -dynamiclib -std=gnu99 $(OBJ) $(LFT) -current_version 1.0 -compatibility_version 1.0 -o $(NAME)
# endif
# ifeq ($(KERNEL),Linux)
	COMP = gcc -o $(NAME) $(OBJ) $(LFT) $(CFLAGS) -shared
# endif

all: $(OBJ) $(NAME) $(LINK)

$(OBJ): $(SRC) $(INC_DIR)malloc.h

$(NAME): $(OBJ) $(LFT)
	mkdir -p $(LIB_DIR)
	$(COMP)

$(LINK): $(OBJ)
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

os:
	@echo $(KERNEL)

	