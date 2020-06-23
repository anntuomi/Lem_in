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
#SRC = count_max_path_count.c determine_used_routes.c find_next_unique.c \
free_memory.c ft_append.c get_branches_to_end.c handle_room.c \
initialize_variables.c input_and_output.c is_connected_to_end.c main.c \
move_ants.c order_routes.c set_branches.c set_links.c solve.c
SRC = free_memory.c get_branches_to_end.c handle_room.c input_and_output.c \
is_connected_to_end.c main.c order_routes.c set_branches.c set_links.c
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
			make clean -C libft/

fclean:		clean
			/bin/rm -f $(NAME)
			#make fclean -C libft/

re:			fclean all