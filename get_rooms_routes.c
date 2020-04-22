#include "lemin.h"

t_routes		**del_null_elems(t_routes **routes, int size)
{
	t_routes	**new;
	int			*new_size;
	int			i1;
	int			i2;

	new_size = count_routes_len(routes, size);
	if (!(new = (t_routes **)malloc(sizeof(t_routes *) * (new_size[0] + 1))))
		handle_error();
	free(new_size);
	i1 = 0;
	i2 = 0;
	while (i1 < size)
	{
		if (routes[i1])
		{
			new[i2++] = routes[i1];
		}
		i1++;
	}
	new[i2] = NULL;
	free(routes);
	return (new);
}

static t_routes	*get_routes_to_route(t_route *route, int rooms)
{
	t_routes	*routes;

	if (!(routes = (t_routes *)malloc(sizeof(t_routes))))
		handle_error();
	routes->route = route;
	routes->rooms = rooms;
	routes->next = NULL;
	return (routes);
}

static void		add_route(t_routes **dst, int i, t_routes *src)
{
	t_routes	*routes;

	if (!dst[i])
		dst[i] = get_routes_to_route(src->route, src->rooms);
	else
	{
		routes = dst[i];
		while (routes->next)
			routes = routes->next;
		routes->next = get_routes_to_route(src->route, src->rooms);
	}
}

static void		set_index(int *index, t_route *route, t_routes *rooms,
int place)
{
	t_route		*room;

	room = rooms->route;
	while (room && room->room->id != route->room->id)
		room = room->next;
	if (route->index == place)
		*index = room->index;
	else if (room)
		*index = 0;
}

t_routes		**get_rooms_routes(t_routes *rooms, int start,
t_routes **routes)
{
	t_routes	**rooms_routes;
	t_route		*route;
	int			index;
	int			i;

	rooms_routes = get_array_routes(rooms->rooms);
	i = 0;
	while (routes[i])
	{
		index = -1;
		route = routes[i]->route;
		while (route)
		{
			set_index(&index, route, rooms, start ? 2 : routes[i]->rooms - 1);
			if (!index)
				break ;
			route = route->next;
		}
		if (index)
			add_route(rooms_routes, index - 1, routes[i]);
		i++;
	}
	rooms_routes = del_null_elems(rooms_routes, rooms->rooms);
	return (rooms_routes);
}
