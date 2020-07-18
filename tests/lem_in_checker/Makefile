# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jmetelin <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/07/13 14:03:36 by jmetelin          #+#    #+#              #
#    Updated: 2020/07/13 14:03:37 by jmetelin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIB = libft
NAME = lem-in-checker
INCS = libft/includes
SRCS = main.c handle_error.c get_rooms.c set_links.c
OBJS = $(SRCS:.c=.o)

all: $(LIB) $(NAME)

$(LIB):
			make -C libft/ re

$(NAME):
			gcc -I $(INCS) -c $(SRCS)
			gcc $(OBJS) libft/libft.a -o $(NAME)

clean:
			/bin/rm -f $(OBJS)
			make -C libft/ clean

fclean:		clean
			/bin/rm -f $(NAME)
			make -C libft/ fclean

re:			fclean all

.PHONY: all clean fclean re libft