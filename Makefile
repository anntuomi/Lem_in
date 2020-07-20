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
rebuild_routes_dfs.c save_dfs_forks.c set_branches.c set_links.c solve.c \
optimize_path_count.c
OBJECT = $(SRC:%.c=%.o)
LIBRARY = libft/libft.a
CHECKER = lem-in-checker
CHECKSRC = tests/lem_in_checker/lem_in_checker_main.c \
tests/lem_in_checker/lem_in_checker_handle_error.c \
tests/lem_in_checker/lem_in_checker_get_rooms.c \
tests/lem_in_checker/lem_in_checker_set_links.c
CHECKEROBJ = lem_in_checker_main.o \
lem_in_checker_handle_error.o \
lem_in_checker_get_rooms.o \
lem_in_checker_set_links.o
GEN = gen
GENSRC = tests/simple_map_gen/simple_map_gen.c
GENOBJ = simple_map_gen.o
all: $(LIBRARY) $(NAME)

$(LIBRARY):
			make -C libft/

$(NAME):
			gcc -Wall -Wextra -Werror -I $(INC) -c $(SRC)
			gcc -Wall -Wextra -Werror $(OBJECT) $(LIBRARY) -o $(NAME)

clean:
			/bin/rm -f $(OBJECT)
			/bin/rm -f $(CHECKEROBJ)
			/bin/rm -f $(GENOBJ)
			make -C libft/ clean

fclean:		clean
			make -C libft/ fclean
			/bin/rm -f $(NAME)
			/bin/rm -f $(GEN)
			/bin/rm -f $(CHECKER)

re:			fclean all

bonus: 		$(LIBRARY) $(CHECKER) $(GEN)

$(CHECKER):
			gcc -Wall -Wextra -Werror -I $(INC) -c $(CHECKSRC)
			gcc -Wall -Wextra -Werror $(CHECKEROBJ) $(LIBRARY) -o $(CHECKER)

$(GEN):
			gcc -Wall -Wextra -Werror -I $(INC) -c $(GENSRC)
			gcc -Wall -Wextra -Werror $(GENOBJ) $(LIBRARY) -o $(GEN)

bonus_re: 	fclean bonus

.PHONY: all clean fclean re bonus bonus_re
