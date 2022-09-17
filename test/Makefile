NAME = malloc_test
SRC = test.c
OBJ	= $(SRC:.c=.o)
# CFLAGS	=-Wall -Wextra -Werror
CFLAGS	=
MAL_DIR = ~/Desktop/42/mal/
MAL_LIB = $(MAL_DIR)libft_malloc.so
MAL_HEADER = $(MAL_DIR)inc


all:
	@gcc $(SRC) $(MAL_LIB) -I $(MAL_HEADER) -o $(NAME)
	./malloc_test

clean:
	-rm $(OBJ)

fclean: clean
	-rm $(NAME)

re: fclean all