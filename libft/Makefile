# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: babdelka <babdelka@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/30 20:55:07 by babdelka          #+#    #+#              #
#    Updated: 2019/07/27 03:10:51 by babdelka         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRC = ft_putchar.c \
	  ft_putnbr.c \
	  ft_putstr.c 
BNR = ft_putchar.o \
	  ft_putnbr.o \
	  ft_putstr.o 
CFLAGS = -Wall -Werror -Wextra
all: $(NAME)

$(NAME): $(BNR)
	@ar rc  $(NAME) $(BNR)

clean:
	@rm -f $(BNR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
