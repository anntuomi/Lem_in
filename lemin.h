#ifndef LEMIN_H
# define LEMIN_H

# include <unistd.h>
# include <stdlib.h>
# include "libft.h"
# include "get_next_line.h"

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
	int				ant_count;
	t_route			**ants;
	t_room			*rooms;
	t_room			*start;
	t_room			*end;
	t_routes		*routes;
	int				route_count;
	t_routes		**ordered;
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

char			*ft_append(char const *s1, char const *s2, int *len,
				char c);
char			*move_ants(t_route **ants);
int				calculate_moves(t_routes **routes, int path_count,
				int ant_count);
int				count_max_path_count(t_routes *routes, t_routes *shortest);
int				count_routes(t_routes *routes);
int				count_unvisited(t_path *path, t_route *route);
int				determine_room_type(char *line);
int				is_unvisited(t_room *room, t_route *route);
t_route			*get_route(t_room *room, int index);
t_routes		**determine_used_routes(t_farm *farm);
t_routes		**routes_to_array(int route_count, t_routes *routes);
t_routes		*del_dead_ends(t_routes *routes);
t_routes		*find_next_unique(t_routes **ordered, t_routes **used_routes);
t_routes		*get_routes(t_room *start);
t_routes		*get_routes_to_end(t_room *start);
void			*set_route_before_fork(t_routes *routes, t_routes *before_fork);
void			create_room_list(t_room **head, char **line, t_input **input);
void			handle_error(void);
void			initialize_variables(int path_count, t_routes ***used_routes,
				t_routes ***tmp, t_var *var);
void			order_routes(t_routes **routes);
void			print_input(t_input *input);
void			print_output(char **output, int *len);
void			set_input(t_input **input, char *line, int room);
void			set_links(char *line, t_room *room, t_input **input);
void			set_routes(t_routes *routes, t_room *room);
void			solve(t_farm farm);
void			print_input(t_input *input);
void			free_memory(t_farm farm);

#endif
