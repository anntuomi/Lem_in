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
	struct s_route	*next;
}				t_route;

typedef struct	s_farm
{
	int				ant_count;
	t_route			**ants;
	t_room			*rooms;
	t_room			*start;
	t_room			*end;
	t_route			*routes;
	int				route_count;
	t_route			**ordered;
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

char			*ft_append(char const *s1, char const *s2, int *len, char c);
//char            *move_ants(t_route **ants);
//int             calculate_moves(t_routes **routes, int path_count,
//				int ant_count);
//int             count_max_path_count(t_routes *routes, t_routes *shortest);
int				count_routes(t_route *routes);
int				count_unvisited(t_path *path, t_room *prev, t_fork *forks);
int				determine_room_type(char *line);
int				is_unvisited(t_room *room, int normal, t_room *prev,
				t_fork *fork);
void			set_fork(t_route *route, t_fork **fork, t_room *from,
				t_room *to);
void			set_route_before_fork(t_route *route, t_fork **fork,
				t_route *before_fork);
void			del_route(t_route **route, t_route *prev);
t_route			*get_route(void);
t_route			*get_routes(t_room *start);
//t_routes        **determine_used_routes(t_farm *farm);
t_route			**routes_to_array(int route_count, t_route *routes);
//t_routes        *find_next_unique(t_routes **ordered, t_routes **used_routes);
void			create_room_list(t_room **head, char **line, t_input **input);
//void            free_memory(t_farm farm);
void			ft_delete(char **array);
void			handle_error(void);
//void            initialize_variables(int path_count, t_routes ***used_routes,
//				t_routes ***tmp, t_var *var);
void			order_routes(t_route **routes);
void			print_input(t_input *input);
void			print_output(char **output, int *len);
void			set_input(t_input **input, char *line, int room);
void			set_links(char *line, t_room *room, t_input **input);
void			set_routes(t_route *route, t_room *room, t_room *prev_room);
//void            solve(t_farm farm);

#endif
