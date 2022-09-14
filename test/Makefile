NAME = malloc_test
SRC = test.c
OBJ	= $(SRC:.c=.o)
# CFLAGS	=-Wall -Wextra -Werror
CFLAGS	=
MALLOC_LIB = ../libft_malloc.so


all: $(OBJ)
	@gcc $(CFLAGS) $(OBJ) $(MALLOC_LIB) ../libft/libft.a -o $(NAME)
	./malloc_test

clean:
	-rm $(OBJ)

fclean: clean
	-rm $(NAME)

re: fclean all