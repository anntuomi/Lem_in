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
# define SET_FLAGS_FOR_FUTURE_USE NULL
# define SIMPLE 0
# define FLOWS 1

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
	int				visited;
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
	int				id;
	t_route			*route;
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
}				t_farm;

typedef struct	s_group
{
	t_route			**routes;
	int				path_count;
	int				moves;
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
	int				end_counter;
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

typedef struct	s_block
{
	char		*ant_room_name;
	char		*ant_nbr;
}				t_block;

typedef struct	s_branch_set
{
	t_branch	*branch;
	t_branch	*prev;
}				t_branch_set;

char			*add_to_command_line(char *line, t_block *block, int *first);
char			*ft_append(char const *s1, char const *s2, int *len, char c);
char			*move_ants(t_ant **ants);
int				calculate_moves(t_route **routes, int path_count,
				int ant_count);
int				count_routes(t_branch *branch);
int				del_route(t_branch **branch, t_branch **prev_branch,
				t_route **route, t_route *prev_route);
int				determine_room_type(char *line, int room_type);
int				edmonds_karp_traverse(t_farm farm);
int				handle_room_is_end(t_branch_set *set, t_route **prev_route,
				t_route **route);
int				is_connected_to_end(t_room *room, t_room **end, int *fork,
				int state);
t_branch		*get_branches_to_end(t_room *start, int state);
t_fork			*create_fork(t_room *from, t_room *to);
t_fork			*save_dfs_forks(t_room *start, t_room *current, int *length,
				t_room *end);
t_group			*rebuild_routes_dfs(t_farm farm, int orig_group_size,
				t_room **starting_rooms, int j);
t_level			*create_level(t_level *previous);
t_level			*create_starting_level(t_room *start);
t_room			**count_group_size(t_room *start, int *group_size);
t_room			**reorder(t_room **original, int *group_size, int reorder_by,
				int offset);
t_route			**routes_to_array(int count, t_branch *branch);
t_route			*get_fork_route(t_route *before_fork, t_room *room);
void			create_room_list(t_room **head, char **line, t_input **lines);
void			delete_branches(t_branch *branch);
void			delete_forks(t_fork *fork_head);
void			delete_group(t_group *group);
void			find_best_routes(t_farm *farm);
void			find_edges(t_room **head, t_room **start, t_room **end);
void			free_memory(t_farm farm);
void			free_route(t_route *route);
void			ft_delete(char **array);
void			handle_error(int flags, char *str);
void			order_routes(t_route **array);
void			print_input(t_input *input);
void			print_output(char **output, int *len);
void			set_branches(t_branch **head, int state);
void			set_fork(t_route *route, t_room *from, t_room *to);
void			set_input(t_input **input, char *line);
void			set_links(char *line, t_room *room, t_input **input);
void			solve(t_farm farm, int flags);
void			optimize_path_count(t_group *group, int ant_count,
				int all_moves);

#endif
