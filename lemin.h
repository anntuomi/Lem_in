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
	int			amount;
	t_route		**ants;
	t_room		*rooms;
	t_room		*start;
	t_room		*end;
	t_routes	*routes;
	int			count;
	t_routes	**ordered;
	int			path_count;
	int			needed_routes;
}				t_farm;

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
int				count_routes(t_routes *routes);
t_routes		**routes_to_array(int route_count, t_routes *routes);
void			order_routes(t_routes **routes);
int				count_needed_routes(t_routes *routes, t_routes *shortest);
void			solve(t_farm farm, t_routes **ordered, int path_count);
t_routes		**determine_used_routes(t_routes **ordered, int *path_count,
				int i, int j);
void			initialize_arrays(int path_count, t_routes ***used_routes,
				t_routes ***most_uniques, int *most_paths);
void			ants_to_end(t_farm farm);
void			handle_error(void);

#endif
