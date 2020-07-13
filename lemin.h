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

# define ERROR 1
# define TURNS 2
# define ERROR_TURNS 3
# define INT_MIN -2147483648
# define INT_MAX 2147483647
# define START 0
# define NORMAL 1
# define END 2
# define NOT_ROOM 0
# define IS_ROOM 1
# define UNUSED -10

typedef struct	s_input
{
	char			*line;
	struct s_input	*next;
}				t_input;

typedef struct	s_path
{
	void			*room;
	int				flow;
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
	int				connection;
	int				used;
	int				used2;
	void			*branch;
	void			*route;
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
	int				id;
	void			*branch;
	t_fork			*forks;
	int				rooms;
	t_room			*room;
	t_room			*prev;
	t_fork			*fork;
	struct s_route	*next;
}				t_route;

typedef struct	s_branch
{
	int				id;
	t_route			*route;
	t_route			**array;
	int				routes;
	struct s_branch	*next;
}				t_branch;

typedef struct	s_ant
{
	t_fork			*forks;
	t_room			*room;
	t_room			*prev;
	t_fork			*fork;
}				t_ant;

typedef struct	s_farm
{
	int				ant_count;
	t_ant			**ants;
	t_room			*rooms;
	t_room			*start;
	t_room			*end;
	t_branch		*branches;
	int				route_count;
	t_route			**ordered;
	int				path_count;
}				t_farm;

typedef struct	s_group
{
	t_route			**routes;
	int				path_count;
	int				moves;
	struct s_group	*next;

}				t_group;

typedef struct	s_node
{
	t_room			*prev_room;
	t_room			*room;
	int				id;
	struct s_node	*next;
	struct s_node	*previous;
}				t_node;

typedef struct	s_level
{
	t_node			*nodes;
	int				depth;
	int				size;
	int				end_counter;
	int				end_capacity;
	struct s_level	*next;
}				t_level;

typedef struct	s_var
{
	int				least_moves;
	int				moves;
	int				current_path_count;
	int				new_path_count;
	int				max_path_count;
}				t_var;

char			*ft_append(char const *s1, char const *s2, int *len, char c,
				int flags);
char			*move_ants(t_ant **ants, int flags);
int				calculate_moves(t_route **routes, int path_count,
				int ant_count);
int				count_routes(t_branch *branch);
int				determine_room_type(char *line);
int				edmonds_karp_traverse(t_farm farm, int flags);
int				is_connected_to_end(t_room *room, t_room **end, int *fork);
int				is_unvisited(t_room *room, t_room *prev, t_fork *fork);
t_branch		*get_branches_to_end(t_room *start, int flags);
t_route			**routes_to_array(int count, t_branch *branch, int flags);
t_route			*get_fork_route(t_route *before_fork, t_room *room, int flags);
void			create_room_list(t_room **head, char **line, t_input **input,
				int flags);
void			del_route(t_branch **branch, t_branch **prev_branch,
				t_route **route, t_route *prev_route);
void			find_best_routes(t_farm *farm, int flags);
void			free_memory(t_farm farm);
void			free_route(t_route *route);
void			ft_delete(char **array);
void			handle_error(int flags, char *str);
void			order_routes(t_route **array);
void			print_input(t_input *input, int flags);
void			print_output(char **output, int *len);
void			set_branches(t_branch **head, int flags);
void			set_fork(t_route *route, t_room *from, t_room *to, int flags);
void			set_input(t_input **input, char *line, int room, int flags);
void			set_links(char *line, t_room *room, t_input **input, int flags);
void			solve(t_farm farm, int flags);

#endif
