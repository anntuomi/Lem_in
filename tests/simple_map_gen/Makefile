# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: atuomine <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/15 13:38:08 by atuomine          #+#    #+#              #
#    Updated: 2020/01/24 11:57:56 by atuomine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = gen
INC = ../../libft/includes
SRC = simple_map_gen.c
OBJECT = $(SRC:%.c=%.o)
LIBRARY = libft

all: $(LIBRARY) $(NAME)

$(LIBRARY):
			make -C ../../libft/

$(NAME):
			gcc -Wall -Wextra -Werror -I $(INC) -c $(SRC)
			gcc -Wall -Wextra -Werror $(OBJECT) ../../libft/libft.a -o $(NAME)

clean:
			/bin/rm -f $(OBJECT)
			make clean -C ../../libft/

fclean:		clean
			/bin/rm -f $(NAME)
			make fclean -C ../../libft/

re:			fclean all

.PHONY: all clean fclean re