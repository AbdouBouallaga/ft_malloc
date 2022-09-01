NAME = malloc_test
SRC = test.c
OBJ	= $(SRC:.c=.o)
CFLAGS	=-Wall -Wextra -Werror
MALLOC_LIB = libft_malloc.so


all: ./test/$(OBJ)
	gcc $(CFLAGS) ./test/$(OBJ) $(MALLOC_LIB) -o $(NAME)

clean:
	-rm ./test/$(OBJ)

fclean: clean
	-rm $(NAME)

re: fclean all