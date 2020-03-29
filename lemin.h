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
	int				visited;
	int				ant_count;
	t_path			*paths;
	struct s_room	*next;
}				t_room;

typedef struct	s_link
{
	t_room			*room1;
	t_room			*room2;
	struct s_link	*next;
}				t_link;

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
	t_link		*links;
	t_room		*start;
	t_room		*end;
	t_routes	*routes;
	t_routes	*shortest_route;
}				t_farm;

t_route			**get_ants(int *amount);
void			create_room_list(t_room **head, char **line);
void			ants_to_start(t_route **ants, int amount, t_routes *shortest);
t_link			*get_links(char *line, t_room *room);
void			set_links(t_room *rooms_head, t_link *links_head);
t_routes		*get_routes_to_end(t_room *start);
void			set_routes(t_routes *routes, t_room *room);
t_routes		*get_routes(t_room *start);
void			*set_route_before_fork(t_routes *routes, t_routes *before_fork);
t_route			*get_route(t_room *room, int index);
int				count_unvisited(t_path *path, t_route *route);
int				is_unvisited(t_room *room, t_route *route);
t_routes		*del_dead_ends(t_routes *routes);
t_routes		*get_shortest_route(t_routes *routes);
void			solve(t_farm farm, t_routes **ordered);
void			handle_error(void);

#endif
