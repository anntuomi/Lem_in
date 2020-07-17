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
SRC = add_to_command_line.c count_group_size.c delete.c \
edmonds_karp_traverse.c ek_create_level.c find_best_routes.c find_edges.c \
free_memory.c ft_append.c get_branches_to_end.c handle_error.c handle_room.c \
input_and_output.c is_connected_to_end.c main.c move_ants.c order_routes.c \
rebuild_routes_dfs.c save_dfs_forks.c set_branches.c set_links.c solve.c
OBJECT = $(SRC:%.c=%.o)
LIBRARY = lib

all: $(LIBRARY) $(NAME)

$(LIBRARY):
			make -C libft/

$(NAME):
			gcc -Wall -Wextra -Werror -I $(INC) -c $(SRC)
			gcc -Wall -Wextra -Werror $(OBJECT) libft/libft.a -o $(NAME)

clean:
			/bin/rm -f $(OBJECT)
			make clean -C libft/

fclean:		clean
			/bin/rm -f $(NAME)
			make fclean -C libft/

re:			fclean all
