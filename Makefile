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

NAME = lem-in
INC = libft/includes
SRC = ants_to_end.c del_dead_ends.c get_ants.c get_routes.c get_routes_to_end.c \
handle_error.c handle_room.c main.c set_links.c solve.c order_routes.c \
determine_used_routes.c initialize_arrays.c count_needed_routes.c
OBJECT = $(SRC:%.c=%.o)
LIBRARY = libft

all: $(LIBRARY) $(NAME)

$(LIBRARY):
			make -C libft/
		
$(NAME):
			gcc -I $(INC) -c $(SRC) 
			gcc $(OBJECT) libft/libft.a -o $(NAME)
	
clean:
			/bin/rm -f $(OBJECT)

fclean:		clean
			/bin/rm -f $(NAME)

re:			fclean all
