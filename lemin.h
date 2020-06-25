/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemin.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:03:28 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:03:29 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEMIN_H
# define LEMIN_H

# include <unistd.h>
# include <stdlib.h>
# include "libft.h"
# include "get_next_line.h"
# include <stdio.h>

# define INT_MIN -2147483648
# define INT_MAX 2147483647
# define START 0
# define NORMAL 1
# define END 2
# define NOT_ROOM 0
# define IS_ROOM 1

typedef struct	s_input
{
	char			*line;
	struct s_input	*next;
}				t_input;

typedef struct	s_path
{
	void			*room;
	struct s_path	*next;
}				t_path;

typedef struct	s_room
{
	char			*name;
	int				x;
	int				y;
	int				type;
	int				id;
	int				ant_count;
	int				start_connection;
	t_path			*paths;
	struct s_room	*prev;
	struct s_room	*next;
}				t_room;

typedef struct	s_fork
{
	t_room			*from;
	t_room			*to;
	struct s_fork	*next;
}				t_fork;

typedef struct	s_route
{
	t_fork			*forks;
	int				rooms;
	t_room			*room;
	t_room			*prev;
	t_fork			*fork;
	struct s_route	*next;
}				t_route;

typedef struct	s_branch
{
	t_route			*route;
	t_route			**array;
	int				routes;
	struct s_branch	*next;
}				t_branch;

typedef struct	s_farm
{
	int				ant_count;
	t_route			**ants;
	t_room			*rooms;
	t_room			*start;
	t_room			*end;
	t_branch		*branches;
	int				branch_count;
	t_branch		**ordered;
	int				path_count;
}				t_farm;

typedef struct	s_var
{
	int				least_moves;
	int				moves;
	int				current_path_count;
	int				new_path_count;
	int				max_path_count;
}				t_var;

int				determine_room_type(char *line);
void			create_room_list(t_room **head, char **line, t_input **input);
void			ft_delete(char **array);
void			handle_error(void);
void			set_input(t_input **input, char *line, int room);
void			set_links(char *line, t_room *room, t_input **input);
/*char			*ft_append(char const *s1, char const *s2, int *len, char c);
char			*move_ants(t_route **ants);
int				calculate_moves(t_routes **routes, int path_count,
				int ant_count);
int				count_max_path_count(t_routes *routes, t_routes *shortest);
t_routes		**determine_used_routes(t_farm *farm);
t_routes		*find_next_unique(t_routes **ordered, t_routes **used_routes);
void			free_memory(t_farm farm);
void			initialize_variables(int path_count, t_routes ***used_routes,
				t_routes ***tmp, t_var *var);
void			print_input(t_input *input);
void			print_output(char **output, int *len);
void			solve(t_farm farm);*/
int				is_connected_to_end(t_room *room, t_room **end, int *fork);
int				is_unvisited(t_room *room, t_room *prev, t_fork *fork);
t_branch		*get_branches_to_end(t_room *start);
t_route			*get_fork_route(t_route *before_fork, t_room *room);
void			*del_empty_branches(t_branch **head);
void			del_route(t_branch *branch, t_route **route, t_route *prev);
void			set_branches(t_branch *head);
void			set_fork(t_route *route, t_room *from, t_room *to);
int				count_branches(t_branch *branch);
t_branch		**branches_to_array(int count, t_branch *branch,
				t_route *route);
void			order_routes(t_branch **branch, t_route *tmp);

#endif
