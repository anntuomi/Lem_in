#ifndef LEMIN_H
# define LEMIN_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "libft.h"
# include "get_next_line.h"
# include "ft_printf.h"

# define START 0
# define NORMAL 1
# define END 2
# define NOTROOM 0
# define ISROOM 1

typedef struct	s_input
{
	char			*line;
	struct s_input	*next;
}				t_input;

typedef struct	s_path
{
	void			*content;
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
	int				route_count;
	t_path			*paths;
	struct s_room	*next;
}				t_room;

typedef struct	s_route
{
	t_room			*room;
	int				index;
	struct s_route	*next;
}				t_route;

typedef struct	s_routes
{
	t_route			*route;
	int				rooms;
	struct s_routes	*next;
}				t_routes;

typedef struct	s_farm
{
	int				amount;
	t_route			**ants;
	t_room			*rooms;
	t_room			*start;
	t_room			*end;
	t_routes		*routes;
	int				count;
	t_routes		**ordered;
	int				path_count;
	int				needed_routes;
}				t_farm;

typedef struct	s_variables
{
	double		least_moves;
	double		moves;
	int			current_path_count;
	int			new_path_count;
	int			orig_path_count;
}				t_variables;

t_route			**get_ants(int *amount, t_input **input);
void			create_room_list(t_room **head, char **line, t_input **input);
int				determine_room_type(char *line);
void			set_input(t_input **input, char *line, int room);
void			set_links(char *line, t_room *room, t_input **input);
int				count_words(void **array);
t_routes		*get_routes_to_end(t_room *start);
void			set_routes(t_routes *routes, t_room *room);
t_routes		*get_routes(t_room *start);
void			*set_route_before_fork(t_routes *routes, t_routes *before_fork);
t_route			*get_route(t_room *room, int index);
int				count_unvisited(t_path *path, t_route *route);
int				is_unvisited(t_room *room, t_route *route);
t_routes		*del_dead_ends(t_routes *routes);
t_routes		*del_route(t_routes *routes);
int				count_routes(t_routes *routes);
t_routes		**routes_to_array(int route_count, t_routes *routes);
void			order_routes(t_routes **routes);
int				count_needed_routes(t_routes *routes, t_routes *shortest);
void			solve(t_farm farm, t_routes **ordered, int path_count);
t_routes		**determine_used_routes(t_farm farm, int *path_count,
				int i, int j);
double			calculate_moves(t_routes **used, int path_count, \
				int ant_amount);
void			initialize_arrays(int path_count, t_routes ***used_routes,
				t_routes ***most_uniques, t_variables *var);
t_routes		**get_routes_to_use(t_farm farm);
t_routes		**get_routes_array(int size);
t_routes		**get_rooms_routes(t_routes *rooms, int start,
				t_routes **routes);
t_routes		*get_routes_to_route(t_route *route, int rooms);
t_routes		**del_null_elems(t_routes **routes, int size);
int				*count_routes_len(t_routes **routes, int size);
t_routes		**get_unique_routes(t_routes **routes);
void			del_routes_linked_list_array(t_routes **routes);
void			ants_to_end(t_farm farm, t_routes **unique_routes);
void			print_start_end_rooms(t_routes *start_end_rooms);
void			print_rooms_routes(t_routes **rooms_routes);
void			print_unique_routes(t_routes **routes);
void			print_null_routes(t_routes **routes, int size);
void			handle_error(void);

#endif
