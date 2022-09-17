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
	clang -dynamiclib -std=gnu99 $(OBJ) $(LFT) -current_version 1.0 -compatibility_version 1.0 -fvisibility=hidden -o $(NAME)
	

$(LINK):
	-ln -s $(NAME) $(LINK)
	-security import cert/malloc.cer -k ~/Library/Keychains/login.keychain-db
	codesign --sign malloc libft_malloc.so
	codesign -vvvv libft_malloc.so

$(LFT):
	make -C ./libft

clean:
	-rm $(SRC_DIR)malloc.o

fclean: clean
	-rm $(NAME)
	-rm $(LINK)
	make -C ./libft fclean

re: fclean all
